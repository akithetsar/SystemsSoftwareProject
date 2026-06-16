/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 8 "parser.y"

    #include "item.h"

#line 52 "build/parser.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    GLOBAL = 258,
    EXTERN = 259,
    SECTION = 260,
    WORD = 261,
    SKIP = 262,
    ASCII = 263,
    EQU = 264,
    END = 265,
    HALT = 266,
    INT = 267,
    IRET = 268,
    CALL = 269,
    RET = 270,
    JMP = 271,
    BEQ = 272,
    BNE = 273,
    BGT = 274,
    PUSH = 275,
    POP = 276,
    XCHG = 277,
    ADD = 278,
    SUB = 279,
    MUL = 280,
    DIV = 281,
    NOT = 282,
    AND = 283,
    OR = 284,
    XOR = 285,
    SHL = 286,
    SHR = 287,
    LD = 288,
    ST = 289,
    CSRRD = 290,
    CSRWR = 291,
    COMMA = 292,
    COLON = 293,
    LBRACKET = 294,
    RBRACKET = 295,
    PLUS = 296,
    DOLLAR = 297,
    EOL = 298,
    PERCENT = 299,
    NUMBER = 300,
    IDENT = 301,
    STRING = 302,
    GPR = 303,
    CSR = 304
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 11 "parser.y"

    int num;
    char* str;
    Item item;

#line 119 "build/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */
