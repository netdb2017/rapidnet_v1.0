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

#ifndef __PARSER_UTIL_H__
#define __PARSER_UTIL_H__

#include <deque>
#include <iostream>
#include <map>
#include "all-values.h"
#include <string>
#include <typeinfo>

#include "ns3/log.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet_compiler {

class OlLexer;
class OlContext;

inline bool fieldNameEq (string n1, string n2)
{
  if ( (n1[0] == '@' && n2[0] == '@') || (n1[0] != '@' && n2[0] != '@'))
    return (n1 == n2);
  else if (n1[0] == '@' && n2[0] != '@')
    return ( (n1.substr (1)) == n2);
  else
    return ( (n2.substr (1)) == n1);
}

/**
 * \ingroup rapidnet_compiler
 *
 * \brief A ParseExpr represents expressions in the Overlog language
 */
class ParseExpr
{
public:
  ParseExpr () :
    m_position (-1)
  {
  }

  ParseExpr (ValuePtr val) :
    value (val), m_position (-1)
  {
  }

  ParseExpr (ParseExpr* val) :
    value (val->value), m_position (-1)
  {
  }

  virtual ~ParseExpr ()
  {
  }

  virtual bool operator== (const ParseExpr &e)
  {
    return value == e.value;
  }

  virtual string ToString () = 0;

  virtual void Position (int p)
  {
    m_position = p;
  }

  virtual int Position ()
  {
    return m_position;
  }

  ValuePtr value;

protected:
  int m_position;
};

typedef deque<ParseExpr*> ParseExprList;

typedef deque<ParseExprList*> ParseExprListList;

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Boxing up a ValuePtr so we can pass it through the Bison parser.
 */
class ParseVal: public ParseExpr
{
public:
  ParseVal (ValuePtr val) :
    ParseExpr (val), m_id (false)
  {
  }

  virtual void Id (bool i)
  {
    m_id = i;
  }

  virtual bool Id ()
  {
    return m_id;
  }

  virtual string ToString ()
  {
    return value->ToString ();
  }

  virtual ValuePtr Value ()
  {
    return value;
  }

protected:

  bool m_id;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseVar: public ParseExpr
{
public:
  ParseVar (ValuePtr var);

  ParseVar (const string& var);

  /** Returns the location specifier marker along with the name of the
   variable */
  string ToLocString ();

  /** Returns the name of the variable */
  string ToString ();

  bool LocSpec ();

  void SetLocSpec ();

  void CancelLocSpec ();


protected:

  bool m_locSpec;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseAgg: public ParseExpr
{
public:
  string oper;

  static ParseExpr* DONT_CARE;

  ParseAgg (ParseExpr* v, string o, ValuePtr p) :
    ParseExpr (v), oper (o), parameter (p)
  {
    ParseVar *pv = dynamic_cast<ParseVar*> (v);
    if (pv != NULL)
      {
        m_locSpec = pv->LocSpec ();
      }
    else
      {
        m_locSpec = false;
      }
  }

  virtual bool operator== (const ParseExpr &e);

  virtual string ToString ();
  virtual string AggName ();

  // The parameter of the aggregate, if it's parametric
  ValuePtr parameter;

  /** Am I the loc spec? */
  bool m_locSpec;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseBool: public ParseExpr
{
public:
  enum Operator
  {
      NOT,
      AND,
      OR,
      EQ,
      NEQ,
      GT,
      LT,
      LTE,
      GTE,
      RANGE,
      NOP
  };

  ParseBool (Operator o, ParseExpr *l, ParseExpr *r = NULL, bool id = false);
  virtual ~ParseBool ()
  {
    delete lhs;
    if (rhs)
      delete rhs;
  }

  virtual bool operator== (const ParseExpr &e);

  virtual string ToString ();

  Operator oper;
  ParseExpr *lhs;
  ParseExpr *rhs;
  bool m_id;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseRange: public ParseExpr
{
public:
  enum Interval
  {
      RANGEOO,
      RANGEOC,
      RANGECO,
      RANGECC
  };

  ParseRange (Interval i, ParseExpr *l, ParseExpr *r) :
    type (i), lhs (l), rhs (r)
  {
  }

  virtual bool operator== (const ParseExpr &e);

  virtual string ToString ();

  Interval type;
  ParseExpr *lhs;
  ParseExpr *rhs;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseMath: public ParseExpr
{
public:
  enum Operator
  {
      LSHIFT,
      RSHIFT,
      PLUS,
      MINUS,
      TIMES,
      DIVIDE,
      MODULUS,
      BIT_XOR,
      BIT_AND,
      BIT_OR,
      BIT_NOT,
      NOP
  };
  ParseMath (Operator o, ParseExpr *l, ParseExpr *r = NULL, bool i = false) :
    oper (o), id (i), lhs (l), rhs (r)
  {
    // TODO: if (oper != NOP && rhs == NULL) ERROR!
  }

  ~ParseMath ()
  {
    delete lhs;
    if (rhs)
      delete rhs;
  }

  virtual bool operator== (const ParseExpr &e);

  virtual string ToString ();

  //virtual ValuePtr value();

  Operator oper;
  bool id;
  ParseExpr *lhs;
  ParseExpr *rhs;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseFunction: public ParseExpr
{
public:
  ParseFunction (ParseExpr *n, ParseExprList *a) :
    ParseExpr (n), m_args (a)
  {
  }

  ~ParseFunction ()
  {
    delete m_args;
  }

  virtual string ToString ();

  string Name ()
  {
    return value->ToString ();
  }

  void Arg (ParseExpr *arg)
  {
    m_args->push_back (arg);
  }

  ParseExpr* Arg (int i)
  {
    return m_args->at (i);
  }

  int Args ()
  {
    return m_args->size ();
  }

  ParseExprList* m_args;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseTerm
{
public:
  virtual ~ParseTerm ()
  {
  }

  virtual string ToString () = 0;

  void Position (int p)
  {
    m_position = p;
  }

  int Position ()
  {
    return m_position;
  }

  int m_position;
};

typedef deque<ParseTerm *> ParseTermList;

/**
 * \ingroup rapidnet_compiler
 */
class ParseFunctorName
{
public:
  ParseFunctorName (ParseExpr *n);

  string ToString ();

  string name;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseFunctor: public ParseTerm
{
public:
  ParseFunctor (ParseFunctorName* f, ParseExprList* a, ParseExpr* dest = NULL,
    ParseExpr* sayer = NULL, bool auth = false, bool encrypt = false);

  virtual ~ParseFunctor ()
  {
    delete fName;
    delete m_args;
  }

  virtual string ToString ();

  /** If this functor contains an aggregate, return its field position,
   otherwise -1 */
  int Aggregate ();

  /** If this functor contains an aggregate, return its operator name.*/
  string AggregateOperator ();

  int Find (ParseExpr *arg);

  int Find (string argname);

  void arg (ParseExpr *arg)
  {
    m_args->push_back (arg);
  }

  ParseExpr* Arg (int i)
  {
    return m_args->at (i);
  }

  int Args ()
  {
    return m_args->size ();
  }

  string GetLocSpec ();

  /**return location specifier without @ */
  string GetLocSpecValue ();

  void replace (int p, ParseExpr *e);

  /** The name of this tuple */
  ParseFunctorName* fName;

  ParseExprList *m_args;

  string m_loc;

  /**
   * \brief SeNDlog: The destination for outgoing tuple in the rule header.
   */
  ValuePtr m_dest;

  /**
   * \brief SeNDlog: Whether this tuple is authenticated
   */
  bool m_auth;

  /**
   * \brief SeNDlog: The principle who should sign the authenticated tuple
   */
  ParseExpr* m_sayer;

  /**
   * \brief SeNDlog: Whether this tuple is encrypted
   */
  bool m_encrypt;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseAssign: public ParseTerm
{
public:
  ParseAssign (ParseExpr *v, ParseExpr *a) :
    var (NULL), assign (a)
  {
    var = dynamic_cast<ParseVar*> (v);
  }

  virtual ~ParseAssign ()
  {
    delete var;
    delete assign;
  }

  virtual string ToString ();

  ParseVar *var;
  ParseExpr *assign;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseSelect: public ParseTerm
{
public:
  ParseSelect (ParseExpr *s)
  {
    select = dynamic_cast<ParseBool*> (s);
  }

  virtual ~ParseSelect ()
  {
    delete select;
  }

  virtual string ToString ();

  ParseBool *select;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseAggTerm: public ParseTerm
{
public:
  ParseAggTerm (string oper, ParseExprList *groupByFields,
    ParseExprList *aggFields, ParseTerm *baseTerm) :
    m_groupByFields (groupByFields), m_aggFields (aggFields),
        m_baseTerm (baseTerm), m_oper (oper)
  {
  }

  ParseExprList *m_groupByFields;
  ParseExprList *m_aggFields;
  ParseTerm *m_baseTerm;
  string m_oper;
  virtual string ToString ();
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseKey: public ParseExpr
{
public:
  ParseKey (ParseExpr* attr);

  ParseKey (ParseExpr* attr, ParseExpr* type);

  string ToString ();

  string GetType ();

  ParseVal*
  getAttribute ();

private:
  /* The key attribute is identified by numeric location. */
  ParseVal* m_attr;

  string m_type;
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseRule: public ParseExpr
{
public:
  ParseRule (void *rule)
  {
    m_rule = rule;
  }

  void* GetRule ()
  {
    return m_rule;
  }

  string ToString ();

private:
  void* m_rule;
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif /* __PARSER_UTIL_H__ */
