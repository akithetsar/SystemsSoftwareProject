#ifndef RELOCATOR_H
#define RELOCATOR_H

#include "object_reader.h"

/*
 * Applies every relocation in every section of every object file,
 * patching ObjSection.data in place. Must run after resolve_symbols()
 * has populated ObjSymbol.final_addr for every symbol, and after
 * place_sections() has populated ObjSection.final_base for every
 * section.
 *
 * Returns 0 on success. Returns -1 if any PCREL12/JMP_LIT displacement
 * doesn't fit in a signed 12-bit field, printing which relocation
 * failed and in which file/section.
 */
int apply_relocations(ObjectFile *objs, int objCount);

#endif