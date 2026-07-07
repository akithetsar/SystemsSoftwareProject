#ifndef SECTIONS_H
#define SECTIONS_H


#include <stdint.h>
#include "relocations.h"

#define MAX_SECTIONS 64

/*
Section definition structure

  - char *name           -        The name of the section
  - uint8_t data         -        The content/bytes that the section contains
  - Relocation* relocs   -        List of relocations that this section requires
  - int relocCount       -        Number of relocations
  - relocCapacity        -        Current capacity of the relocation list
  - name_offset          -        Offset within the elf file .shstrtab       

*/
typedef struct {
    char *name;
    uint8_t *data;
    uint32_t length;
    
    Relocation *relocs;
    int relocCount;
    int relocCapacity;

    uint32_t name_offset;

} SectionDefinition;


//List containing section definitions for this assembling runtime
extern SectionDefinition *sectionDefinitions;
extern int currentSection;
extern int sectionCount;

/**
 * Switches current section that is being processed to the section with the given name.
 *
 * @param name Section name.
 */
void switch_section(const char *name);


/** 
 * Emits a byte into the current sections data.
 *
 * @param byte Emmitted byte.
 */
void section_emit_byte(uint8_t byte);


/** 
 * Retrieves a byte in a specific section at the given address.
 *
 * @param section Section index.
 * @param address Byte address.
 * @return Byte data.
 */
uint8_t get_section_byte(int section, uint32_t address);


/** 
 * Emits a word(four bytes) into the current sections data.
 *
 * @param word Emmitted word.
 */
void section_emit_word(uint32_t value);


/** 
 * Retrieves a word in a specific section at the given address.
 *
 * @param section Section index.
 * @param address Byte address.
 * @return Word data.
 */
uint32_t get_section_word(int section, uint32_t address);


/** 
 * Retrieves the index of a section with a given name.
 *
 * @param name Section name.
 * @return -1 if not found, section index in list if found.
 */
int find_section(const char *name);



void init_sections();
int create_section(const char *name);
void print_sections_debug();
void section_patch_word(int section, uint32_t offset, uint32_t value);


#endif