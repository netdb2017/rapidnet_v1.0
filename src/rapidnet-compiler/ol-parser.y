%{
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

%}
%debug
%defines
%verbose
%pure-parser

%left OL_OR
%left OL_AND
%left OL_BITOR
%left OL_BITXOR
%left OL_BITAND
%left OL_BITNOT
%left OL_EQ OL_NEQ
%nonassoc OL_GT OL_GTE OL_LT OL_LTE
%left OL_LSHIFT OL_RSHIFT
%left OL_PLUS OL_MINUS 
%left OL_TIMES OL_DIVIDE OL_MODULUS
%right OL_NOT
%left OL_IN
%left OL_ID
%nonassoc OL_ASSIGN


// used to disambiguate aggregates from relative expressions
%token OL_AT 
%token<v> OL_NAME 
%token OL_COMMA
%token OL_DOT
%token OL_EOF 
%token OL_IF 
%token<v> OL_STRING
%token<v> OL_VALUE
%token<v> OL_VAR
%token<v> OL_AGGFUNCNAME
%token<v> OL_FUNCTION
%token<v> OL_NULL
%token OL_RPAR
%token OL_LPAR
%token OL_LSQUB
%token OL_RSQUB
%token OL_LCURB
%token OL_RCURB
%token OL_COLON
%token OL_DEL
%token OL_QUERY
%token OL_MATERIALIZE
%token OL_KEYS
%token OL_SAYS
%token OL_ENCRYPTS
%token OL_CONTEXT
%token OL_WATCH
%token OL_WATCHFINE
%token OL_STAGE
%token OL_TRACE
%token OL_TRACETABLE

%start program
%file-prefix "ol-parser"
%name-prefix "ol_parser_"
%parse-param { OlContext *ctxt }
%lex-param { OlContext *ctxt }
%union {

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

%type<u_termlist>    termlist;
%type<u_exprlist>    functorbody functorargs functionargs primarykeys keylist; 
%type<u_functorname> functorname;
%type<u_term>        term functor assign select;
%type<u_aggterm>     aggview;
%type<v>             functorarg functionarg tablearg atom rel_atom math_atom function math_expr bool_expr range_expr range_atom aggregate;
%type<u_boper>       rel_oper;
%type<u_moper>       math_oper;
%type<u_aoper>       agg_oper;
%type<u_key>         key;
%type<u_rule>        rule namedRule unnamedRule weightedRule unweightedRule;
%%

program:	OL_EOF { YYACCEPT; }
		| clauselist OL_EOF { YYACCEPT; }
		;

clauselist:	clause
		| clause clauselist
                ;

clause:		  rule
		| fact
                | materialize
                | watch
                | watchfine
		| trace
                | TraceTable
                | query
                | stage
                | context
		;

materialize:	OL_MATERIALIZE OL_LPAR OL_NAME OL_COMMA
		tablearg OL_COMMA tablearg OL_COMMA primarykeys OL_RPAR OL_DOT 
			{ ctxt->table($3, $5, $7, $9); } 
		;

tablearg:	OL_VALUE
			{ $$ = $1; }
		;

primarykeys:	OL_KEYS OL_LPAR keylist OL_RPAR {
			$$ = $3;
		}
		| OL_KEYS OL_LPAR OL_RPAR {
			$$ = NULL; // This is going to be KeyID
		}
		;

keylist:	key { $$ = new ParseExprList(); $$->push_front($1); }
		| 
		key OL_COMMA keylist { $3->push_front($1); $$=$3; }
		;

key:            OL_VALUE OL_COLON OL_NAME { $$ = new ParseKey ($1, $3); }
                |
                OL_VALUE { $$ = new ParseKey ($1); }
                ;

watch:		OL_WATCH OL_LPAR OL_NAME OL_RPAR OL_DOT {
                ctxt->Watch($3, ""); /* no modifiers */
		}
		;

watchfine:	OL_WATCHFINE OL_LPAR OL_NAME OL_COMMA OL_STRING OL_RPAR OL_DOT {
                ctxt->Watch($3, $5->ToString()); /* With modifiers */
		}
		;

stage:		OL_STAGE OL_LPAR OL_STRING OL_COMMA OL_NAME OL_COMMA OL_NAME OL_RPAR OL_DOT {
			ctxt->Stage($3,$5,$7);
		}
		;

trace:		OL_TRACE OL_LPAR OL_NAME OL_RPAR OL_DOT {
			ctxt->TraceTuple($3);
		}
		;

TraceTable:	OL_TRACETABLE OL_LPAR OL_NAME OL_RPAR OL_DOT {
			ctxt->TraceTable($3);
		}
		;

fact:           functor OL_DOT { ctxt->Fact($1); }
                ;

rule:	        namedRule
                { ctxt->AddRule ($1); }
                |
                unnamedRule
                { ctxt->AddRule ($1); }
                ;
				
namedRule:      OL_NAME unnamedRule
                { ((OlContext::Rule*) $2->GetRule ())->SetName ($1); $$ = $2; }
                ;			
unnamedRule:	weightedRule 
				{ $$ = $1; }
				|
				unweightedRule 
				{ $$ = $1; }
				;

weightedRule:	OL_VALUE unweightedRule 
				{ ((OlContext::Rule*) $2->GetRule ())->SetWeight ($1); $$ = $2; }
				;

unweightedRule:	functor OL_IF termlist OL_DOT
                { $$ = new ParseRule (ctxt->CreateRule($1, $3, false)); }

                | OL_DEL functor OL_IF termlist OL_DOT
                { $$ = new ParseRule (ctxt->CreateRule($2, $4, true)); }

                | functor OL_IF OL_DEL termlist OL_DOT
                { $$ = new ParseRule (ctxt->CreateRule($1, $4, false, NULL, NULL, true)); }
      
                | OL_DEL functor OL_DEL OL_IF termlist OL_DOT
                { $$ = new ParseRule (ctxt->CreateRule($2, $5, true, NULL, NULL, true)); }

                | functor OL_IF aggview OL_DOT
                { $$ = new ParseRule (ctxt->CreateAggRule($1, $3, false)); }
                ;

context:        OL_CONTEXT OL_VAR OL_COLON  
                { ctxt->SetContext($2); }
                ;

query:          OL_QUERY functorname functorbody OL_DOT {
                  ctxt->Query(new ParseFunctor($2, $3)); }
                ;

termlist:	term { $$ = new ParseTermList(); $$->push_front($1); }
		| term OL_COMMA termlist { $3->push_front($1); $$=$3; } 
		;

term:		functor | assign | select { $$=$1; }
		;

functor:	functorname functorbody 
		     { $$=new ParseFunctor($1, $2); } 

                | OL_VAR OL_SAYS functorname functorbody {
			$$=new ParseFunctor($3, $4, NULL, $1, true); }

                | OL_ENCRYPTS functorname functorbody {
		  $$=new ParseFunctor($2, $3, NULL, NULL, false, true); }

     		| OL_ENCRYPTS OL_VAR OL_SAYS functorname functorbody {
      			$$=new ParseFunctor($4, $5, NULL, $2, true, true); }     

                | functorname functorbody OL_AT OL_VAR 
		    { $$=new ParseFunctor($1, $2, $4); } 

	        | OL_VAR OL_SAYS functorname functorbody OL_AT OL_VAR {
			$$=new ParseFunctor($3, $4, $6, $1, true); }

                | OL_ENCRYPTS functorname functorbody OL_AT OL_VAR {
		  $$=new ParseFunctor($2, $3, $5, NULL, false, true); }

     		| OL_ENCRYPTS OL_VAR OL_SAYS functorname functorbody OL_AT OL_VAR {
      			$$=new ParseFunctor($4, $5, $7, $2, true, true); }     
                ;

aggview:        agg_oper OL_LPAR functorbody OL_COMMA functorbody OL_COMMA functor OL_RPAR 
                        { $$ = new ParseAggTerm($1, $3, $5, $7); }
                ;

functorname:	OL_NAME 
			{ $$ = new ParseFunctorName($1); }
		;

functorbody:	OL_LPAR OL_RPAR 
			{ $$=new ParseExprList(); }
		| OL_LPAR functorargs OL_RPAR 
			{ $$=$2; };

functorargs:	functorarg
                        { 
                          $$ = new ParseExprList(); 
                          $$->push_front($1);
                        }
		| functorarg OL_COMMA functorargs {
			$3->push_front($1); 
			$$=$3; }
                | OL_AT atom
                        {
                          $$ = new ParseExprList(); 
                          ParseVar *pv = dynamic_cast<ParseVar*>($2);
                          if (!pv) {
                            ostringstream oss;
                            oss << "location specifier is not a variable";
                            ctxt->ReportError(oss.str());
                          }
                          else {
                            pv->SetLocSpec();
                            $$->push_front($2); 
                          }
                        }
                | OL_AT atom OL_COMMA functorargs
                        {
                          ParseVar *pv = dynamic_cast<ParseVar*>($2);
                          if (!pv) {
                            ostringstream oss;
                            oss << "location specifier is not a variable";
                            ctxt->ReportError(oss.str());
                          }
                          else {
                            pv->SetLocSpec();
                            $4->push_front($2); 
                            $$=$4; 
                          }
                        }
;

functorarg:	atom
			{ $$ = $1; }
		| aggregate
			{
                          $$ = $1;
                        }
		;


function:	OL_FUNCTION OL_LPAR functionargs OL_RPAR
			{ $$ = new ParseFunction($1, $3); }
		| OL_FUNCTION OL_LPAR OL_RPAR 
			{ $$ = new ParseFunction($1, new ParseExprList()); }
		;

functionargs:	functionarg { 
			$$ = new ParseExprList(); 
			$$->push_front($1); }
		| functionarg OL_COMMA functionargs { 
			$3->push_front($1); 
			$$=$3; }
		;

functionarg:	math_expr 
			{ $$ = $1; }
		| atom
			{ $$ = $1; }
		;

select:    	bool_expr
			{ $$ = new ParseSelect($1); }
		;

assign:		OL_VAR OL_ASSIGN rel_atom
			{ $$ = new ParseAssign($1, $3); }
		| OL_VAR OL_ASSIGN bool_expr
			{ $$ = new ParseAssign($1, $3); }
		;

bool_expr:	OL_LPAR bool_expr OL_RPAR 
			{ $$ = $2; }
		| OL_VAR OL_IN range_expr 
			{ $$ = new ParseBool(ParseBool::RANGE, $1, $3); } 
		| OL_NOT bool_expr 
			{ $$ = new ParseBool(ParseBool::NOT, $2 ); } 
		| bool_expr OL_OR bool_expr
			{ $$ = new ParseBool(ParseBool::OR, $1, $3 ); }
		| bool_expr OL_AND bool_expr
			{ $$ = new ParseBool(ParseBool::AND, $1, $3 ); }
		| rel_atom rel_oper rel_atom
			{ $$ = new ParseBool($2, $1, $3 ); }
		;

rel_atom:	math_expr 
			{ $$ = $1; }
		| function
			{ $$ = $1; }
		| atom
			{ $$ = $1; }
		;

rel_oper:	  OL_EQ  { $$ = ParseBool::EQ; } 
		| OL_NEQ { $$ = ParseBool::NEQ; }
		| OL_GT  { $$ = ParseBool::GT; }
		| OL_LT  { $$ = ParseBool::LT; }
		| OL_GTE { $$ = ParseBool::GTE; }
		| OL_LTE { $$ = ParseBool::LTE; }
		;

math_expr:	math_expr math_oper math_atom
			{ $$ = new ParseMath($2, $1, $3 ); }
		| math_atom math_oper math_atom
			{ $$ = new ParseMath($2, $1, $3 ); }
		;

math_atom:	atom 
			{ $$ = $1; }
		| function
			{ $$ = $1; }
		| OL_LPAR math_expr OL_RPAR 
			{ $$ = $2; }
		;

math_oper:	  OL_LSHIFT  { $$ = ParseMath::LSHIFT; } 
		| OL_RSHIFT  { $$ = ParseMath::RSHIFT; }
		| OL_PLUS    { $$ = ParseMath::PLUS; }
		| OL_MINUS   { $$ = ParseMath::MINUS; }
		| OL_TIMES   { $$ = ParseMath::TIMES; }
		| OL_DIVIDE  { $$ = ParseMath::DIVIDE; }
		| OL_MODULUS { $$ = ParseMath::MODULUS; }
		| OL_BITXOR  { $$ = ParseMath::BIT_XOR; }
		| OL_BITAND  { $$ = ParseMath::BIT_AND; }
		| OL_BITOR   { $$ = ParseMath::BIT_OR; }
		| OL_BITNOT  { $$ = ParseMath::BIT_NOT; }
		;


range_expr:	OL_LPAR range_atom OL_COMMA range_atom OL_RPAR 
			{ $$ = new ParseRange(ParseRange::RANGEOO, $2, $4); } 
		| OL_LPAR range_atom OL_COMMA range_atom OL_RSQUB
			{ $$ = new ParseRange(ParseRange::RANGEOC, $2, $4); } 
		| OL_LSQUB range_atom OL_COMMA range_atom OL_RPAR
			{ $$ = new ParseRange(ParseRange::RANGECO, $2, $4); } 
		| OL_LSQUB range_atom OL_COMMA range_atom OL_RSQUB
			{ $$ = new ParseRange(ParseRange::RANGECC, $2, $4); } 
		;

range_atom:	math_expr
			{ $$ = $1; }
		| atom
			{ $$ = $1; }
		;

atom:		OL_VALUE | OL_VAR | OL_STRING | OL_NULL
			{ $$ = $1; }
		;

aggregate:	agg_oper OL_LT OL_VAR OL_GT 
                        { $$ = new ParseAgg($3, $1, ValuePtr()); }
		|
		agg_oper OL_LT OL_AT OL_VAR OL_GT 
                        {
                          // Make the variable a location specifier
                          ParseVar *pv = dynamic_cast<ParseVar*>($4);
                          pv->SetLocSpec();
                          $$ = new ParseAgg($4, $1, ValuePtr());
                        }
		|
		agg_oper OL_LT OL_TIMES OL_GT 
			{ $$ = new ParseAgg(ParseAgg::DONT_CARE, $1, ValuePtr()); }
		;

agg_oper:	OL_AGGFUNCNAME
                {
                  $$ = $1->value->ToString ().c_str ();
                }
;

%%

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

