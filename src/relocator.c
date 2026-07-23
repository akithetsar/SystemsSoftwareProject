#include "../include/relocator.h"
  /* displacement_fits_12bit() */
#include <stdio.h>

int displacement_fits_12bits(
    int32_t d
)
{
    return d >= -2048 &&
           d <= 2047;
}
/* Reads/writes a little-endian 32-bit word directly in section data. */
static uint32_t read_word_le(uint8_t *data, uint32_t offset) {
    return (uint32_t)data[offset]
         | ((uint32_t)data[offset + 1] << 8)
         | ((uint32_t)data[offset + 2] << 16)
         | ((uint32_t)data[offset + 3] << 24);
}

static void write_word_le(uint8_t *data, uint32_t offset, uint32_t value) {
    data[offset + 0] = (uint8_t)(value & 0xFF);
    data[offset + 1] = (uint8_t)((value >> 8) & 0xFF);
    data[offset + 2] = (uint8_t)((value >> 16) & 0xFF);
    data[offset + 3] = (uint8_t)((value >> 24) & 0xFF);
}

/* Patches only the low 12 bits (the Disp field) of an existing
   instruction word, leaving OC/MOD/RegA/RegB/RegC untouched. */
static void patch_disp12(uint8_t *data, uint32_t offset, int32_t d) {
    uint32_t word = read_word_le(data, offset);
    word = (word & 0xFFFFF000u) | ((uint32_t)d & 0xFFFu);
    write_word_le(data, offset, word);
}

static int apply_one_reloc(ObjectFile *obj, ObjSection *sec, ObjReloc *r) {
    switch (r->type) {

        case OBJ_ABS32: {
            /* Full 32-bit absolute address, e.g. patching a .word
               placeholder. value = symbol's resolved address + addend. */
            uint32_t value = r->addend;
            if (r->symbolIndex >= 0) {
                value += obj->symbols[r->symbolIndex].final_addr;
            }
            write_word_le(sec->data, r->offset, value);
            return 0;
        }

        case OBJ_JMP_LIT: {
            /* d = litbase + addend - pc
               litbase = final_base of the section that owns the
                         referenced symbol (usually .lit), addend =
                         that symbol's offset within its own section.
               pc = address of the *next* instruction (this instr + 4),
                    since the emitted LOAD uses %pc(=r15) addressing and
                    pc has already advanced by execution time. */
            if (r->symbolIndex < 0) {
                fprintf(stderr,
                    "linker: JMP_LIT relocation with no symbol in '%s' section '%s' offset 0x%X\n",
                    obj->filename, sec->name, r->offset);
                return -1;
            }
            ObjSymbol *sym = &obj->symbols[r->symbolIndex];
            uint32_t litbase = obj->sections[sym->obj_section_idx].final_base;
            uint32_t pc = sec->final_base + r->offset + 4;
            int32_t d = (int32_t)(litbase + (uint32_t)r->addend) - (int32_t)pc;

            if (!displacement_fits_12bits(d)) {
                fprintf(stderr,
                    "linker: JMP_LIT displacement out of 12-bit range (%d) for symbol '%s' in '%s'\n",
                    d, sym->name, obj->filename);
                return -1;
            }
            patch_disp12(sec->data, r->offset, d);
            return 0;
        }

        case OBJ_PCREL12: {
            /* d = addend - (section_base + offset + 4)
               No symbol involved - addend already carries whatever
               target value the assembler computed. */
            uint32_t pc = sec->final_base + r->offset + 4;
            int32_t d = (int32_t)r->addend - (int32_t)pc;
            printf("offset:%X\n", r->offset);
            printf("addend:%X\n", r->addend);
            printf("pc:%X\n", pc);
            printf("d:%X\n", d);
            if (!displacement_fits_12bits(d)) {
                fprintf(stderr,
                    "linker: PCREL12 displacement out of 12-bit range (%d) in '%s' section '%s' offset 0x%X\n",
                    d, obj->filename, sec->name, r->offset);
                return -1;
            }
            patch_disp12(sec->data, r->offset, d);
            return 0;
        }

        default:
            fprintf(stderr, "linker: unknown relocation type %d in '%s'\n",
                    (int)r->type, obj->filename);
            return -1;
    }
}

int apply_relocations(ObjectFile *objs, int objCount) {
    int errorCount = 0;

    for (int i = 0; i < objCount; i++) {
        ObjectFile *obj = &objs[i];
        for (int s = 0; s < obj->sectionCount; s++) {
            ObjSection *sec = &obj->sections[s];
            for (int r = 0; r < sec->relocCount; r++) {
                if (apply_one_reloc(obj, sec, &sec->relocs[r]) != 0) {
                    errorCount++;
                }
            }
        }
    }

    return errorCount ? -1 : 0;
}