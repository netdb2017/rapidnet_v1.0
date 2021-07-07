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

#include "byte-array-value.h"
#include "rapidnet-types.h"
#include "ns3/tuple.h"
#include <sstream>

using namespace ns3;
using namespace rapidnet;

ByteArrayValue::ByteArrayValue (uint8_t* buf, uint32_t len) :
  Value (BYTE_ARRAY), m_array (buf), m_len (len)
{
}

ByteArrayValue::~ByteArrayValue ()
{
  if (m_array != NULL)
    {
      delete[] m_array;
    }
}

uint32_t
ByteArrayValue::GetSerializedSize (void) const
{
  // 1 byte per char, 4 bytes for length
  return Value::GetSerializedSize () + 4 + 2 * m_len;
}

void
ByteArrayValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  start.WriteHtonU32 (m_len);

  for (uint32_t i = 0; i < m_len; i++)
    {
      start.WriteHtonU16 ((uint16_t) m_array[i]);
    }
}

uint32_t
ByteArrayValue::Deserialize (Buffer::Iterator& end)
{
  m_len = end.ReadNtohU32 ();
  m_array = new uint8_t[m_len];

  for (uint32_t i = 0; i < m_len; i++)
    {
      m_array[i] = (uint8_t) end.ReadNtohU16 ();
    }

  return GetSerializedSize ();
}

string
ByteArrayValue::ToString () const
{
  stringstream ss;
  ss << "<byte-array[" << m_len << "]>";
  return ss.str ();
}

Ptr<Value>
ByteArrayValue::Clone () const
{
  uint8_t* newArray = new uint8_t[m_len];
  memcpy (newArray, m_array, m_len);
  return ByteArrayValue::New (newArray, m_len);
}

bool
ByteArrayValue::Equals (const Ptr<Value> v) const
{
  if (GetType () != v->GetType ())
    {
      return false;
    }
  Ptr<ByteArrayValue> other = DynamicCast<ByteArrayValue, Value> (v);

  if ( (m_len == other->GetByteArrayLen ()) && (memcmp (m_array,
    other->GetByteArrayPtr (), m_len) == 0))
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
ByteArrayValue::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<ByteArrayValue> other = DynamicCast<ByteArrayValue, Value> (v);
  int len = min (m_len, other->GetByteArrayLen ());
  if (memcmp (m_array, other->GetByteArrayPtr (), len) < 0)
    {
      return true;
    }

  else if (memcmp (m_array, other->GetByteArrayPtr (), len) < 0)
    {
      return false;
    }
  else
    {
      return (m_len < other->GetByteArrayLen ());
    }
}

Ptr<Value>
ByteArrayValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;

  switch (op)
  {
  // currently ByteArrayValue does not support any operator
  default:
    {
      AssertUnhandledOperator (op);
      retval = Ptr<Value> (NULL);
    }
  }
  return retval;
}

Ptr<Value>
ByteArrayValue::New (uint8_t* value, uint32_t len)
{
  Ptr<ByteArrayValue> retval = Create<ByteArrayValue> ();
  retval->m_array = value;
  retval->m_len = len;
  return retval;
}
