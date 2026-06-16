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
// REALLOCATIONS

typedef enum {
    ABS32
} RelocationType;

typedef struct Relocation {
    uint32_t offset;
    int symbolIndex;
    RelocationType type;
} Relocation;

void init_relocations();

void add_relocation(
    int section,
    uint32_t offset,
    int symbolIndex,
    RelocationType type
);

void print_relocation_table();

typedef struct {
    char *name;
    uint8_t *data;
    uint32_t length;
    
    Relocation *relocs;
    int relocCount;
    int relocCapacity;
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



// Encoding


typedef enum ArithModes{
    ARITH_ADD = 0,
    ARITH_SUB = 1,
    ARITH_MUL = 2,
    ARITH_DIV = 3
} ArithModes;

uint32_t form_arithmetic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c);

typedef enum LogicModes{
    LOGIC_NOT = 0,
    LOGIC_AND = 1,
    LOGIC_OR = 2,
    LOGIC_XOR = 3
} LogicModes;

uint32_t form_logic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c);


typedef enum ShiftModes{
    SHIFT_LEFT = 0,
    SHIFT_RIGHT = 1,
} ShiftModes;

uint32_t form_shift_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c);

typedef enum LoadModes {
    LOAD_GPR_FROM_CSR            = 0,
    LOAD_GPR_FROM_GPR_PLUS_D     = 1,
    LOAD_GPR_FROM_MEM_INDEXED    = 2,
    LOAD_GPR_FROM_MEM_POSTINC    = 3,
    LOAD_CSR_FROM_GPR            = 4,
    LOAD_CSR_OR_D                = 5,
    LOAD_CSR_FROM_MEM_INDEXED    = 6,
    LOAD_CSR_FROM_MEM_POSTINC    = 7
} LoadModes;

uint32_t form_load_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d);

typedef enum StoreModes {
    STORE_MEM_INDEXED      = 0,  // mem32[gpr[A]+gpr[B]+D] <= gpr[C]
    STORE_MEM_POSTINC      = 1,  // gpr[A]+=D; mem32[gpr[A]] <= gpr[C]
    STORE_MEM_INDIRECT     = 2   // mem32[mem32[gpr[A]+gpr[B]+D]] <= gpr[C]
} StoreModes;

uint32_t form_store_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d);


uint32_t form_pop_instruction(uint8_t gpr);   
uint32_t form_pop_csr_instruction(uint8_t csr);     
uint32_t form_push_instruction(uint8_t gpr);  
#endif