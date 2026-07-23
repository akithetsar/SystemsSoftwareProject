#include "../include/linker.h"
#include "../include/object_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_linker_config(LinkerConfig *cfg) {
    memset(cfg, 0, sizeof(*cfg));
}

static void add_place(LinkerConfig *cfg, const char *name, uint32_t addr) {
    if (cfg->placeCount == cfg->placeCapacity) {
        cfg->placeCapacity = cfg->placeCapacity ? cfg->placeCapacity * 2 : 4;
        cfg->places = (PlaceEntry*)realloc(cfg->places, cfg->placeCapacity * sizeof(PlaceEntry));
    }
    cfg->places[cfg->placeCount].name = strdup(name);
    cfg->places[cfg->placeCount].address = addr;
    cfg->placeCount++;
}

static void add_input_file(LinkerConfig *cfg, const char *path) {
    if (cfg->inputCount == cfg->inputCapacity) {
        cfg->inputCapacity = cfg->inputCapacity ? cfg->inputCapacity * 2 : 4;
        cfg->input_files = (char**)realloc(cfg->input_files, cfg->inputCapacity * sizeof(char*));
    }
    cfg->input_files[cfg->inputCount] = strdup(path);
    cfg->inputCount++;
}

static int parse_place_arg(const char *arg_after_eq, LinkerConfig *cfg) {
    const char *at = strchr(arg_after_eq, '@');
    if (!at) {
        fprintf(stderr, "linker: invalid -place argument '%s' (expected name@address)\n", arg_after_eq);
        return -1;
    }

    size_t name_len = (size_t)(at - arg_after_eq);
    char *name = (char*)malloc(name_len + 1);
    memcpy(name, arg_after_eq, name_len);
    name[name_len] = '\0';

    const char *addr_str = at + 1;
    char *endptr = NULL;
    unsigned long addr = strtoul(addr_str, &endptr, 0);

    if (*addr_str == '\0' || *endptr != '\0') {
        fprintf(stderr, "linker: invalid address in -place argument '%s'\n", arg_after_eq);
        free(name);
        return -1;
    }

    add_place(cfg, name, (uint32_t)addr);
    free(name);
    return 0;
}

int parse_args(int argc, char **argv, LinkerConfig *cfg) {
    init_linker_config(cfg);

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (strcmp(arg, "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "linker: -o requires an argument\n");
                return -1;
            }
            cfg->output_file = strdup(argv[++i]);

        } else if (strncmp(arg, "-place=", 7) == 0) {
            if (parse_place_arg(arg + 7, cfg) != 0)
                return -1;

        } else if (strcmp(arg, "-hex") == 0) {
            cfg->hex = 1;

        } else if (strcmp(arg, "-relocatable") == 0) {
            cfg->relocatable = 1;

        } else if (arg[0] == '-') {
            fprintf(stderr, "linker: unknown option '%s'\n", arg);
            return -1;

        } else {
            add_input_file(cfg, arg);
        }
    }

    if (cfg->inputCount == 0) {
        fprintf(stderr, "linker: no input files given\n");
        return -1;
    }

    if (!cfg->output_file) {
        fprintf(stderr, "linker: -o <output file> is required\n");
        return -1;
    }

    if (cfg->hex == cfg->relocatable) {
        fprintf(stderr, "linker: exactly one of -hex or -relocatable must be specified\n");
        return -1;
    }

    return 0;
}

void print_linker_config(const LinkerConfig *cfg) {
    printf("output_file = %s\n", cfg->output_file ? cfg->output_file : "(none)");
    printf("hex = %d, relocatable = %d\n", cfg->hex, cfg->relocatable);

    printf("places (%d):\n", cfg->placeCount);
    for (int i = 0; i < cfg->placeCount; i++)
        printf("  %s @ 0x%08X\n", cfg->places[i].name, cfg->places[i].address);

    printf("input files (%d):\n", cfg->inputCount);
    for (int i = 0; i < cfg->inputCount; i++)
        printf("  %s\n", cfg->input_files[i]);
}



int main(int argc, char **argv) {
    LinkerConfig cfg;

    if (parse_args(argc, argv, &cfg) != 0) {
        return 1;
    }

    print_linker_config(&cfg);

    if (argc < 2) { fprintf(stderr, "usage: %s <file.o>\n", argv[0]); return 1; }

    ObjectFile obj;
    if (read_object_file(argv[1], &obj) != 0) return 1;
    printf("testMain\n");
    print_object_file(&obj);
    free_object_file(&obj);
    return 0;

}