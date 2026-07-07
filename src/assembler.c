#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "../build/parser.tab.h"

/* =========================
   GLOBAL STATE
   ========================= */



int add_symbol(
    const char *name,
    uint32_t value,
    int ndx,
    SymbolType type,
    SymbolBind bind,
    int defined
);

int main(int argc, char **argv)
{
    const char *elf_out = "program.o";
    const char *input_file = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                elf_out = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o output.o] input.s\n", argv[0]);
                return 1;
        }
    }

    if (optind < argc)
        input_file = argv[optind];

    if (input_file)
        freopen(input_file, "r", stdin);

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
    write_elf_file(elf_out);
    return 0;
}



