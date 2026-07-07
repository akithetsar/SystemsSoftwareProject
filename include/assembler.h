#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "sections.h"
#include "literal_pool.h"
#include "instruction_encoding.h"
#include "elf_writer.h"

// Global helpers
uint32_t shstrtab_add(const char *str);

extern uint8_t *shstrtab_data;
extern uint32_t shstrtab_size;
   
extern uint8_t *strtab_data;
extern uint32_t strtab_size;
extern uint32_t strtab_add(const char *str);

extern ElfSymbol *symtab_data;
extern uint32_t symtab_size;


extern int litSection;

#endif