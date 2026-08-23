#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "object_reader.h"

/*
 * Writes the -hex memory-initialization record: one line per 8-byte
 * aligned row that contains at least one initialized byte, in the
 * format:
 *
 *   0000: 00 01 02 03 04 05 06 07
 *
 * Must be called after resolve_symbols() and apply_relocations(),
 * since it reads the final, relocated bytes directly out of each
 * ObjSection.data using ObjSection.final_base as the load address.
 *
 * Returns 0 on success, -1 if the output file could not be opened.
 */
int write_hex_output(const char *path, ObjectFile *objs, int objCount);

#endif