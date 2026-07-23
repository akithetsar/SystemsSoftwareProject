#ifndef SECTION_PLACER_H
#define SECTION_PLACER_H

#include <stdint.h>
#include "object_reader.h"
#include "linker.h"

typedef struct {
    char *name;
    uint32_t base;
    uint32_t size;
    int explicit_place;
} MergedSection;

int place_sections(ObjectFile *objs, int objCount,
                    PlaceEntry *places, int placeCount,
                    MergedSection **out_merged, int *out_mergedCount);

void print_merged_sections(const MergedSection *merged, int count);

#endif