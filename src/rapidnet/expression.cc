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

#include "expression.h"
#include "tuple.h"
#include "ns3/value.h"
#include "ns3/bool-value.h"

using namespace ns3;
using namespace ns3::rapidnet;


string
ns3::rapidnet::OpString (Operator op)
{
  switch (op)
  {
  case RN_NO_OP: return "";
  case RN_PLUS: return "+";
  case RN_MINUS: return "-";
  case RN_TIMES: return "*";
  case RN_DIVIDE: return "/";
  case RN_MODULUS: return "%";
  case RN_LSHIFT: return "<<";
  case RN_RSHIFT: return ">>";
  case RN_NOT: return "!";
  case RN_AND: return "&&";
  case RN_OR: return "||";
  case RN_EQ: return "==";
  case RN_NEQ: return "!=";
  case RN_GT: return ">";
  case RN_LT: return "<";
  case RN_GTE: return ">=";
  case RN_LTE: return "<=";
  case RN_IN_RANGE: return "[()]";
  case RN_BIT_XOR: return "^";
  case RN_BIT_AND: return "&";
  case RN_BIT_OR: return "|";
  case RN_BIT_NOT: return "~";
  default: return "UNKNOWN";
  }
}

Expression::Expression ()
{
}

Expression::~Expression ()
{
}

ostream&
ns3::rapidnet::operator << (ostream& os, const Ptr<Expression>& expr)
{
  os << expr->ToString ();
  return os;
}

Operation::Operation ()
  : m_op (RN_NO_OP)
{
}

Operation::~Operation ()
{
}

Ptr<Value>
Operation::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> retval;
  switch (m_type)
  {
    case UNARY:
      retval = m_left->Eval (tuple)->Eval (m_op, tuple);
      break;
    case BINARY:
      retval = m_left->Eval (tuple)->Eval (m_op, tuple, m_right);
      break;
    default:
      // This should never execute
      NS_ASSERT (false);
      retval = Ptr<Value> (NULL);
  }
  return retval;
}

string
Operation::ToString () const
{
  string retval;
  switch (m_type)
  {
    case UNARY:
      retval = OpString (m_op) + " (" + m_left->ToString () + ")";
      break;
    case BINARY:
      retval = "(" + m_left->ToString () + ") " + OpString (m_op) +
        " (" + m_right->ToString () + ")";
      break;
    default:
      // This should never execute
      NS_ASSERT (false);
      retval = "BAD EXPRESSION";
  }
  return retval;
}

Ptr<Expression>
Operation::New (Operator op, Ptr<Expression> left)
{
  Ptr<Operation> expr = Create<Operation> ();
  expr->m_type = UNARY;
  expr->m_op = op;
  expr->m_left = left;
  return expr;
}

Ptr<Expression>
Operation::New (Operator op, Ptr<Expression> left, Ptr<Expression> right)
{
  Ptr<Operation> expr = Create<Operation> ();
  expr->m_type = BINARY;
  expr->m_op = op;
  expr->m_left = left;
  expr->m_right = right;
  return expr;
}

ValueExpr::ValueExpr ()
{
}

ValueExpr::~ValueExpr ()
{
}

Ptr<Expression>
ValueExpr::New (Ptr<Value> value)
{
  Ptr<ValueExpr> expr = Create<ValueExpr> ();
  expr->m_value = value;
  return expr;
}

Ptr<Value>
ValueExpr::Eval (Ptr<Tuple> tuple)
{
  return m_value;
}

string
ValueExpr::ToString () const
{
  return m_value->ToString ();
}


VarExpr::VarExpr ()
{
}

VarExpr::~VarExpr ()
{
}

Ptr<Expression>
VarExpr::New (string name)
{
  Ptr<VarExpr> expr = Create<VarExpr> ();
  expr->m_name = name;
  return expr;
}

Ptr<Value>
VarExpr::Eval (Ptr<Tuple> tuple)
{
  return tuple->GetAttribute (m_name)->GetValue ();
}

string
VarExpr::ToString () const
{
  return "var:" + m_name;
}

FunctionExpr::FunctionExpr ()
{
}

FunctionExpr::~FunctionExpr ()
{
}

string
FunctionExpr::ToString () const
{
  return "FunctionExpr";
}

Ptr<Value>
RangeExpr::Eval (Ptr<Tuple> tuple)
{
  // Note: This range evaluation does wrap around
  // checks, example: used for the cyclic chord ring
  Ptr<Expression> expr;

  if (rn_bool (Operation::New (RN_EQ, m_start, m_end)->Eval (tuple)))
    {
      switch (m_type)
      {
        case RANGECC:
        case RANGECO:
        case RANGEOC:
          expr = ValueExpr::New (BoolValue::New (true));
          break;
        case RANGEOO:
          expr = Operation::New (RN_NEQ, m_start, m_testExpr);
          break;
      }
    }
  else if (rn_bool (Operation::New (RN_GT, m_start, m_end)->Eval (tuple)))
    {
      switch (m_type)
      {
        case RANGECC:
          expr = Operation::New (RN_OR,
            Operation::New (RN_LTE, m_start, m_testExpr),
            Operation::New (RN_GTE, m_end, m_testExpr));
          break;
        case RANGECO:
          expr = Operation::New (RN_OR,
            Operation::New (RN_LTE, m_start, m_testExpr),
            Operation::New (RN_GT, m_end, m_testExpr));
          break;
        case RANGEOC:
          expr = Operation::New (RN_OR,
            Operation::New (RN_LT, m_start, m_testExpr),
            Operation::New (RN_GTE, m_end, m_testExpr));
          break;
        case RANGEOO:
          expr = Operation::New (RN_OR,
            Operation::New (RN_LT, m_start, m_testExpr),
            Operation::New (RN_GT, m_end, m_testExpr));
          break;
      }
    }
  else
    {
      switch (m_type)
      {
        case RANGECC:
          expr = Operation::New (RN_AND,
            Operation::New (RN_LTE, m_start, m_testExpr),
            Operation::New (RN_GTE, m_end, m_testExpr));
          break;
        case RANGECO:
          expr = Operation::New (RN_AND,
            Operation::New (RN_LTE, m_start, m_testExpr),
            Operation::New (RN_GT, m_end, m_testExpr));
          break;
        case RANGEOC:
          expr = Operation::New (RN_AND,
            Operation::New (RN_LT, m_start, m_testExpr),
            Operation::New (RN_GTE, m_end, m_testExpr));
          break;
        case RANGEOO:
          expr = Operation::New (RN_AND,
            Operation::New (RN_LT, m_start, m_testExpr),
            Operation::New (RN_GT, m_end, m_testExpr));
          break;
      }
    }
  return expr->Eval (tuple);
}

string
RangeExpr::ToString () const
{
  // Dummy
  return "range_expr";
}

Ptr<Expression>
RangeExpr::New (Interval type, Ptr<Expression> testExpr,
  Ptr<Expression> start, Ptr<Expression> end)
{
  Ptr<RangeExpr> retval = Create<RangeExpr> ();
  retval->m_type = type;
  retval->m_testExpr = testExpr;
  retval->m_start = start;
  retval->m_end = end;
  return retval;
}
