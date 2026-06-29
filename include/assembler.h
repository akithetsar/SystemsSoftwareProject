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

typedef struct {
    char *name;
    uint8_t *data;
    uint32_t length;
    
    Relocation *relocs;
    int relocCount;
    int relocCapacity;

    uint32_t name_offset; // for .shstrtab

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
    uint32_t name_offset;
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

void emit_symbol_word(char* sym_name);

//Literal Pool
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

typedef enum JumpModes {
    JMP_BASE           = 0, // pc <= gpr[A] + D
    JMP_EQ             = 1, // if (B == C)
    JMP_NE             = 2, // if (B != C)
    JMP_GT             = 3, // if (B > C signed)
    JMP_MEM_BASE       = 8, // pc <= mem32[gpr[A] + D]
    JMP_MEM_EQ         = 9,
    JMP_MEM_NE         = 10,
    JMP_MEM_GT         = 11
} JumpModes;

uint32_t form_jump_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d);

void emit_symbol_jmp(char *sym_name, uint8_t b, uint8_t c, uint8_t mode_rel, uint8_t mode_mem);

typedef enum CallModes {
    CALL_REL           = 0, 
    CALL_MEM           = 1, 
    
} CallModes;


uint32_t form_call_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint16_t d);


void emit_symbol_call(char *sym_name);
void emit_large_literal_load(int32_t literal, uint8_t dst_reg);
void emit_symbol_address_load(char *sym_name, uint8_t dst_reg);

// ELF File writing

typedef struct {
    uint8_t  e_ident[16];   // "magic" + metadata
    uint16_t e_type;        // relocatable = 1
    uint16_t e_machine;     // your custom ISA id
    uint32_t e_version;

    uint32_t e_entry;       // entry point (you can leave 0 for now)
    uint32_t e_phoff;      // <-- missing
    uint32_t e_shoff;       // section header table offset

    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;  // <-- missing
    uint16_t e_phnum;      // <-- missing

    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ElfHeader;

typedef struct {
    uint32_t sh_name;      
    uint32_t sh_type;

    uint32_t sh_flags;
    uint32_t sh_addr;

    uint32_t sh_offset;    // file offset
    uint32_t sh_size;

    uint32_t sh_link;
    uint32_t sh_info;

    uint32_t sh_addralign;
    uint32_t sh_entsize;
} SectionHeader;

typedef struct
{
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    uint8_t  st_info;
    uint8_t  st_other;
    uint16_t st_shndx;
} ElfSymbol;
#endif