/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         ol_parser_parse
#define yylex           ol_parser_lex
#define yyerror         ol_parser_error
#define yydebug         ol_parser_debug
#define yynerrs         ol_parser_nerrs


/* Copy the first part of user declarations.  */
#line 1 "ol-parser.y" /* yacc.c:339  */

/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

  // Prolog[ue]
  #include <deque>
  #include <iostream>
  #include "all-values.h"
  #include "ol-context.h"
  
  using namespace std;
  using namespace ns3;
  using namespace ns3::rapidnet_compiler;

  union YYSTYPE;
  static int ol_parser_lex(YYSTYPE *lvalp, OlContext *ctxt);
  static void ol_parser_error(OlContext *ctxt, string msg);


#line 108 "ol-parser.cc" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "ol-parser.hh".  */
#ifndef YY_OL_PARSER_OL_PARSER_HH_INCLUDED
# define YY_OL_PARSER_OL_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ol_parser_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OL_OR = 258,
    OL_AND = 259,
    OL_BITOR = 260,
    OL_BITXOR = 261,
    OL_BITAND = 262,
    OL_BITNOT = 263,
    OL_EQ = 264,
    OL_NEQ = 265,
    OL_GT = 266,
    OL_GTE = 267,
    OL_LT = 268,
    OL_LTE = 269,
    OL_LSHIFT = 270,
    OL_RSHIFT = 271,
    OL_PLUS = 272,
    OL_MINUS = 273,
    OL_TIMES = 274,
    OL_DIVIDE = 275,
    OL_MODULUS = 276,
    OL_NOT = 277,
    OL_IN = 278,
    OL_ID = 279,
    OL_ASSIGN = 280,
    OL_AT = 281,
    OL_NAME = 282,
    OL_COMMA = 283,
    OL_DOT = 284,
    OL_EOF = 285,
    OL_IF = 286,
    OL_STRING = 287,
    OL_VALUE = 288,
    OL_VAR = 289,
    OL_AGGFUNCNAME = 290,
    OL_FUNCTION = 291,
    OL_NULL = 292,
    OL_RPAR = 293,
    OL_LPAR = 294,
    OL_LSQUB = 295,
    OL_RSQUB = 296,
    OL_LCURB = 297,
    OL_RCURB = 298,
    OL_COLON = 299,
    OL_DEL = 300,
    OL_QUERY = 301,
    OL_MATERIALIZE = 302,
    OL_KEYS = 303,
    OL_SAYS = 304,
    OL_ENCRYPTS = 305,
    OL_CONTEXT = 306,
    OL_WATCH = 307,
    OL_WATCHFINE = 308,
    OL_STAGE = 309,
    OL_TRACE = 310,
    OL_TRACETABLE = 311
  };
#endif
/* Tokens.  */
#define OL_OR 258
#define OL_AND 259
#define OL_BITOR 260
#define OL_BITXOR 261
#define OL_BITAND 262
#define OL_BITNOT 263
#define OL_EQ 264
#define OL_NEQ 265
#define OL_GT 266
#define OL_GTE 267
#define OL_LT 268
#define OL_LTE 269
#define OL_LSHIFT 270
#define OL_RSHIFT 271
#define OL_PLUS 272
#define OL_MINUS 273
#define OL_TIMES 274
#define OL_DIVIDE 275
#define OL_MODULUS 276
#define OL_NOT 277
#define OL_IN 278
#define OL_ID 279
#define OL_ASSIGN 280
#define OL_AT 281
#define OL_NAME 282
#define OL_COMMA 283
#define OL_DOT 284
#define OL_EOF 285
#define OL_IF 286
#define OL_STRING 287
#define OL_VALUE 288
#define OL_VAR 289
#define OL_AGGFUNCNAME 290
#define OL_FUNCTION 291
#define OL_NULL 292
#define OL_RPAR 293
#define OL_LPAR 294
#define OL_LSQUB 295
#define OL_RSQUB 296
#define OL_LCURB 297
#define OL_RCURB 298
#define OL_COLON 299
#define OL_DEL 300
#define OL_QUERY 301
#define OL_MATERIALIZE 302
#define OL_KEYS 303
#define OL_SAYS 304
#define OL_ENCRYPTS 305
#define OL_CONTEXT 306
#define OL_WATCH 307
#define OL_WATCHFINE 308
#define OL_STAGE 309
#define OL_TRACE 310
#define OL_TRACETABLE 311

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 96 "ol-parser.y" /* yacc.c:355  */


  ns3::rapidnet_compiler::ParseBool::Operator  u_boper;
  ns3::rapidnet_compiler::ParseMath::Operator  u_moper;
  const char*                                   u_aoper;
  ns3::rapidnet_compiler::ParseTermList        *u_termlist;
  ns3::rapidnet_compiler::ParseTerm            *u_term;
  ns3::rapidnet_compiler::ParseFunctorName     *u_functorname;
  ns3::rapidnet_compiler::ParseExprList        *u_exprlist;
  ns3::rapidnet_compiler::ParseExprListList    *u_exprlistlist;
  ns3::rapidnet_compiler::ParseExpr            *v;
  ns3::rapidnet_compiler::ParseAggTerm         *u_aggterm;
  ns3::rapidnet_compiler::ParseKey             *u_key;
  ns3::rapidnet_compiler::ParseRule            *u_rule;

#line 276 "ol-parser.cc" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int ol_parser_parse (OlContext *ctxt);

#endif /* !YY_OL_PARSER_OL_PARSER_HH_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 292 "ol-parser.cc" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  50
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   334

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  245

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   125,   125,   126,   129,   130,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   145,   150,   154,   157,
     162,   164,   167,   169,   172,   177,   182,   187,   192,   197,
     200,   203,   207,   210,   213,   217,   221,   224,   227,   230,
     233,   237,   241,   245,   246,   249,   249,   249,   252,   255,
     258,   261,   264,   267,   270,   273,   277,   281,   285,   287,
     290,   295,   298,   312,   328,   330,   337,   339,   343,   346,
     351,   353,   357,   361,   363,   367,   369,   371,   373,   375,
     377,   381,   383,   385,   389,   390,   391,   392,   393,   394,
     397,   399,   403,   405,   407,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   425,   427,   429,   431,
     435,   437,   441,   441,   441,   441,   445,   448,   456,   460
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OL_OR", "OL_AND", "OL_BITOR",
  "OL_BITXOR", "OL_BITAND", "OL_BITNOT", "OL_EQ", "OL_NEQ", "OL_GT",
  "OL_GTE", "OL_LT", "OL_LTE", "OL_LSHIFT", "OL_RSHIFT", "OL_PLUS",
  "OL_MINUS", "OL_TIMES", "OL_DIVIDE", "OL_MODULUS", "OL_NOT", "OL_IN",
  "OL_ID", "OL_ASSIGN", "OL_AT", "OL_NAME", "OL_COMMA", "OL_DOT", "OL_EOF",
  "OL_IF", "OL_STRING", "OL_VALUE", "OL_VAR", "OL_AGGFUNCNAME",
  "OL_FUNCTION", "OL_NULL", "OL_RPAR", "OL_LPAR", "OL_LSQUB", "OL_RSQUB",
  "OL_LCURB", "OL_RCURB", "OL_COLON", "OL_DEL", "OL_QUERY",
  "OL_MATERIALIZE", "OL_KEYS", "OL_SAYS", "OL_ENCRYPTS", "OL_CONTEXT",
  "OL_WATCH", "OL_WATCHFINE", "OL_STAGE", "OL_TRACE", "OL_TRACETABLE",
  "$accept", "program", "clauselist", "clause", "materialize", "tablearg",
  "primarykeys", "keylist", "key", "watch", "watchfine", "stage", "trace",
  "TraceTable", "fact", "rule", "namedRule", "unnamedRule", "weightedRule",
  "unweightedRule", "context", "query", "termlist", "term", "functor",
  "aggview", "functorname", "functorbody", "functorargs", "functorarg",
  "function", "functionargs", "functionarg", "select", "assign",
  "bool_expr", "rel_atom", "rel_oper", "math_expr", "math_atom",
  "math_oper", "range_expr", "range_atom", "atom", "aggregate", "agg_oper", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311
};
# endif

#define YYPACT_NINF -144

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-144)))

#define YYTABLE_NINF -112

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     102,     7,  -144,     3,   -35,    -3,    18,    19,    -8,    47,
      44,    61,    65,    74,    76,   118,    92,   132,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,    49,    86,  -144,  -144,    99,  -144,    18,   -13,
      86,   104,    89,    86,    95,   119,   123,   128,   136,   140,
    -144,  -144,  -144,  -144,   167,   138,   142,    86,    87,   143,
     151,   162,    18,   171,  -144,   169,   180,   194,   186,   187,
     159,  -144,  -144,     2,  -144,   166,  -144,   159,    87,   197,
     199,  -144,   210,   213,  -144,  -144,    94,   280,   230,   230,
     247,   201,   108,  -144,  -144,   203,   214,  -144,  -144,   231,
     209,   232,   227,    87,  -144,   224,    86,   225,   241,   228,
     234,   242,   244,   251,  -144,    63,   159,   177,    12,    69,
     250,  -144,    87,  -144,   159,   159,  -144,  -144,  -144,  -144,
    -144,  -144,   249,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,   249,   249,    86,   256,  -144,   243,
      82,  -144,   238,  -144,   258,  -144,   267,   270,  -144,  -144,
     259,   271,  -144,  -144,   249,   249,  -144,    94,   280,  -144,
     249,  -144,   260,   272,   230,    -5,  -144,  -144,  -144,  -144,
     297,  -144,  -144,  -144,  -144,  -144,   274,   243,  -144,   292,
     273,   293,  -144,  -144,   224,   275,   276,   279,   230,   282,
      41,   283,    69,  -144,   249,    86,  -144,  -144,   301,  -144,
     285,  -144,  -144,   277,   249,   249,  -144,   286,  -144,   268,
     288,    22,    24,    -3,   269,   281,  -144,  -144,  -144,  -144,
    -144,   284,     5,   289,  -144,   287,  -144,   290,   295,  -144,
     294,  -144,   291,  -144,  -144
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    57,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     8,     9,
      10,    14,    11,    12,     7,     6,    30,    31,    33,    34,
      15,    13,     0,     0,    57,    32,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     3,     5,    29,     0,     0,    48,     0,     0,     0,
       0,     0,     0,    50,    41,     0,     0,     0,     0,     0,
       0,   114,   112,   113,   119,     0,   115,     0,     0,     0,
      43,    45,     0,    82,    47,    46,    72,     0,    81,     0,
      83,     0,     0,   113,    58,     0,    60,    64,    65,     0,
       0,    49,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,   113,    77,     0,     0,     0,     0,    81,
       0,    36,     0,    40,     0,     0,    84,    85,    86,    88,
      87,    89,     0,   104,   102,   103,   105,    95,    96,    97,
      98,    99,   100,   101,     0,     0,     0,    62,    59,     0,
       0,    52,     0,    37,     0,    17,     0,    51,    54,    24,
       0,     0,    27,    28,     0,     0,    76,    74,    73,    67,
       0,    93,     0,    68,    70,    92,    75,    94,    38,    44,
      78,    79,    80,    90,    92,    91,     0,     0,    61,     0,
       0,     0,    53,    39,     0,     0,     0,     0,   110,     0,
      92,     0,     0,    66,     0,     0,    63,   118,     0,   116,
       0,    55,    25,     0,     0,     0,    69,     0,   117,     0,
       0,     0,     0,     0,     0,     0,    26,   106,   107,   108,
     109,     0,     0,     0,    56,    23,    19,     0,    20,    16,
       0,    18,     0,    22,    21
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -144,  -144,   303,  -144,  -144,   131,  -144,    84,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,   326,  -144,   327,
    -144,  -144,   -49,  -144,     0,  -144,     4,   -36,  -138,  -144,
    -109,   125,  -144,  -144,  -144,   -57,   -88,  -144,   -71,   -17,
     245,  -144,  -143,   -53,  -144,   278
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    15,    16,    17,    18,   156,   225,   237,   238,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    79,    80,    81,    82,    33,    56,    95,    96,
      83,   172,   173,    84,    85,    86,    87,   132,    88,    89,
     144,   166,   199,    90,    98,    99
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    36,    97,    36,    60,    39,   119,    63,   171,   102,
      40,   188,    43,   114,    38,   124,   125,    32,    58,    34,
     118,   101,   201,   -71,    34,   115,    42,   116,   168,   120,
      34,     4,    59,   -71,    34,   171,   171,     4,   235,   147,
       3,     4,    57,   236,   182,    34,   174,     8,     5,   206,
     176,    38,     5,     8,   154,   171,   171,     8,    41,   167,
     227,   171,   229,   228,   175,   230,   106,   180,   181,  -111,
     157,   221,   222,   179,   133,   134,   135,   136,    53,  -111,
      54,    44,  -111,    45,   137,   138,   139,   140,   141,   142,
     143,   184,   184,   198,   198,   171,    97,   124,   125,   202,
      46,   189,   164,   165,    47,   171,   171,   177,   190,    70,
     186,   200,   200,    48,    34,    49,   191,   184,    50,    71,
      72,    73,    51,    75,    76,    55,    77,   183,   185,     1,
      54,    61,     2,   174,    97,     3,     4,     8,    62,    64,
      71,    72,    93,   198,   198,    76,    65,     5,     6,     7,
      66,   175,     8,     9,    10,    11,    12,    13,    14,     1,
      67,   200,   200,    68,    92,     3,     4,    69,   100,   217,
      71,    72,    93,    74,   103,    76,    94,     5,     6,     7,
     104,    70,     8,     9,    10,    11,    12,    13,    14,    70,
     105,    71,    72,   113,    34,    75,    76,   107,    77,    71,
      72,    73,    74,    75,    76,   117,    77,   108,   109,    71,
      72,    93,    78,    75,    76,   169,   170,     8,   -93,   -93,
     -93,   -93,   110,   231,   111,   112,   121,   122,   -93,   -93,
     -93,   -93,   -93,   -93,   -93,   133,   134,   135,   136,   123,
     146,   148,   149,   151,   150,   137,   138,   139,   140,   141,
     142,   143,   -92,   -92,   -92,   -92,   153,   155,   152,   158,
     160,   161,   -92,   -92,   -92,   -92,   -92,   -92,   -92,    92,
     159,   162,   192,   163,   115,    71,    72,    93,    74,   178,
      76,    71,    72,    93,   187,    75,    76,   193,   170,   126,
     127,   128,   129,   130,   131,   194,   195,   196,   203,   197,
     204,   125,   205,   207,   209,   212,   213,   208,   232,   211,
     214,   215,   218,   219,   223,   220,   224,   226,   239,   233,
      52,   243,   234,   242,   235,   210,   244,    35,   241,   216,
      37,   240,    91,     0,   145
};

static const yytype_int16 yycheck[] =
{
       0,     1,    55,     3,    40,     5,    77,    43,   117,    58,
       6,   149,     8,    70,    49,     3,     4,    17,    31,    27,
      77,    57,   165,    28,    27,    23,    34,    25,   116,    78,
      27,    34,    45,    38,    27,   144,   145,    34,    33,    92,
      33,    34,    38,    38,   132,    27,   117,    50,    45,   187,
      38,    49,    45,    50,   103,   164,   165,    50,    39,   116,
      38,   170,    38,    41,   117,    41,    62,   124,   125,    28,
     106,   214,   215,   122,     5,     6,     7,     8,    29,    38,
      31,    34,    41,    39,    15,    16,    17,    18,    19,    20,
      21,   144,   145,   164,   165,   204,   149,     3,     4,   170,
      39,    19,    39,    40,    39,   214,   215,    38,    26,    22,
     146,   164,   165,    39,    27,    39,    34,   170,     0,    32,
      33,    34,    30,    36,    37,    39,    39,   144,   145,    27,
      31,    27,    30,   204,   187,    33,    34,    50,    49,    44,
      32,    33,    34,   214,   215,    37,    27,    45,    46,    47,
      27,   204,    50,    51,    52,    53,    54,    55,    56,    27,
      32,   214,   215,    27,    26,    33,    34,    27,    26,   205,
      32,    33,    34,    35,    31,    37,    38,    45,    46,    47,
      29,    22,    50,    51,    52,    53,    54,    55,    56,    22,
      28,    32,    33,    34,    27,    36,    37,    26,    39,    32,
      33,    34,    35,    36,    37,    39,    39,    38,    28,    32,
      33,    34,    45,    36,    37,    38,    39,    50,     5,     6,
       7,     8,    28,   223,    38,    38,    29,    28,    15,    16,
      17,    18,    19,    20,    21,     5,     6,     7,     8,    29,
      39,    38,    28,    34,    13,    15,    16,    17,    18,    19,
      20,    21,     5,     6,     7,     8,    29,    33,    26,    34,
      32,    27,    15,    16,    17,    18,    19,    20,    21,    26,
      29,    29,    34,    29,    23,    32,    33,    34,    35,    29,
      37,    32,    33,    34,    28,    36,    37,    29,    39,     9,
      10,    11,    12,    13,    14,    28,    26,    38,    38,    28,
      28,     4,    28,    11,    11,    29,    27,    34,    39,    34,
      28,    28,    11,    28,    28,    38,    48,    29,    29,    38,
      17,    27,    38,    28,    33,   194,   242,     1,    38,   204,
       3,    44,    54,    -1,    89
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    27,    30,    33,    34,    45,    46,    47,    50,    51,
      52,    53,    54,    55,    56,    58,    59,    60,    61,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    81,    83,    27,    74,    81,    76,    49,    81,
      83,    39,    34,    83,    34,    39,    39,    39,    39,    39,
       0,    30,    59,    29,    31,    39,    84,    83,    31,    45,
      84,    27,    49,    84,    44,    27,    27,    32,    27,    27,
      22,    32,    33,    34,    35,    36,    37,    39,    45,    79,
      80,    81,    82,    87,    90,    91,    92,    93,    95,    96,
     100,   102,    26,    34,    38,    85,    86,   100,   101,   102,
      26,    84,    79,    31,    29,    28,    83,    26,    38,    28,
      28,    38,    38,    34,    92,    23,    25,    39,    92,    95,
      79,    29,    28,    29,     3,     4,     9,    10,    11,    12,
      13,    14,    94,     5,     6,     7,     8,    15,    16,    17,
      18,    19,    20,    21,    97,    97,    39,   100,    38,    28,
      13,    34,    26,    29,    79,    33,    62,    84,    34,    29,
      32,    27,    29,    29,    39,    40,    98,    92,    93,    38,
      39,    87,    88,    89,    95,   100,    38,    38,    29,    79,
      92,    92,    93,    96,   100,    96,    84,    28,    85,    19,
      26,    34,    34,    29,    28,    26,    38,    28,    95,    99,
     100,    99,    95,    38,    28,    28,    85,    11,    34,    11,
      62,    34,    29,    27,    28,    28,    88,    84,    11,    28,
      38,    99,    99,    28,    48,    63,    29,    38,    41,    38,
      41,    81,    39,    38,    38,    33,    38,    64,    65,    29,
      44,    38,    28,    27,    64
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    61,    62,    63,    63,
      64,    64,    65,    65,    66,    67,    68,    69,    70,    71,
      72,    72,    73,    74,    74,    75,    76,    76,    76,    76,
      76,    77,    78,    79,    79,    80,    80,    80,    81,    81,
      81,    81,    81,    81,    81,    81,    82,    83,    84,    84,
      85,    85,    85,    85,    86,    86,    87,    87,    88,    88,
      89,    89,    90,    91,    91,    92,    92,    92,    92,    92,
      92,    93,    93,    93,    94,    94,    94,    94,    94,    94,
      95,    95,    96,    96,    96,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    98,    98,    98,    98,
      99,    99,   100,   100,   100,   100,   101,   101,   101,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,    11,     1,     4,     3,
       1,     3,     3,     1,     5,     7,     9,     5,     5,     2,
       1,     1,     2,     1,     1,     2,     4,     5,     5,     6,
       4,     3,     4,     1,     3,     1,     1,     1,     2,     4,
       3,     5,     4,     6,     5,     7,     8,     1,     2,     3,
       1,     3,     2,     4,     1,     1,     4,     3,     1,     3,
       1,     1,     1,     3,     3,     3,     3,     2,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     5,     5,     5,
       1,     1,     1,     1,     1,     1,     4,     5,     4,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (ctxt, YY_("syntax error: cannot back up")); \
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
                  Type, Value, ctxt); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, OlContext *ctxt)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (ctxt);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, OlContext *ctxt)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, ctxt);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, OlContext *ctxt)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , ctxt);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, ctxt); \
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, OlContext *ctxt)
{
  YYUSE (yyvaluep);
  YYUSE (ctxt);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (OlContext *ctxt)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = yylex (&yylval, ctxt);
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 2:
#line 125 "ol-parser.y" /* yacc.c:1646  */
    { YYACCEPT; }
#line 1569 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 3:
#line 126 "ol-parser.y" /* yacc.c:1646  */
    { YYACCEPT; }
#line 1575 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 16:
#line 147 "ol-parser.y" /* yacc.c:1646  */
    { ctxt->table((yyvsp[-8].v), (yyvsp[-6].v), (yyvsp[-4].v), (yyvsp[-2].u_exprlist)); }
#line 1581 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 17:
#line 151 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 1587 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 18:
#line 154 "ol-parser.y" /* yacc.c:1646  */
    {
			(yyval.u_exprlist) = (yyvsp[-1].u_exprlist);
		}
#line 1595 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 19:
#line 157 "ol-parser.y" /* yacc.c:1646  */
    {
			(yyval.u_exprlist) = NULL; // This is going to be KeyID
		}
#line 1603 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 20:
#line 162 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_exprlist) = new ParseExprList(); (yyval.u_exprlist)->push_front((yyvsp[0].u_key)); }
#line 1609 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 21:
#line 164 "ol-parser.y" /* yacc.c:1646  */
    { (yyvsp[0].u_exprlist)->push_front((yyvsp[-2].u_key)); (yyval.u_exprlist)=(yyvsp[0].u_exprlist); }
#line 1615 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 22:
#line 167 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_key) = new ParseKey ((yyvsp[-2].v), (yyvsp[0].v)); }
#line 1621 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 23:
#line 169 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_key) = new ParseKey ((yyvsp[0].v)); }
#line 1627 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 24:
#line 172 "ol-parser.y" /* yacc.c:1646  */
    {
                ctxt->Watch((yyvsp[-2].v), ""); /* no modifiers */
		}
#line 1635 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 25:
#line 177 "ol-parser.y" /* yacc.c:1646  */
    {
                ctxt->Watch((yyvsp[-4].v), (yyvsp[-2].v)->ToString()); /* With modifiers */
		}
#line 1643 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 26:
#line 182 "ol-parser.y" /* yacc.c:1646  */
    {
			ctxt->Stage((yyvsp[-6].v),(yyvsp[-4].v),(yyvsp[-2].v));
		}
#line 1651 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 27:
#line 187 "ol-parser.y" /* yacc.c:1646  */
    {
			ctxt->TraceTuple((yyvsp[-2].v));
		}
#line 1659 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 28:
#line 192 "ol-parser.y" /* yacc.c:1646  */
    {
			ctxt->TraceTable((yyvsp[-2].v));
		}
#line 1667 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 29:
#line 197 "ol-parser.y" /* yacc.c:1646  */
    { ctxt->Fact((yyvsp[-1].u_term)); }
#line 1673 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 30:
#line 201 "ol-parser.y" /* yacc.c:1646  */
    { ctxt->AddRule ((yyvsp[0].u_rule)); }
#line 1679 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 31:
#line 204 "ol-parser.y" /* yacc.c:1646  */
    { ctxt->AddRule ((yyvsp[0].u_rule)); }
#line 1685 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 32:
#line 208 "ol-parser.y" /* yacc.c:1646  */
    { ((OlContext::Rule*) (yyvsp[0].u_rule)->GetRule ())->SetName ((yyvsp[-1].v)); (yyval.u_rule) = (yyvsp[0].u_rule); }
#line 1691 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 33:
#line 211 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = (yyvsp[0].u_rule); }
#line 1697 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 34:
#line 214 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = (yyvsp[0].u_rule); }
#line 1703 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 35:
#line 218 "ol-parser.y" /* yacc.c:1646  */
    { ((OlContext::Rule*) (yyvsp[0].u_rule)->GetRule ())->SetWeight ((yyvsp[-1].v)); (yyval.u_rule) = (yyvsp[0].u_rule); }
#line 1709 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 36:
#line 222 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[-3].u_term), (yyvsp[-1].u_termlist), false)); }
#line 1715 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 37:
#line 225 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[-3].u_term), (yyvsp[-1].u_termlist), true)); }
#line 1721 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 38:
#line 228 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[-4].u_term), (yyvsp[-1].u_termlist), false, NULL, NULL, true)); }
#line 1727 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 39:
#line 231 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[-4].u_term), (yyvsp[-1].u_termlist), true, NULL, NULL, true)); }
#line 1733 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 40:
#line 234 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_rule) = new ParseRule (ctxt->CreateAggRule((yyvsp[-3].u_term), (yyvsp[-1].u_aggterm), false)); }
#line 1739 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 41:
#line 238 "ol-parser.y" /* yacc.c:1646  */
    { ctxt->SetContext((yyvsp[-1].v)); }
#line 1745 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 42:
#line 241 "ol-parser.y" /* yacc.c:1646  */
    {
                  ctxt->Query(new ParseFunctor((yyvsp[-2].u_functorname), (yyvsp[-1].u_exprlist))); }
#line 1752 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 43:
#line 245 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_termlist) = new ParseTermList(); (yyval.u_termlist)->push_front((yyvsp[0].u_term)); }
#line 1758 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 44:
#line 246 "ol-parser.y" /* yacc.c:1646  */
    { (yyvsp[0].u_termlist)->push_front((yyvsp[-2].u_term)); (yyval.u_termlist)=(yyvsp[0].u_termlist); }
#line 1764 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 47:
#line 249 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_term)=(yyvsp[0].u_term); }
#line 1770 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 48:
#line 253 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_term)=new ParseFunctor((yyvsp[-1].u_functorname), (yyvsp[0].u_exprlist)); }
#line 1776 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 49:
#line 255 "ol-parser.y" /* yacc.c:1646  */
    {
			(yyval.u_term)=new ParseFunctor((yyvsp[-1].u_functorname), (yyvsp[0].u_exprlist), NULL, (yyvsp[-3].v), true); }
#line 1783 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 50:
#line 258 "ol-parser.y" /* yacc.c:1646  */
    {
		  (yyval.u_term)=new ParseFunctor((yyvsp[-1].u_functorname), (yyvsp[0].u_exprlist), NULL, NULL, false, true); }
#line 1790 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 51:
#line 261 "ol-parser.y" /* yacc.c:1646  */
    {
      			(yyval.u_term)=new ParseFunctor((yyvsp[-1].u_functorname), (yyvsp[0].u_exprlist), NULL, (yyvsp[-3].v), true, true); }
#line 1797 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 52:
#line 265 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_term)=new ParseFunctor((yyvsp[-3].u_functorname), (yyvsp[-2].u_exprlist), (yyvsp[0].v)); }
#line 1803 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 53:
#line 267 "ol-parser.y" /* yacc.c:1646  */
    {
			(yyval.u_term)=new ParseFunctor((yyvsp[-3].u_functorname), (yyvsp[-2].u_exprlist), (yyvsp[0].v), (yyvsp[-5].v), true); }
#line 1810 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 54:
#line 270 "ol-parser.y" /* yacc.c:1646  */
    {
		  (yyval.u_term)=new ParseFunctor((yyvsp[-3].u_functorname), (yyvsp[-2].u_exprlist), (yyvsp[0].v), NULL, false, true); }
#line 1817 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 55:
#line 273 "ol-parser.y" /* yacc.c:1646  */
    {
      			(yyval.u_term)=new ParseFunctor((yyvsp[-3].u_functorname), (yyvsp[-2].u_exprlist), (yyvsp[0].v), (yyvsp[-5].v), true, true); }
#line 1824 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 56:
#line 278 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_aggterm) = new ParseAggTerm((yyvsp[-7].u_aoper), (yyvsp[-5].u_exprlist), (yyvsp[-3].u_exprlist), (yyvsp[-1].u_term)); }
#line 1830 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 57:
#line 282 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_functorname) = new ParseFunctorName((yyvsp[0].v)); }
#line 1836 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 58:
#line 286 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_exprlist)=new ParseExprList(); }
#line 1842 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 59:
#line 288 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_exprlist)=(yyvsp[-1].u_exprlist); }
#line 1848 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 60:
#line 291 "ol-parser.y" /* yacc.c:1646  */
    { 
                          (yyval.u_exprlist) = new ParseExprList(); 
                          (yyval.u_exprlist)->push_front((yyvsp[0].v));
                        }
#line 1857 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 61:
#line 295 "ol-parser.y" /* yacc.c:1646  */
    {
			(yyvsp[0].u_exprlist)->push_front((yyvsp[-2].v)); 
			(yyval.u_exprlist)=(yyvsp[0].u_exprlist); }
#line 1865 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 62:
#line 299 "ol-parser.y" /* yacc.c:1646  */
    {
                          (yyval.u_exprlist) = new ParseExprList(); 
                          ParseVar *pv = dynamic_cast<ParseVar*>((yyvsp[0].v));
                          if (!pv) {
                            ostringstream oss;
                            oss << "location specifier is not a variable";
                            ctxt->ReportError(oss.str());
                          }
                          else {
                            pv->SetLocSpec();
                            (yyval.u_exprlist)->push_front((yyvsp[0].v)); 
                          }
                        }
#line 1883 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 63:
#line 313 "ol-parser.y" /* yacc.c:1646  */
    {
                          ParseVar *pv = dynamic_cast<ParseVar*>((yyvsp[-2].v));
                          if (!pv) {
                            ostringstream oss;
                            oss << "location specifier is not a variable";
                            ctxt->ReportError(oss.str());
                          }
                          else {
                            pv->SetLocSpec();
                            (yyvsp[0].u_exprlist)->push_front((yyvsp[-2].v)); 
                            (yyval.u_exprlist)=(yyvsp[0].u_exprlist); 
                          }
                        }
#line 1901 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 64:
#line 329 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 1907 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 65:
#line 331 "ol-parser.y" /* yacc.c:1646  */
    {
                          (yyval.v) = (yyvsp[0].v);
                        }
#line 1915 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 66:
#line 338 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseFunction((yyvsp[-3].v), (yyvsp[-1].u_exprlist)); }
#line 1921 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 67:
#line 340 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseFunction((yyvsp[-2].v), new ParseExprList()); }
#line 1927 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 68:
#line 343 "ol-parser.y" /* yacc.c:1646  */
    { 
			(yyval.u_exprlist) = new ParseExprList(); 
			(yyval.u_exprlist)->push_front((yyvsp[0].v)); }
#line 1935 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 69:
#line 346 "ol-parser.y" /* yacc.c:1646  */
    { 
			(yyvsp[0].u_exprlist)->push_front((yyvsp[-2].v)); 
			(yyval.u_exprlist)=(yyvsp[0].u_exprlist); }
#line 1943 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 70:
#line 352 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 1949 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 71:
#line 354 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 1955 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 72:
#line 358 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_term) = new ParseSelect((yyvsp[0].v)); }
#line 1961 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 73:
#line 362 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_term) = new ParseAssign((yyvsp[-2].v), (yyvsp[0].v)); }
#line 1967 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 74:
#line 364 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_term) = new ParseAssign((yyvsp[-2].v), (yyvsp[0].v)); }
#line 1973 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 75:
#line 368 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[-1].v); }
#line 1979 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 76:
#line 370 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseBool(ParseBool::RANGE, (yyvsp[-2].v), (yyvsp[0].v)); }
#line 1985 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 77:
#line 372 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseBool(ParseBool::NOT, (yyvsp[0].v) ); }
#line 1991 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 78:
#line 374 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseBool(ParseBool::OR, (yyvsp[-2].v), (yyvsp[0].v) ); }
#line 1997 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 79:
#line 376 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseBool(ParseBool::AND, (yyvsp[-2].v), (yyvsp[0].v) ); }
#line 2003 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 80:
#line 378 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseBool((yyvsp[-1].u_boper), (yyvsp[-2].v), (yyvsp[0].v) ); }
#line 2009 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 81:
#line 382 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2015 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 82:
#line 384 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2021 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 83:
#line 386 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2027 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 84:
#line 389 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_boper) = ParseBool::EQ; }
#line 2033 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 85:
#line 390 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_boper) = ParseBool::NEQ; }
#line 2039 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 86:
#line 391 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_boper) = ParseBool::GT; }
#line 2045 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 87:
#line 392 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_boper) = ParseBool::LT; }
#line 2051 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 88:
#line 393 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_boper) = ParseBool::GTE; }
#line 2057 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 89:
#line 394 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_boper) = ParseBool::LTE; }
#line 2063 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 90:
#line 398 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseMath((yyvsp[-1].u_moper), (yyvsp[-2].v), (yyvsp[0].v) ); }
#line 2069 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 91:
#line 400 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseMath((yyvsp[-1].u_moper), (yyvsp[-2].v), (yyvsp[0].v) ); }
#line 2075 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 92:
#line 404 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2081 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 93:
#line 406 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2087 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 94:
#line 408 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[-1].v); }
#line 2093 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 95:
#line 411 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::LSHIFT; }
#line 2099 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 96:
#line 412 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::RSHIFT; }
#line 2105 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 97:
#line 413 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::PLUS; }
#line 2111 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 98:
#line 414 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::MINUS; }
#line 2117 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 99:
#line 415 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::TIMES; }
#line 2123 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 100:
#line 416 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::DIVIDE; }
#line 2129 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 101:
#line 417 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::MODULUS; }
#line 2135 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 102:
#line 418 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::BIT_XOR; }
#line 2141 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 103:
#line 419 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::BIT_AND; }
#line 2147 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 104:
#line 420 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::BIT_OR; }
#line 2153 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 105:
#line 421 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.u_moper) = ParseMath::BIT_NOT; }
#line 2159 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 106:
#line 426 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseRange(ParseRange::RANGEOO, (yyvsp[-3].v), (yyvsp[-1].v)); }
#line 2165 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 107:
#line 428 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseRange(ParseRange::RANGEOC, (yyvsp[-3].v), (yyvsp[-1].v)); }
#line 2171 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 108:
#line 430 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseRange(ParseRange::RANGECO, (yyvsp[-3].v), (yyvsp[-1].v)); }
#line 2177 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 109:
#line 432 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseRange(ParseRange::RANGECC, (yyvsp[-3].v), (yyvsp[-1].v)); }
#line 2183 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 110:
#line 436 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2189 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 111:
#line 438 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2195 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 115:
#line 442 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 2201 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 116:
#line 446 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseAgg((yyvsp[-1].v), (yyvsp[-3].u_aoper), ValuePtr()); }
#line 2207 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 117:
#line 449 "ol-parser.y" /* yacc.c:1646  */
    {
                          // Make the variable a location specifier
                          ParseVar *pv = dynamic_cast<ParseVar*>((yyvsp[-1].v));
                          pv->SetLocSpec();
                          (yyval.v) = new ParseAgg((yyvsp[-1].v), (yyvsp[-4].u_aoper), ValuePtr());
                        }
#line 2218 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 118:
#line 457 "ol-parser.y" /* yacc.c:1646  */
    { (yyval.v) = new ParseAgg(ParseAgg::DONT_CARE, (yyvsp[-3].u_aoper), ValuePtr()); }
#line 2224 "ol-parser.cc" /* yacc.c:1646  */
    break;

  case 119:
#line 461 "ol-parser.y" /* yacc.c:1646  */
    {
                  (yyval.u_aoper) = (yyvsp[0].v)->value->ToString ().c_str ();
                }
#line 2232 "ol-parser.cc" /* yacc.c:1646  */
    break;


#line 2236 "ol-parser.cc" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
      yyerror (ctxt, YY_("syntax error"));
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (ctxt, yymsgp);
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
                      yytoken, &yylval, ctxt);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
                  yystos[yystate], yyvsp, ctxt);
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
  yyerror (ctxt, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ctxt);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, ctxt);
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
#line 466 "ol-parser.y" /* yacc.c:1906  */


// Epilog

#undef yylex
#include "ol-lexer.h"

int
ol_parser_lex(YYSTYPE *lvalp, OlContext *ctxt)
{
  return ctxt->lexer->yylex(lvalp, ctxt);
}

void
ol_parser_error(OlContext *ctxt, string msg)
{
  ctxt->ReportError(msg);
}

