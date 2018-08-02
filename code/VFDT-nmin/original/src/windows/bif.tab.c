/* A Bison parser, made from core/bif.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse bifparse
#define yylex biflex
#define yyerror biferror
#define yylval biflval
#define yychar bifchar
#define yydebug bifdebug
#define yynerrs bifnerrs
# define	tInteger	257
# define	tFloat	258
# define	tString	259
# define	tNetwork	260
# define	tVariable	261
# define	tProbability	262
# define	tProperty	263
# define	tVariableType	264
# define	tDiscrete	265
# define	tDefaultValue	266
# define	tTable	267
# define	tEOF	268

#line 1 "core/bif.y"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "vfml.h"
#line 8 "core/bif.y"

int biflex(void);
int biferror(const char *);

/* HERE figure out how to give better error messages */
/* BUG needs a \n at the end of the names file */

/* These tmps are allocated at the begining of parsing and then
    used during parsing.  After parsing a
    statement, the associated tmp is added to the appropriate
    global list, and a new tmp is allocated.  Finally, at the
    end of parsing, all the tmps are freed
*/

BeliefNet             bn;
BeliefNetNode         bnn;
BeliefNetNode         currentNode;
int                   gLineNumber;
int                   seenProbabilityEntry;

//IntListPtr            probVarList;
FloatListPtr          floatList;
VoidListPtr           stringList;

void _SetProbsFromTable(BeliefNetNode bnn, FloatListPtr fList);


#line 36 "core/bif.y"
#ifndef YYSTYPE
typedef union {
   int integer;
   float f;
   char *string;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		74
#define	YYFLAG		-32768
#define	YYNTBASE	22

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 268 ? yytranslate[x] : 45)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      20,    21,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    19,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    17,     2,    18,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    15,     2,    16,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     3,     8,    11,    13,    16,    18,    21,    23,
      25,    27,    28,    33,    37,    40,    43,    45,    47,    57,
      59,    66,    67,    68,    73,    76,    78,    80,    82,    84,
      86,    90,    96,   100,   103,   105,   108,   110,   113,   115,
     118
};
static const short yyrhs[] =
{
      23,    26,     0,     6,     5,    15,    24,     0,    25,    16,
       0,    16,     0,    25,    33,     0,    33,     0,    26,    27,
       0,    27,     0,    28,     0,    34,     0,     0,     7,     5,
      29,    30,     0,    15,    31,    16,     0,    31,    33,     0,
      31,    32,     0,    32,     0,    33,     0,    10,    11,    17,
       3,    18,    15,    43,    16,    19,     0,     9,     0,     8,
      20,    43,    21,    35,    36,     0,     0,     0,    15,    37,
      38,    16,     0,    38,    39,     0,    39,     0,    33,     0,
      40,     0,    41,     0,    42,     0,    12,    44,    19,     0,
      20,    43,    21,    44,    19,     0,    13,    44,    19,     0,
      43,     5,     0,     5,     0,    43,     3,     0,     3,     0,
      44,     4,     0,     4,     0,    44,     3,     0,     3,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,    52,    54,    58,    58,    60,    60,    62,    62,    63,
      63,    65,    65,    69,    70,    70,    71,    72,    75,    86,
      91,    93,   116,   116,   119,   119,   121,   121,   122,   123,
     126,   137,   168,   179,   181,   184,   195,   208,   210,   213,
     216
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "tInteger", "tFloat", "tString", "tNetwork", 
  "tVariable", "tProbability", "tProperty", "tVariableType", "tDiscrete", 
  "tDefaultValue", "tTable", "tEOF", "'{'", "'}'", "'['", "']'", "';'", 
  "'('", "')'", "CompilationUnit", "NetworkDeclaration", "NetworkContent", 
  "PropertyList", "GraphItemList", "GraphItem", "VariableDeclaration", 
  "@1", "VariableContent", "VariableContentItemList", "VariableDiscrete", 
  "Property", "ProbabilityDecl", "ActionInitCPT", "ProbabilityContent", 
  "@2", "ProbabilityContentItemList", "ProbabilityContentItem", 
  "ProbabilityDefaultEntry", "ProbabilityEntry", "ProbabilityTable", 
  "SList", "FList", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    22,    23,    24,    24,    25,    25,    26,    26,    27,
      27,    29,    28,    30,    31,    31,    31,    31,    32,    33,
      34,    35,    37,    36,    38,    38,    39,    39,    39,    39,
      40,    41,    42,    43,    43,    43,    43,    44,    44,    44,
      44
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     2,     4,     2,     1,     2,     1,     2,     1,     1,
       1,     0,     4,     3,     2,     2,     1,     1,     9,     1,
       6,     0,     0,     4,     2,     1,     1,     1,     1,     1,
       3,     5,     3,     2,     1,     2,     1,     2,     1,     2,
       1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,     0,     0,     0,     0,     1,     8,     9,    10,
       0,    11,     0,     7,    19,     4,     2,     0,     6,     0,
      36,    34,     0,     3,     5,     0,    12,    35,    33,    21,
       0,     0,    16,    17,     0,     0,    13,    15,    14,    22,
      20,     0,     0,     0,     0,     0,     0,    26,     0,    25,
      27,    28,    29,     0,    40,    38,     0,     0,     0,    23,
      24,     0,    39,    37,    30,    32,     0,     0,     0,     0,
      31,    18,     0,     0,     0
};

static const short yydefgoto[] =
{
      72,     2,    16,    17,     6,     7,     8,    19,    26,    31,
      32,    47,     9,    34,    40,    42,    48,    49,    50,    51,
      52,    22,    56
};

static const short yypact[] =
{
      20,     0,    35,    38,    49,    36,    35,-32768,-32768,-32768,
       7,-32768,    43,-32768,-32768,-32768,-32768,    29,-32768,    42,
  -32768,-32768,    -1,-32768,-32768,    40,-32768,-32768,-32768,-32768,
      23,    31,-32768,-32768,    44,    41,-32768,-32768,-32768,-32768,
  -32768,    52,    24,    45,    48,    48,    43,-32768,    19,-32768,
  -32768,-32768,-32768,    46,-32768,-32768,     6,     8,     3,-32768,
  -32768,    43,-32768,-32768,-32768,-32768,    48,    14,    10,    47,
  -32768,-32768,    60,    62,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,    58,-32768,-32768,-32768,-32768,
      34,   -10,-32768,-32768,-32768,-32768,-32768,    21,-32768,-32768,
  -32768,   -43,   -44
};


#define	YYLAST		69


static const short yytable[] =
{
      18,    57,    27,    58,    28,     3,    27,    24,    28,    62,
      63,    62,    63,    62,    63,    33,    14,    27,    67,    28,
      29,    38,    68,    15,    66,    64,     1,    65,    14,    70,
      69,    44,    45,    14,    35,    59,    44,    45,    14,    46,
      14,    30,     4,     5,    46,    23,    20,    36,    21,    14,
      30,    54,    55,    10,    11,    43,    12,    25,    41,    39,
      73,    61,    74,    53,    13,    37,    71,     0,     0,    60
};

static const short yycheck[] =
{
      10,    45,     3,    46,     5,     5,     3,    17,     5,     3,
       4,     3,     4,     3,     4,    25,     9,     3,    61,     5,
      21,    31,    66,    16,    21,    19,     6,    19,     9,    19,
      16,    12,    13,     9,    11,    16,    12,    13,     9,    20,
       9,    10,     7,     8,    20,    16,     3,    16,     5,     9,
      10,     3,     4,    15,     5,     3,    20,    15,    17,    15,
       0,    15,     0,    18,     6,    31,    19,    -1,    -1,    48
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


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
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 2:
#line 54 "core/bif.y"
{
   BNSetName(bn, yyvsp[-2].string);
;
    break;}
case 11:
#line 65 "core/bif.y"
{bnn = BNNewNode(bn, yyvsp[0].string); ;
    break;}
case 18:
#line 75 "core/bif.y"
{
   int i;

   for(i = 0 ; i < VLLength(stringList) ; i++) {
      BNNodeAddValue(bnn, (char *)VLIndex(stringList, i));
   }

   VLFree(stringList);
   stringList = VLNew();
;
    break;}
case 19:
#line 86 "core/bif.y"
{
   //printf("parsed property\n");
;
    break;}
case 21:
#line 93 "core/bif.y"
{
   int i;
   BeliefNetNode child, parent;


   child = BNLookupNode(bn, (char *)VLIndex(stringList, 0));

   for(i = 1 ; i < VLLength(stringList) ; i++) {
      parent = BNLookupNode(bn, (char *)VLIndex(stringList, i));

      BNNodeAddParent(child, parent);
   }

   BNNodeInitCPT(child);
   currentNode = child;

   for(i = 0 ; i < ILLength(stringList) ; i++) {
      MFreePtr(VLIndex(stringList, i));
   }
   VLFree(stringList);
   stringList = VLNew();
;
    break;}
case 22:
#line 116 "core/bif.y"
{ seenProbabilityEntry = 0; ;
    break;}
case 30:
#line 126 "core/bif.y"
{
  if(seenProbabilityEntry) {
      printf("WARNING default seen after a specific entry\n");
   }

   FLFree(floatList);
   floatList = FLNew();
   printf("WARNING parsed default but didn't interperate it\n");
;
    break;}
case 31:
#line 137 "core/bif.y"
{
   BeliefNetNode parent;
   int value, i;
   ExamplePtr e = ExampleNew(BNGetExampleSpec(bn));

   seenProbabilityEntry = 1;

   for(i = 0 ; i < VLLength(stringList) ; i++) {
      parent = BNNodeGetParent(currentNode, i);
      value = BNNodeLookupValue(parent, (char *)VLIndex(stringList, i));
      ExampleSetDiscreteAttributeValue(e, BNNodeGetID(parent), value);
   }

   for(i = 0 ; i < BNNodeGetNumValues(currentNode) ; i++){
      ExampleSetDiscreteAttributeValue(e, BNNodeGetID(currentNode), i);
      BNNodeSetCP(currentNode, e, FLIndex(floatList, i));
   }

   /* free all memory and reset lists */
   ExampleFree(e);

   for(i = 0 ; i < ILLength(stringList) ; i++) {
      MFreePtr(VLIndex(stringList, i));
   }
   VLFree(stringList);
   stringList = VLNew();

   FLFree(floatList);
   floatList = FLNew();
;
    break;}
case 32:
#line 168 "core/bif.y"
{
   if(seenProbabilityEntry) {
      printf("WARNING table seen after a specific entry\n");
   }

   _SetProbsFromTable(currentNode, floatList);

   FLFree(floatList);
   floatList = FLNew();
;
    break;}
case 33:
#line 179 "core/bif.y"
{
              VLAppend(stringList, yyvsp[0].string);
           ;
    break;}
case 34:
#line 182 "core/bif.y"
{ 
              VLAppend(stringList, yyvsp[0].string);
           ;
    break;}
case 35:
#line 185 "core/bif.y"
{
              char tmp[100], *theString;

              //sprintf(tmp, "%d\0", $2);
              sprintf(tmp, "%d", yyvsp[0].integer);
              theString = MNewPtr(sizeof(char) * (strlen(tmp) + 1));
              sprintf(theString, "%d", yyvsp[0].integer);
              //sprintf(theString, "%d\0", $2);
              
              VLAppend(stringList, theString);
           ;
    break;}
case 36:
#line 196 "core/bif.y"
{ 
              char tmp[100], *theString;

              sprintf(tmp, "%d", yyvsp[0].integer);
              //sprintf(tmp, "%d\0", $1);
              theString = MNewPtr(sizeof(char) * (strlen(tmp) + 1));
              sprintf(theString, "%d", yyvsp[0].integer);
              //sprintf(theString, "%d\0", $1);
              
              VLAppend(stringList, theString);
           ;
    break;}
case 37:
#line 208 "core/bif.y"
{ 
              FLAppend(floatList, yyvsp[0].f);
           ;
    break;}
case 38:
#line 211 "core/bif.y"
{ 
              FLAppend(floatList, yyvsp[0].f);
           ;
    break;}
case 39:
#line 214 "core/bif.y"
{ 
              FLAppend(floatList, (float)yyvsp[0].integer);
           ;
    break;}
case 40:
#line 217 "core/bif.y"
{ 
              FLAppend(floatList, (float)yyvsp[0].integer);
           ;
    break;}
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 220 "core/bif.y"


void _SetProbsFromTable(BeliefNetNode bnn, FloatListPtr fList) {
   BeliefNetNode parent;
   int i, j, incrementDone, value;
   ExamplePtr e = ExampleNew(BNGetExampleSpec(bn));


   for(i = 0 ; i < BNNodeGetNumParents(bnn) ; i++) {
      ExampleSetDiscreteAttributeValue(e, BNNodeGetParentID(bnn, i), 0);
   }
   ExampleSetDiscreteAttributeValue(e, BNNodeGetID(bnn), 0);

   for(i = 0 ; i < FLLength(fList) ; i++) {
      BNNodeSetCP(bnn, e, FLIndex(fList, i));

      incrementDone = 0;
      for(j = BNNodeGetNumParents(bnn) - 1 ; j >= 0 && !incrementDone ; j--) {
         parent = BNNodeGetParent(bnn, j);
         value = ExampleGetDiscreteAttributeValue(e, BNNodeGetID(parent));

         if((value + 1) < BNNodeGetNumValues(parent)) {
            /* +1 in the if corrects for zero indexing in the values */

            ExampleSetDiscreteAttributeValue(e, BNNodeGetParentID(bnn, j),
                                                                   value + 1);
            incrementDone = 1;
         } else {
            /* this attribute is used up, spill over to the next one */
            ExampleSetDiscreteAttributeValue(e, BNNodeGetParentID(bnn, j), 0);
         }
      }

      if(!incrementDone) {
         /* changed all parents, need to change node value */
         value = ExampleGetDiscreteAttributeValue(e, BNNodeGetID(bnn));

         if((value + 1) < BNNodeGetNumValues(bnn)) {
            /* +1 in the if corrects for zero indexing in the values */

            ExampleSetDiscreteAttributeValue(e, BNNodeGetID(bnn),
                                                             value + 1);
         } else {
            /* we already got a value for entry in the CPT */
            ExampleFree(e);
            return;
         }
      }
   }

   /* free all memory and reset lists */
   ExampleFree(e);
}

void BIFSetFile(FILE *file);

int biferror(const char *msg) {
   fprintf(stderr, "%s line %d\n", msg, gLineNumber);
   return 0;
}

//BeliefNet BIFParse(const char *file) {
BeliefNet BIFParse(FILE *input) {
   BIFSetFile(input);

   bn = BNNew();
   //bnn = BNNodeNew();

   floatList = FLNew();
   stringList = VLNew();

   gLineNumber = 0;

   if(bifparse()) {
      /* parse failed! */
      fprintf(stderr, "Error in parsing belief net\n");
   }

   //fclose(input);

   /* free extra bnn, and lists */
   VLFree(stringList);
   FLFree(floatList);

   return bn;
}
