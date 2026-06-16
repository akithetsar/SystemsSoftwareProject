#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../build/parser.tab.h"
/* =========================
   GLOBAL STATE
   ========================= */

int locationCounter = 0;

OutputContent outputContent;

#define MAX_SECTIONS 64

SectionDefinition *sectionDefinitions = NULL;
int sectionCount = 0;
int currentSection = -1;

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
/* =========================
   OUTPUT BUFFER
   ========================= */

static void ensure_capacity(uint32_t needed) {
    if (outputContent.capacity >= needed)
        return;

    uint32_t newCap = (outputContent.capacity == 0) ? 64 : outputContent.capacity;

    while (newCap < needed)
        newCap *= 2;

    outputContent.data = (uint8_t*)realloc(outputContent.data, newCap);
    outputContent.capacity = newCap;
}

void emit_byte(uint8_t byte) {
    ensure_capacity(outputContent.size + 1);
    outputContent.data[outputContent.size++] = byte;
}
/* =========================
   SECTIONS
   ========================= */

void init_sections() {
    sectionDefinitions = (SectionDefinition*)calloc(MAX_SECTIONS, sizeof(SectionDefinition));
    sectionCount = 0;
    currentSection = -1;
}
int find_section(const char *name) {
    for (int i = 0; i < sectionCount; i++) {
        
        if (strcmp(sectionDefinitions[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
int create_section(const char *name) {
    int idx = sectionCount++;

    sectionDefinitions[idx].name = strdup(name);
    sectionDefinitions[idx].data = NULL;
    sectionDefinitions[idx].length = 0;

    sectionDefinitions[idx].relocCount = 0;
    sectionDefinitions[idx].relocCapacity = 16;
    sectionDefinitions[idx].relocs = (Relocation*)calloc(16, sizeof(Relocation));
    add_symbol(name, 0x00000000, symbolCount, SYM_SCTN, SYM_LOC, 1);
    return idx;
}
int get_or_create_section(const char *name) {
    int idx = find_section(name);
    if (idx != -1) return idx;

    return create_section(name);
}
void switch_section(const char *name) {
    printf("switched to section %s\n", name);
    currentSection = get_or_create_section(name);
}
void section_emit_byte(uint8_t byte) {
    if (currentSection < 0) {
        printf("ERROR: no active section\n");
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[currentSection];

    sec->data = (uint8_t*)realloc(sec->data, sec->length + 1);
    sec->data[sec->length] = byte;
    sec->length++;
}

uint8_t get_section_byte(int section, uint32_t address)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (address >= sec->length) {
        fprintf(stderr,
                "Byte read out of bounds (section=%d, addr=%u)\n",
                section, address);
        exit(1);
    }

    return sec->data[address];
}

void section_patch_byte(int section, uint32_t offset, uint8_t value)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (offset >= sec->length) {
        fprintf(stderr,
                "Byte patch out of bounds (section=%d, offset=%u)\n",
                section, offset);
        exit(1);
    }

    sec->data[offset] = value;
}


void section_emit_word(uint32_t value)
{
    SectionDefinition *sec = &sectionDefinitions[currentSection];
    if (!sec) return;

    sec->data = (uint8_t*)realloc(sec->data, sec->length + 4);

    // little-endian write
    sec->data[sec->length + 0] = (uint8_t)(value & 0xFF);
    sec->data[sec->length + 1] = (uint8_t)((value >> 8) & 0xFF);
    sec->data[sec->length + 2] = (uint8_t)((value >> 16) & 0xFF);
    sec->data[sec->length + 3] = (uint8_t)((value >> 24) & 0xFF);

    sec->length += 4;
}

uint32_t get_section_word(int section, uint32_t address)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (address + 3 >= sec->length) {
        fprintf(stderr,
                "Word read out of bounds (section=%d, addr=%u)\n",
                section, address);
        exit(1);
    }

    return ((uint32_t)sec->data[address + 0]) |
           ((uint32_t)sec->data[address + 1] << 8) |
           ((uint32_t)sec->data[address + 2] << 16) |
           ((uint32_t)sec->data[address + 3] << 24);
}

void section_patch_word(int section, uint32_t offset, uint32_t value)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (offset + 3 >= sec->length) {
        fprintf(stderr,
                "Word patch out of bounds (section=%d, offset=%u)\n",
                section, offset);
        exit(1);
    }

    /* little-endian */
    sec->data[offset + 0] = (uint8_t)(value & 0xFF);
    sec->data[offset + 1] = (uint8_t)((value >> 8) & 0xFF);
    sec->data[offset + 2] = (uint8_t)((value >> 16) & 0xFF);
    sec->data[offset + 3] = (uint8_t)((value >> 24) & 0xFF);
}

void print_sections_debug() {
    printf("\n================ SECTION DUMP ================\n");

    for (int i = 0; i < sectionCount; i++) {
        SectionDefinition *sec = &sectionDefinitions[i];

        printf("\n[%d] SECTION: %s\n", i, sec->name);
        printf("SIZE: %u bytes\n", sec->length);
        printf("---------------------------------------------\n");

        for (uint32_t j = 0; j < sec->length; j++) {

            // print offset
            if (j % 16 == 0)
                printf("%08x  ", j);

            // print byte in hex
            printf("%02x ", sec->data[j]);

            // ASCII column + newline handling
            if ((j + 1) % 16 == 0 || j + 1 == sec->length) {

                // padding if last line is incomplete
                int remaining = 15 - (j % 16);
                for (int k = 0; k < remaining; k++)
                    printf("   ");

                printf(" |");

                // print ASCII
                int start = (j / 16) * 16;
                int end = j + 1;

                for (int k = start; k < end; k++) {
                    char c = sec->data[k];
                    printf("%c", isprint(c) ? c : '.');
                }

                printf("|\n");
            }
        }

        printf("---------------------------------------------\n");
    }

    printf("=============================================\n\n");
}
/* =========================
   SYMBOL TABLE
   ========================= */

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


/* =========================
   RELOCATIONS
   ========================= */

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
    RelocationType type
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
}

static const char* relocation_type_to_string(RelocationType type)
{
    switch(type)
    {
        case ABS32:
            return "ABS32";

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

/* =========================
   ENCODING
   ========================= */

uint32_t form_arithmetic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c)
{
    return (0x5u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12);
}


uint32_t form_logic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c)
{
    return (0x6u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12);
}


uint32_t form_shift_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c)
{
    return (0x7u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12);
}

uint32_t form_load_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d)
{
    return (0x9u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12) |
           (d & 0xFFF);
}

uint32_t form_store_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d)
{
    return (0x8u << 28) |
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12) |
           (d & 0xFFF);
}

uint32_t form_pop_instruction(uint8_t gpr){
    // sp == r14
    return form_load_instruction(LOAD_GPR_FROM_MEM_POSTINC, gpr, 14, 0, 4);
}   
uint32_t form_pop_csr_instruction(uint8_t csr){
    // sp == r14
    return form_load_instruction(LOAD_CSR_FROM_MEM_POSTINC, csr, 14, 0, 4);
}   
uint32_t form_push_instruction(uint8_t gpr){
    // sp == r14
    return form_store_instruction(STORE_MEM_POSTINC, 14, 0, gpr, -4);
}   

uint32_t form_jump_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d)
{
    return (0x3u << 28) |              // opcode = JMP
           ((mode & 0xF) << 24) |
           ((a    & 0xF) << 20) |
           ((b    & 0xF) << 16) |
           ((c    & 0xF) << 12) |
           (d & 0xFFF);
}


/* =========================
   INIT
   ========================= */

int init_assembler() {
    locationCounter = 0;

    outputContent.data = NULL;
    outputContent.size = 0;
    outputContent.capacity = 0;

    return 0;
}

int write_output_file(const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        perror("fopen");
        return 1;
    }

    fwrite(outputContent.data, 1, outputContent.size, f);

    fclose(f);

    printf("Wrote %u bytes to %s\n",
           outputContent.size,
           filename);

    return 0;
}

int main(int argc, char **argv)
{
    const char *out = "out.bin";

    if (argc > 1)
        freopen(argv[1], "r", stdin);

    init_assembler();
    init_sections();
    init_symbol_table();

    printf("Assembler initialized\n");
    printf("locationCounter=%i\n", locationCounter);

    int code = yyparse();
    if(code == 1){
        printf("Parsing finished with code: %i\n", code);
        return 1;
    }
    printf("Parsing finished with code: %i\n", code);
    write_output_file(out);
    print_sections_debug();
    print_symbol_table();
    print_relocation_table();
    return 0;
}



