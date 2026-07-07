#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

SectionDefinition *sectionDefinitions = NULL;
int sectionCount = 0;
int currentSection = -1;

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

    sectionDefinitions[idx].relocCount = 0;
    sectionDefinitions[idx].relocCapacity = 16;
    sectionDefinitions[idx].relocs = (Relocation*)calloc(16, sizeof(Relocation));
    sectionDefinitions[idx].name_offset =
            shstrtab_add(name);
            

    add_symbol(name, 0x00000000, symbolCount, SYM_SCTN, SYM_LOC, 1);
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

uint8_t get_section_byte(int section, uint32_t address)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (address >= sec->length) {
        fprintf(stderr,
                "Byte read out of bounds (section=%d, addr=%u)\n",
                section, address);
        exit(1);
    }

    return sec->data[address];
}

void section_patch_byte(int section, uint32_t offset, uint8_t value)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (offset >= sec->length) {
        fprintf(stderr,
                "Byte patch out of bounds (section=%d, offset=%u)\n",
                section, offset);
        exit(1);
    }

    sec->data[offset] = value;
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

uint32_t get_section_word(int section, uint32_t address)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (address + 3 >= sec->length) {
        fprintf(stderr,
                "Word read out of bounds (section=%d, addr=%u)\n",
                section, address);
        exit(1);
    }

    return ((uint32_t)sec->data[address + 0]) |
           ((uint32_t)sec->data[address + 1] << 8) |
           ((uint32_t)sec->data[address + 2] << 16) |
           ((uint32_t)sec->data[address + 3] << 24);
}

void section_patch_word(int section, uint32_t offset, uint32_t value)
{
    if (section < 0 || section >= sectionCount) {
        fprintf(stderr, "Invalid section index %d\n", section);
        exit(1);
    }

    SectionDefinition *sec = &sectionDefinitions[section];

    if (offset + 3 >= sec->length) {
        fprintf(stderr,
                "Word patch out of bounds (section=%d, offset=%u)\n",
                section, offset);
        exit(1);
    }

    /* little-endian */
    sec->data[offset + 0] = (uint8_t)(value & 0xFF);
    sec->data[offset + 1] = (uint8_t)((value >> 8) & 0xFF);
    sec->data[offset + 2] = (uint8_t)((value >> 16) & 0xFF);
    sec->data[offset + 3] = (uint8_t)((value >> 24) & 0xFF);
}

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