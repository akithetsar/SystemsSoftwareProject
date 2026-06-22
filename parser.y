%{
#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
int yylex();
void yyerror(const char *s);
%}
%code requires {
    #include "item.h"
    #include "operand.h"
}
%union {
    int num;
    char* str;
    Item item;
    Operand opr;
}


%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END
%token HALT INT IRET CALL RET JMP BEQ BNE BGT
%token PUSH POP XCHG ADD SUB MUL DIV NOT
%token AND OR XOR SHL SHR LD ST
%token CSRRD CSRWR
%token COMMA COLON LBRACKET RBRACKET PLUS
%token DOLLAR EOL PERCENT
%token <num> NUMBER
%token <str> IDENT STRING
%token <num> GPR
%token <num> CSR

%type <item> item
%type <opr> operand
%%

program:
        lines
    ;

lines:
        /* empty */
        | lines source_line
    ;

source_line:
      EOL                    /* empty line */
    | line EOL
;

line:
          directive
    | instruction
    | label
    | label directive
    | label instruction

    ;

label:
    IDENT COLON
    {
        printf("label: %s\n", $1);
        SectionDefinition *sec = &sectionDefinitions[currentSection];
        int ndx = find_symbol(sec->name);
        Symbol* sym = get_symbol($1);
        if(sym == NULL){
            add_symbol($1, sec->length, ndx, SYM_NOTYP, SYM_LOC, 1);
        }
        else{
            if(sym->defined == 1){
                printf("Parsing Error: Defined two same symbols with the name: %s\n", $1);
                YYABORT;

            }
            else{
                sym->value = sec->length;
                sym->ndx = find_symbol(sectionDefinitions[currentSection].name);
                sym->defined = 1;
                backpatch(sym);
            }
        }
        

        free($1);
    }
; 
directive:
        GLOBAL symbol_list_glob
            {printf(".global parsed\n");}
        |
        EXTERN symbol_list_extern
            {printf(".extern parsed\n");}
        | 
        SECTION IDENT
            {printf(".section parsed: %s\n", $2);
                switch_section($2);            
            }
        |
        WORD init_list_word
            {
                printf(".word parsed\n");

            }
        |
        SKIP NUMBER
            {
                printf(".skip parsed\n");
                int n = $2;
                int i;
                for(i = 0; i < n; ++i){
                    section_emit_byte(0x00);
                }
            }
        |
        ASCII STRING
            {
                char* c = $2;
                printf(".ascii parsed: %s\n", c);
                while(*c != '\0'){
                    section_emit_byte(*c);
                    c++;
                }
                section_emit_byte(*c);
                
            }

        |
        END
            {
                printf(".end parsed\n");
                YYACCEPT;
            }
    ;

instruction:
        HALT
        {
            printf("parsed halt\n");
            section_emit_word(0x00000000);
        }
        |
        INT
        {
            printf("parsed int\n");
            section_emit_word(0x10000000);
        }
        |
        IRET
        {
            printf("parsed iret\n");
            uint32_t instruction = form_pop_instruction(15); // pop pc
            section_emit_word(instruction);
            instruction = form_pop_csr_instruction(0); // pop status
            section_emit_word(instruction);
        }
        |
        CALL item
        {
            printf("parsed call\n");
            uint32_t instruction = form_push_instruction(15);
            section_emit_word(instruction);
            if($2.kind == ITEM_SYM)
            {
                emit_symbol_call($2.sym);
            }
            else if($2.kind == ITEM_LITERAL)
            {
                int32_t d = $2.value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_call_instruction(
                            CALL_REL,
                            0, 0,
                            (uint16_t)d
                        );

                    section_emit_word(instr);
                    printf("Instruction jmp: %08X \n", instr);
                }
                else
                {
                    printf("Literal out of range for PC-relative JMP\n");
                }
            }
        }
        |
        RET
        {
            printf("parsed ret\n");
            uint32_t instruction = form_pop_instruction(15);
            section_emit_word(instruction);
        }
        |
        JMP item
        {
            printf("parsed jmp\n");
            if($2.kind == ITEM_SYM)
            {
                emit_symbol_jmp($2.sym, 0, 0, JMP_BASE, JMP_MEM_BASE);
            }
            else if($2.kind == ITEM_LITERAL)
            {
                int32_t d = $2.value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_BASE,
                            0, 0, 0,
                            (uint16_t)d
                        );

                    section_emit_word(instr);
                    printf("Instruction jmp: %08X \n", instr);
                }
                else
                {
                    printf("Literal out of range for PC-relative JMP\n");
                }
            }
            // if same section
                // if d elm [-2048, 2047] -> pc relative
                // else -> trough literal pool
            // else if different sections -> trough literal pool
        }
            
            
        
        |
        BEQ PERCENT GPR COMMA PERCENT GPR COMMA item
        {
            printf("parsed beq\n");
            if($8.kind == ITEM_SYM)
            {
                emit_symbol_jmp($8.sym, $3, $6, JMP_EQ, JMP_MEM_EQ);
            }
            else if($8.kind == ITEM_LITERAL)
            {
                int32_t d = $8.value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_EQ,
                            0, $3, $6,
                            (uint16_t)d
                        );

                    section_emit_word(instr);
                    printf("Instruction jmp: %08X \n", instr);
                }
                else
                {
                    printf("Literal out of range for PC-relative JMP\n");
                }
            }
        }
        |
        BNE PERCENT GPR COMMA PERCENT GPR COMMA item
        {
            printf("parsed bne\n");
            if($8.kind == ITEM_SYM)
            {
                emit_symbol_jmp($8.sym, $3, $6, JMP_NE, JMP_MEM_NE);
            }
            else if($8.kind == ITEM_LITERAL)
            {
                int32_t d = $8.value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_NE,
                            0, $3, $6,
                            (uint16_t)d
                        );

                    section_emit_word(instr);
                    printf("Instruction jmp: %08X \n", instr);
                }
                else
                {
                    printf("Literal out of range for PC-relative JMP\n");
                }
            }
        
        }
        |
        BGT PERCENT GPR COMMA PERCENT GPR COMMA item
        {
            printf("parsed bgt\n");
            if($8.kind == ITEM_SYM)
            {
                emit_symbol_jmp($8.sym, $3, $6, JMP_GT, JMP_MEM_GT);
            }
            else if($8.kind == ITEM_LITERAL)
            {
                int32_t d = $8.value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_GT,
                            0, $3, $6,
                            (uint16_t)d
                        );

                    section_emit_word(instr);
                    printf("Instruction jmp: %08X \n", instr);
                }
                else
                {
                    printf("Literal out of range for PC-relative JMP\n");
                }
            }
        }
        |
        PUSH PERCENT GPR
        {
            printf("parsed push\n");
            uint32_t instruction = form_push_instruction($3);
            section_emit_word(instruction);
        }
        |
        POP PERCENT GPR
        {
            printf("parsed pop\n");
            uint32_t instruction = form_pop_instruction($3);
            section_emit_word(instruction);
        }
        |
        XCHG PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed xchg\n");
            section_emit_word(
                (0x4u << 28) |
                (($6 & 0xF) << 16) |
                (($3 & 0xF) << 12)
            );
        }
        |
        ADD PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed add\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_ADD, $6, $3, $6);
            section_emit_word(instruction);
        }
        |
        SUB PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed sub\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_SUB, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        MUL PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed mul\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_MUL, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        DIV PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed div\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_DIV, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        NOT PERCENT GPR
        {
            printf("parsed not\n");
            uint32_t instruction = form_logic_instruction(LOGIC_NOT, $3, $3, 0);
            section_emit_word(instruction);
        }
        |
        AND PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed and\n");
            uint32_t instruction = form_logic_instruction(LOGIC_AND, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        OR PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed or\n");
            uint32_t instruction = form_logic_instruction(LOGIC_OR, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        XOR PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed xor\n");
            uint32_t instruction = form_logic_instruction(LOGIC_XOR, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        SHL PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed shl\n");
            uint32_t instruction = form_shift_instruction(SHIFT_LEFT, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        SHR PERCENT GPR COMMA PERCENT GPR
        {
            printf("parsed shr\n");
            uint32_t instruction = form_shift_instruction(SHIFT_RIGHT, $6, $6, $3);
            section_emit_word(instruction);
        }
        |
        LD operand COMMA PERCENT GPR
        {
            printf("parsed ld\n");
            
            if($2.kind == OPERAND_REG_VALUE){
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_GPR_PLUS_D,
                    $5,
                    $2.reg,
                    0,
                    0);
                section_emit_word(instruction);
            }
            else if($2.kind == OPERAND_REG_ADDR){
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_MEM_INDEXED,
                    $5,
                    $2.reg,
                    0,
                    0);
                section_emit_word(instruction);
            }
            else if($2.kind == OPERAND_REG_ADD_LITERAL){
                int32_t d = $2.literal;
                if (d < -2048 || d > 2047) {
                    printf("Literal doesn't fit within 12 bits\n");
                    exit(1);
                }
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_MEM_INDEXED,
                    $5,
                    $2.reg,
                    0,
                    d);
                section_emit_word(instruction);
            }
            else if($2.kind == OPERAND_REG_ADD_SYMBOL){
                Symbol* sym = get_symbol($2.sym);
                if(sym == NULL){
                    printf("Symbol not defined for ld instruction\n");
                    exit(1);
                }
                if(sym->ndx == -1){
                    printf("Symbol value not defined for ld instruction\n");
                    exit(1);
                }
                int32_t d = sym->value;
                if (d < -2048 || d > 2047) {
                    printf("Symbl value doesn't fit within 12 bits\n");
                    exit(1);
                }
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_MEM_INDEXED,
                    $5,
                    $2.reg,
                    0,
                    d);
                section_emit_word(instruction);
            }
            
        }
        |
        ST PERCENT GPR COMMA operand
        {printf("parsed st\n");}
        |
        CSRRD PERCENT CSR COMMA PERCENT GPR
        {
            printf("parsed csrrd\n");
            uint32_t instruction = form_load_instruction(LOAD_GPR_FROM_CSR, $6, $3, 0, 0);
            section_emit_word(instruction);
        }
        |
        CSRWR PERCENT GPR COMMA PERCENT CSR
        {
            printf("parsed csrwr\n");
            uint32_t instruction = form_load_instruction(LOAD_CSR_FROM_GPR, $6, $3, 0, 0);
            section_emit_word(instruction);
        }
    ;

/* Redosled:
- `%reg` done 
- `[%reg]` done
- `[%reg + literal]` done
- `[%reg + symbol]` done
- `$literal`
- `$symbol`
- `symbol`
- `literal` */
symbol_list:
        IDENT
        | symbol_list COMMA IDENT
    ;

symbol_list_extern:
        item_extern
        | symbol_list_extern COMMA item_extern
    ;

item_extern:
    IDENT
        {
            Symbol *sym = get_symbol($1);
            if(sym != NULL){
                //Symbol in table
                printf("Turn to extern symbol: %s\n", $1);
                sym->bind = SYM_GLOB; 
            }
            else{
                //Symbol not in table
                add_symbol($1, 0xFFFFFFFF, -1, SYM_NOTYP, SYM_GLOB, 0);
            }
        }


symbol_list_glob:
        item_glob
        | symbol_list_glob COMMA item_glob
    ;
item_glob:
    IDENT
        {
            Symbol *sym = get_symbol($1);
            if(sym != NULL){
                //Symbol in table
                printf("Turn global symbol: %s\n", $1);
                sym->bind = SYM_GLOB; 
            }
            else{
                //Symbol not in table
                add_symbol($1, 0xFFFFFFFF, -1, SYM_NOTYP, SYM_GLOB, 0);
            }
        }

item_word:
        IDENT 
            {
                emit_symbol_word($1);
            } 
        | NUMBER {section_emit_word($1);}
    ;

item:
    IDENT
    {
       $$.kind = ITEM_SYM;
       $$.sym = $1;
    }
    | 
    NUMBER
    {
        $$.kind = ITEM_LITERAL;
        $$.value = $1;
    }
;

init_list_word:
        item_word | init_list_word COMMA item_word
    ;



operand:
        DOLLAR NUMBER
        {
            $$.kind = OPERAND_LITERAL_VALUE;
            $$.literal = $2;
        }
        |
        DOLLAR IDENT
        {
            $$.kind = OPERAND_SYMBOL_VALUE;
            $$.sym = $2;
        }
        |
        PERCENT GPR
        {
            $$.kind = OPERAND_REG_VALUE;
            $$.reg = $2;
        }
        |
        NUMBER
        {
            $$.kind = OPERAND_LITERAL_ADDR;
            $$.literal = $1;
        }
        |
        IDENT
        {
            $$.kind = OPERAND_SYMBOL_ADDR;
            $$.sym = $1;
        }
        |
        LBRACKET PERCENT GPR RBRACKET
        {
            $$.kind = OPERAND_REG_ADDR;
            $$.reg = $3;
        }
        |
        LBRACKET PERCENT CSR RBRACKET
        {
            $$.kind = OPERAND_REG_ADDR;
            $$.reg = $3;
        }
        |
        LBRACKET PERCENT GPR PLUS NUMBER RBRACKET
        {
            $$.kind = OPERAND_REG_ADD_LITERAL;
            $$.reg = $3;
            $$.literal = $5;
        }
        |
        LBRACKET PERCENT GPR PLUS IDENT RBRACKET
        {
            $$.kind = OPERAND_REG_ADD_SYMBOL;
            $$.reg = $3;
            $$.sym = $5;
        }
        |
        LBRACKET PERCENT CSR PLUS NUMBER RBRACKET
        {
            $$.kind = OPERAND_CSR_ADD_LITERAL;
            $$.reg = $3;
            $$.literal = $5;
        }
        |
        LBRACKET PERCENT CSR PLUS IDENT RBRACKET
        {
            $$.kind = OPERAND_CSR_ADD_SYMBOL;
            $$.reg = $3;
            $$.sym = $5;
        }
    ;

%%

void yyerror(const char *s)
{
    printf("Error: %s\n", s);
}

