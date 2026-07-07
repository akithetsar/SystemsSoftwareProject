#ifndef ELF_WRITER_H
#define ELF_WRITER_H


#include <stdint.h>




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

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    int32_t  r_addend;
} ElfRela;
int write_elf_file(const char *filename);
uint32_t shstrtab_add(const char *str);

extern uint8_t *shstrtab_data;
extern uint32_t shstrtab_size;
   
extern uint8_t *strtab_data;
extern uint32_t strtab_size;
extern uint32_t strtab_add(const char *str);

extern ElfSymbol *symtab_data;
extern uint32_t symtab_size;


#endif