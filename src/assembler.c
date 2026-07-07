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
uint32_t shstrtab_add(const char *str);

uint8_t *shstrtab_data = NULL;
uint32_t shstrtab_size = 0;
   
uint8_t *strtab_data = NULL;
uint32_t strtab_size = 0;
uint32_t strtab_add(const char *str);

ElfSymbol *symtab_data = NULL;
uint32_t symtab_size = 0;


int litSection = 0;



int add_symbol(
    const char *name,
    uint32_t value,
    int ndx,
    SymbolType type,
    SymbolBind bind,
    int defined
);

/* =========================
   SECTIONS
   ========================= */

/* =========================
   SYMBOL TABLE
   ========================= */




/* =========================
   RELOCATIONS
   ========================= */

/* =========================
   Literal Pool
   ========================= */

/* =========================
   ENCODING
   ========================= */


/* =========================
   ELF WRITING
   ========================= */



/* =========================
   INIT
   ========================= */




int main(int argc, char **argv)
{

    if (argc > 1)
        freopen(argv[1], "r", stdin);
    shstrtab_add(""); 
    strtab_add("");
    init_sections();
    init_symbol_table();
    init_literal_pool();    
    litSection = create_section(".lit");
    printf("Assembler initialized\n");

    int code = yyparse();
    if(code == 1){
        printf("Parsing finished with code: %i\n", code);
        return 1;
    }
    printf("Parsing finished with code: %i\n", code);
    print_sections_debug();
    print_symbol_table();
    print_relocation_table();
    write_elf_file("./program.o");
    return 0;
}



