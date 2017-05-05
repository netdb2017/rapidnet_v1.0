/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse ol_parser_parse
#define yylex   ol_parser_lex
#define yyerror ol_parser_error
#define yylval  ol_parser_lval
#define yychar  ol_parser_char
#define yydebug ol_parser_debug
#define yynerrs ol_parser_nerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     OL_OR = 258,
     OL_AND = 259,
     OL_BITOR = 260,
     OL_BITXOR = 261,
     OL_BITAND = 262,
     OL_BITNOT = 263,
     OL_NEQ = 264,
     OL_EQ = 265,
     OL_LTE = 266,
     OL_LT = 267,
     OL_GTE = 268,
     OL_GT = 269,
     OL_RSHIFT = 270,
     OL_LSHIFT = 271,
     OL_MINUS = 272,
     OL_PLUS = 273,
     OL_MODULUS = 274,
     OL_DIVIDE = 275,
     OL_TIMES = 276,
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
#define OL_NEQ 264
#define OL_EQ 265
#define OL_LTE 266
#define OL_LT 267
#define OL_GTE 268
#define OL_GT 269
#define OL_RSHIFT 270
#define OL_LSHIFT 271
#define OL_MINUS 272
#define OL_PLUS 273
#define OL_MODULUS 274
#define OL_DIVIDE 275
#define OL_TIMES 276
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




/* Copy the first part of user declarations.  */
#line 1 "src/rapidnet-compiler/ol-parser.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 96 "src/rapidnet-compiler/ol-parser.y"
{

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
}
/* Line 187 of yacc.c.  */
#line 268 "src/rapidnet-compiler/ol-parser.cc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 281 "src/rapidnet-compiler/ol-parser.cc"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  46
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   326

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNRULES -- Number of states.  */
#define YYNSTATES  241

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    45,    47,    52,
      56,    58,    62,    66,    68,    74,    82,    92,    98,   104,
     107,   109,   111,   114,   119,   125,   131,   138,   143,   147,
     152,   154,   158,   160,   162,   164,   167,   172,   176,   182,
     187,   194,   200,   208,   217,   219,   222,   226,   228,   232,
     235,   240,   242,   244,   249,   253,   255,   259,   261,   263,
     265,   269,   273,   277,   281,   284,   288,   292,   296,   298,
     300,   302,   304,   306,   308,   310,   312,   314,   318,   322,
     324,   326,   330,   332,   334,   336,   338,   340,   342,   344,
     346,   348,   350,   352,   358,   364,   370,   376,   378,   380,
     382,   384,   386,   388,   393,   399,   404
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      58,     0,    -1,    30,    -1,    59,    30,    -1,    60,    -1,
      60,    59,    -1,    72,    -1,    71,    -1,    61,    -1,    66,
      -1,    67,    -1,    69,    -1,    70,    -1,    76,    -1,    68,
      -1,    75,    -1,    47,    39,    27,    28,    62,    28,    62,
      28,    63,    38,    29,    -1,    33,    -1,    48,    39,    64,
      38,    -1,    48,    39,    38,    -1,    65,    -1,    65,    28,
      64,    -1,    33,    44,    27,    -1,    33,    -1,    52,    39,
      27,    38,    29,    -1,    53,    39,    27,    28,    32,    38,
      29,    -1,    54,    39,    32,    28,    27,    28,    27,    38,
      29,    -1,    55,    39,    27,    38,    29,    -1,    56,    39,
      27,    38,    29,    -1,    79,    29,    -1,    73,    -1,    74,
      -1,    27,    74,    -1,    79,    31,    77,    29,    -1,    45,
      79,    31,    77,    29,    -1,    79,    31,    45,    77,    29,
      -1,    45,    79,    45,    31,    77,    29,    -1,    79,    31,
      80,    29,    -1,    51,    34,    44,    -1,    46,    81,    82,
      29,    -1,    78,    -1,    78,    28,    77,    -1,    79,    -1,
      89,    -1,    88,    -1,    81,    82,    -1,    34,    49,    81,
      82,    -1,    50,    81,    82,    -1,    50,    34,    49,    81,
      82,    -1,    81,    82,    26,    34,    -1,    34,    49,    81,
      82,    26,    34,    -1,    50,    81,    82,    26,    34,    -1,
      50,    34,    49,    81,    82,    26,    34,    -1,   100,    39,
      82,    28,    82,    28,    79,    38,    -1,    27,    -1,    39,
      38,    -1,    39,    83,    38,    -1,    84,    -1,    84,    28,
      83,    -1,    26,    98,    -1,    26,    98,    28,    83,    -1,
      98,    -1,    99,    -1,    36,    39,    86,    38,    -1,    36,
      39,    38,    -1,    87,    -1,    87,    28,    86,    -1,    93,
      -1,    98,    -1,    90,    -1,    34,    25,    91,    -1,    34,
      25,    90,    -1,    39,    90,    38,    -1,    34,    23,    96,
      -1,    22,    90,    -1,    90,     3,    90,    -1,    90,     4,
      90,    -1,    91,    92,    91,    -1,    93,    -1,    85,    -1,
      98,    -1,    10,    -1,     9,    -1,    14,    -1,    12,    -1,
      13,    -1,    11,    -1,    93,    95,    94,    -1,    94,    95,
      94,    -1,    98,    -1,    85,    -1,    39,    93,    38,    -1,
      16,    -1,    15,    -1,    18,    -1,    17,    -1,    21,    -1,
      20,    -1,    19,    -1,     6,    -1,     7,    -1,     5,    -1,
       8,    -1,    39,    97,    28,    97,    38,    -1,    39,    97,
      28,    97,    41,    -1,    40,    97,    28,    97,    38,    -1,
      40,    97,    28,    97,    41,    -1,    93,    -1,    98,    -1,
      33,    -1,    34,    -1,    32,    -1,    37,    -1,   100,    12,
      34,    14,    -1,   100,    12,    26,    34,    14,    -1,   100,
      12,    21,    14,    -1,    35,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   125,   125,   126,   129,   130,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   145,   150,   154,   157,
     162,   164,   167,   169,   172,   177,   182,   187,   192,   197,
     200,   203,   208,   213,   216,   219,   222,   225,   229,   233,
     237,   238,   241,   241,   241,   244,   247,   250,   253,   256,
     259,   262,   265,   269,   273,   277,   279,   282,   287,   290,
     304,   320,   322,   329,   331,   335,   338,   343,   345,   349,
     353,   355,   359,   361,   363,   365,   367,   369,   373,   375,
     377,   381,   382,   383,   384,   385,   386,   389,   391,   395,
     397,   399,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   417,   419,   421,   423,   427,   429,   433,
     433,   433,   433,   437,   440,   448,   452
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OL_OR", "OL_AND", "OL_BITOR",
  "OL_BITXOR", "OL_BITAND", "OL_BITNOT", "OL_NEQ", "OL_EQ", "OL_LTE",
  "OL_LT", "OL_GTE", "OL_GT", "OL_RSHIFT", "OL_LSHIFT", "OL_MINUS",
  "OL_PLUS", "OL_MODULUS", "OL_DIVIDE", "OL_TIMES", "OL_NOT", "OL_IN",
  "OL_ID", "OL_ASSIGN", "OL_AT", "OL_NAME", "OL_COMMA", "OL_DOT", "OL_EOF",
  "OL_IF", "OL_STRING", "OL_VALUE", "OL_VAR", "OL_AGGFUNCNAME",
  "OL_FUNCTION", "OL_NULL", "OL_RPAR", "OL_LPAR", "OL_LSQUB", "OL_RSQUB",
  "OL_LCURB", "OL_RCURB", "OL_COLON", "OL_DEL", "OL_QUERY",
  "OL_MATERIALIZE", "OL_KEYS", "OL_SAYS", "OL_ENCRYPTS", "OL_CONTEXT",
  "OL_WATCH", "OL_WATCHFINE", "OL_STAGE", "OL_TRACE", "OL_TRACETABLE",
  "$accept", "program", "clauselist", "clause", "materialize", "tablearg",
  "primarykeys", "keylist", "key", "watch", "watchfine", "stage", "trace",
  "TraceTable", "fact", "rule", "namedRule", "unnamedRule", "context",
  "query", "termlist", "term", "functor", "aggview", "functorname",
  "functorbody", "functorargs", "functorarg", "function", "functionargs",
  "functionarg", "select", "assign", "bool_expr", "rel_atom", "rel_oper",
  "math_expr", "math_atom", "math_oper", "range_expr", "range_atom",
  "atom", "aggregate", "agg_oper", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
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

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    61,    62,    63,    63,
      64,    64,    65,    65,    66,    67,    68,    69,    70,    71,
      72,    72,    73,    74,    74,    74,    74,    74,    75,    76,
      77,    77,    78,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    79,    80,    81,    82,    82,    83,    83,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      89,    89,    90,    90,    90,    90,    90,    90,    91,    91,
      91,    92,    92,    92,    92,    92,    92,    93,    93,    94,
      94,    94,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    96,    96,    96,    96,    97,    97,    98,
      98,    98,    98,    99,    99,    99,   100
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,    11,     1,     4,     3,
       1,     3,     3,     1,     5,     7,     9,     5,     5,     2,
       1,     1,     2,     4,     5,     5,     6,     4,     3,     4,
       1,     3,     1,     1,     1,     2,     4,     3,     5,     4,
       6,     5,     7,     8,     1,     2,     3,     1,     3,     2,
       4,     1,     1,     4,     3,     1,     3,     1,     1,     1,
       3,     3,     3,     3,     2,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     5,     5,     5,     1,     1,     1,
       1,     1,     1,     4,     5,     4,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    54,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     8,     9,    10,
      14,    11,    12,     7,     6,    30,    31,    15,    13,     0,
       0,    54,    32,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     3,     5,    29,
       0,     0,    45,     0,     0,     0,     0,     0,     0,    47,
      38,     0,     0,     0,     0,     0,     0,   111,   109,   110,
     116,     0,   112,     0,     0,     0,    40,    42,     0,    79,
      44,    43,    69,     0,    78,     0,    80,     0,     0,   110,
      55,     0,    57,    61,    62,     0,     0,    46,     0,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      74,     0,     0,     0,     0,    78,     0,    33,     0,    37,
       0,     0,    82,    81,    86,    84,    85,    83,     0,   101,
      99,   100,   102,    93,    92,    95,    94,    98,    97,    96,
       0,     0,     0,    59,    56,     0,     0,    49,     0,    34,
       0,    17,     0,    48,    51,    24,     0,     0,    27,    28,
       0,     0,    73,    71,    70,    64,     0,    90,     0,    65,
      67,    89,    72,    91,    35,    41,    75,    76,    77,    87,
      89,    88,     0,     0,    58,     0,     0,     0,    50,    36,
       0,     0,     0,     0,   107,     0,    89,     0,     0,    63,
       0,     0,    60,   115,     0,   113,     0,    52,    25,     0,
       0,     0,    66,     0,   114,     0,     0,     0,     0,     0,
       0,     0,    26,   103,   104,   105,   106,     0,     0,     0,
      53,    23,    19,     0,    20,    16,     0,    18,     0,    22,
      21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,   152,   221,   233,   234,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      75,    76,    77,    78,    30,    52,    91,    92,    79,   168,
     169,    80,    81,    82,    83,   128,    84,    85,   140,   162,
     195,    86,    94,    95
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -145
static const yytype_int16 yypact[] =
{
      24,    -4,  -145,   -35,    54,     6,    43,    16,    57,    63,
      72,    80,    81,    82,    99,    92,   104,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,    74,
      84,  -145,  -145,   101,     6,   -13,    84,   106,    87,    84,
      93,   112,   113,   110,   114,   116,  -145,  -145,  -145,  -145,
     139,   235,   120,    84,   158,   117,   124,   136,     6,   142,
    -145,   127,   141,   149,   132,   143,   166,  -145,  -145,    -1,
    -145,   140,  -145,   166,   158,   153,   155,  -145,   157,   205,
    -145,  -145,    23,   290,   222,   222,   239,   148,    52,  -145,
    -145,   163,   165,  -145,  -145,   184,   170,   180,   178,   158,
    -145,   176,    84,   181,   185,   186,   189,   188,   202,   209,
    -145,    70,   166,   251,    17,   109,   204,  -145,   158,  -145,
     166,   166,  -145,  -145,  -145,  -145,  -145,  -145,   259,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
     259,   259,    84,   206,  -145,   245,     8,  -145,   201,  -145,
     207,  -145,   220,   223,  -145,  -145,   212,   224,  -145,  -145,
     259,   259,  -145,    23,   290,  -145,   259,  -145,   213,   225,
     222,    45,  -145,  -145,  -145,  -145,   258,  -145,  -145,  -145,
    -145,  -145,   236,   245,  -145,   249,   231,   252,  -145,  -145,
     176,   240,   246,   254,   222,   248,    -3,   266,   109,  -145,
     259,    84,  -145,  -145,   272,  -145,   269,  -145,  -145,   267,
     259,   259,  -145,   278,  -145,   260,   280,    49,    60,    54,
     268,   273,  -145,  -145,  -145,  -145,  -145,   274,    26,   281,
    -145,   270,  -145,   275,   287,  -145,   289,  -145,   284,  -145,
    -145
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,   302,  -145,  -145,   129,  -145,    83,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,   319,  -145,  -145,
     -46,  -145,     0,  -145,     5,   -34,  -134,  -145,  -104,   122,
    -145,  -145,  -145,   -60,   -97,  -145,   -66,   -96,   238,  -145,
    -144,   -48,  -145,   276
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -109
static const yytype_int16 yytable[] =
{
      29,    33,    56,    93,    35,    59,   110,   115,    98,   167,
      36,   184,    39,   114,    34,   164,    29,   197,    54,    97,
     120,   121,   111,    31,   112,  -108,   120,   121,   116,   185,
       3,   178,    55,    31,   186,  -108,   167,   167,  -108,    53,
     143,     4,   187,    31,   179,   181,     7,   170,    34,   202,
      38,     1,   163,   150,     2,   172,   167,   167,     3,   231,
     176,   177,   167,   102,   232,   171,   217,   218,   153,     4,
       5,     6,   175,   -68,     7,     8,     9,    10,    11,    12,
      13,    31,    37,   -68,    67,    68,    89,   223,     3,    72,
     224,    40,   180,   180,   194,   194,   167,    93,   225,    46,
     198,   226,    41,    49,     7,    50,   167,   167,   182,   160,
     161,    42,   196,   196,   129,   130,   131,   132,   180,    43,
      44,    45,    47,    51,   133,   134,   135,   136,   137,   138,
     139,     1,    50,    57,   170,    93,    58,    60,     3,    61,
      62,    64,    63,    65,   194,   194,    96,   173,    99,     4,
       5,     6,   171,   100,     7,     8,     9,    10,    11,    12,
      13,    66,   196,   196,   101,   104,    31,   213,   103,   105,
     107,    67,    68,    69,    70,    71,    72,   106,    73,   113,
      66,   108,   117,   118,    74,    31,   119,   142,    66,     7,
      67,    68,    69,   145,    71,    72,   146,    73,    67,    68,
     109,   144,    71,    72,   147,    73,   148,   149,     7,   151,
     -90,   -90,   -90,   -90,   155,   154,   157,   158,   156,   227,
     -90,   -90,   -90,   -90,   -90,   -90,   -90,   129,   130,   131,
     132,   159,   111,   174,   183,   188,   189,   133,   134,   135,
     136,   137,   138,   139,   -89,   -89,   -89,   -89,   190,   191,
     192,   199,   193,   200,   -89,   -89,   -89,   -89,   -89,   -89,
     -89,    88,   121,   203,   201,   204,   205,    67,    68,    89,
      70,    88,    72,    90,   207,   208,   210,    67,    68,    89,
      70,   209,    72,    67,    68,    89,   214,    71,    72,   165,
     166,    67,    68,    89,   211,    71,    72,   215,   166,   122,
     123,   124,   125,   126,   127,   216,   219,   228,   220,   222,
     235,   229,   230,   237,   236,   238,   239,   231,    48,   206,
      32,   240,   212,   141,     0,     0,    87
};

static const yytype_int16 yycheck[] =
{
       0,     1,    36,    51,     4,    39,    66,    73,    54,   113,
       5,   145,     7,    73,    49,   112,    16,   161,    31,    53,
       3,     4,    23,    27,    25,    28,     3,     4,    74,    21,
      34,   128,    45,    27,    26,    38,   140,   141,    41,    34,
      88,    45,    34,    27,   140,   141,    50,   113,    49,   183,
      34,    27,   112,    99,    30,    38,   160,   161,    34,    33,
     120,   121,   166,    58,    38,   113,   210,   211,   102,    45,
      46,    47,   118,    28,    50,    51,    52,    53,    54,    55,
      56,    27,    39,    38,    32,    33,    34,    38,    34,    37,
      41,    34,   140,   141,   160,   161,   200,   145,    38,     0,
     166,    41,    39,    29,    50,    31,   210,   211,   142,    39,
      40,    39,   160,   161,     5,     6,     7,     8,   166,    39,
      39,    39,    30,    39,    15,    16,    17,    18,    19,    20,
      21,    27,    31,    27,   200,   183,    49,    44,    34,    27,
      27,    27,    32,    27,   210,   211,    26,    38,    31,    45,
      46,    47,   200,    29,    50,    51,    52,    53,    54,    55,
      56,    22,   210,   211,    28,    38,    27,   201,    26,    28,
      38,    32,    33,    34,    35,    36,    37,    28,    39,    39,
      22,    38,    29,    28,    45,    27,    29,    39,    22,    50,
      32,    33,    34,    28,    36,    37,    12,    39,    32,    33,
      34,    38,    36,    37,    34,    39,    26,    29,    50,    33,
       5,     6,     7,     8,    29,    34,    27,    29,    32,   219,
      15,    16,    17,    18,    19,    20,    21,     5,     6,     7,
       8,    29,    23,    29,    28,    34,    29,    15,    16,    17,
      18,    19,    20,    21,     5,     6,     7,     8,    28,    26,
      38,    38,    28,    28,    15,    16,    17,    18,    19,    20,
      21,    26,     4,    14,    28,    34,    14,    32,    33,    34,
      35,    26,    37,    38,    34,    29,    28,    32,    33,    34,
      35,    27,    37,    32,    33,    34,    14,    36,    37,    38,
      39,    32,    33,    34,    28,    36,    37,    28,    39,     9,
      10,    11,    12,    13,    14,    38,    28,    39,    48,    29,
      29,    38,    38,    38,    44,    28,    27,    33,    16,   190,
       1,   238,   200,    85,    -1,    -1,    50
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    27,    30,    34,    45,    46,    47,    50,    51,    52,
      53,    54,    55,    56,    58,    59,    60,    61,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    79,
      81,    27,    74,    79,    49,    79,    81,    39,    34,    81,
      34,    39,    39,    39,    39,    39,     0,    30,    59,    29,
      31,    39,    82,    81,    31,    45,    82,    27,    49,    82,
      44,    27,    27,    32,    27,    27,    22,    32,    33,    34,
      35,    36,    37,    39,    45,    77,    78,    79,    80,    85,
      88,    89,    90,    91,    93,    94,    98,   100,    26,    34,
      38,    83,    84,    98,    99,   100,    26,    82,    77,    31,
      29,    28,    81,    26,    38,    28,    28,    38,    38,    34,
      90,    23,    25,    39,    90,    93,    77,    29,    28,    29,
       3,     4,     9,    10,    11,    12,    13,    14,    92,     5,
       6,     7,     8,    15,    16,    17,    18,    19,    20,    21,
      95,    95,    39,    98,    38,    28,    12,    34,    26,    29,
      77,    33,    62,    82,    34,    29,    32,    27,    29,    29,
      39,    40,    96,    90,    91,    38,    39,    85,    86,    87,
      93,    98,    38,    38,    29,    77,    90,    90,    91,    94,
      98,    94,    82,    28,    83,    21,    26,    34,    34,    29,
      28,    26,    38,    28,    93,    97,    98,    97,    93,    38,
      28,    28,    83,    14,    34,    14,    62,    34,    29,    27,
      28,    28,    86,    82,    14,    28,    38,    97,    97,    28,
      48,    63,    29,    38,    41,    38,    41,    79,    39,    38,
      38,    33,    38,    64,    65,    29,    44,    38,    28,    27,
      64
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (ctxt, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, ctxt)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, ctxt); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, OlContext *ctxt)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, ctxt)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    OlContext *ctxt;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (ctxt);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, OlContext *ctxt)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, ctxt)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    OlContext *ctxt;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, ctxt);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, OlContext *ctxt)
#else
static void
yy_reduce_print (yyvsp, yyrule, ctxt)
    YYSTYPE *yyvsp;
    int yyrule;
    OlContext *ctxt;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , ctxt);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, ctxt); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, OlContext *ctxt)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, ctxt)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    OlContext *ctxt;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (ctxt);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (OlContext *ctxt);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (OlContext *ctxt)
#else
int
yyparse (ctxt)
    OlContext *ctxt;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
     `$$ = $1'.

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
#line 125 "src/rapidnet-compiler/ol-parser.y"
    { YYACCEPT; }
    break;

  case 3:
#line 126 "src/rapidnet-compiler/ol-parser.y"
    { YYACCEPT; }
    break;

  case 16:
#line 147 "src/rapidnet-compiler/ol-parser.y"
    { ctxt->table((yyvsp[(3) - (11)].v), (yyvsp[(5) - (11)].v), (yyvsp[(7) - (11)].v), (yyvsp[(9) - (11)].u_exprlist)); }
    break;

  case 17:
#line 151 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 18:
#line 154 "src/rapidnet-compiler/ol-parser.y"
    {
			(yyval.u_exprlist) = (yyvsp[(3) - (4)].u_exprlist);
		}
    break;

  case 19:
#line 157 "src/rapidnet-compiler/ol-parser.y"
    {
			(yyval.u_exprlist) = NULL; // This is going to be KeyID
		}
    break;

  case 20:
#line 162 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_exprlist) = new ParseExprList(); (yyval.u_exprlist)->push_front((yyvsp[(1) - (1)].u_key)); }
    break;

  case 21:
#line 164 "src/rapidnet-compiler/ol-parser.y"
    { (yyvsp[(3) - (3)].u_exprlist)->push_front((yyvsp[(1) - (3)].u_key)); (yyval.u_exprlist)=(yyvsp[(3) - (3)].u_exprlist); }
    break;

  case 22:
#line 167 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_key) = new ParseKey ((yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v)); }
    break;

  case 23:
#line 169 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_key) = new ParseKey ((yyvsp[(1) - (1)].v)); }
    break;

  case 24:
#line 172 "src/rapidnet-compiler/ol-parser.y"
    {
                ctxt->Watch((yyvsp[(3) - (5)].v), ""); /* no modifiers */
		}
    break;

  case 25:
#line 177 "src/rapidnet-compiler/ol-parser.y"
    {
                ctxt->Watch((yyvsp[(3) - (7)].v), (yyvsp[(5) - (7)].v)->ToString()); /* With modifiers */
		}
    break;

  case 26:
#line 182 "src/rapidnet-compiler/ol-parser.y"
    {
			ctxt->Stage((yyvsp[(3) - (9)].v),(yyvsp[(5) - (9)].v),(yyvsp[(7) - (9)].v));
		}
    break;

  case 27:
#line 187 "src/rapidnet-compiler/ol-parser.y"
    {
			ctxt->TraceTuple((yyvsp[(3) - (5)].v));
		}
    break;

  case 28:
#line 192 "src/rapidnet-compiler/ol-parser.y"
    {
			ctxt->TraceTable((yyvsp[(3) - (5)].v));
		}
    break;

  case 29:
#line 197 "src/rapidnet-compiler/ol-parser.y"
    { ctxt->Fact((yyvsp[(1) - (2)].u_term)); }
    break;

  case 30:
#line 201 "src/rapidnet-compiler/ol-parser.y"
    { ctxt->AddRule ((yyvsp[(1) - (1)].u_rule)); }
    break;

  case 31:
#line 204 "src/rapidnet-compiler/ol-parser.y"
    { ctxt->AddRule ((yyvsp[(1) - (1)].u_rule)); }
    break;

  case 32:
#line 209 "src/rapidnet-compiler/ol-parser.y"
    { ((OlContext::Rule*) (yyvsp[(2) - (2)].u_rule)->GetRule ())->SetName ((yyvsp[(1) - (2)].v)); (yyval.u_rule) = (yyvsp[(2) - (2)].u_rule); }
    break;

  case 33:
#line 214 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[(1) - (4)].u_term), (yyvsp[(3) - (4)].u_termlist), false)); }
    break;

  case 34:
#line 217 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[(2) - (5)].u_term), (yyvsp[(4) - (5)].u_termlist), true)); }
    break;

  case 35:
#line 220 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[(1) - (5)].u_term), (yyvsp[(4) - (5)].u_termlist), false, NULL, true)); }
    break;

  case 36:
#line 223 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_rule) = new ParseRule (ctxt->CreateRule((yyvsp[(2) - (6)].u_term), (yyvsp[(5) - (6)].u_termlist), true, NULL, true)); }
    break;

  case 37:
#line 226 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_rule) = new ParseRule (ctxt->CreateAggRule((yyvsp[(1) - (4)].u_term), (yyvsp[(3) - (4)].u_aggterm), false)); }
    break;

  case 38:
#line 230 "src/rapidnet-compiler/ol-parser.y"
    { ctxt->SetContext((yyvsp[(2) - (3)].v)); }
    break;

  case 39:
#line 233 "src/rapidnet-compiler/ol-parser.y"
    {
                  ctxt->Query(new ParseFunctor((yyvsp[(2) - (4)].u_functorname), (yyvsp[(3) - (4)].u_exprlist))); }
    break;

  case 40:
#line 237 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_termlist) = new ParseTermList(); (yyval.u_termlist)->push_front((yyvsp[(1) - (1)].u_term)); }
    break;

  case 41:
#line 238 "src/rapidnet-compiler/ol-parser.y"
    { (yyvsp[(3) - (3)].u_termlist)->push_front((yyvsp[(1) - (3)].u_term)); (yyval.u_termlist)=(yyvsp[(3) - (3)].u_termlist); }
    break;

  case 44:
#line 241 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_term)=(yyvsp[(1) - (1)].u_term); }
    break;

  case 45:
#line 245 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_term)=new ParseFunctor((yyvsp[(1) - (2)].u_functorname), (yyvsp[(2) - (2)].u_exprlist)); }
    break;

  case 46:
#line 247 "src/rapidnet-compiler/ol-parser.y"
    {
			(yyval.u_term)=new ParseFunctor((yyvsp[(3) - (4)].u_functorname), (yyvsp[(4) - (4)].u_exprlist), NULL, (yyvsp[(1) - (4)].v), true); }
    break;

  case 47:
#line 250 "src/rapidnet-compiler/ol-parser.y"
    {
		  (yyval.u_term)=new ParseFunctor((yyvsp[(2) - (3)].u_functorname), (yyvsp[(3) - (3)].u_exprlist), NULL, NULL, false, true); }
    break;

  case 48:
#line 253 "src/rapidnet-compiler/ol-parser.y"
    {
      			(yyval.u_term)=new ParseFunctor((yyvsp[(4) - (5)].u_functorname), (yyvsp[(5) - (5)].u_exprlist), NULL, (yyvsp[(2) - (5)].v), true, true); }
    break;

  case 49:
#line 257 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_term)=new ParseFunctor((yyvsp[(1) - (4)].u_functorname), (yyvsp[(2) - (4)].u_exprlist), (yyvsp[(4) - (4)].v)); }
    break;

  case 50:
#line 259 "src/rapidnet-compiler/ol-parser.y"
    {
			(yyval.u_term)=new ParseFunctor((yyvsp[(3) - (6)].u_functorname), (yyvsp[(4) - (6)].u_exprlist), (yyvsp[(6) - (6)].v), (yyvsp[(1) - (6)].v), true); }
    break;

  case 51:
#line 262 "src/rapidnet-compiler/ol-parser.y"
    {
		  (yyval.u_term)=new ParseFunctor((yyvsp[(2) - (5)].u_functorname), (yyvsp[(3) - (5)].u_exprlist), (yyvsp[(5) - (5)].v), NULL, false, true); }
    break;

  case 52:
#line 265 "src/rapidnet-compiler/ol-parser.y"
    {
      			(yyval.u_term)=new ParseFunctor((yyvsp[(4) - (7)].u_functorname), (yyvsp[(5) - (7)].u_exprlist), (yyvsp[(7) - (7)].v), (yyvsp[(2) - (7)].v), true, true); }
    break;

  case 53:
#line 270 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_aggterm) = new ParseAggTerm((yyvsp[(1) - (8)].u_aoper), (yyvsp[(3) - (8)].u_exprlist), (yyvsp[(5) - (8)].u_exprlist), (yyvsp[(7) - (8)].u_term)); }
    break;

  case 54:
#line 274 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_functorname) = new ParseFunctorName((yyvsp[(1) - (1)].v)); }
    break;

  case 55:
#line 278 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_exprlist)=new ParseExprList(); }
    break;

  case 56:
#line 280 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_exprlist)=(yyvsp[(2) - (3)].u_exprlist); }
    break;

  case 57:
#line 283 "src/rapidnet-compiler/ol-parser.y"
    { 
                          (yyval.u_exprlist) = new ParseExprList(); 
                          (yyval.u_exprlist)->push_front((yyvsp[(1) - (1)].v));
                        }
    break;

  case 58:
#line 287 "src/rapidnet-compiler/ol-parser.y"
    {
			(yyvsp[(3) - (3)].u_exprlist)->push_front((yyvsp[(1) - (3)].v)); 
			(yyval.u_exprlist)=(yyvsp[(3) - (3)].u_exprlist); }
    break;

  case 59:
#line 291 "src/rapidnet-compiler/ol-parser.y"
    {
                          (yyval.u_exprlist) = new ParseExprList(); 
                          ParseVar *pv = dynamic_cast<ParseVar*>((yyvsp[(2) - (2)].v));
                          if (!pv) {
                            ostringstream oss;
                            oss << "location specifier is not a variable";
                            ctxt->ReportError(oss.str());
                          }
                          else {
                            pv->SetLocSpec();
                            (yyval.u_exprlist)->push_front((yyvsp[(2) - (2)].v)); 
                          }
                        }
    break;

  case 60:
#line 305 "src/rapidnet-compiler/ol-parser.y"
    {
                          ParseVar *pv = dynamic_cast<ParseVar*>((yyvsp[(2) - (4)].v));
                          if (!pv) {
                            ostringstream oss;
                            oss << "location specifier is not a variable";
                            ctxt->ReportError(oss.str());
                          }
                          else {
                            pv->SetLocSpec();
                            (yyvsp[(4) - (4)].u_exprlist)->push_front((yyvsp[(2) - (4)].v)); 
                            (yyval.u_exprlist)=(yyvsp[(4) - (4)].u_exprlist); 
                          }
                        }
    break;

  case 61:
#line 321 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 62:
#line 323 "src/rapidnet-compiler/ol-parser.y"
    {
                          (yyval.v) = (yyvsp[(1) - (1)].v);
                        }
    break;

  case 63:
#line 330 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseFunction((yyvsp[(1) - (4)].v), (yyvsp[(3) - (4)].u_exprlist)); }
    break;

  case 64:
#line 332 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseFunction((yyvsp[(1) - (3)].v), new ParseExprList()); }
    break;

  case 65:
#line 335 "src/rapidnet-compiler/ol-parser.y"
    { 
			(yyval.u_exprlist) = new ParseExprList(); 
			(yyval.u_exprlist)->push_front((yyvsp[(1) - (1)].v)); }
    break;

  case 66:
#line 338 "src/rapidnet-compiler/ol-parser.y"
    { 
			(yyvsp[(3) - (3)].u_exprlist)->push_front((yyvsp[(1) - (3)].v)); 
			(yyval.u_exprlist)=(yyvsp[(3) - (3)].u_exprlist); }
    break;

  case 67:
#line 344 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 68:
#line 346 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 69:
#line 350 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_term) = new ParseSelect((yyvsp[(1) - (1)].v)); }
    break;

  case 70:
#line 354 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_term) = new ParseAssign((yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v)); }
    break;

  case 71:
#line 356 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_term) = new ParseAssign((yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v)); }
    break;

  case 72:
#line 360 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(2) - (3)].v); }
    break;

  case 73:
#line 362 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseBool(ParseBool::RANGE, (yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v)); }
    break;

  case 74:
#line 364 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseBool(ParseBool::NOT, (yyvsp[(2) - (2)].v) ); }
    break;

  case 75:
#line 366 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseBool(ParseBool::OR, (yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v) ); }
    break;

  case 76:
#line 368 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseBool(ParseBool::AND, (yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v) ); }
    break;

  case 77:
#line 370 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseBool((yyvsp[(2) - (3)].u_boper), (yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v) ); }
    break;

  case 78:
#line 374 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 79:
#line 376 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 80:
#line 378 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 81:
#line 381 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_boper) = ParseBool::EQ; }
    break;

  case 82:
#line 382 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_boper) = ParseBool::NEQ; }
    break;

  case 83:
#line 383 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_boper) = ParseBool::GT; }
    break;

  case 84:
#line 384 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_boper) = ParseBool::LT; }
    break;

  case 85:
#line 385 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_boper) = ParseBool::GTE; }
    break;

  case 86:
#line 386 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_boper) = ParseBool::LTE; }
    break;

  case 87:
#line 390 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseMath((yyvsp[(2) - (3)].u_moper), (yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v) ); }
    break;

  case 88:
#line 392 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseMath((yyvsp[(2) - (3)].u_moper), (yyvsp[(1) - (3)].v), (yyvsp[(3) - (3)].v) ); }
    break;

  case 89:
#line 396 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 90:
#line 398 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 91:
#line 400 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(2) - (3)].v); }
    break;

  case 92:
#line 403 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::LSHIFT; }
    break;

  case 93:
#line 404 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::RSHIFT; }
    break;

  case 94:
#line 405 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::PLUS; }
    break;

  case 95:
#line 406 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::MINUS; }
    break;

  case 96:
#line 407 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::TIMES; }
    break;

  case 97:
#line 408 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::DIVIDE; }
    break;

  case 98:
#line 409 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::MODULUS; }
    break;

  case 99:
#line 410 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::BIT_XOR; }
    break;

  case 100:
#line 411 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::BIT_AND; }
    break;

  case 101:
#line 412 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::BIT_OR; }
    break;

  case 102:
#line 413 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.u_moper) = ParseMath::BIT_NOT; }
    break;

  case 103:
#line 418 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseRange(ParseRange::RANGEOO, (yyvsp[(2) - (5)].v), (yyvsp[(4) - (5)].v)); }
    break;

  case 104:
#line 420 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseRange(ParseRange::RANGEOC, (yyvsp[(2) - (5)].v), (yyvsp[(4) - (5)].v)); }
    break;

  case 105:
#line 422 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseRange(ParseRange::RANGECO, (yyvsp[(2) - (5)].v), (yyvsp[(4) - (5)].v)); }
    break;

  case 106:
#line 424 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseRange(ParseRange::RANGECC, (yyvsp[(2) - (5)].v), (yyvsp[(4) - (5)].v)); }
    break;

  case 107:
#line 428 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 108:
#line 430 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 112:
#line 434 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = (yyvsp[(1) - (1)].v); }
    break;

  case 113:
#line 438 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseAgg((yyvsp[(3) - (4)].v), (yyvsp[(1) - (4)].u_aoper), ValuePtr()); }
    break;

  case 114:
#line 441 "src/rapidnet-compiler/ol-parser.y"
    {
                          // Make the variable a location specifier
                          ParseVar *pv = dynamic_cast<ParseVar*>((yyvsp[(4) - (5)].v));
                          pv->SetLocSpec();
                          (yyval.v) = new ParseAgg((yyvsp[(4) - (5)].v), (yyvsp[(1) - (5)].u_aoper), ValuePtr());
                        }
    break;

  case 115:
#line 449 "src/rapidnet-compiler/ol-parser.y"
    { (yyval.v) = new ParseAgg(ParseAgg::DONT_CARE, (yyvsp[(1) - (4)].u_aoper), ValuePtr()); }
    break;

  case 116:
#line 453 "src/rapidnet-compiler/ol-parser.y"
    {
                  (yyval.u_aoper) = (yyvsp[(1) - (1)].v)->value->ToString ().c_str ();
                }
    break;


/* Line 1267 of yacc.c.  */
#line 2274 "src/rapidnet-compiler/ol-parser.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (ctxt, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (ctxt, yymsg);
	  }
	else
	  {
	    yyerror (ctxt, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (ctxt, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, ctxt);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 458 "src/rapidnet-compiler/ol-parser.y"


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


