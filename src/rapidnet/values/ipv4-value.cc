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

#include "ipv4-value.h"
#include <sstream>
#include "rapidnet-types.h"
#include "ns3/tuple.h"

using namespace ns3;
using namespace rapidnet;

Ipv4Value::Ipv4Value (Ipv4Address value)
  : Value (IPV4), m_value (value)
{
}

Ipv4Value::~Ipv4Value ()
{
}

uint32_t
Ipv4Value::GetSerializedSize (void) const
{
  return Value::GetSerializedSize () + 4;
}

void
Ipv4Value::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  start.WriteHtonU32 (m_value.Get ());
}

uint32_t
Ipv4Value::Deserialize (Buffer::Iterator& end)
{
  m_value = Ipv4Address (end.ReadNtohU32 ());
  return GetSerializedSize ();
}

string
Ipv4Value::ToString () const
{
  stringstream ss;
  ss << m_value;
  return ss.str ();
}

Ptr<Value>
Ipv4Value::Clone () const
{
  return Ipv4Value::New (m_value);
}

bool
Ipv4Value::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<Ipv4Value> other = DynamicCast<Ipv4Value, Value> (v);

  return GetIpv4Value ().Get () == other->GetIpv4Value ().Get ();
}

bool
Ipv4Value::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<Ipv4Value> other = DynamicCast<Ipv4Value, Value> (v);

  return GetIpv4Value ().Get () < other->GetIpv4Value ().Get ();
}

Ptr<Value>
Ipv4Value::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;
  Ptr<Value> operand = expr->Eval (tuple);

  switch (op)
  {
    case RN_EQ:
      {
        if (V_InstanceOf (operand, Ipv4Value))
          {
            retval = BoolValue::New (m_value == rn_ipv4 (operand));
          }
        else if (V_InstanceOf (operand, StrValue))
          {
            retval = BoolValue::New (ToString () == rn_str (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_NEQ:
      {
        if (V_InstanceOf (operand, Ipv4Value))
          {
            retval = BoolValue::New (m_value != rn_ipv4 (operand));
          }
        else if (V_InstanceOf (operand, StrValue))
          {
            retval = BoolValue::New (ToString () != rn_str (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GT:
      {
        if (V_InstanceOf (operand, Ipv4Value))
          {
            retval = BoolValue::New (ToString().compare (operand->ToString ()) > 0);
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LT:
      {
        if (V_InstanceOf (operand, Ipv4Value))
          {
            retval = BoolValue::New (ToString().compare (operand->ToString ()) < 0);
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
Ipv4Value::New (Ipv4Address value)
{
  Ptr<Ipv4Value> retval = Create<Ipv4Value> ();
  retval->m_value = value;
  return retval;
}

