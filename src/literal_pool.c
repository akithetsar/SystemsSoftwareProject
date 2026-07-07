#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

LiteralPoolEntry *literalPool = NULL;
int literalPoolCount = 0;
int literalPoolCapacity = 0;



void init_literal_pool()
{
    literalPoolCapacity = 64;
    literalPoolCount = 0;

    literalPool = (LiteralPoolEntry*)
        calloc(literalPoolCapacity,
               sizeof(LiteralPoolEntry));
}

void ensure_literal_pool_capacity()
{
    if(literalPoolCount < literalPoolCapacity)
        return;

    literalPoolCapacity *= 2;

    literalPool = (LiteralPoolEntry*)
        realloc(
            literalPool,
            literalPoolCapacity *
            sizeof(LiteralPoolEntry)
        );
}

LiteralPoolEntry* get_literal_pool_entry(
    int symbolIndex,
    int section
)
{
    for(int i = 0; i < literalPoolCount; i++)
    {
        if(literalPool[i].symbolIndex == symbolIndex &&
           literalPool[i].section == section)
        {
            return &literalPool[i];
        }
    }

    return NULL;
}

LiteralPoolEntry* add_literal_pool_entry(Symbol *sym)
{
    ensure_literal_pool_capacity();

    LiteralPoolEntry *entry =
        &literalPool[literalPoolCount++];

    entry->symbolIndex = sym->num;
    entry->section = litSection;

    SectionDefinition *lit = &sectionDefinitions[litSection];

    entry->offset = lit->length;

    uint32_t value = sym->value;

    lit->data = (uint8_t*)realloc(lit->data, lit->length + 4);

    lit->data[lit->length + 0] = (uint8_t)(value & 0xFF);
    lit->data[lit->length + 1] = (uint8_t)((value >> 8) & 0xFF);
    lit->data[lit->length + 2] = (uint8_t)((value >> 16) & 0xFF);
    lit->data[lit->length + 3] = (uint8_t)((value >> 24) & 0xFF);

    lit->length += 4;

    if (sym->defined == 0 || sym->value == 0xFFFFFFFF)
    {
        add_relocation(
            litSection,
            entry->offset,
            sym->num,
            ABS32,
            0
        );
    }

    return entry;
}

LiteralPoolEntry* get_or_create_literal_pool_entry(Symbol *sym)
{
    for (int i = 0; i < literalPoolCount; i++)
    {
        if (literalPool[i].symbolIndex == sym->num &&
            literalPool[i].section == litSection)
        {
            return &literalPool[i];
        }
    }

    return add_literal_pool_entry(sym);
}


int displacement_fits_12bit(
    int32_t d
)
{
    return d >= -2048 &&
           d <= 2047;
}

int32_t calculate_literal_displacement(
    uint32_t instructionOffset,
    LiteralPoolEntry *entry
)
{
    return
        (int32_t)entry->offset -
        (int32_t)instructionOffset;
}
