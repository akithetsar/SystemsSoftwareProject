#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================
   GLOBAL STATE
   ========================= */

int locationCounter = 0;

OutputContent outputContent;

SectionDefinition **sectionDefinitions = NULL;
SectionDefinition *currentSection = NULL;

/* =========================
   OUTPUT BUFFER (BASIC API)
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
   INIT
   ========================= */

int init_assembler() {
    locationCounter = 0;

    outputContent.data = NULL;
    outputContent.size = 0;
    outputContent.capacity = 0;

    return 0;
}