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

#include "int32-value.h"
#include <sstream>
#include "rapidnet-types.h"
#include "ns3/tuple.h"

using namespace ns3;
using namespace ns3::rapidnet;

Int32Value::Int32Value (int32_t value)
  : Value (INT32), m_value (value)
{
}

Int32Value::~Int32Value ()
{
}

uint32_t
Int32Value::GetSerializedSize (void) const
{
  return Value::GetSerializedSize () + 4;
}

void
Int32Value::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  start.WriteHtonU32 (m_value);
}

uint32_t
Int32Value::Deserialize (Buffer::Iterator& end)
{
  m_value = end.ReadNtohU32 ();
  return GetSerializedSize ();
}

string
Int32Value::ToString () const
{
  stringstream ss;
  ss << m_value;
  return ss.str ();
}

Ptr<Value>
Int32Value::Clone () const
{
  return Int32Value::New (m_value);
}

bool
Int32Value::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<Int32Value> other = DynamicCast<Int32Value, Value> (v);

  return GetInt32Value () == other->GetInt32Value ();
}

bool
Int32Value::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<Int32Value> other = DynamicCast<Int32Value, Value> (v);

  return GetInt32Value () < other->GetInt32Value ();
}

Ptr<Value>
Int32Value::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;
  Ptr<Value> operand;

  // Unary operator
  if (op != RN_BIT_NOT)
    {
      operand = expr->Eval (tuple);
    }

  switch (op)
  {
    case RN_PLUS:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value + rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value + rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_MINUS:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value - rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value - rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_TIMES:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value * rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value * rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_DIVIDE:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value / rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value / rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_MODULUS:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value % rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LSHIFT:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value << rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_RSHIFT:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value >> rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_BIT_AND:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value & rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_BIT_OR:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value | rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_BIT_XOR:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = Int32Value::New (m_value ^ rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_BIT_NOT:
      {
        retval = Int32Value::New (~m_value);
        break;
      }
    case RN_EQ:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value == rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value == rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_NEQ:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value != rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value != rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GT:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value > rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value > rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LT:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value < rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value < rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GTE:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value >= rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value >= rn_real (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LTE:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value <= rn_int32 (operand));
          }
        else if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value <= rn_real (operand));
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
Int32Value::New (int32_t value)
{
  Ptr<Int32Value> retval = Create<Int32Value> ();
  retval->m_value = value;
  return retval;
}
