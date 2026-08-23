#ifndef LIT_POOL_H
#define LIT_POOL_H

#include <stdint.h>
#include "symbol_table.h"

/*
 * Literal pool redesign
 * ----------------------
 * The old design kept a single, global ".lit" section for the whole
 * translation unit. Every literal/address load in the program, no matter
 * which user section it lived in, pointed into that one section. Because
 * the referencing instructions use a *signed 12-bit, PC-relative*
 * displacement (range -2048..+2047 bytes), any program whose code grew
 * past roughly 2KB away from the ".lit" section could no longer encode
 * the load at all.
 *
 * The fix: each user section (e.g. ".text") owns its own literal pool,
 * and that pool is emitted *inline*, directly inside that same section,
 * right after the code that uses it (jumped over so it isn't executed as
 * instructions). Because the pool lives in the very same section as the
 * instructions referencing it, the displacement no longer depends on
 * where the linker ends up placing the section - it only depends on the
 * (small) distance between the load instruction and the pool, which the
 * assembler actively keeps under the 12-bit limit by flushing the pool
 * ("literal pool dump") before that distance gets too large.
 */

#define LIT_POOL_SAFETY_MARGIN 96u   /* bytes of headroom kept below 2047 */

typedef enum {
    LIT_CONST,   /* raw 32-bit constant (e.g. $0x12345678)               */
    LIT_SYMBOL   /* address (or value) of a symbol, resolved via reloc    */
} LiteralKind;

typedef struct {
    LiteralKind kind;

    int symbolIndex;      /* valid when kind == LIT_SYMBOL   */
    uint32_t constValue;  /* valid when kind == LIT_CONST    */

    uint32_t offset;      /* final offset within the owning section, only
                              valid after the pool has been flushed        */

    uint32_t *refInstrOffsets; /* instruction word offsets (within the
                                   same section) whose low 12 bits need to
                                   be patched with the displacement to
                                   this entry once it is flushed          */
    int refCount;
    int refCapacity;
} LiteralPoolEntry;

typedef struct {
    LiteralPoolEntry *entries;
    int count;
    int capacity;

    int hasOpen;          /* is there an unflushed epoch in this section  */
    uint32_t openOffset;  /* section length when the epoch's first
                              reference was made (i.e. the earliest
                              instruction still waiting on this pool)     */
} SectionLiteralPool;

/* One pool per possible section, parallel to sectionDefinitions[]. */
extern SectionLiteralPool *literalPools;

void init_literal_pool(void);

/*
 * Registers a use of a constant/symbol literal by the instruction word
 * that was just written at `instrOffset` within `section`. Returns the
 * pool entry backing it (its ->offset is NOT valid yet - it is only
 * known once the pool is flushed, at which point the instruction's
 * placeholder displacement is patched automatically).
 */
LiteralPoolEntry* literal_pool_use_const(int section, uint32_t value, uint32_t instrOffset);
LiteralPoolEntry* literal_pool_use_symbol(int section, int symbolIndex, uint32_t instrOffset);

/*
 * Called after anything is appended to a section (instructions or data).
 * Cheaply checks whether that section's pending literal pool is at risk
 * of exceeding the 12-bit displacement range and, if so, flushes it.
 */
void literal_pool_after_section_grow(int section);

/* Force-flushes a single section's pending pool (emits a branch-over
 * jump followed by the pooled words, and patches every instruction that
 * referenced them). Safe to call when there is nothing pending. */
void literal_pool_flush(int section);

/* Flushes every section that still has a pending pool. Call this once at
 * the very end of assembly (before writing the object file) so nothing
 * is left dangling. */
void literal_pool_flush_all(void);

int displacement_fits_12bit(int32_t d);

#endif
