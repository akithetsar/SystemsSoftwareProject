#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>
#include <stdlib.h>


extern int locationCounter;

int init_assembler();
int write_output_file(const char *filename);

/* ---------------- Output buffer ---------------- */

typedef struct {
    uint8_t *data;
    uint32_t size;
    uint32_t capacity;
} OutputContent;

extern OutputContent outputContent;

void emit_byte(uint8_t byte);

/* ---------------- Sections ---------------- */

typedef struct {
    char *name;
    uint8_t *data;
    uint32_t length;
} SectionDefinition;

extern SectionDefinition *sectionDefinitions;
extern int currentSection;
extern int sectionCount;

void switch_section(const char *name);
void section_emit_byte(uint8_t byte);
void section_emit_word(uint32_t value);
#endif