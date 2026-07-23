#include "../include/symbol_resolver.h"
#include "../include/symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int defined;
    uint32_t value;
    int objIndex;
} GlobalEntry;

static GlobalEntry *g_table = NULL;
static int g_count = 0;
static int g_capacity = 0;

static void reset_global_table(void) {
    for (int i = 0; i < g_count; i++) free(g_table[i].name);
    free(g_table);
    g_table = NULL;
    g_count = 0;
    g_capacity = 0;
}

static GlobalEntry* find_global(const char *name) {
    for (int i = 0; i < g_count; i++)
        if (strcmp(g_table[i].name, name) == 0) return &g_table[i];
    return NULL;
}

static GlobalEntry* get_or_create_global(const char *name) {
    GlobalEntry *e = find_global(name);
    if (e) return e;
    if (g_count == g_capacity) {
        g_capacity = g_capacity ? g_capacity * 2 : 16;
        g_table = (GlobalEntry*)realloc(g_table, g_capacity * sizeof(GlobalEntry));
    }
    e = &g_table[g_count++];
    e->name = strdup(name);
    e->defined = 0;
    e->value = 0;
    e->objIndex = -1;
    return e;
}

int resolve_symbols(ObjectFile *objs, int objCount) {
    int errorCount = 0;
    reset_global_table();

    /* Pass 1: compute final_addr for symbols defined in their own file,
       register GLOBAL definitions, catch multiple definitions. */
    for (int i = 0; i < objCount; i++) {
        ObjectFile *obj = &objs[i];

        for (int s = 0; s < obj->symbolCount; s++) {
            ObjSymbol *sym = &obj->symbols[s];

            if (!sym->name || sym->name[0] == '\0')
                continue; /* skip the ELF null symbol at index 0 */

            if (sym->obj_section_idx < 0)
                continue; /* undefined here -> handled in pass 2 */

            ObjSection *sec = &obj->sections[sym->obj_section_idx];
            sym->final_addr = sec->final_base + sym->value;

            if ((SymbolBind)sym->bind != SYM_GLOB)
                continue; /* local: only visible within its own file */

            GlobalEntry *e = get_or_create_global(sym->name);
            if (e->defined) {
                fprintf(stderr,
                    "linker: multiple definition of symbol '%s' (in '%s' and '%s')\n",
                    sym->name, objs[e->objIndex].filename, obj->filename);
                errorCount++;
                continue;
            }
            e->defined = 1;
            e->value = sym->final_addr;
            e->objIndex = i;
        }
    }

    /* Pass 2: resolve GLOBAL symbols left undefined in their own file. */
    for (int i = 0; i < objCount; i++) {
        ObjectFile *obj = &objs[i];

        for (int s = 0; s < obj->symbolCount; s++) {
            ObjSymbol *sym = &obj->symbols[s];

            if (!sym->name || sym->name[0] == '\0')
                continue;

            if (sym->obj_section_idx >= 0)
                continue; /* already resolved in pass 1 */

            if ((SymbolBind)sym->bind != SYM_GLOB) {
                /* A local symbol with no definition in its own file is
                   invalid input - shouldn't happen for a well-formed
                   object file. Flag it loudly instead of silently
                   leaving final_addr = 0. */
                fprintf(stderr,
                    "linker: undefined local symbol '%s' in '%s' "
                    "(local symbols must be defined in their own file)\n",
                    sym->name, obj->filename);
                errorCount++;
                continue;
            }

            GlobalEntry *e = find_global(sym->name);
            if (!e || !e->defined) {
                fprintf(stderr,
                    "linker: undefined reference to symbol '%s' (referenced from '%s')\n",
                    sym->name, obj->filename);
                errorCount++;
                continue;
            }

            sym->final_addr = e->value;
        }
    }

    reset_global_table();
    return errorCount ? -1 : 0;
}