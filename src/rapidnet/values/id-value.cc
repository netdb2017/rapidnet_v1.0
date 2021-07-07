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

#include "id-value.h"
#include <sstream>
#include "rapidnet-types.h"
#include "ns3/tuple.h"

using namespace ns3;
using namespace ns3::rapidnet;

IdValue::IdValue (string value, uint32_t base)
  : Value (ID)
{
  mpz_class temp (value, base);
  //cout << "creating id for value: " << value << " mpz: " << temp.get_str (2) << endl;
  m_value = bitset<ID_LEN> (temp.get_str (2));
}

IdValue::~IdValue ()
{
}

uint32_t
IdValue::GetSerializedSize (void) const
{
  // Assumption: ID_LEN is divisible by 8
  return Value::GetSerializedSize () + ID_LEN / 8;
}

void
IdValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);

  // Assumption: ID_LEN divisible by 32
  uint32_t uint32Value = 0;
  for (uint32_t i = ID_LEN; i >= 1; --i)
    {
      uint32Value = (uint32Value << 1) | m_value [i - 1];
      if (i % 32 == 1)
        {
          start.WriteHtonU32 (uint32Value);
          uint32Value = 0;
        }
    }
}

uint32_t
IdValue::Deserialize (Buffer::Iterator& end)
{
  uint32_t uint32Value = 0;
  for (uint32_t i = 1; i <= ID_LEN / 32; i++)
    {
      uint32Value = end.ReadNtohU32();
      for (uint32_t j = 0; j < 32; ++j)
        {
          m_value.set ((j + ID_LEN - 32 * i), (uint32Value & 0x00000001));
          uint32Value = uint32Value >> 1;
        }
    }

  return GetSerializedSize ();
}

string
IdValue::ToString () const
{
  mpz_class hex (m_value.to_string (), 2);
  return hex.get_str (16);
}

string
IdValue::ToFractionString () const
{
  mpf_class hex (m_value.to_string (), 2);
  mpf_class max (MAX_ID_HEX_STRING);
  mp_exp_t x;
  mpf_class result = hex / max;

  return result.get_str (x, 10, 3);
}

Ptr<Value>
IdValue::Clone () const
{
  // Passed by value
  return IdValue::New (m_value.to_string ());
}

bool
IdValue::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<IdValue> other = DynamicCast<IdValue, Value> (v);

  return GetIdValue () == other->GetIdValue ();
}

bool
IdValue::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<IdValue> other = DynamicCast<IdValue, Value> (v);

  for (uint32_t i = ID_LEN; i >= 1; --i)
    {
      if (m_value[i - 1] > other->m_value[i - 1])
        {
          return false;
        }
      else if (m_value[i - 1] < other->m_value[i - 1])
        {
          return true;
        }
    }
  return false;
}

Ptr<Value>
IdValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;
  Ptr<Value> operand = expr->Eval (tuple);

  switch (op)
  {
    case RN_PLUS:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            Ptr<IdValue> arg = DynamicCast<IdValue, Value> (operand);
            mpz_class result = GetMpz () + arg->GetMpz ();
            retval = IdValue::New (result.get_str (2));
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            mpz_class arg (rn_int32 (operand));
            mpz_class result = GetMpz () + arg;
            retval = IdValue::New (result.get_str (2));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_MINUS:
      {
        mpz_class result;
        if (V_InstanceOf (operand, IdValue))
          {
            Ptr<IdValue> arg = DynamicCast<IdValue, Value> (operand);
            result = GetMpz () - arg->GetMpz ();
          }
        else if (V_InstanceOf (operand, Int32Value))
          {
            mpz_class arg (rn_int32 (operand));
            result = GetMpz () - arg;

          }
        // Handle -ve values
        if (result < mpz_class ("0"))
          {
            result = MAX_ID + mpz_class ("1") + result;
          }
        retval = IdValue::New (result.get_str (2));
        break;
      }
    case RN_LSHIFT:
      {
        if (V_InstanceOf (operand, Int32Value))
          {
            retval = IdValue::New (m_value << rn_int32 (operand));
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
            retval = IdValue::New (m_value >> rn_int32 (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_EQ:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            retval = BoolValue::New (m_value == rn_id (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_NEQ:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            retval = BoolValue::New (m_value != rn_id (operand));
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LT:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            Ptr<IdValue> arg = DynamicCast<IdValue, Value> (operand);
            retval = BoolValue::New (GetMpz () < arg->GetMpz ());
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GT:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            Ptr<IdValue> arg = DynamicCast<IdValue, Value> (operand);
            retval = BoolValue::New (GetMpz () > arg->GetMpz ());
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_LTE:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            Ptr<IdValue> arg = DynamicCast<IdValue, Value> (operand);
            retval = BoolValue::New (GetMpz () <= arg->GetMpz ());
          }
        else
          {
            AssertUnknownExpression (op, operand);
          }
        break;
      }
    case RN_GTE:
      {
        if (V_InstanceOf (operand, IdValue))
          {
            Ptr<IdValue> arg = DynamicCast<IdValue, Value> (operand);
            retval = BoolValue::New (GetMpz () >= arg->GetMpz ());
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

bitset<ID_LEN>
IdValue::GetIdValue () const
{
  return m_value;
}


mpz_class
IdValue::GetMpz () const
{
  return mpz_class (ToString (), 16);
}

Ptr<Value>
IdValue::New (string value, uint32_t base)
{
  Ptr<IdValue> retval = Create<IdValue> ();
  mpz_class temp (value, base);
  retval->m_value = bitset<ID_LEN> (temp.get_str (2));
  return retval;
}

Ptr<Value>
IdValue::New (mpz_class value)
{
  Ptr<IdValue> retval = Create<IdValue> ();
  retval->m_value = bitset<ID_LEN> (value.get_str (2));
  return retval;
}

Ptr<Value>
IdValue::New (bitset<ID_LEN> value)
{
  Ptr<IdValue> retval = Create<IdValue> ();
  retval->m_value = value;
  return retval;
}
