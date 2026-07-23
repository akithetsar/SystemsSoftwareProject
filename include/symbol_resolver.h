#ifndef SYMBOL_RESOLVER_H
#define SYMBOL_RESOLVER_H

#include "object_reader.h"

int resolve_symbols(ObjectFile *objs, int objCount);

#endif