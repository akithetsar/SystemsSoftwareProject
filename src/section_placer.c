#include "../include/section_placer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    uint32_t base;
    uint32_t size;
    int explicit_place;
    int seen;
} MergedBuild;

static MergedBuild *find_or_add(MergedBuild **list, int *count, int *capacity, const char *name) {
    for (int i = 0; i < *count; i++)
        if (strcmp((*list)[i].name, name) == 0)
            return &(*list)[i];

    if (*count == *capacity) {
        *capacity = *capacity ? *capacity * 2 : 8;
        *list = realloc(*list, (*capacity) * sizeof(MergedBuild));
    }

    MergedBuild *m = &(*list)[*count];
    memset(m, 0, sizeof(*m));
    m->name = strdup(name);
    (*count)++;
    return m;
}

static const PlaceEntry *find_place(PlaceEntry *places, int placeCount, const char *name) {
    for (int i = 0; i < placeCount; i++)
        if (strcmp(places[i].name, name) == 0)
            return &places[i];
    return NULL;
}

int place_sections(ObjectFile *objs, int objCount,
                    PlaceEntry *places, int placeCount,
                    MergedSection **out_merged, int *out_mergedCount) {

    MergedBuild *list = NULL;
    int count = 0, capacity = 0;

    for (int f = 0; f < objCount; f++) {
        for (int s = 0; s < objs[f].sectionCount; s++) {
            const char *name = objs[f].sections[s].name;
            MergedBuild *m = find_or_add(&list, &count, &capacity, name);
            m->size += objs[f].sections[s].size;
        }
    }

    uint32_t highwater = 0;

    for (int i = 0; i < count; i++) {
        const PlaceEntry *p = find_place(places, placeCount, list[i].name);
        if (!p)
            continue;

        list[i].base = p->address;
        list[i].explicit_place = 1;
        list[i].seen = 1;

        uint32_t end = list[i].base + list[i].size;
        if (end > highwater)
            highwater = end;
    }

    for (int i = 0; i < count; i++) {
        if (list[i].seen)
            continue;

        list[i].base = highwater;
        list[i].explicit_place = 0;
        list[i].seen = 1;

        highwater = list[i].base + list[i].size;
    }

    for (int i = 0; i < count; i++) {
        uint32_t running = list[i].base;

        for (int f = 0; f < objCount; f++) {
            for (int s = 0; s < objs[f].sectionCount; s++) {
                ObjSection *sec = &objs[f].sections[s];
                if (strcmp(sec->name, list[i].name) != 0)
                    continue;

                sec->final_base = running;
                running += sec->size;
            }
        }
    }

    int *order = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) order[i] = i;

    for (int i = 0; i < count; i++)
        for (int j = i + 1; j < count; j++)
            if (list[order[j]].base < list[order[i]].base) {
                int tmp = order[i]; order[i] = order[j]; order[j] = tmp;
            }

    int overlap_found = 0;
    for (int i = 0; i + 1 < count; i++) {
        MergedBuild *a = &list[order[i]];
        MergedBuild *b = &list[order[i + 1]];

        if (a->size == 0 || b->size == 0)
            continue;

        uint32_t a_end = a->base + a->size;
        if (a_end > b->base) {
            fprintf(stderr,
                "linker: section overlap: '%s' [0x%08X-0x%08X) overlaps '%s' [0x%08X-0x%08X)\n",
                a->name, a->base, a_end, b->name, b->base, b->base + b->size);
            overlap_found = 1;
        }
    }
    free(order);

    if (overlap_found) {
        for (int i = 0; i < count; i++) free(list[i].name);
        free(list);
        return -1;
    }

    MergedSection *merged = malloc(count * sizeof(MergedSection));
    for (int i = 0; i < count; i++) {
        merged[i].name = list[i].name;
        merged[i].base = list[i].base;
        merged[i].size = list[i].size;
        merged[i].explicit_place = list[i].explicit_place;
    }
    free(list);

    *out_merged = merged;
    *out_mergedCount = count;
    return 0;
}

void print_merged_sections(const MergedSection *merged, int count) {
    printf("merged sections (%d):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  %-10s base=0x%08X size=%u end=0x%08X %s\n",
               merged[i].name, merged[i].base, merged[i].size,
               merged[i].base + merged[i].size,
               merged[i].explicit_place ? "(explicit)" : "(auto)");
    }
}