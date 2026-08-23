#include "../include/output_writer.h"
#include <stdio.h>
#include <stdlib.h>

/* One initialized byte at a final (post-relocation) memory address. */
typedef struct {
    uint32_t addr;
    uint8_t  value;
} ByteEntry;

static int compare_byte_entries(const void *a, const void *b) {
    uint32_t aa = ((const ByteEntry*)a)->addr;
    uint32_t bb = ((const ByteEntry*)b)->addr;
    if (aa < bb) return -1;
    if (aa > bb) return 1;
    return 0;
}

int write_hex_output(const char *path, ObjectFile *objs, int objCount) {
    /* Every byte in every section has a defined initial value by the time
       we get here (.skip/.word/.ascii are all zero/value-initialized at
       assembly time), so we just collect every section byte at its
       final_base + local offset. */
    long totalBytes = 0;
    for (int i = 0; i < objCount; i++)
        for (int s = 0; s < objs[i].sectionCount; s++)
            totalBytes += (long)objs[i].sections[s].size;

    FILE *f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "linker: cannot open output file '%s'\n", path);
        return -1;
    }

    if (totalBytes == 0) {
        fclose(f);
        return 0;
    }

    ByteEntry *entries = (ByteEntry*)malloc((size_t)totalBytes * sizeof(ByteEntry));
    if (!entries) {
        fprintf(stderr, "linker: out of memory while writing hex output\n");
        fclose(f);
        return -1;
    }

    long n = 0;
    for (int i = 0; i < objCount; i++) {
        ObjectFile *obj = &objs[i];
        for (int s = 0; s < obj->sectionCount; s++) {
            ObjSection *sec = &obj->sections[s];
            for (uint32_t b = 0; b < sec->size; b++) {
                entries[n].addr  = sec->final_base + b;
                entries[n].value = sec->data[b];
                n++;
            }
        }
    }

    qsort(entries, (size_t)n, sizeof(ByteEntry), compare_byte_entries);

    long idx = 0;
    while (idx < n) {
        uint32_t row_base = (entries[idx].addr / 8) * 8;
        uint32_t row_end   = row_base + 8; /* exclusive */

        uint8_t row[8] = {0};

        while (idx < n && entries[idx].addr < row_end) {
            uint32_t off = entries[idx].addr - row_base;
            row[off] = entries[idx].value;
            idx++;
        }

        fprintf(f, "%04X:", row_base);
        for (int k = 0; k < 8; k++)
            fprintf(f, " %02X", row[k]);
        fprintf(f, "\n");
    }

    free(entries);
    fclose(f);
    return 0;
}