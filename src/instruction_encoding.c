#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
uint32_t form_arithmetic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c)
{
    return (0x5u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12);
}


uint32_t form_logic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c)
{
    return (0x6u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12);
}


uint32_t form_shift_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c)
{
    return (0x7u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12);
}

uint32_t form_load_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d)
{
    return (0x9u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12) |
           (d & 0xFFF);
}

uint32_t form_store_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d)
{
    return (0x8u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12) |
           (d & 0xFFF);
}

uint32_t form_pop_instruction(uint8_t gpr){
    // sp == r14
    return form_load_instruction(LOAD_GPR_FROM_MEM_POSTINC, gpr, 14, 0, 4);
}   
uint32_t form_pop_csr_instruction(uint8_t csr){
    // sp == r14
    return form_load_instruction(LOAD_CSR_FROM_MEM_POSTINC, csr, 14, 0, 4);
}   
uint32_t form_push_instruction(uint8_t gpr){
    // sp == r14
    return form_store_instruction(STORE_MEM_POSTINC, 14, 0, gpr, -4);
}   

uint32_t form_jump_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d)
{
    return (0x3u << 28) |              // opcode = JMP
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12) |
           (d & 0xFFF);
}

int can_use_pcrel(Symbol *sym)
{
    if(sym == NULL || sym->defined == 0){
        return 0;
    }
    Symbol* sym_sec = get_symbol(sectionDefinitions[currentSection].name);
    if(sym->ndx != sym_sec->ndx){
        
        return 0;
    }
        
    int32_t d =
    (int32_t)sym->value -
    (int32_t)(sectionDefinitions[currentSection].length + 4);

    return d >= -2048 && d <= 2047;
}

void emit_jmp_literal(Symbol *sym, uint8_t b, uint8_t c, uint8_t mode)
{
    LiteralPoolEntry *entry =
        get_or_create_literal_pool_entry(sym);

    uint32_t instr =
        form_jump_instruction(
            mode,
            15,
            b,
            c,
            entry->offset   // placeholder, NOT real displacement
        );

    uint32_t offset =
        sectionDefinitions[currentSection].length;

    section_emit_word(instr);

    add_relocation(
        currentSection,
        offset,
        entry->symbolIndex,
        JMP_LIT,
        entry->offset
    );
}

void emit_jmp_pcrel(Symbol *sym, uint8_t b, uint8_t c, uint8_t mode)
{
    int32_t pc =
        (int32_t)sectionDefinitions[currentSection].length + 4;

    int32_t d =
        (int32_t)sym->value - pc;

    uint32_t instr =
        form_jump_instruction(
            mode,
            15,
            b,
            c,
            (uint16_t)d
        );

    section_emit_word(instr);
}

void emit_symbol_jmp(char *sym_name, uint8_t b, uint8_t c, uint8_t mode_rel, uint8_t mode_mem)
{
    Symbol *sym = get_symbol(sym_name);

    if(sym == NULL)
    {
        add_symbol(sym_name, 0xFFFFFFFF, -1, SYM_NOTYP, SYM_LOC, 0);
    }
    sym = get_symbol(sym_name);

    if(can_use_pcrel(sym))
    {
        emit_jmp_pcrel(sym, b, c, mode_rel);

    }
    else
    {
        emit_jmp_literal(sym, b, c, mode_mem);
    }
}


uint32_t form_call_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint16_t d)
{
    return (0x2u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           (d & 0xFFF);
}

void emit_call_literal(Symbol *sym)
{
    LiteralPoolEntry *entry =
        get_or_create_literal_pool_entry(sym);

    uint32_t instr =
        form_call_instruction(
            CALL_MEM,
            15,
            0,
            entry->offset   // placeholder, NOT real displacement
        );

    uint32_t offset =
        sectionDefinitions[currentSection].length;

    section_emit_word(instr);

    add_relocation(
        currentSection,
        offset,
        entry->symbolIndex,
        JMP_LIT,
        entry->offset
    );
}

void emit_call_pcrel(Symbol *sym)
{
    int32_t pc =
        (int32_t)sectionDefinitions[currentSection].length + 4;

    int32_t d =
        (int32_t)sym->value - pc;

    uint32_t instr =
        form_call_instruction(
            CALL_REL,
            15,
            0,
            (uint16_t)d
        );

    section_emit_word(instr);
}

void emit_symbol_call(char *sym_name)
{
    Symbol *sym = get_symbol(sym_name);

    if(sym == NULL)
    {
        add_symbol(sym_name, 0xFFFFFFFF, -1, SYM_NOTYP, SYM_LOC, 0);
    }
    sym = get_symbol(sym_name);

    if(can_use_pcrel(sym))
    {
        emit_call_pcrel(sym);

    }
    else
    {
        emit_call_literal(sym);
    }
}