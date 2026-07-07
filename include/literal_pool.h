#ifndef LIT_POOL_H
#define LIT_POOL_H

#include <stdint.h>
#include "symbol_table.h"
typedef struct {
    int symbolIndex;
    uint32_t offset;
    int section;
} LiteralPoolEntry;

extern LiteralPoolEntry *literalPool;
extern int literalPoolCount;
extern int literalPoolCapacity;

void init_literal_pool();
void ensure_literal_pool_capacity();
LiteralPoolEntry* get_literal_pool_entry(
    int symbolIndex,
    int section
);
LiteralPoolEntry* add_literal_pool_entry(
    Symbol *sym
);
LiteralPoolEntry* get_or_create_literal_pool_entry(
    Symbol *sym
);
int displacement_fits_12bit(
    int32_t d
);
int32_t calculate_literal_displacement(
    uint32_t instructionOffset,
    LiteralPoolEntry *entry
);

#endif