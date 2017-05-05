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

#include "bool-value.h"
#include <sstream>
#include "rapidnet-types.h"
#include "ns3/tuple.h"

using namespace ns3;
using namespace ns3::rapidnet;

BoolValue::BoolValue (bool value)
  : Value (BOOL), m_value (value)
{
}

BoolValue::~BoolValue ()
{
}

uint32_t
BoolValue::GetSerializedSize (void) const
{
  return Value::GetSerializedSize () + 2;
}

void
BoolValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  start.WriteHtonU16 (m_value ? 1 : 0);
}

uint32_t
BoolValue::Deserialize (Buffer::Iterator& end)
{
  m_value = end.ReadNtohU16 () == 1;
  return GetSerializedSize ();
}

string
BoolValue::ToString () const
{
  stringstream ss;
  ss << (m_value ? "true" : "false");
  return ss.str ();
}

Ptr<Value>
BoolValue::Clone () const
{
  return BoolValue::New (m_value);
}

bool
BoolValue::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<BoolValue> other = DynamicCast<BoolValue, Value> (v);

  return GetBoolValue () == other->GetBoolValue ();
}

bool
BoolValue::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<BoolValue> other = DynamicCast<BoolValue, Value> (v);

  int value = (GetBoolValue () ? 1 : 0);
  int other_value = (other->GetBoolValue () ? 1 : 0);

  return value < other_value;
}

Ptr<Value>
BoolValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;

  switch (op)
  {
    case RN_AND:
      {
        // Short-circuit operation
        if (!m_value)
          {
            return BoolValue::New (false);
          }

        Ptr<Value> operand = expr->Eval (tuple);
        if (V_InstanceOf (operand, BoolValue))
          {
            retval = BoolValue::New (m_value && rn_bool (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_OR:
      {
        // Short-circuit operation
        if (m_value)
          {
            return BoolValue::New (true);
          }
        Ptr<Value> operand = expr->Eval (tuple);

        if (V_InstanceOf (operand, BoolValue))
          {

            retval = BoolValue::New (m_value || rn_bool (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_NOT:
      {
        // Parser should ensure that this is a unary operation
        retval = BoolValue::New (!m_value);
        break;
      }
    case RN_EQ:
      {
        Ptr<Value> operand = expr->Eval (tuple);
        if (V_InstanceOf (operand, BoolValue))
          {
            Ptr<Value> operand = expr->Eval (tuple);
            retval = BoolValue::New (Equals (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_NEQ:
      {
        Ptr<Value> operand = expr->Eval (tuple);
        if (V_InstanceOf (operand, BoolValue))
          {
           retval = BoolValue::New (!Equals (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    default:
      {
        AssertUnhandledOperator (op);
        retval = Ptr<Value> (NULL);
      }
  }
  return retval;
}

Ptr<Value>
BoolValue::New (bool value)
{
  Ptr<BoolValue> retval = Create<BoolValue> ();
  retval->m_value = value;
  return retval;
}
