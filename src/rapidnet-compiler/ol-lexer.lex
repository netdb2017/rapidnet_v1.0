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

%option c++
%option yylineno
%option noyywrap
%option yyclass="OlLexer"
%option prefix="OLBase"
%start CCOMMENT CSTRING
%{
#include "ol-lexer.h"

#include <limits.h>
#include <stdlib.h>

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet_compiler;

#ifdef YY_DECL
#undef YY_DECL
#endif
#define YY_DECL int OlLexer::yylex (YYSTYPE *lvalp, OlContext *ctxt)

int dcvar = 0;

%}

DIGIT           [0-9]
EXP		[eE][+-]?{DIGIT}+
DECIM		\.{DIGIT}+
ALNUM		[_0-9a-zA-Z]
HEXDIGIT	[0-9a-fA-F]

VARIABLE	[A-Z]{ALNUM}*
LOC_VARIABLE	_{
QATOM		\'[^\']*\'
WHITESPACE	[ \t\r\n]+

%%

<INITIAL><<EOF>> { return OL_EOF; }
<INITIAL>{WHITESPACE}		;
<INITIAL>%%.*			; // Ignore %% comments
<INITIAL>\/\/.*			; // Ignore // comments
<INITIAL>^#!.*			; // Ignore '#' directives

<INITIAL>\/\* { 
  if ( comment_depth == 0 ) {
    BEGIN(CCOMMENT); 
  }
  ++comment_depth;
}
 
<CCOMMENT>[^\*]|(\*[^/]) ;
<CCOMMENT>\*+\/	{
  if ( comment_depth > 0 ) {
   if( --comment_depth == 0 ) {
     BEGIN(INITIAL);
   }
 } else {
   REJECT;
 }
}
<INITIAL>\" { 
  assert(cstring == NULL);
  cstring = new ostringstream();
  BEGIN(CSTRING); 
}
<CSTRING>\" { 
  assert(cstring != NULL);
  lvalp->v = new ParseVal(ValStr::New(cstring->str()));
  delete cstring;
  cstring = NULL;
  BEGIN(INITIAL); 
  return OL_STRING; 
}
<CSTRING>\\.	{
  assert(cstring != NULL);
  // An escaped character literal
  switch (yytext[1]) {
  case 'n': (*cstring) << "\n"; break;
  case 'r': (*cstring) << "\r"; break;
  case 't': (*cstring) << "\t"; break;
  case 'f': (*cstring) << "\f"; break;
  default: (*cstring) << yytext[1]; break;
  }
}
<CSTRING>[^\\\"]+ { 
    assert(cstring != NULL);
    (*cstring) << yytext; 
}

<INITIAL>materialize { return OL_MATERIALIZE; }
<INITIAL>keys { return OL_KEYS; }
<INITIAL>says { return OL_SAYS; }
<INITIAL>encrypts { return OL_ENCRYPTS; }
<INITIAL>At { return OL_CONTEXT; }
<INITIAL>in { return OL_IN; }
<INITIAL>id { return OL_ID; }
<INITIAL>"@" { return OL_AT; }
<INITIAL>"," { return OL_COMMA; }
<INITIAL>"(" { return OL_LPAR; }
<INITIAL>")" { return OL_RPAR; }
<INITIAL>"[" { return OL_LSQUB; }
<INITIAL>"]" { return OL_RSQUB; }
<INITIAL>"{" { return OL_LCURB; }
<INITIAL>"}" { return OL_RCURB; }
<INITIAL>":" { return OL_COLON; }

 /* Relational operators */
<INITIAL>"<" { return OL_LT; }
<INITIAL>">" { return OL_GT; }
<INITIAL>"<=" { return OL_LTE; }
<INITIAL>">=" { return OL_GTE; }
<INITIAL>"!=" { return OL_NEQ; }
<INITIAL>"<>" { return OL_NEQ; }
<INITIAL>"==" { return OL_EQ; }

 /* Arithmetic operations */
<INITIAL>"+" { return OL_PLUS; }
<INITIAL>"-" { return OL_MINUS; }
<INITIAL>"*" { return OL_TIMES; }
<INITIAL>"/" { return OL_DIVIDE; }
<INITIAL>"%" { return OL_MODULUS; }
<INITIAL>"^" { return OL_BITXOR; }
<INITIAL>"&" { return OL_BITAND; }
<INITIAL>"|" { return OL_BITOR; }
<INITIAL>"~" { return OL_BITNOT; }
<INITIAL>">>" { return OL_RSHIFT; }
<INITIAL>"<<" { return OL_LSHIFT; }

 /* Boolean operations */
<INITIAL>"!" { return OL_NOT; }
<INITIAL>"&&" { return OL_AND; }
<INITIAL>"||" { return OL_OR; } 

<INITIAL>":=" { return OL_ASSIGN; }
<INITIAL>"." { return OL_DOT; }
<INITIAL>":-" { return OL_IF; }
<INITIAL>"watch" { return OL_WATCH; }
<INITIAL>"watchmod" { return OL_WATCHFINE; }
<INITIAL>"stage" { return OL_STAGE; }
<INITIAL>"TraceTable" {return OL_TRACETABLE;}
<INITIAL>"trace" {return OL_TRACE;}
<INITIAL>"delete" { return OL_DEL; }
<INITIAL>"Query" { return OL_QUERY; }
<INITIAL>"null" { 
  lvalp->v = new ParseVal(ValNull::New()); 
  return OL_NULL; }

<INITIAL>a_[a-zA-Z0-9]+ { 
  string aggName(yytext);
  lvalp->v = new ParseVar(ValStr::New(aggName.substr(2))); 
  return OL_AGGFUNCNAME;
 }

<INITIAL>f_[a-zA-Z0-9]+ { 
  lvalp->v = new ParseVar(ValStr::New(yytext)); 
  return OL_FUNCTION; }

<INITIAL>[A-Z]{ALNUM}* { 
  lvalp->v = new ParseVar(ValStr::New(yytext)); 
  return OL_VAR; }

<INITIAL>_ { 
  ostringstream oss;
  oss << "$_" << dcvar++; 
  lvalp->v = new ParseVar(ValStr::New(oss.str())); 
  return OL_VAR; }

<INITIAL>infinity {
  // Unsigned integer literal (including octal and/or hex)
  lvalp->v = new ParseVal(ValInt32::New(-1));
  return OL_VALUE;
}

<INITIAL>[a-z]{ALNUM}* { 
  lvalp->v = new ParseVal(ValStr::New(yytext)); 
  return OL_NAME; 
}

<INITIAL>({DIGIT}+|0[xX]{HEXDIGIT}+)U {
  // Unsigned integer literal (including octal and/or hex)
  lvalp->v = new ParseVal(ValUInt32::New(strtoull(yytext,NULL,0)));
  return OL_VALUE;
}

<INITIAL>(-?{DIGIT}+|0[xX]{HEXDIGIT}+) {
  // Some integer literal (including octal and/or hex)
  lvalp->v = new ParseVal(ValInt32::New(strtoll(yytext,NULL,0)));
  return OL_VALUE;
}

<INITIAL>-?{DIGIT}+(({DECIM}{EXP}?)|{EXP}) {
  // Double-precision literal
  lvalp->v = new ParseVal(ValDouble::New(strtod(yytext,NULL)));
  return OL_VALUE;
}

<INITIAL>0[xX]{HEXDIGIT}+I {
  // IDs are read in only in hexadecimal with an I appended to the end
  string hex(yytext);
  string choppedString = hex.substr(2, hex.size() - 3);
  ParseVal *val = new ParseVal (ValId::New(choppedString));
  val->Id (true);
  lvalp->v = val;
  return OL_VALUE;
}

%%

// Default: yyin == cin.
OlLexer::OlLexer(istream *str) 
  : comment_depth(0), cstring(NULL) 
{
  bufstate = yy_create_buffer( str , YY_BUF_SIZE);
  bufstate->yy_is_our_buffer = 0;
  yy_switch_to_buffer( bufstate );
};

// Give it a string...
OlLexer::OlLexer(const char *prog) 
  : comment_depth(0), cstring(NULL), strb(prog)
{
  bufstate = yy_create_buffer( &strb, strlen(prog));
  yy_switch_to_buffer( bufstate );
};

OlLexer::~OlLexer() { 
  // if (bufstate) yy_delete_buffer(bufstate); 
};
