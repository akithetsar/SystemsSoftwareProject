#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>
#include <stdlib.h>

extern int locationCounter;

int init_assembler();

/* ---------------- Output buffer ---------------- */

typedef struct {
    uint8_t *data;
    uint32_t size;
    uint32_t capacity;
} OutputContent;

extern OutputContent outputContent;

/* ---------------- Sections ---------------- */

typedef struct {
    char *name;
    uint32_t base;
    uint32_t length;
} SectionDefinition;

extern SectionDefinition **sectionDefinitions;
extern SectionDefinition *currentSection;

#endif