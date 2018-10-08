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

#include "real-value.h"
#include <sstream>
#include "rapidnet-types.h"
#include "ns3/tuple.h"

using namespace ns3;
using namespace rapidnet;

RealValue::RealValue (double value)
  : Value (REAL), m_value (value)
{
}

RealValue::~RealValue ()
{
}

uint32_t
RealValue::GetSerializedSize (void) const
{
  return Value::GetSerializedSize () + 8;
}

void
RealValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  start.WriteHtonU64 (*(reinterpret_cast<const uint64_t*> (&m_value)));
}

uint32_t
RealValue::Deserialize (Buffer::Iterator& end)
{
  uint64_t temp = end.ReadNtohU64 ();
  m_value = *(reinterpret_cast<double*> (&temp));

  return GetSerializedSize ();
}

string
RealValue::ToString () const
{
  stringstream ss;
  ss << m_value;
  return ss.str ();
}

Ptr<Value>
RealValue::Clone () const
{
  return RealValue::New (m_value);
}

bool
RealValue::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<RealValue> other = DynamicCast<RealValue, Value> (v);

  return GetRealValue () == other->GetRealValue ();
}

bool
RealValue::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<RealValue> other = DynamicCast<RealValue, Value> (v);

  return GetRealValue () < other->GetRealValue ();
}

Ptr<Value>
RealValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;
  Ptr<Value> operand = expr->Eval (tuple);

  switch (op)
  {
    case RN_PLUS:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value + rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = RealValue::New (m_value + rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_MINUS:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value - rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = RealValue::New (m_value - rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_TIMES:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value * rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = RealValue::New (m_value * rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_DIVIDE:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = RealValue::New (m_value / rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = RealValue::New (m_value / rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_EQ:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value == rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value == rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_NEQ:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value != rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value != rn_int32 (operand));
          }

        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GT:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value > rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value > rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LT:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value < rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value < rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GTE:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value >= rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value >= rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LTE:
      {
        if (V_InstanceOf (operand, RealValue))
          {
            retval = BoolValue::New (m_value <= rn_real (operand));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            retval = BoolValue::New (m_value <= rn_int32 (operand));
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
RealValue::New (double value)
{
  Ptr<RealValue> retval = Create<RealValue> ();
  retval->m_value = value;
  return retval;
}
