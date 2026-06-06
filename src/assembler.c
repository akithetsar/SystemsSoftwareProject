#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../build/parser.tab.h"
/* =========================
   GLOBAL STATE
   ========================= */

int locationCounter = 0;

OutputContent outputContent;

#define MAX_SECTIONS 64

SectionDefinition *sectionDefinitions = NULL;
int sectionCount = 0;
int currentSection = -1;
/* =========================
   OUTPUT BUFFER
   ========================= */

static void ensure_capacity(uint32_t needed) {
    if (outputContent.capacity >= needed)
        return;

    uint32_t newCap = (outputContent.capacity == 0) ? 64 : outputContent.capacity;

    while (newCap < needed)
        newCap *= 2;

    outputContent.data = (uint8_t*)realloc(outputContent.data, newCap);
    outputContent.capacity = newCap;
}

void emit_byte(uint8_t byte) {
    ensure_capacity(outputContent.size + 1);
    outputContent.data[outputContent.size++] = byte;
}
/* =========================
   SECTIONS
   ========================= */

void init_sections() {
    sectionDefinitions = (SectionDefinition*)calloc(MAX_SECTIONS, sizeof(SectionDefinition));
    sectionCount = 0;
    currentSection = -1;
}
int find_section(const char *name) {
    for (int i = 0; i < sectionCount; i++) {
        if (strcmp(sectionDefinitions[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
int create_section(const char *name) {
    int idx = sectionCount++;

    sectionDefinitions[idx].name = strdup(name);
    sectionDefinitions[idx].data = NULL;
    sectionDefinitions[idx].length = 0;

    return idx;
}
int get_or_create_section(const char *name) {
    int idx = find_section(name);
    if (idx != -1) return idx;

    return create_section(name);
}
void switch_section(const char *name) {
    printf("switched to section %s\n", name);
    currentSection = get_or_create_section(name);
}
void section_emit_byte(uint8_t byte) {
    if (currentSection < 0) {
        printf("ERROR: no active section\n");
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[currentSection];

    sec->data = (uint8_t*)realloc(sec->data, sec->length + 1);
    sec->data[sec->length] = byte;
    sec->length++;
}


void section_emit_word(uint32_t value)
{
    SectionDefinition *sec = &sectionDefinitions[currentSection];
    if (!sec) return;

    sec->data = (uint8_t*)realloc(sec->data, sec->length + 4);

    // little-endian write
    sec->data[sec->length + 0] = (uint8_t)(value & 0xFF);
    sec->data[sec->length + 1] = (uint8_t)((value >> 8) & 0xFF);
    sec->data[sec->length + 2] = (uint8_t)((value >> 16) & 0xFF);
    sec->data[sec->length + 3] = (uint8_t)((value >> 24) & 0xFF);

    sec->length += 4;
}

#include <stdio.h>
#include <ctype.h>

void print_sections_debug() {
    printf("\n================ SECTION DUMP ================\n");

    for (int i = 0; i < sectionCount; i++) {
        SectionDefinition *sec = &sectionDefinitions[i];

        printf("\n[%d] SECTION: %s\n", i, sec->name);
        printf("SIZE: %u bytes\n", sec->length);
        printf("---------------------------------------------\n");

        for (uint32_t j = 0; j < sec->length; j++) {

            // print offset
            if (j % 16 == 0)
                printf("%08x  ", j);

            // print byte in hex
            printf("%02x ", sec->data[j]);

            // ASCII column + newline handling
            if ((j + 1) % 16 == 0 || j + 1 == sec->length) {

                // padding if last line is incomplete
                int remaining = 15 - (j % 16);
                for (int k = 0; k < remaining; k++)
                    printf("   ");

                printf(" |");

                // print ASCII
                int start = (j / 16) * 16;
                int end = j + 1;

                for (int k = start; k < end; k++) {
                    char c = sec->data[k];
                    printf("%c", isprint(c) ? c : '.');
                }

                printf("|\n");
            }
        }

        printf("---------------------------------------------\n");
    }

    printf("=============================================\n\n");
}

/* =========================
   INIT
   ========================= */

int init_assembler() {
    locationCounter = 0;

    outputContent.data = NULL;
    outputContent.size = 0;
    outputContent.capacity = 0;

    return 0;
}

int write_output_file(const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        perror("fopen");
        return 1;
    }

    fwrite(outputContent.data, 1, outputContent.size, f);

    fclose(f);

    printf("Wrote %u bytes to %s\n",
           outputContent.size,
           filename);

    return 0;
}

int main(int argc, char **argv)
{
    const char *out = "out.bin";

    if (argc > 1)
        freopen(argv[1], "r", stdin);

    init_assembler();
    init_sections();
    printf("Assembler initialized\n");
    printf("locationCounter=%i\n", locationCounter);

    yyparse();

    write_output_file(out);
    print_sections_debug();
    return 0;
}



