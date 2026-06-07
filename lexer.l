%option noyywrap

%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

"#".*                     ;   


".global"                 { return GLOBAL; }
".extern"                 { return EXTERN; }
".section"                { return SECTION; }
".word"                   { return WORD; }
".skip"                   { return SKIP; }
".ascii"                  { return ASCII; }
".equ"                    { return EQU; }
".end"                    { return END; }


"halt"      { return HALT; }
"int"       { return INT; }
"iret"      { return IRET; }
"call"      { return CALL; }
"ret"       { return RET; }
"jmp"       { return JMP; }
"beq"       { return BEQ; }
"bne"       { return BNE; }
"bgt"       { return BGT; }

"push"      { return PUSH; }
"pop"       { return POP; }

"xchg"      { return XCHG; }

"add"       { return ADD; }
"sub"       { return SUB; }
"mul"       { return MUL; }
"div"       { return DIV; }

"not"       { return NOT; }
"and"       { return AND; }
"or"        { return OR; }
"xor"       { return XOR; }

"shl"       { return SHL; }
"shr"       { return SHR; }

"ld"        { return LD; }
"st"        { return ST; }

"csrrd"     { return CSRRD; }
"csrwr"     { return CSRWR; }


","                       { return COMMA; }
":"                       { return COLON; }

"[" { return LBRACKET; };

"]" { return RBRACKET; };

"$" { return DOLLAR; };

"%" { return PERCENT; }

"+" { return PLUS; };

r([0-9]|1[0-5]) {
    yylval.num = atoi(yytext + 1);
    return GPR;
}

sp {
    yylval.num = 14;
    return GPR;
}

pc {
    yylval.num = 15;
    return GPR;
}

status {
  yylval.num = 0;
  return CSR;
}

handler {
  yylval.num = 1;
  return CSR;
}

cause {
  yylval.num = 2;
  return CSR;
}

\"([^\"\n])*\"          {
                          size_t len = yyleng - 2;  /* exclude opening and closing quotes */

                          yylval.str = (char*)malloc(len + 1);
                          memcpy(yylval.str, yytext + 1, len);
                          yylval.str[len] = '\0';

                          return STRING;
                      }

0[xX][0-9a-fA-F]+ {
    yylval.num = (int)strtol(yytext, NULL, 16);
    return NUMBER;
}

[0-9]+                    {
                            yylval.num = atoi(yytext);
                            return NUMBER;
                          }

[a-zA-Z_.][a-zA-Z0-9_]*    {
                            yylval.str = strdup(yytext);
                            return IDENT;
                          }

[ \t\r]+ ;



"\n" { return EOL; };



%%