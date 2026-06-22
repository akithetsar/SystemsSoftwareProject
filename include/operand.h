// item.h
#ifndef OPERAND_H
#define OPERAND_H

#include <stdint.h>
typedef enum OPERAND_KIND{
    OPERAND_LITERAL_VALUE = 0,
    OPERAND_SYMBOL_VALUE = 1,
    OPERAND_LITERAL_ADDR = 2,
    OPERAND_SYMBOL_ADDR = 3,
    OPERAND_REG_VALUE = 4,
    OPERAND_REG_ADDR = 5,
    OPERAND_REG_ADD_LITERAL = 6,
    OPERAND_REG_ADD_SYMBOL = 7,
    OPERAND_CSR_ADD_LITERAL = 8,
    OPERAND_CSR_ADD_SYMBOL = 11,


} OPERAND_KIND;

typedef struct {
    OPERAND_KIND kind;
    uint32_t literal;
    int reg;
    char* sym;
} Operand;

#endif