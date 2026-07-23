#include "../include/object_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t  e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;

    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;

    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;

    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} RawElfHeader;

typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;

    uint32_t sh_flags;
    uint32_t sh_addr;

    uint32_t sh_offset;
    uint32_t sh_size;

    uint32_t sh_link;
    uint32_t sh_info;

    uint32_t sh_addralign;
    uint32_t sh_entsize;
} RawSectionHeader;

typedef struct {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    uint8_t  st_info;
    uint8_t  st_other;
    uint16_t st_shndx;
} RawElfSymbol;

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    int32_t  r_addend;
} RawElfRela;

#define SHT_NULL     0
#define SHT_PROGBITS 1
#define SHT_SYMTAB   2
#define SHT_STRTAB   3
#define SHT_RELA     4

static char *dupstr(const char *s) {
    size_t n = strlen(s) + 1;
    char *r = malloc(n);
    memcpy(r, s, n);
    return r;
}

static ObjSection *find_section_by_raw_index(ObjectFile *obj, int raw_idx) {
    for (int i = 0; i < obj->sectionCount; i++)
        if (obj->sections[i].raw_shdr_index == raw_idx)
            return &obj->sections[i];
    return NULL;
}

static void add_reloc(ObjSection *sec, ObjReloc r) {
    sec->relocs = realloc(sec->relocs, (sec->relocCount + 1) * sizeof(ObjReloc));
    sec->relocs[sec->relocCount++] = r;
}

int read_object_file(const char *path, ObjectFile *out) {
    memset(out, 0, sizeof(*out));
    out->filename = dupstr(path);

    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "linker: cannot open '%s'\n", path);
        return -1;
    }

    RawElfHeader eh;
    if (fread(&eh, sizeof(eh), 1, f) != 1) {
        fprintf(stderr, "linker: '%s': failed to read ELF header\n", path);
        fclose(f);
        return -1;
    }

    if (eh.e_ident[0] != 0x7f || eh.e_ident[1] != 'E' ||
        eh.e_ident[2] != 'L' || eh.e_ident[3] != 'F') {
        fprintf(stderr, "linker: '%s': not a valid object file\n", path);
        fclose(f);
        return -1;
    }

    RawSectionHeader *shdrs = malloc(eh.e_shnum * sizeof(RawSectionHeader));
    fseek(f, eh.e_shoff, SEEK_SET);
    if (fread(shdrs, sizeof(RawSectionHeader), eh.e_shnum, f) != eh.e_shnum) {
        fprintf(stderr, "linker: '%s': failed to read section headers\n", path);
        free(shdrs);
        fclose(f);
        return -1;
    }

    RawSectionHeader *shstrtab_hdr = &shdrs[eh.e_shstrndx];
    char *shstrtab = malloc(shstrtab_hdr->sh_size);
    fseek(f, shstrtab_hdr->sh_offset, SEEK_SET);
    fread(shstrtab, 1, shstrtab_hdr->sh_size, f);

    int symtab_hdr_idx = -1;
    for (uint16_t i = 0; i < eh.e_shnum; i++) {
        if (shdrs[i].sh_type == SHT_SYMTAB) {
            symtab_hdr_idx = i;
            break;
        }
    }

    char *strtab = NULL;
    RawElfSymbol *raw_symbols = NULL;
    uint32_t raw_symbol_count = 0;

    if (symtab_hdr_idx >= 0) {
        RawSectionHeader *symtab_hdr = &shdrs[symtab_hdr_idx];
        RawSectionHeader *strtab_hdr = &shdrs[symtab_hdr->sh_link];

        strtab = malloc(strtab_hdr->sh_size);
        fseek(f, strtab_hdr->sh_offset, SEEK_SET);
        fread(strtab, 1, strtab_hdr->sh_size, f);

        raw_symbol_count = symtab_hdr->sh_size / sizeof(RawElfSymbol);
        raw_symbols = malloc(symtab_hdr->sh_size);
        fseek(f, symtab_hdr->sh_offset, SEEK_SET);
        fread(raw_symbols, sizeof(RawElfSymbol), raw_symbol_count, f);
    }

    for (uint16_t i = 0; i < eh.e_shnum; i++) {
        if (shdrs[i].sh_type != SHT_PROGBITS)
            continue;

        out->sections = realloc(out->sections, (out->sectionCount + 1) * sizeof(ObjSection));
        ObjSection *sec = &out->sections[out->sectionCount++];
        memset(sec, 0, sizeof(*sec));

        sec->name = dupstr(shstrtab + shdrs[i].sh_name);
        sec->size = shdrs[i].sh_size;
        sec->raw_shdr_index = i;
        sec->final_base = 0;

        if (sec->size > 0) {
            sec->data = malloc(sec->size);
            fseek(f, shdrs[i].sh_offset, SEEK_SET);
            fread(sec->data, 1, sec->size, f);
        }
    }

    out->symbolCount = raw_symbol_count;
    out->symbols = malloc(raw_symbol_count * sizeof(ObjSymbol));

    for (uint32_t i = 0; i < raw_symbol_count; i++) {
        RawElfSymbol *rs = &raw_symbols[i];
        ObjSymbol *s = &out->symbols[i];

        s->name = dupstr(strtab + rs->st_name);
        s->value = rs->st_value;
        s->size = rs->st_size;
        s->bind = (rs->st_info >> 4) & 0xF;
        s->type = rs->st_info & 0xF;
        s->shndx = rs->st_shndx;
        s->final_addr = 0;

        if (s->shndx == 0) {
            s->obj_section_idx = -1;
        } else {
            ObjSection *sec = find_section_by_raw_index(out, s->shndx);
            s->obj_section_idx = sec ? (int)(sec - out->sections) : -1;
        }
    }

    for (uint16_t i = 0; i < eh.e_shnum; i++) {
        if (shdrs[i].sh_type != SHT_RELA)
            continue;

        ObjSection *target = find_section_by_raw_index(out, shdrs[i].sh_info);
        if (!target) {
            fprintf(stderr, "linker: '%s': relocation section refers to unknown section\n", path);
            continue;
        }

        uint32_t count = shdrs[i].sh_size / sizeof(RawElfRela);
        RawElfRela *raw_relocs = malloc(shdrs[i].sh_size);
        fseek(f, shdrs[i].sh_offset, SEEK_SET);
        fread(raw_relocs, sizeof(RawElfRela), count, f);

        for (uint32_t j = 0; j < count; j++) {
            ObjReloc r;
            r.offset = raw_relocs[j].r_offset;
            r.symbolIndex = (int)(raw_relocs[j].r_info >> 8);
            r.type = (ObjRelocType)(raw_relocs[j].r_info & 0xFF);
            r.addend = raw_relocs[j].r_addend;
            add_reloc(target, r);
        }

        free(raw_relocs);
    }

    free(raw_symbols);
    free(strtab);
    free(shstrtab);
    free(shdrs);
    fclose(f);
    return 0;
}

void free_object_file(ObjectFile *obj) {
    for (int i = 0; i < obj->sectionCount; i++) {
        free(obj->sections[i].name);
        free(obj->sections[i].data);
        free(obj->sections[i].relocs);
    }
    free(obj->sections);

    for (int i = 0; i < obj->symbolCount; i++)
        free(obj->symbols[i].name);
    free(obj->symbols);

    free(obj->filename);
    memset(obj, 0, sizeof(*obj));
}

static const char *reloc_type_name(ObjRelocType t) {
    switch (t) {
        case OBJ_ABS32:   return "ABS32";
        case OBJ_JMP_LIT:  return "JMP_LIT";
        case OBJ_PCREL12:  return "PCREL12";
        default:            return "UNKNOWN";
    }
}

void print_object_file(const ObjectFile *obj) {
    printf("=== %s ===\n", obj->filename);

    printf("sections (%d):\n", obj->sectionCount);
    for (int i = 0; i < obj->sectionCount; i++) {
        ObjSection *s = &obj->sections[i];
        printf("  [%d] %-10s size=%u relocs=%d\n", s->raw_shdr_index, s->name, s->size, s->relocCount);
        for (int j = 0; j < s->relocCount; j++) {
            ObjReloc *r = &s->relocs[j];
            printf("      offset=0x%X sym=%d(%s) type=%s addend=%d\n",
                   r->offset, r->symbolIndex,
                   (r->symbolIndex >= 0 && r->symbolIndex < obj->symbolCount) ? obj->symbols[r->symbolIndex].name : "?",
                   reloc_type_name(r->type), r->addend);
        }
    }

    printf("symbols (%d):\n", obj->symbolCount);
    for (int i = 0; i < obj->symbolCount; i++) {
        ObjSymbol *s = &obj->symbols[i];
        printf("  [%d] %-16s value=0x%X bind=%s type=%s shndx=%d sec=%s\n",
               i, s->name, s->value,
               s->bind == 1 ? "GLOBAL" : "LOCAL",
               s->type == 1 ? "SECTION" : "NOTYPE",
               s->shndx,
               s->obj_section_idx >= 0 ? obj->sections[s->obj_section_idx].name : "UND");
    }
}