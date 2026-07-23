#ifndef LINKER_H
#define LINKER_H

#include <stdint.h>

typedef struct {
    char *name;
    uint32_t address;
} PlaceEntry;

typedef struct {
    char *output_file;      /* -o <file>            */
    int hex;                 /* -hex flag             */
    int relocatable;         /* -relocatable flag     */

    PlaceEntry *places;      /* -place=name@addr, repeatable */
    int placeCount;
    int placeCapacity;

    char **input_files;      /* positional args */
    int inputCount;
    int inputCapacity;
} LinkerConfig;

void init_linker_config(LinkerConfig *cfg);
int parse_args(int argc, char **argv, LinkerConfig *cfg);
void print_linker_config(const LinkerConfig *cfg);

#endif