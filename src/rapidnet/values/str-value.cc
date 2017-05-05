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

#include "str-value.h"
#include <sstream>
#include "rapidnet-types.h"
#include "ns3/tuple.h"

using namespace ns3;
using namespace rapidnet;

StrValue::StrValue (string value)
  : Value (STR), m_value (value)
{
}

StrValue::~StrValue ()
{
}

uint32_t
StrValue::GetSerializedSize (void) const
{
  // String length 4 bytes and 2 bytes per character in the string
  return Value::GetSerializedSize () + 4 + 2 * m_value.length ();
}

void
StrValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  uint32_t len = m_value.length ();
  start.WriteHtonU32 (len);

  for (string::const_iterator it = m_value.begin(); it < m_value.end(); it++)
    {
      start.WriteHtonU16 (uint16_t (*it));
    }
}

uint32_t
StrValue::Deserialize (Buffer::Iterator& end)
{
  uint32_t len = end.ReadNtohU32();
  uint8_t* buffer = new uint8_t[len+1];

  for(uint32_t i = 0; i < len; ++i)
    {
      buffer[i] = (uint8_t) end.ReadNtohU16 ();
    }

  buffer[len] = '\0';
  m_value.assign ((char*) buffer, len);
  delete[] buffer;

  return GetSerializedSize ();
}

string
StrValue::ToString () const
{
  stringstream ss;
  ss << m_value;
  return ss.str ();
}

Ptr<Value>
StrValue::Clone () const
{
  return StrValue::New (m_value);
}

bool
StrValue::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<StrValue> other = DynamicCast<StrValue, Value> (v);

  return GetStrValue () == other->GetStrValue ();
}

bool
StrValue::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<StrValue> other = DynamicCast<StrValue, Value> (v);

  return GetStrValue ().compare (other->GetStrValue ()) < 0;
}

Ptr<Value>
StrValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;
  Ptr<Value> operand = expr->Eval (tuple);

  switch (op)
  {
    case RN_PLUS:
      {
        retval = StrValue::New (m_value + operand->ToString ());
        break;
      }
    case RN_EQ:
      {
        retval = BoolValue::New (Equals (operand));
        break;
      }
    case RN_NEQ:
      {
        retval = BoolValue::New (!Equals (operand));
        break;
      }
    case RN_GT:
      {
        if (V_InstanceOf (operand, StrValue))
          {
            retval = BoolValue::New (m_value.compare (rn_str (operand)) > 0);
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LT:
      {
        if (V_InstanceOf (operand, StrValue))
          {
            retval = BoolValue::New (m_value.compare (rn_str (operand)) < 0);
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GTE:
      {
        if (V_InstanceOf (operand, StrValue))
          {
            retval = BoolValue::New (m_value.compare (rn_str (operand)) >= 0);
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LTE:
      {
        if (V_InstanceOf (operand, StrValue))
          {
            retval = BoolValue::New (m_value.compare (rn_str (operand)) <= 0);
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
StrValue::New (string value)
{
  Ptr<StrValue> retval = Create<StrValue> ();
  retval->m_value = value;
  return retval;
}
