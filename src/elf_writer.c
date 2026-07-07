#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
uint32_t shstrtab_add(const char *str)
{
    uint32_t offset = shstrtab_size;

    uint32_t len = strlen(str) + 1;

    shstrtab_data = (uint8_t*)realloc(shstrtab_data, shstrtab_size + len);

    memcpy(shstrtab_data + shstrtab_size, str, len);

    shstrtab_size += len;

    return offset;
}

uint32_t strtab_add(const char *str)
{
    uint32_t offset = strtab_size;

    uint32_t len = strlen(str) + 1;

    strtab_data = (uint8_t*)realloc(strtab_data, strtab_size + len);

    memcpy(strtab_data + strtab_size, str, len);

    strtab_size += len;

    return offset;
}

uint16_t get_elf_section_index(const char *section_name)
{
    int idx = find_section(section_name);
    if (idx < 0)
        return 0; // SHN_UNDEF

    return (uint16_t)(idx + 1); 
    // +1 because ELF section 0 is NULL
}

void build_symtab(void)
{
    symtab_size = symbolCount * sizeof(ElfSymbol);

    symtab_data = (ElfSymbol*)calloc(symbolCount, sizeof(ElfSymbol));

    for (int i = 0; i < symbolCount; i++)
    {
        Symbol *s = &symbolTable[i];
        ElfSymbol *e = &symtab_data[i];

        // name
        e->st_name = s->name_offset;

        // value
        e->st_value = s->value;

        // size (you currently don't compute it → OK for now)
        e->st_size = s->size;

        // bind + type
        e->st_info =
            ((s->bind & 0xF) << 4) |
            (s->type & 0xF);

        e->st_other = 0;

        // section index
        if (!s->defined)
        {
            e->st_shndx = 0; // SHN_UNDEF
        }
        else if (s->type == SYM_SCTN)
        {
            // section symbol → its own section
            e->st_shndx = get_elf_section_index(s->name);
        }
        else
        {
            // normal symbol → belongs to its section
            const char *sec_name =
                get_symbols_section_name(s->name);

            if (sec_name)
                e->st_shndx = get_elf_section_index(sec_name);
            else
                e->st_shndx = 0;
        }
    }
}

static uint32_t relocation_to_elf(RelocationType t)
{
    switch(t)
    {
        case ABS32:   return 1;
        case JMP_LIT: return 2;
        case PCREL12: return 3;
        default:      return 0;
    }
}
static uint32_t make_r_info(int sym, int type)
{
    return ((sym << 8) | (type & 0xff));
}
int write_elf_file(const char *filename)
{

    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    // =========================
    // 1. BUILD SHSTRTAB
    // =========================


    uint32_t shstrtab_name = shstrtab_add(".shstrtab");
    uint32_t strtab_name = shstrtab_add(".strtab");
    uint32_t symtab_name = shstrtab_add(".symtab");
    // =========================
    // 2. SETUP SECTION LAYOUT
    // =========================
int rela_section_count = 0;

for (int i = 0; i < sectionCount; i++)
{
    if (sectionDefinitions[i].relocCount > 0)
        rela_section_count++;
}
    int secCount = sectionCount;

    int strtab_index   = secCount + 1;
    int shstrtab_index = secCount + 2;
    int symtab_index = secCount + 3;
    int totalSections  = secCount + 4 + rela_section_count;
    int rela_start = secCount + 4;
    int rela_idx = rela_start;
    SectionHeader *shdrs =
        (SectionHeader*)calloc(totalSections, sizeof(SectionHeader));

    // =========================
    // 3. ELF HEADER
    // =========================

    ElfHeader eh;
    memset(&eh, 0, sizeof(eh));

    memset(eh.e_ident, 0, 16);

    eh.e_ident[0] = 0x7f;
    eh.e_ident[1] = 'E';
    eh.e_ident[2] = 'L';
    eh.e_ident[3] = 'F';

    eh.e_ident[4] = 1; // ELFCLASS32
    eh.e_ident[5] = 1; // little endian
    eh.e_ident[6] = 1; // current version

    eh.e_type    = 1;      // REL (object file)
    eh.e_machine = 0xAA;    // your ISA
    eh.e_version = 1;

    eh.e_phoff = 0;
    eh.e_phentsize = 0;
    eh.e_phnum = 0;

    eh.e_ehsize     = sizeof(ElfHeader);
    eh.e_shentsize  = sizeof(SectionHeader);
    eh.e_shnum      = totalSections;

    eh.e_shstrndx   = shstrtab_index;

    // =========================
    // 4. SECTION HEADERS (names only)
    // =========================

    // NULL section
    shdrs[0].sh_name = 0;
    shdrs[0].sh_type = 0;
int section_index_map[MAX_SECTIONS];
    // user sections
    for (int i = 0; i < secCount; i++)
    {
        SectionDefinition *sec = &sectionDefinitions[i];

        shdrs[i + 1].sh_name = sec->name_offset; // MUST come from shstrtab_add
        shdrs[i + 1].sh_type = 1; // PROGBITS
        shdrs[i + 1].sh_size = sec->length;
        shdrs[i + 1].sh_addralign = 4;
        section_index_map[i] = i + 1;
    }
build_symtab();
    // shstrtab section
    shdrs[shstrtab_index].sh_name = shstrtab_name;
    shdrs[shstrtab_index].sh_type = 3; // STRTAB
    shdrs[shstrtab_index].sh_addralign = 1;
    shdrs[shstrtab_index].sh_size = shstrtab_size;

    // strtab section
    shdrs[strtab_index].sh_name      = strtab_name;
    shdrs[strtab_index].sh_type      = 3;      // SHT_STRTAB
    shdrs[strtab_index].sh_addralign = 1;
    shdrs[strtab_index].sh_size      = strtab_size;


    shdrs[symtab_index].sh_name = symtab_name;
    shdrs[symtab_index].sh_type = 2; // SHT_SYMTAB
    shdrs[symtab_index].sh_flags = 0;
    shdrs[symtab_index].sh_addr = 0;

    shdrs[symtab_index].sh_offset = 0; // filled later
    shdrs[symtab_index].sh_size = symtab_size;

    shdrs[symtab_index].sh_link = strtab_index; // IMPORTANT
    int last_local = 0;

for (int i = 0; i < symbolCount; i++)
{
    if (symbolTable[i].bind == SYM_LOC)
    {
        last_local = i;
    }
}
    shdrs[symtab_index].sh_info = last_local + 1;

    shdrs[symtab_index].sh_addralign = 4;
    shdrs[symtab_index].sh_entsize = sizeof(ElfSymbol);
//relocs

for (int i = 0; i < sectionCount; i++)
{
    SectionDefinition *sec = &sectionDefinitions[i];

    if (sec->relocCount == 0)
        continue;

    char rela_name[128];
    sprintf(rela_name, ".rela%s", sec->name);

    uint32_t name_off = shstrtab_add(rela_name);

    shdrs[rela_idx].sh_name = name_off;
    shdrs[rela_idx].sh_type = 4; // SHT_RELA
    shdrs[rela_idx].sh_addralign = 4;
    shdrs[rela_idx].sh_entsize = sizeof(ElfRela);
    shdrs[rela_idx].sh_size = sec->relocCount * sizeof(ElfRela);

    // link to symtab
    shdrs[rela_idx].sh_link = symtab_index;

    // IMPORTANT: section index this reloc applies to
    shdrs[rela_idx].sh_info = section_index_map[i];
    rela_idx++;
}
    // =========================
    // 5. COMPUTE OFFSETS
    // =========================

    uint32_t offset =
        sizeof(ElfHeader) +
        totalSections * sizeof(SectionHeader);

    offset = (offset + 3) & ~3;

    // section data
    for (int i = 0; i < secCount; i++)
    {
        shdrs[i + 1].sh_offset = offset;

        offset += sectionDefinitions[i].length;
        offset = (offset + 3) & ~3;
    }

    // strtab
    shdrs[strtab_index].sh_offset = offset;

    offset += strtab_size;
    offset = (offset + 3) & ~3;

    // SYMTAB
    shdrs[symtab_index].sh_offset = offset;

    offset += symtab_size;
    offset = (offset + 3) & ~3;

for (int i = 0; i < totalSections; i++) {
    if (shdrs[i].sh_type != 4) continue; // SHT_RELA

    shdrs[i].sh_offset = offset;

    offset += shdrs[i].sh_size;
    offset = (offset + 3) & ~3;
}
    // shstrtab
    shdrs[shstrtab_index].sh_offset = offset;
    

    offset += shstrtab_size;
    offset = (offset + 3) & ~3;

    // =========================
    // 6. WRITE ELF HEADER
    // =========================

    eh.e_shoff = sizeof(ElfHeader);

    fseek(f, 0, SEEK_SET);
    fwrite(&eh, sizeof(eh), 1, f);

    // =========================
    // 7. WRITE SECTION HEADERS
    // =========================

    fwrite(shdrs, sizeof(SectionHeader), totalSections, f);

    // =========================
    // 8. WRITE SECTION DATA
    // =========================

    for (int i = 0; i < secCount; i++)
    {
        fwrite(sectionDefinitions[i].data,
               1,
               sectionDefinitions[i].length,
               f);

        while (ftell(f) % 4)
        {
            uint8_t pad = 0;
            fwrite(&pad, 1, 1, f);
        }
    }

    // =========================
    // 9. WRITE SHSTRTAB
    // =========================

    fwrite(strtab_data, 1, strtab_size, f);

    while (ftell(f) % 4)
    {
        uint8_t pad = 0;
        fwrite(&pad, 1, 1, f);
    }
    
    printf("symbolCount = %d\n", symbolCount);
    printf("symtab_size = %u\n", symtab_size);
    fwrite(symtab_data, sizeof(ElfSymbol), symbolCount, f);

printf("REL COUNT TOTAL:\n");
for (int i = 0; i < sectionCount; i++)
{
    printf("%s -> %d\n",
        sectionDefinitions[i].name,
        sectionDefinitions[i].relocCount);
}
for (int i = 0; i < sectionCount; i++)
{
    SectionDefinition *sec = &sectionDefinitions[i];

    if (sec->relocCount == 0)
        continue;

    for (int r = 0; r < sec->relocCount; r++)
    {
        Relocation *rel = &sec->relocs[r];

        ElfRela e;

        e.r_offset = rel->offset;
        e.r_addend = rel->addend;

        e.r_info = make_r_info(
            rel->symbolIndex,
            relocation_to_elf(rel->type)
        );

        fwrite(&e, sizeof(ElfRela), 1, f);
    }
}
    while (ftell(f) % 4)
    {
        uint8_t pad = 0;
        fwrite(&pad, 1, 1, f);
    }
    fwrite(shstrtab_data, 1, shstrtab_size, f);

    while (ftell(f) % 4)
    {
        uint8_t pad = 0;
        fwrite(&pad, 1, 1, f);
    }

    // =========================
    // 10. DONE
    // =========================

    fclose(f);
    free(shdrs);

    printf("ELF object written successfully\n");
    return 0;
}