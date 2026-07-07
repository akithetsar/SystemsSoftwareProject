#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>



static void ensure_reloc_capacity(SectionDefinition *sec)
{
    if (sec->relocCount < sec->relocCapacity)
        return;

    sec->relocCapacity *= 2;

    sec->relocs = (Relocation*)realloc(
        sec->relocs,
        sec->relocCapacity * sizeof(Relocation)
    );
}

void add_relocation(
    int section,
    uint32_t offset,
    int symbolIndex,
    RelocationType type,
    uint32_t addend
)
{
    SectionDefinition *sec =
        &sectionDefinitions[section];

    ensure_reloc_capacity(sec);

    Relocation *rel =
        &sec->relocs[sec->relocCount++];

    rel->offset = offset;
    rel->symbolIndex = symbolIndex;
    rel->type = type;
    rel->addend = addend;
}

static const char* relocation_type_to_string(RelocationType type)
{
    switch(type)
    {
        case ABS32:
            return "ABS32";
        case JMP_LIT:
            return "JMP_LIT";
        case PCREL12:
            return "PCREL12";
        default:
            return "???";
    }
}

void print_relocation_table()
{
    for (int s = 0; s < sectionCount; s++)
    {
        SectionDefinition *sec =
            &sectionDefinitions[s];

        printf("\n#.rela%s\n", sec->name);

        printf("%-10s %-10s %-10s %-10s\n",
               "Offset",
               "Type",
               "Symbol",
               "SymbolNdx");

        for (int i = 0; i < sec->relocCount; i++)
        {
            Relocation *rel =
                &sec->relocs[i];

            printf("%08X %-10s %-10s %-10i\n",
                   rel->offset,
                   relocation_type_to_string(rel->type),
                   symbolTable[rel->symbolIndex].name, rel->symbolIndex);
        }
    }
}
