#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

SectionLiteralPool *literalPools = NULL;

/* Re-entrancy guard: the flush routine itself calls section_emit_word(),
 * which would otherwise recursively re-trigger the "did we grow past the
 * threshold" check while we are in the middle of emitting the pool. */
static int g_flushing_section = -1;

void init_literal_pool(void)
{
    literalPools = (SectionLiteralPool*)
        calloc(MAX_SECTIONS, sizeof(SectionLiteralPool));
}

int displacement_fits_12bit(int32_t d)
{
    return d >= -2048 && d <= 2047;
}

static void ensure_entry_capacity(SectionLiteralPool *p)
{
    if (p->count < p->capacity)
        return;

    p->capacity = p->capacity ? p->capacity * 2 : 8;
    p->entries = (LiteralPoolEntry*)
        realloc(p->entries, p->capacity * sizeof(LiteralPoolEntry));
}

static void ensure_ref_capacity(LiteralPoolEntry *e)
{
    if (e->refCount < e->refCapacity)
        return;

    e->refCapacity = e->refCapacity ? e->refCapacity * 2 : 4;
    e->refInstrOffsets = (uint32_t*)
        realloc(e->refInstrOffsets, e->refCapacity * sizeof(uint32_t));
}

static void note_epoch_open(SectionLiteralPool *p, uint32_t instrOffset)
{
    if (!p->hasOpen)
    {
        p->hasOpen = 1;
        p->openOffset = instrOffset;
    }
}

static LiteralPoolEntry* find_or_add_const(SectionLiteralPool *p, uint32_t value)
{
    for (int i = 0; i < p->count; i++)
        if (p->entries[i].kind == LIT_CONST && p->entries[i].constValue == value)
            return &p->entries[i];

    ensure_entry_capacity(p);
    LiteralPoolEntry *e = &p->entries[p->count++];
    memset(e, 0, sizeof(*e));
    e->kind = LIT_CONST;
    e->constValue = value;
    return e;
}

static LiteralPoolEntry* find_or_add_symbol(SectionLiteralPool *p, int symbolIndex)
{
    for (int i = 0; i < p->count; i++)
        if (p->entries[i].kind == LIT_SYMBOL && p->entries[i].symbolIndex == symbolIndex)
            return &p->entries[i];

    ensure_entry_capacity(p);
    LiteralPoolEntry *e = &p->entries[p->count++];
    memset(e, 0, sizeof(*e));
    e->kind = LIT_SYMBOL;
    e->symbolIndex = symbolIndex;
    return e;
}

LiteralPoolEntry* literal_pool_use_const(int section, uint32_t value, uint32_t instrOffset)
{
    SectionLiteralPool *p = &literalPools[section];

    LiteralPoolEntry *e = find_or_add_const(p, value);

    ensure_ref_capacity(e);
    e->refInstrOffsets[e->refCount++] = instrOffset;

    note_epoch_open(p, instrOffset);
    literal_pool_after_section_grow(section);

    return e;
}

LiteralPoolEntry* literal_pool_use_symbol(int section, int symbolIndex, uint32_t instrOffset)
{
    SectionLiteralPool *p = &literalPools[section];

    LiteralPoolEntry *e = find_or_add_symbol(p, symbolIndex);

    ensure_ref_capacity(e);
    e->refInstrOffsets[e->refCount++] = instrOffset;

    note_epoch_open(p, instrOffset);
    literal_pool_after_section_grow(section);

    return e;
}

/* Patches only the low 12 bits (Disp field) of an already-emitted
 * instruction word, leaving OC/MOD/RegA/RegB/RegC untouched. */
static void patch_disp12(int section, uint32_t offset, int32_t d)
{
    uint32_t word = get_section_word(section, offset);
    word = (word & 0xFFFFF000u) | ((uint32_t)d & 0xFFFu);
    section_patch_word(section, offset, word);
}

void literal_pool_flush(int section)
{
    if (section < 0 || section >= sectionCount)
        return;

    SectionLiteralPool *p = &literalPools[section];

    if (!p->hasOpen || p->count == 0)
    {
        p->hasOpen = 0;
        p->count = 0;
        return;
    }

    int prevFlushing = g_flushing_section;
    g_flushing_section = section;

    int savedSection = currentSection;
    currentSection = section;

    /* 1) Emit an unconditional branch that jumps clean over the pool, so
     *    the pooled data is never fetched/executed as an instruction.
     *    pc (as seen by the jump) is already the address of the *next*
     *    instruction (offset+4), so landing exactly poolBytes further
     *    lands right after the last pooled word. */
    uint32_t poolBytes = (uint32_t)p->count * 4u;

    uint32_t jmpInstr = form_jump_instruction(JMP_BASE, 15, 0, 0, (uint16_t)poolBytes);
    section_emit_word(jmpInstr);

    /* 2) Emit the pooled words themselves and remember their final
     *    offsets, then patch every instruction that referenced them. */
    for (int i = 0; i < p->count; i++)
    {
        LiteralPoolEntry *e = &p->entries[i];
        e->offset = sectionDefinitions[section].length;

        if (e->kind == LIT_CONST)
        {
            section_emit_word(e->constValue);
        }
        else /* LIT_SYMBOL */
        {
            Symbol *sym = &symbolTable[e->symbolIndex];
            section_emit_word(sym->defined ? sym->value : 0);

            /* Always attach a relocation for the slot's content - it will
             * be overwritten with the symbol's final address once the
             * linker knows it (works uniformly for local, global and
             * still-undefined/extern symbols). */
            add_relocation(section, e->offset, e->symbolIndex, ABS32, 0);
        }

        for (int r = 0; r < e->refCount; r++)
        {
            uint32_t instrOffset = e->refInstrOffsets[r];
            int32_t d = (int32_t)e->offset - (int32_t)instrOffset - 4;

            if (!displacement_fits_12bit(d))
            {
                fprintf(stderr,
                    "assembler: internal error - literal pool displacement "
                    "%d out of 12-bit range in section '%s' (offset 0x%X)\n",
                    d, sectionDefinitions[section].name, instrOffset);
                exit(1);
            }

            patch_disp12(section, instrOffset, d);
        }

        free(e->refInstrOffsets);
    }

    p->count = 0;
    p->hasOpen = 0;

    currentSection = savedSection;
    g_flushing_section = prevFlushing;
}

void literal_pool_after_section_grow(int section)
{
    if (section < 0 || section >= sectionCount)
        return;

    if (g_flushing_section == section)
        return; /* avoid recursing while the flush itself emits words */

    SectionLiteralPool *p = &literalPools[section];

    if (!p->hasOpen || p->count == 0)
        return;

    uint32_t distanceSoFar = sectionDefinitions[section].length - p->openOffset;

    /* Worst case additional distance still to come before we could next
     * check in: the branch-over jump (4 bytes) plus all currently
     * pending pool entries. Flush proactively before that could push the
     * *earliest* still-open reference outside the signed 12-bit range. */
    uint32_t worstCase = distanceSoFar + 4u + (uint32_t)p->count * 4u;

    if (worstCase + LIT_POOL_SAFETY_MARGIN > 2047u)
    {
        literal_pool_flush(section);
    }
}

void literal_pool_flush_all(void)
{
    for (int i = 0; i < sectionCount; i++)
        literal_pool_flush(i);
}
