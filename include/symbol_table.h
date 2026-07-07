#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>

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

void init_symbol_table();
void print_symbol_table();

#endif