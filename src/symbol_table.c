#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

Symbol *symbolTable = NULL;

int symbolCount = 0;
int symbolCapacity = 0;


int add_symbol(
    const char *name,
    uint32_t value,
    int ndx,
    SymbolType type,
    SymbolBind bind,
    int defined
);

static void ensure_symbol_capacity()
{
    if (symbolCount < symbolCapacity)
        return;

    symbolCapacity *= 2;

    symbolTable = (Symbol*)realloc(
        symbolTable,
        symbolCapacity * sizeof(Symbol)
    );
}

int add_symbol(
    const char *name,
    uint32_t value,
    int ndx,
    SymbolType type,
    SymbolBind bind,
    int defined
)
{
    ensure_symbol_capacity();

    int idx = symbolCount++;

    symbolTable[idx].num = idx;
    symbolTable[idx].value = value;
    symbolTable[idx].size = 0;

    symbolTable[idx].type = type;
    symbolTable[idx].bind = bind;

    symbolTable[idx].ndx = ndx;

    symbolTable[idx].name = strdup(name);
    symbolTable[idx].name_offset = strtab_add(name);

    symbolTable[idx].defined = defined;
    symbolTable[idx].flink = NULL;
    return idx;
}

void init_symbol_table()
{
    symbolCapacity = 64;
    symbolCount = 0;

    symbolTable = (Symbol*)calloc(
        symbolCapacity,
        sizeof(Symbol)
    );

    add_symbol("", 0x00000000, -1, SYM_NOTYP, SYM_LOC, 1);

}



int find_symbol(const char *name)
{
    for (int i = 0; i < symbolCount; i++)
    {
        if (strcmp(symbolTable[i].name, name) == 0)
            return i;
    }

    return -1;
}

Symbol* get_symbol(const char *name)
{
    int idx = find_symbol(name);

    if (idx == -1)
        return NULL;

    return &symbolTable[idx];
}


const char* get_symbols_section_name(const char *name){
    int idx = find_symbol(name);
    if(idx == -1)
        return NULL;
    int sectionIdx = symbolTable[idx].ndx;
    return symbolTable[sectionIdx].name;
}


void add_flink(Symbol* sym){
    ST_ForwardRefs *new_flink = (ST_ForwardRefs*)calloc(1, sizeof(Symbol));
    new_flink->section = currentSection;
    new_flink->nlink = NULL;
    new_flink->adress = sectionDefinitions[currentSection].length;
    ST_ForwardRefs *curr, *prev;

    if(sym->flink == NULL){
        sym->flink = new_flink;
        return;
    }
    curr = sym->flink;
    prev = sym->flink;

    while(curr != NULL){
        prev = curr;
        curr = curr->nlink;
    }

    prev->nlink = new_flink;


}

void backpatch(Symbol* sym){
    ST_ForwardRefs *curr = sym->flink;

    while(curr != NULL){
        section_patch_word(curr->section, curr->adress, sym->value);
        curr = curr->nlink;
    }

}

void emit_symbol_word(char* sym_name){
    
    //for backpatch
    Symbol *sym = get_symbol(sym_name);
    if(sym != NULL){
        //Symbol in table
        if(sym->defined == 1){
            //symbol defined
            printf("Emit section word with symbol: %s\n", sym_name);
            section_emit_word(sym->value);
        }
        else if (sym->defined == 0){
            //symbol undefined
            add_flink(sym);
            section_emit_word(0x00000000);
            }
        }
    else{
        //Symbol not in table
        add_symbol(sym_name, 0xFFFFFFFF, -1, SYM_NOTYP, SYM_LOC, 0);
        add_flink(get_symbol(sym_name));
        section_emit_word(0x00000000);
    }
    //for reloc
    sym = get_symbol(sym_name);

    uint32_t patchOffset = sectionDefinitions[currentSection].length - 4;

    add_relocation(
        currentSection,
        patchOffset,
        sym->num,
        ABS32,
        0
    );
}

void emit_large_literal_load(int32_t literal, uint8_t dst_reg)
{
    /* A plain numeric constant that doesn't fit in 12 bits. It's not the
     * address of anything, so it goes straight into the pool as raw data
     * with no relocation attached - its value never changes. */
    uint32_t instr =
        form_load_instruction(
            LOAD_GPR_FROM_MEM_INDEXED,
            dst_reg,
            15,
            0,
            0   // placeholder, patched once the literal pool flushes
        );

    uint32_t offset =
        sectionDefinitions[currentSection].length;

    section_emit_word(instr);

    literal_pool_use_const(currentSection, (uint32_t)literal, offset);
}

void emit_symbol_address_load(char *sym_name, uint8_t dst_reg)
{
    Symbol *sym = get_symbol(sym_name);

    if(sym == NULL)
    {
        add_symbol(
            sym_name,
            0xFFFFFFFF,
            -1,
            SYM_NOTYP,
            SYM_LOC,
            0
        );

        sym = get_symbol(sym_name);
    }

    /*
     * The symbol's final, linked address is never known at assembly
     * time (it depends on where the linker ends up placing its section),
     * so - regardless of how small its current, pre-link value looks -
     * this always has to go through the literal pool with a relocation
     * that the linker fills in once the real address is known.
     */
    uint32_t instr =
        form_load_instruction(
            LOAD_GPR_FROM_MEM_INDEXED,
            dst_reg,
            15,   // pc
            0,
            0   // placeholder, patched once the literal pool flushes
        );

    uint32_t offset =
        sectionDefinitions[currentSection].length;

    section_emit_word(instr);

    literal_pool_use_symbol(currentSection, sym->num, offset);
}

const char* symbol_type_to_string(SymbolType type)
{
    switch(type)
    {
        case SYM_SCTN: return "SCTN";
        case SYM_NOTYP: return "NOTYP";
        default: return "???";
    }
}

const char* symbol_bind_to_string(SymbolBind bind)
{
    switch(bind)
    {
        case SYM_LOC:  return "LOC";
        case SYM_GLOB: return "GLOB";
        default: return "???";
    }
}



void print_symbol_table()
{
    printf("\n#.symtab\n");
    printf("%-4s %-8s %-6s %-8s %-6s %-4s %s %s\n",
           "Num",
           "Value",
           "Size",
           "Type",
           "Bind",
           "Defined",
           "Ndx",
           "Name"
           );

    for(int i = 0; i < symbolCount; i++)
    {
        Symbol *sym = &symbolTable[i];

        printf("%-4d %08X %-6u %-8s %-6s %-6i",
               sym->num,
               sym->value,
               sym->size,
               symbol_type_to_string(sym->type),
               symbol_bind_to_string(sym->bind),
               sym->defined
               );

        if(sym->ndx == -1)
            printf("%-4s ", "UND");
        else
            printf("%-4d ", sym->ndx);

        printf("%s", sym->name);
        printf("\t-->\t");
        
        ST_ForwardRefs *curr = sym->flink;
        while(curr != NULL){
            printf("{section: %i, adress: %08X}\t-->\t", curr->section, curr->adress);
            curr = curr->nlink;
        }
        printf("NULL");

        printf("\n");
    }

    printf("\n");
}

