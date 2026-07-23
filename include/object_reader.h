#ifndef OBJECT_READER_H
#define OBJECT_READER_H

#include <stdint.h>

typedef enum {
    OBJ_ABS32   = 1,
    OBJ_JMP_LIT = 2,
    OBJ_PCREL12 = 3
} ObjRelocType;

typedef struct {
    uint32_t offset;
    int symbolIndex;
    ObjRelocType type;
    int32_t addend;
} ObjReloc;

typedef struct {
    char *name;
    uint8_t *data;
    uint32_t size;

    ObjReloc *relocs;
    int relocCount;

    int raw_shdr_index;
    uint32_t final_base;
} ObjSection;

typedef struct {
    char *name;
    uint32_t value;
    uint32_t size;
    uint8_t bind;
    uint8_t type;
    int shndx;
    int obj_section_idx;

    uint32_t final_addr;
} ObjSymbol;

typedef struct {
    char *filename;

    ObjSection *sections;
    int sectionCount;

    ObjSymbol *symbols;
    int symbolCount;
} ObjectFile;

int read_object_file(const char *path, ObjectFile *out);
void free_object_file(ObjectFile *obj);
void print_object_file(const ObjectFile *obj);

#endif