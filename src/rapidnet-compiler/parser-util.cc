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

#ifndef __PARSER_UTIL_C__
#define __PARSER_UTIL_C__

#include "parser-util.h"
#include "ol-context.h"

using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("ParserUtil");

ParseVar::ParseVar (ValuePtr var) :
  ParseExpr (var), m_locSpec (false)
{
}

ParseVar::ParseVar (const string& var) :
  ParseExpr (ValStr::New (var)), m_locSpec (false)
{
}

string ParseVar::ToLocString ()
{
  if (!LocSpec ())
    {
      return value->ToString ();
    }
  else
    {
      return ("@" + value->ToString ());
    }
}

string ParseVar::ToString ()
{
  return value->ToString ();
}

bool ParseVar::LocSpec ()
{
  return m_locSpec;
}

void ParseVar::SetLocSpec ()
{
  m_locSpec = true;
}

void ParseVar::CancelLocSpec ()
{
  m_locSpec = false;
}

//=====================================

ParseExpr* ParseAgg::DONT_CARE = new ParseVar (ValStr::New ("*"));

bool ParseAgg::operator== (const ParseExpr &e)
{
  try
    {
      const ParseAgg& a = dynamic_cast<const ParseAgg&> (e);
      return ParseExpr::operator== (e) && oper == a.oper;
    }
  catch (bad_cast e)
    {
      return false;
    }
}

string ParseAgg::ToString ()
{
  ostringstream a;
  a << oper << "< ";
  a << value->ToString () << " >";
  return a.str ();
}

string ParseAgg::AggName ()
{
  return oper;
}

ParseBool::ParseBool (ParseBool::Operator o, ParseExpr *l, ParseExpr *r,
  bool id) :
  oper (o), lhs (l), rhs (r), m_id (id)
{
  // TODO: if (oper != NOT && rhs == NULL) ERROR!
}

bool ParseBool::operator== (const ParseExpr &e)
{
  try
    {
      const ParseBool& b = dynamic_cast<const ParseBool&> (e);
      return *lhs == *b.lhs && *rhs == *b.rhs && oper == b.oper;
    }
  catch (bad_cast e)
    {
      return false;
    }
}

string ParseBool::ToString ()
{
  ostringstream b;
  if (oper == NOT)
    {
      b << "!(" << lhs->ToString () << ")";
    }
  else if (oper == RANGE)
    {
      b << lhs->ToString () << " in " << rhs->ToString ();
    }
  else
    {
      if (dynamic_cast<ParseBool*> (lhs) != NULL)
        b << "(";
      b << lhs->ToString ();
      if (dynamic_cast<ParseBool*> (lhs) != NULL)
        b << ")";
      switch (oper)
      {
      case AND:
        b << " && ";
        break;
      case OR:
        b << " || ";
        break;
      case EQ:
        b << " == ";
        break;
      case NEQ:
        b << " != ";
        break;
      case GT:
        b << " > ";
        break;
      case LT:
        b << " < ";
        break;
      case LTE:
        b << " <= ";
        break;
      case GTE:
        b << " >= ";
        break;
      default:
        assert(0);
      }
      if (dynamic_cast<ParseBool*> (rhs) != NULL)
        b << "(";
      b << rhs->ToString ();
      if (dynamic_cast<ParseBool*> (rhs) != NULL)
        b << ")";
    }

  return b.str ();
}

bool ParseRange::operator== (const ParseExpr &e)
{
  try
    {
      const ParseRange& r = dynamic_cast<const ParseRange&> (e);
      return *lhs == *r.lhs && *rhs == *r.rhs && type == r.type;
    }
  catch (bad_cast e)
    {
      return false;
    }
}

string ParseRange::ToString ()
{
  ostringstream r;
  switch (type)
  {
  case RANGEOO:
    r << "(" << lhs->ToString () << ", " << rhs->ToString () << ")";
    break;
  case RANGEOC:
    r << "(" << lhs->ToString () << ", " << rhs->ToString () << "]";
    break;
  case RANGECO:
    r << "[" << lhs->ToString () << ", " << rhs->ToString () << ")";
    break;
  case RANGECC:
    r << "[" << lhs->ToString () << ", " << rhs->ToString () << "]";
    break;
  }
  return r.str ();
}

bool ParseMath::operator== (const ParseExpr &e)
{
  try
    {
      const ParseMath& m = dynamic_cast<const ParseMath&> (e);
      return *lhs == *m.lhs && *rhs == *m.rhs && oper == m.oper;
    }
  catch (bad_cast e)
    {
      return false;
    }
}

string ParseMath::ToString ()
{
  ostringstream m;
  bool lpar = (dynamic_cast<ParseMath*> (lhs) != NULL);
  bool rpar = (dynamic_cast<ParseMath*> (rhs) != NULL);

  if (lpar)
    m << "(";
  m << lhs->ToString ();
  if (lpar)
    m << ")";

  switch (oper)
  {
  case LSHIFT:
    m << "<<";
    break;
  case RSHIFT:
    m << ">>";
    break;
  case PLUS:
    m << "+";
    break;
  case MINUS:
    m << "-";
    break;
  case TIMES:
    m << "*";
    break;
  case DIVIDE:
    m << "/";
    break;
  case MODULUS:
    m << "%";
    break;
  case BIT_AND:
    m << "&";
    break;
  case BIT_OR:
    m << "|";
    break;
  case BIT_XOR:
    m << "^";
    break;
  case BIT_NOT:
    m << "~";
    break;
  default:
    assert(0);
  }
  if (id)
    m << "id ";
  else
    m << " ";

  if (rpar)
    m << "(";
  m << rhs->ToString ();
  if (rpar)
    m << ")";

  return m.str ();
}

ParseFunctorName::ParseFunctorName (ParseExpr *n)
{
  name = n->value->ToString ();
  delete n;
}

string ParseFunctorName::ToString ()
{
  ostringstream fn;
  fn << name;
  return fn.str ();
}

ParseFunctor::ParseFunctor (ParseFunctorName* f, ParseExprList* a,
  ParseExpr* dest, ParseExpr* sayer, bool auth, bool encrypt) :
  fName (f), m_args (a), m_auth (auth), m_encrypt (encrypt)

{
  if (m_auth)
    {
      m_sayer = sayer;
    }

  if (dest)
    {
      //SeNDlog:Functor as format: tuple(X,Y..)@D.
      //Take D as destination and location specifier.
      m_dest = dest->value;
      delete dest;
    }
  else
    {
      (void) GetLocSpec ();
    }
}

string ParseFunctor::GetLocSpec ()
{
  ParseVar *p;
  ParseAgg * a;

  // if the loc_ field was filled in, trust it.  Sometimes this may be
  // done externally to the parser (e.g. during rule localization.)
  // Otherwise, find the locspec among the args.
  if (m_loc.empty ())
    {
      bool found = false;
      for (int k = 0; k < Args (); k++)
        {
          if ( (p = dynamic_cast<ParseVar*> (Arg (k))) && p->LocSpec ())
            {
              if (!found)
                {
                  m_loc = p->ToLocString ();
                  found = true;
                }
              else
                {
                  NS_LOG_ERROR ("PARSER ERROR: More than one location "
                      << "specifier in predicate " << ToString ());
                  m_loc.clear ();
                  break;
                }
            }
          else if ( (a = dynamic_cast<ParseAgg*> (Arg (k))) && a->m_locSpec)
            {
              // This argument is an aggregate and also the location
              // specifier
              if (!found)
                {
                  // The name of the locspec is the aggregation variable
                  m_loc = "@" + a->value->ToString ();
                  found = true;
                }
              else
                {
                  NS_LOG_ERROR ("PARSER ERROR: More than one location "
                      << "specifier in predicate " << ToString ());
                  m_loc.clear ();
                  break;
                }
            }
        }
      if (!found)
        {
          NS_LOG_WARN ("PARSER WARNING: No location specifier in predicate "
              << ToString ());
        }
      // drop through to return
    }
  return (m_loc);
}

string ParseFunctor::GetLocSpecValue ()
{
	if(m_loc.empty())
		GetLocSpec();

	string value = m_loc.substr(1,m_loc.size()-1);
	return value;
}


string ParseFunctor::ToString ()
{
  ostringstream f;
  if (m_encrypt)
    {
      f << " encrypts ";
    }
  if (m_auth)
    {
      ParseVar* var = dynamic_cast<ParseVar*> (m_sayer);
      f << var->ToString () << " says ";
    }
  f << fName->ToString () << "(";
  for (int i = 0; i < Args (); i++)
    {
      ParseExpr* nextArg = Arg (i);
      ParseVar* var = dynamic_cast<ParseVar*> (nextArg);
      if (var == NULL)
        {
          // Wasn't a variable, so just call normal ToString
          f << nextArg->ToString ();
        }
      else
        {
          // It's a variable, so call ToString with locspecs
          f << var->ToLocString ();
        }
      if (i + 1 < Args ())
        {
          f << ", ";
        }
      else
        {
          f << ")";
        }
    }
  return f.str ();
}

int ParseFunctor::Find (ParseExpr *arg)
{
  return Find (arg->value->ToString ());
}

int ParseFunctor::Find (string argname)
{
  int p = 0;
  for (; p < Args () && Arg (p)->ToString () != argname; p++)
    ;
  return (p < Args ()) ? p : -1;
}

int ParseFunctor::Aggregate ()
{
  for (int i = 0; i < Args (); i++)
    {
      if (dynamic_cast<ParseAgg*> (Arg (i)) != NULL)
        {
          return i;
        }
    }
  return -1;
}

string ParseFunctor::AggregateOperator ()
{
  int aggregatePos = Aggregate ();
  if (aggregatePos < 0)
    {
      return "";
    }
  ParseAgg* aggregate = dynamic_cast<ParseAgg*> (Arg (aggregatePos));
  return aggregate->oper;
}

void ParseFunctor::replace (int p, ParseExpr *e)
{
  ParseExprList::iterator next = m_args->erase ( (m_args->begin () + p));
  m_args->insert (next, e);
}

string ParseAssign::ToString ()
{
  return var->ToString () + " := " + assign->ToString ();
}

string ParseSelect::ToString ()
{
  return select->ToString ();
}

string ParseFunction::ToString ()
{
  ostringstream f;
  f << value->ToString () << "(";
  for (int i = 0; i < Args (); i++)
    {
      f << Arg (i)->ToString ();
      if (i + 1 < Args ())
        f << ", ";
    }
  f << ")";
  return f.str ();
}

string ParseAggTerm::ToString ()
{

  ostringstream aggFieldStr;
  ostringstream groupByFieldStr;

  aggFieldStr << "(";
  groupByFieldStr << "(";

  for (unsigned k = 0; k < m_groupByFields->size (); k++)
    {
      groupByFieldStr << m_groupByFields->at (k)->ToString ();
      if (k != m_groupByFields->size () - 1)
        {
          groupByFieldStr << ", ";
        }
    }
  groupByFieldStr << ")";

  for (unsigned k = 0; k < m_aggFields->size (); k++)
    {
      aggFieldStr << m_aggFields->at (k)->ToString ();
      if (k != m_aggFields->size () - 1)
        {
          aggFieldStr << ", ";
        }
    }
  aggFieldStr << ")";

  return m_oper + "( " + groupByFieldStr.str () + ", " + aggFieldStr.str ()
      + ", " + m_baseTerm->ToString () + " )";
}

ParseKey::ParseKey (ParseExpr* attr)
{
  m_attr = dynamic_cast<ParseVal*> (attr);
  m_type = "IPV4";
  if (m_attr == NULL)
    {
      NS_LOG_ERROR ("Key attribute should be a value type");
    }
}

ParseKey::ParseKey (ParseExpr* attr, ParseExpr* type)
{
  m_attr = dynamic_cast<ParseVal*> (attr);
  m_type = (dynamic_cast<ParseVal*> (type))->value->ToString ();
  if (m_attr == NULL)
    {
      NS_LOG_ERROR ("Key attribute should be a value type");
    }
}

string ParseKey::ToString ()
{
  return m_attr->ToString () + ":" + m_type;
}

string ParseKey::GetType ()
{
  return m_type;
}

ParseVal*
ParseKey::getAttribute ()
{
  return m_attr;
}

string ParseRule::ToString ()
{
  return "";//m_rule->ToString ();
}

#endif /* __PARSER_UTIL_C__ */
