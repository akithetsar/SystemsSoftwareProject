%{
#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
int yylex();
void yyerror(const char *s);

%}

%union {
    int num;
    char* str;
}


%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END
%token HALT INT IRET CALL RET JMP BEQ BNE BGT
%token PUSH POP XCHG ADD SUB MUL DIV NOT
%token AND OR XOR SHL SHR LD ST
%token CSRRD CSRWR
%token GPR CSR
%token COMMA COLON LBRACKET RBRACKET PLUS
%token DOLLAR EOL PERCENT
%token <num> NUMBER
%token <str> IDENT STRING

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
        free($1);
    }
; 
directive:
        GLOBAL symbol_list
            {printf(".global parsed\n");}
        |
        EXTERN symbol_list
            {printf(".extern parsed\n");}
        | 
        SECTION IDENT
            {printf(".section parsed\n");
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
        {printf("parsed halt\n");
            
        }
        |
        INT
        {printf("parsed int\n");
            
        }
        |
        IRET
        {printf("parsed iret\n");
            
        }
        |
        CALL item
        {printf("parsed call\n");}
        |
        RET
        {printf("parsed ret\n");}
        |
        JMP item
        {printf("parsed jmp\n");}
        |
        BEQ PERCENT GPR COMMA PERCENT GPR COMMA item
        {printf("parsed beq\n");}
        |
        BNE PERCENT GPR COMMA PERCENT GPR COMMA item
        {printf("parsed bne\n");}
        |
        BGT PERCENT GPR COMMA PERCENT GPR COMMA item
        {printf("parsed bgt\n");}
        |
        PUSH PERCENT GPR
        {printf("parsed push\n");}
        |
        POP PERCENT GPR
        {printf("parsed pop\n");}
        |
        XCHG PERCENT GPR COMMA PERCENT GPR
        {printf("parsed xchg\n");}
        |
        ADD PERCENT GPR COMMA PERCENT GPR
        {printf("parsed add\n");}
        |
        SUB PERCENT GPR COMMA PERCENT GPR
        {printf("parsed sub\n");}
        |
        MUL PERCENT GPR COMMA PERCENT GPR
        {printf("parsed mul\n");}
        |
        DIV PERCENT GPR COMMA PERCENT GPR
        {printf("parsed div\n");}
        |
        NOT PERCENT GPR
        {printf("parsed not\n");}
        |
        AND PERCENT GPR COMMA PERCENT GPR
        {printf("parsed and\n");}
        |
        OR PERCENT GPR COMMA PERCENT GPR
        {printf("parsed or\n");}
        |
        XOR PERCENT GPR COMMA PERCENT GPR
        {printf("parsed xor\n");}
        |
        SHL PERCENT GPR COMMA PERCENT GPR
        {printf("parsed shl\n");}
        |
        SHR PERCENT GPR COMMA PERCENT GPR
        {printf("parsed shr\n");}
        |
        LD operand COMMA PERCENT GPR
        {printf("parsed ld\n");}
        |
        ST PERCENT GPR COMMA operand
        {printf("parsed st\n");}
        |
        CSRRD PERCENT CSR COMMA PERCENT GPR
        {printf("parsed csrrd\n");}
        |
        CSRWR PERCENT GPR COMMA PERCENT CSR
        {printf("parsed csrwr\n");}

    ;


symbol_list:
        IDENT | symbol_list COMMA IDENT
    ;

item_word:
        IDENT | NUMBER {section_emit_word($1);}
    ;

item:
    IDENT | NUMBER
;

init_list_word:
        item_word | init_list_word COMMA item_word
    ;



operand:
        DOLLAR NUMBER
        |
        DOLLAR IDENT
        |
        PERCENT GPR
        |
        NUMBER
        |
        IDENT
        |
        PERCENT CSR
        |
        LBRACKET PERCENT GPR RBRACKET
        |
        LBRACKET PERCENT CSR RBRACKET
        |
        LBRACKET PERCENT GPR PLUS NUMBER RBRACKET
        |
        LBRACKET PERCENT GPR PLUS IDENT RBRACKET
        |
        LBRACKET PERCENT CSR PLUS NUMBER RBRACKET
        |
        LBRACKET PERCENT CSR PLUS IDENT RBRACKET
    ;

%%

void yyerror(const char *s)
{
    printf("Error: %s\n", s);
}

