/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
int yylex();
void yyerror(const char *s);

#line 78 "build/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    #include "operand.h"

#line 126 "build/parser.tab.c"

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
#line 12 "parser.y"

    int num;
    char* str;
    Item item;
    Operand opr;

#line 194 "build/parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   216

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  189

#define YYUNDEFTOK  2
#define YYMAXUTOK   304


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    37,    37,    40,    42,    46,    47,    51,    52,    53,
      54,    55,    60,    88,    91,    94,    99,   105,   115,   128,
     136,   142,   148,   157,   189,   196,   233,   263,   294,   324,
     331,   338,   348,   355,   362,   369,   376,   383,   390,   397,
     404,   411,   418,   572,   623,   630,   653,   654,   658,   674,
     675,   678,   693,   697,   701,   707,   715,   715,   721,   727,
     733,   739,   745,   751,   757,   763,   770,   777,   784
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "GLOBAL", "EXTERN", "SECTION", "WORD",
  "SKIP", "ASCII", "EQU", "END", "HALT", "INT", "IRET", "CALL", "RET",
  "JMP", "BEQ", "BNE", "BGT", "PUSH", "POP", "XCHG", "ADD", "SUB", "MUL",
  "DIV", "NOT", "AND", "OR", "XOR", "SHL", "SHR", "LD", "ST", "CSRRD",
  "CSRWR", "COMMA", "COLON", "LBRACKET", "RBRACKET", "PLUS", "DOLLAR",
  "EOL", "PERCENT", "NUMBER", "IDENT", "STRING", "GPR", "CSR", "$accept",
  "program", "lines", "source_line", "line", "label", "directive",
  "instruction", "symbol_list_extern", "item_extern", "symbol_list_glob",
  "item_glob", "item_word", "item", "init_list_word", "operand", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

#define YYPACT_NINF (-17)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -17,     7,    -2,   -17,     5,    37,    48,   -10,    50,    49,
     -17,   -17,   -17,   -17,    -8,   -17,    -8,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    40,    69,    70,    71,   -17,    78,   -17,
      74,    42,   -17,   -17,   -17,    81,   -17,   -17,    82,   -17,
     -17,   -17,   -17,   -17,    83,   -17,   -17,   -17,   -17,   -17,
     -17,    45,    73,    75,    76,    77,    79,    80,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    97,    -6,    94,
     -17,   -17,    93,    95,    96,    98,   -17,   -17,   -17,   -17,
       5,    37,   -10,   107,   110,   111,   -17,   -17,   112,   113,
     114,   115,   116,   -17,   117,   118,   119,   120,   121,    32,
     -17,   -17,   -17,   122,   123,   124,   125,   -17,   -17,   -17,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,     2,    47,   139,    40,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   -17,    44,   -17,    46,   -17,   -17,   155,   156,
     167,   169,   170,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,   168,   171,   172,   173,   -17,   -17,    -8,
      -8,    -8,   -17,   -17,   -17,   -17,   -17,   -17,   -17
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,     0,     0,     0,     0,     0,
      19,    20,    21,    22,     0,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,     4,
       0,     9,     7,     8,    51,    13,    49,    48,    14,    46,
      15,    53,    52,    56,    16,    17,    18,    55,    54,    23,
      25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,     0,     0,     0,     0,    12,     6,    10,    11,
       0,     0,     0,     0,     0,     0,    29,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,     0,
      58,    59,    60,     0,     0,     0,     0,    50,    47,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    42,    43,     0,     0,
       0,     0,     0,    31,    32,    33,    34,    35,    37,    38,
      39,    40,    41,     0,     0,     0,     0,    44,    45,     0,
       0,     0,    65,    66,    67,    68,    26,    27,    28
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -17,   -17,   -17,   -17,   -17,   -17,   174,   175,   -17,    31,
     -17,    36,    39,   -16,   -17,    -7
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    39,    40,    41,    42,    43,    48,    49,
      45,    46,    53,    59,    54,    82
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      60,     4,     5,     6,     7,     8,     9,     3,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    51,    52,    57,    58,   110,
     111,    37,   152,   153,    38,     4,     5,     6,     7,     8,
       9,    44,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    77,
     133,   134,    78,    47,    79,    80,    81,   154,   155,   173,
     174,   175,   176,    93,    50,    55,    56,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    83,    84,    85,    86,    87,    90,    91,
      92,    94,   118,    95,    96,    97,   117,    98,    99,   157,
     113,   119,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   112,   114,   120,   115,   116,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,     0,
     136,   137,   138,   186,   187,   188,   135,     0,     0,     0,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,     0,   158,   159,     0,   156,     0,     0,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   177,   179,   178,   180,   181,   182,     0,
       0,   183,   184,   185,     0,    88,    89
};

static const yytype_int16 yycheck[] =
{
      16,     3,     4,     5,     6,     7,     8,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    45,    46,    45,    46,    45,
      46,    43,    40,    41,    46,     3,     4,     5,     6,     7,
       8,    46,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    39,
      48,    49,    42,    46,    44,    45,    46,    40,    41,    45,
      46,    45,    46,    48,    46,    45,    47,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    38,    43,    37,    37,
      37,    48,    91,    48,    48,    48,    90,    48,    48,   136,
      37,    92,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    44,    48,    48,    37,    49,    48,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    -1,
      37,    37,    37,   179,   180,   181,    44,    -1,    -1,    -1,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    -1,    44,    44,    -1,    48,    -1,    -1,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    37,    49,    37,    37,    40,    -1,
      -1,    40,    40,    40,    -1,    41,    41
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    51,    52,     0,     3,     4,     5,     6,     7,     8,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    43,    46,    53,
      54,    55,    56,    57,    46,    60,    61,    46,    58,    59,
      46,    45,    46,    62,    64,    45,    47,    45,    46,    63,
      63,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    39,    42,    44,
      45,    46,    65,    44,    44,    44,    38,    43,    56,    57,
      37,    37,    37,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    44,
      45,    46,    48,    37,    48,    49,    48,    61,    59,    62,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    48,    49,    44,    37,    37,    37,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    40,    41,    40,    41,    48,    65,    44,    44,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    45,    46,    45,    46,    48,    49,    37,
      37,    37,    40,    40,    40,    40,    63,    63,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    52,    53,    53,    54,    54,    54,
      54,    54,    55,    56,    56,    56,    56,    56,    56,    56,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    58,    58,    59,    60,
      60,    61,    62,    62,    63,    63,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     2,     1,     2,     8,     8,     8,     3,
       3,     6,     6,     6,     6,     6,     3,     6,     6,     6,
       6,     6,     5,     5,     6,     6,     1,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     3,     2,     2,
       2,     1,     1,     4,     4,     6,     6,     6,     6
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 12:
#line 61 "parser.y"
    {
        printf("label: %s\n", (yyvsp[-1].str));
        SectionDefinition *sec = &sectionDefinitions[currentSection];
        int ndx = find_symbol(sec->name);
        Symbol* sym = get_symbol((yyvsp[-1].str));
        if(sym == NULL){
            add_symbol((yyvsp[-1].str), sec->length, ndx, SYM_NOTYP, SYM_LOC, 1);
        }
        else{
            if(sym->defined == 1){
                printf("Parsing Error: Defined two same symbols with the name: %s\n", (yyvsp[-1].str));
                YYABORT;

            }
            else{
                sym->value = sec->length;
                sym->ndx = find_symbol(sectionDefinitions[currentSection].name);
                sym->defined = 1;
                backpatch(sym);
            }
        }
        

        free((yyvsp[-1].str));
    }
#line 1512 "build/parser.tab.c"
    break;

  case 13:
#line 89 "parser.y"
            {printf(".global parsed\n");}
#line 1518 "build/parser.tab.c"
    break;

  case 14:
#line 92 "parser.y"
            {printf(".extern parsed\n");}
#line 1524 "build/parser.tab.c"
    break;

  case 15:
#line 95 "parser.y"
            {printf(".section parsed: %s\n", (yyvsp[0].str));
                switch_section((yyvsp[0].str));            
            }
#line 1532 "build/parser.tab.c"
    break;

  case 16:
#line 100 "parser.y"
            {
                printf(".word parsed\n");

            }
#line 1541 "build/parser.tab.c"
    break;

  case 17:
#line 106 "parser.y"
            {
                printf(".skip parsed\n");
                int n = (yyvsp[0].num);
                int i;
                for(i = 0; i < n; ++i){
                    section_emit_byte(0x00);
                }
            }
#line 1554 "build/parser.tab.c"
    break;

  case 18:
#line 116 "parser.y"
            {
                char* c = (yyvsp[0].str);
                printf(".ascii parsed: %s\n", c);
                while(*c != '\0'){
                    section_emit_byte(*c);
                    c++;
                }
                section_emit_byte(*c);
                
            }
#line 1569 "build/parser.tab.c"
    break;

  case 19:
#line 129 "parser.y"
            {
                printf(".end parsed\n");
                YYACCEPT;
            }
#line 1578 "build/parser.tab.c"
    break;

  case 20:
#line 137 "parser.y"
        {
            printf("parsed halt\n");
            section_emit_word(0x00000000);
        }
#line 1587 "build/parser.tab.c"
    break;

  case 21:
#line 143 "parser.y"
        {
            printf("parsed int\n");
            section_emit_word(0x10000000);
        }
#line 1596 "build/parser.tab.c"
    break;

  case 22:
#line 149 "parser.y"
        {
            printf("parsed iret\n");
            uint32_t instruction = form_pop_instruction(15); // pop pc
            section_emit_word(instruction);
            instruction = form_pop_csr_instruction(0); // pop status
            section_emit_word(instruction);
        }
#line 1608 "build/parser.tab.c"
    break;

  case 23:
#line 158 "parser.y"
        {
            printf("parsed call\n");
            uint32_t instruction = form_push_instruction(15);
            section_emit_word(instruction);
            if((yyvsp[0].item).kind == ITEM_SYM)
            {
                emit_symbol_call((yyvsp[0].item).sym);
            }
            else if((yyvsp[0].item).kind == ITEM_LITERAL)
            {
                int32_t d = (yyvsp[0].item).value;

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
#line 1643 "build/parser.tab.c"
    break;

  case 24:
#line 190 "parser.y"
        {
            printf("parsed ret\n");
            uint32_t instruction = form_pop_instruction(15);
            section_emit_word(instruction);
        }
#line 1653 "build/parser.tab.c"
    break;

  case 25:
#line 197 "parser.y"
        {
            printf("parsed jmp\n");
            if((yyvsp[0].item).kind == ITEM_SYM)
            {
                emit_symbol_jmp((yyvsp[0].item).sym, 0, 0, JMP_BASE, JMP_MEM_BASE);
            }
            else if((yyvsp[0].item).kind == ITEM_LITERAL)
            {
                int32_t d = (yyvsp[0].item).value;

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
#line 1690 "build/parser.tab.c"
    break;

  case 26:
#line 234 "parser.y"
        {
            printf("parsed beq\n");
            if((yyvsp[0].item).kind == ITEM_SYM)
            {
                emit_symbol_jmp((yyvsp[0].item).sym, (yyvsp[-5].num), (yyvsp[-2].num), JMP_EQ, JMP_MEM_EQ);
            }
            else if((yyvsp[0].item).kind == ITEM_LITERAL)
            {
                int32_t d = (yyvsp[0].item).value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_EQ,
                            0, (yyvsp[-5].num), (yyvsp[-2].num),
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
#line 1723 "build/parser.tab.c"
    break;

  case 27:
#line 264 "parser.y"
        {
            printf("parsed bne\n");
            if((yyvsp[0].item).kind == ITEM_SYM)
            {
                emit_symbol_jmp((yyvsp[0].item).sym, (yyvsp[-5].num), (yyvsp[-2].num), JMP_NE, JMP_MEM_NE);
            }
            else if((yyvsp[0].item).kind == ITEM_LITERAL)
            {
                int32_t d = (yyvsp[0].item).value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_NE,
                            0, (yyvsp[-5].num), (yyvsp[-2].num),
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
#line 1757 "build/parser.tab.c"
    break;

  case 28:
#line 295 "parser.y"
        {
            printf("parsed bgt\n");
            if((yyvsp[0].item).kind == ITEM_SYM)
            {
                emit_symbol_jmp((yyvsp[0].item).sym, (yyvsp[-5].num), (yyvsp[-2].num), JMP_GT, JMP_MEM_GT);
            }
            else if((yyvsp[0].item).kind == ITEM_LITERAL)
            {
                int32_t d = (yyvsp[0].item).value;

                if(d >= -2048 && d <= 2047)
                {
                    uint32_t instr =
                        form_jump_instruction(
                            JMP_GT,
                            0, (yyvsp[-5].num), (yyvsp[-2].num),
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
#line 1790 "build/parser.tab.c"
    break;

  case 29:
#line 325 "parser.y"
        {
            printf("parsed push\n");
            uint32_t instruction = form_push_instruction((yyvsp[0].num));
            section_emit_word(instruction);
        }
#line 1800 "build/parser.tab.c"
    break;

  case 30:
#line 332 "parser.y"
        {
            printf("parsed pop\n");
            uint32_t instruction = form_pop_instruction((yyvsp[0].num));
            section_emit_word(instruction);
        }
#line 1810 "build/parser.tab.c"
    break;

  case 31:
#line 339 "parser.y"
        {
            printf("parsed xchg\n");
            section_emit_word(
                (0x4u << 28) |
                (((yyvsp[0].num) & 0xF) << 16) |
                (((yyvsp[-3].num) & 0xF) << 12)
            );
        }
#line 1823 "build/parser.tab.c"
    break;

  case 32:
#line 349 "parser.y"
        {
            printf("parsed add\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_ADD, (yyvsp[0].num), (yyvsp[-3].num), (yyvsp[0].num));
            section_emit_word(instruction);
        }
#line 1833 "build/parser.tab.c"
    break;

  case 33:
#line 356 "parser.y"
        {
            printf("parsed sub\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_SUB, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1843 "build/parser.tab.c"
    break;

  case 34:
#line 363 "parser.y"
        {
            printf("parsed mul\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_MUL, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1853 "build/parser.tab.c"
    break;

  case 35:
#line 370 "parser.y"
        {
            printf("parsed div\n");
            uint32_t instruction = form_arithmetic_instruction(ARITH_DIV, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1863 "build/parser.tab.c"
    break;

  case 36:
#line 377 "parser.y"
        {
            printf("parsed not\n");
            uint32_t instruction = form_logic_instruction(LOGIC_NOT, (yyvsp[0].num), (yyvsp[0].num), 0);
            section_emit_word(instruction);
        }
#line 1873 "build/parser.tab.c"
    break;

  case 37:
#line 384 "parser.y"
        {
            printf("parsed and\n");
            uint32_t instruction = form_logic_instruction(LOGIC_AND, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1883 "build/parser.tab.c"
    break;

  case 38:
#line 391 "parser.y"
        {
            printf("parsed or\n");
            uint32_t instruction = form_logic_instruction(LOGIC_OR, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1893 "build/parser.tab.c"
    break;

  case 39:
#line 398 "parser.y"
        {
            printf("parsed xor\n");
            uint32_t instruction = form_logic_instruction(LOGIC_XOR, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1903 "build/parser.tab.c"
    break;

  case 40:
#line 405 "parser.y"
        {
            printf("parsed shl\n");
            uint32_t instruction = form_shift_instruction(SHIFT_LEFT, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1913 "build/parser.tab.c"
    break;

  case 41:
#line 412 "parser.y"
        {
            printf("parsed shr\n");
            uint32_t instruction = form_shift_instruction(SHIFT_RIGHT, (yyvsp[0].num), (yyvsp[0].num), (yyvsp[-3].num));
            section_emit_word(instruction);
        }
#line 1923 "build/parser.tab.c"
    break;

  case 42:
#line 419 "parser.y"
        {
            printf("parsed ld\n");
            
            if((yyvsp[-3].opr).kind == OPERAND_REG_VALUE){
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_GPR_PLUS_D,
                    (yyvsp[0].num),
                    (yyvsp[-3].opr).reg,
                    0,
                    0);
                section_emit_word(instruction);
            }
            else if((yyvsp[-3].opr).kind == OPERAND_REG_ADDR){
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_MEM_INDEXED,
                    (yyvsp[0].num),
                    (yyvsp[-3].opr).reg,
                    0,
                    0);
                section_emit_word(instruction);
            }
            else if((yyvsp[-3].opr).kind == OPERAND_REG_ADD_LITERAL){
                int32_t d = (yyvsp[-3].opr).literal;
                if (d < -2048 || d > 2047) {
                    printf("Literal doesn't fit within 12 bits\n");
                    exit(1);
                }
                uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_MEM_INDEXED,
                    (yyvsp[0].num),
                    (yyvsp[-3].opr).reg,
                    0,
                    d);
                section_emit_word(instruction);
            }
            else if((yyvsp[-3].opr).kind == OPERAND_REG_ADD_SYMBOL){
                Symbol* sym = get_symbol((yyvsp[-3].opr).sym);
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
                    (yyvsp[0].num),
                    (yyvsp[-3].opr).reg,
                    0,
                    d);
                section_emit_word(instruction);
            }
            else if((yyvsp[-3].opr).kind == OPERAND_LITERAL_VALUE){
                int32_t d = (yyvsp[-3].opr).literal;
                if (d >= -2048 && d <= 2047) {
                    printf("Test within\n");
                    uint32_t instruction = form_load_instruction(
                    LOAD_GPR_FROM_GPR_PLUS_D,
                    (yyvsp[0].num),
                    0,
                    0,
                    d);
                    section_emit_word(instruction);
                    printf("%08X\n", instruction);
                }
                else{
                    printf("Test large\n");
                    emit_large_literal_load(d, (yyvsp[0].num));               
                }  
            }
            else if((yyvsp[-3].opr).kind == OPERAND_SYMBOL_VALUE)
            {
                emit_symbol_address_load((yyvsp[-3].opr).sym, (yyvsp[0].num));
            }
            else if((yyvsp[-3].opr).kind == OPERAND_LITERAL_ADDR)
            {
                uint32_t offset =
                    sectionDefinitions[currentSection].length;


                uint32_t instr =
                    form_load_instruction(
                        LOAD_GPR_FROM_MEM_INDEXED,
                        (yyvsp[0].num),
                        15,   // pc
                        0,
                        0     // placeholder
                    );


                section_emit_word(instr);


                add_relocation(
                    currentSection,
                    offset,
                    -1,          // no symbol
                    PCREL12,
                    (yyvsp[-3].opr).literal
                );
            }
            else if((yyvsp[-3].opr).kind == OPERAND_SYMBOL_ADDR)
            {
                Symbol *sym = get_symbol((yyvsp[-3].opr).sym);

                if(sym == NULL)
                {
                    add_symbol(
                        (yyvsp[-3].opr).sym,
                        0xFFFFFFFF,
                        -1,
                        SYM_NOTYP,
                        SYM_LOC,
                        0
                    );

                    sym = get_symbol((yyvsp[-3].opr).sym);
                }


                uint32_t offset =
                    sectionDefinitions[currentSection].length;


                uint32_t instr =
                    form_load_instruction(
                        LOAD_GPR_FROM_MEM_INDEXED,
                        (yyvsp[0].num),
                        15,
                        0,
                        0
                    );

                section_emit_word(instr);


                add_relocation(
                    currentSection,
                    offset,
                    sym->num,
                    PCREL12,
                    0
                );
            }
            
        }
#line 2080 "build/parser.tab.c"
    break;

  case 43:
#line 573 "parser.y"
        {
            printf("parsed st\n");
            if((yyvsp[0].opr).kind == OPERAND_REG_ADDR){
                uint32_t instruction = form_store_instruction(
                    STORE_MEM_INDEXED,
                    (yyvsp[0].opr).reg,
                    0,
                    (yyvsp[-2].num),
                    0);
                section_emit_word(instruction);
            }
            else if((yyvsp[0].opr).kind == OPERAND_REG_ADD_LITERAL){
                int32_t d = (yyvsp[0].opr).literal;
                if (d < -2048 || d > 2047) {
                    printf("Literal doesn't fit within 12 bits\n");
                    exit(1);
                }
                uint32_t instruction = form_store_instruction(
                    STORE_MEM_INDEXED,
                    (yyvsp[0].opr).reg,
                    0,
                    (yyvsp[-2].num),
                    d);
                section_emit_word(instruction);
            }
            else if((yyvsp[0].opr).kind == OPERAND_REG_ADD_SYMBOL){
                Symbol* sym = get_symbol((yyvsp[0].opr).sym);
                if(sym == NULL){
                    printf("Symbol not defined for st instruction\n");
                    exit(1);
                }
                if(sym->ndx == -1){
                    printf("Symbol value not defined for st instruction\n");
                    exit(1);
                }
                int32_t d = sym->value;
                if (d < -2048 || d > 2047) {
                    printf("Symbl value doesn't fit within 12 bits\n");
                    exit(1);
                }
                uint32_t instruction = form_store_instruction(
                    STORE_MEM_INDEXED,
                    (yyvsp[0].opr).reg,
                    0,
                    (yyvsp[-2].num),
                    d);
                section_emit_word(instruction);
            }
        }
#line 2134 "build/parser.tab.c"
    break;

  case 44:
#line 624 "parser.y"
        {
            printf("parsed csrrd\n");
            uint32_t instruction = form_load_instruction(LOAD_GPR_FROM_CSR, (yyvsp[0].num), (yyvsp[-3].num), 0, 0);
            section_emit_word(instruction);
        }
#line 2144 "build/parser.tab.c"
    break;

  case 45:
#line 631 "parser.y"
        {
            printf("parsed csrwr\n");
            uint32_t instruction = form_load_instruction(LOAD_CSR_FROM_GPR, (yyvsp[0].num), (yyvsp[-3].num), 0, 0);
            section_emit_word(instruction);
        }
#line 2154 "build/parser.tab.c"
    break;

  case 48:
#line 659 "parser.y"
        {
            Symbol *sym = get_symbol((yyvsp[0].str));
            if(sym != NULL){
                //Symbol in table
                printf("Turn to extern symbol: %s\n", (yyvsp[0].str));
                sym->bind = SYM_GLOB; 
            }
            else{
                //Symbol not in table
                add_symbol((yyvsp[0].str), 0xFFFFFFFF, -1, SYM_NOTYP, SYM_GLOB, 0);
            }
        }
#line 2171 "build/parser.tab.c"
    break;

  case 51:
#line 679 "parser.y"
        {
            Symbol *sym = get_symbol((yyvsp[0].str));
            if(sym != NULL){
                //Symbol in table
                printf("Turn global symbol: %s\n", (yyvsp[0].str));
                sym->bind = SYM_GLOB; 
            }
            else{
                //Symbol not in table
                add_symbol((yyvsp[0].str), 0xFFFFFFFF, -1, SYM_NOTYP, SYM_GLOB, 0);
            }
        }
#line 2188 "build/parser.tab.c"
    break;

  case 52:
#line 694 "parser.y"
            {
                emit_symbol_word((yyvsp[0].str));
            }
#line 2196 "build/parser.tab.c"
    break;

  case 53:
#line 697 "parser.y"
                 {section_emit_word((yyvsp[0].num));}
#line 2202 "build/parser.tab.c"
    break;

  case 54:
#line 702 "parser.y"
    {
       (yyval.item).kind = ITEM_SYM;
       (yyval.item).sym = (yyvsp[0].str);
    }
#line 2211 "build/parser.tab.c"
    break;

  case 55:
#line 708 "parser.y"
    {
        (yyval.item).kind = ITEM_LITERAL;
        (yyval.item).value = (yyvsp[0].num);
    }
#line 2220 "build/parser.tab.c"
    break;

  case 58:
#line 722 "parser.y"
        {
            (yyval.opr).kind = OPERAND_LITERAL_VALUE;
            (yyval.opr).literal = (yyvsp[0].num);
        }
#line 2229 "build/parser.tab.c"
    break;

  case 59:
#line 728 "parser.y"
        {
            (yyval.opr).kind = OPERAND_SYMBOL_VALUE;
            (yyval.opr).sym = (yyvsp[0].str);
        }
#line 2238 "build/parser.tab.c"
    break;

  case 60:
#line 734 "parser.y"
        {
            (yyval.opr).kind = OPERAND_REG_VALUE;
            (yyval.opr).reg = (yyvsp[0].num);
        }
#line 2247 "build/parser.tab.c"
    break;

  case 61:
#line 740 "parser.y"
        {
            (yyval.opr).kind = OPERAND_LITERAL_ADDR;
            (yyval.opr).literal = (yyvsp[0].num);
        }
#line 2256 "build/parser.tab.c"
    break;

  case 62:
#line 746 "parser.y"
        {
            (yyval.opr).kind = OPERAND_SYMBOL_ADDR;
            (yyval.opr).sym = (yyvsp[0].str);
        }
#line 2265 "build/parser.tab.c"
    break;

  case 63:
#line 752 "parser.y"
        {
            (yyval.opr).kind = OPERAND_REG_ADDR;
            (yyval.opr).reg = (yyvsp[-1].num);
        }
#line 2274 "build/parser.tab.c"
    break;

  case 64:
#line 758 "parser.y"
        {
            (yyval.opr).kind = OPERAND_REG_ADDR;
            (yyval.opr).reg = (yyvsp[-1].num);
        }
#line 2283 "build/parser.tab.c"
    break;

  case 65:
#line 764 "parser.y"
        {
            (yyval.opr).kind = OPERAND_REG_ADD_LITERAL;
            (yyval.opr).reg = (yyvsp[-3].num);
            (yyval.opr).literal = (yyvsp[-1].num);
        }
#line 2293 "build/parser.tab.c"
    break;

  case 66:
#line 771 "parser.y"
        {
            (yyval.opr).kind = OPERAND_REG_ADD_SYMBOL;
            (yyval.opr).reg = (yyvsp[-3].num);
            (yyval.opr).sym = (yyvsp[-1].str);
        }
#line 2303 "build/parser.tab.c"
    break;

  case 67:
#line 778 "parser.y"
        {
            (yyval.opr).kind = OPERAND_CSR_ADD_LITERAL;
            (yyval.opr).reg = (yyvsp[-3].num);
            (yyval.opr).literal = (yyvsp[-1].num);
        }
#line 2313 "build/parser.tab.c"
    break;

  case 68:
#line 785 "parser.y"
        {
            (yyval.opr).kind = OPERAND_CSR_ADD_SYMBOL;
            (yyval.opr).reg = (yyvsp[-3].num);
            (yyval.opr).sym = (yyvsp[-1].str);
        }
#line 2323 "build/parser.tab.c"
    break;


#line 2327 "build/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 792 "parser.y"


void yyerror(const char *s)
{
    printf("Error: %s\n", s);
}

