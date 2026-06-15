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
uint8_t get_section_byte(int section, uint32_t address);
void section_emit_word(uint32_t value);
uint32_t get_section_word(int section, uint32_t address);
int find_section(const char *name);


/* ---------------- Symbol Table ---------------- */

typedef enum {
    SYM_NOTYP,
    SYM_SCTN
} SymbolType;

typedef enum {
    SYM_LOC,
    SYM_GLOB
} SymbolBind;

typedef struct ST_ForwardRefs{
    uint32_t adress; // adress in section
    int section;
    struct ST_ForwardRefs *nlink;
} ST_ForwardRefs;

typedef struct {
    int num;
    uint32_t value;
    uint32_t size;
    SymbolType type;
    SymbolBind bind;
    int ndx; // -1 for UND
    char *name;
    int defined;
    ST_ForwardRefs *flink;
} Symbol;

extern Symbol *symbolTable;
extern int symbolCount;
extern int symbolCapacity;

Symbol* get_symbol(const char *name);
int add_symbol(
    const char *name,
    uint32_t value,
    int ndx,
    SymbolType type,
    SymbolBind bind,
    int defined
);
int find_symbol(const char *name);
const char* get_symbols_section_name(const char *name);
void add_flink(Symbol* sym);
void backpatch(Symbol* sym);


// REALLOCATIONS

typedef enum {
    ABS32
} RelocationType;

typedef struct Relocation {
    int section;
    uint32_t offset;
    int symbolIndex;
    RelocationType type;
} Relocation;

extern Relocation *relocationTable;
extern int relocCount;
extern int relocCapacity;

void init_relocations();

void add_relocation(
    int section,
    uint32_t offset,
    int symbolIndex,
    RelocationType type
);

void print_relocation_table();

#endif