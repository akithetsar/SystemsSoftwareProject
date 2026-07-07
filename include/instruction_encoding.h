#ifndef INSTR_ENCODE_H
#define INSTR_ENCODE_H


#include <stdint.h>

typedef enum ArithModes{
    ARITH_ADD = 0,
    ARITH_SUB = 1,
    ARITH_MUL = 2,
    ARITH_DIV = 3
} ArithModes;

uint32_t form_arithmetic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c);

typedef enum LogicModes{
    LOGIC_NOT = 0,
    LOGIC_AND = 1,
    LOGIC_OR = 2,
    LOGIC_XOR = 3
} LogicModes;

uint32_t form_logic_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c);


typedef enum ShiftModes{
    SHIFT_LEFT = 0,
    SHIFT_RIGHT = 1,
} ShiftModes;

uint32_t form_shift_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c);

typedef enum LoadModes {
    LOAD_GPR_FROM_CSR            = 0,
    LOAD_GPR_FROM_GPR_PLUS_D     = 1,
    LOAD_GPR_FROM_MEM_INDEXED    = 2,
    LOAD_GPR_FROM_MEM_POSTINC    = 3,
    LOAD_CSR_FROM_GPR            = 4,
    LOAD_CSR_OR_D                = 5,
    LOAD_CSR_FROM_MEM_INDEXED    = 6,
    LOAD_CSR_FROM_MEM_POSTINC    = 7
} LoadModes;

uint32_t form_load_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d);

typedef enum StoreModes {
    STORE_MEM_INDEXED      = 0,  // mem32[gpr[A]+gpr[B]+D] <= gpr[C]
    STORE_MEM_POSTINC      = 1,  // gpr[A]+=D; mem32[gpr[A]] <= gpr[C]
    STORE_MEM_INDIRECT     = 2   // mem32[mem32[gpr[A]+gpr[B]+D]] <= gpr[C]
} StoreModes;

uint32_t form_store_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d);


uint32_t form_pop_instruction(uint8_t gpr);   
uint32_t form_pop_csr_instruction(uint8_t csr);     
uint32_t form_push_instruction(uint8_t gpr);  

typedef enum JumpModes {
    JMP_BASE           = 0, // pc <= gpr[A] + D
    JMP_EQ             = 1, // if (B == C)
    JMP_NE             = 2, // if (B != C)
    JMP_GT             = 3, // if (B > C signed)
    JMP_MEM_BASE       = 8, // pc <= mem32[gpr[A] + D]
    JMP_MEM_EQ         = 9,
    JMP_MEM_NE         = 10,
    JMP_MEM_GT         = 11
} JumpModes;

uint32_t form_jump_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint8_t c,
    uint16_t d);

void emit_symbol_jmp(char *sym_name, uint8_t b, uint8_t c, uint8_t mode_rel, uint8_t mode_mem);

typedef enum CallModes {
    CALL_REL           = 0, 
    CALL_MEM           = 1, 
    
} CallModes;


uint32_t form_call_instruction(
    uint8_t mode,
    uint8_t a,
    uint8_t b,
    uint16_t d);


void emit_symbol_call(char *sym_name);
void emit_large_literal_load(int32_t literal, uint8_t dst_reg);
void emit_symbol_address_load(char *sym_name, uint8_t dst_reg);

#endif