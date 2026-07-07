#ifndef RELOCATIONS_H
#define RELOCATIONS_H
#include <stdint.h>
typedef enum {
    ABS32, 
    JMP_LIT, // upisuje u d = litbase+addend - pc
    PCREL12 // upisuje u d = relocation.addend - (section_base + relocation.offset + 4);
} RelocationType;

typedef struct Relocation {
    uint32_t offset;
    int symbolIndex;
    RelocationType type;
    uint32_t addend;
} Relocation;

void init_relocations();

void add_relocation(
    int section,
    uint32_t offset,
    int symbolIndex,
    RelocationType type,
    uint32_t addend
);

void print_relocation_table();

#endif