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

#include "sv-value.h"
#include "rapidnet-types.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

SvValue::SvValue (void)
  : Value (SV)
{
  m_value = bitset<k_NUM_BITS>();
}

SvValue::SvValue (const string & s)
  : Value (SV)
{
  NS_ASSERT_MSG (s.size () == k_NUM_BITS,
                 "The size of string s should be equal to k_NUM_BITS");
  m_value = bitset<k_NUM_BITS > (s);
}

SvValue::SvValue (const bitset<k_NUM_BITS>& bs)
  : Value (SV)
{
  NS_ASSERT_MSG (bs.size () == k_NUM_BITS,
                 "The size of bitset bs should be equal to k_NUM_BITS");
  m_value = bitset<k_NUM_BITS > (bs);
}

uint32_t
SvValue::GetSerializedSize (void) const
{
  return Value::GetSerializedSize () + (SvValue::k_NUM_BITS / 8);
}

void
SvValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);

  uint32_t i = 0;
  uint32_t uint32Value = 0;
  for (i = k_NUM_BITS; i >= 1; i--) // never use i>=0 since i has type uint32_t
    {
      uint32Value = (uint32Value << 1) | m_value[i-1];
      if (i % 32 == 1)
        {
          start.WriteHtonU32(uint32Value);
          uint32Value = 0;
        }
    }
}

uint32_t
SvValue::Deserialize (Buffer::Iterator& end)
{
  uint32_t i = 0, j = 0;
  uint32_t uint32Value = 0;
  for (i = 1; i <= k_NUM_BITS / 32; i++)
    {
      uint32Value = end.ReadNtohU32();
      for (j = 0; j < 32; j++)
        {
          m_value.set((j + k_NUM_BITS - 32 * i), (uint32Value & 0x00000001));
          uint32Value = uint32Value >> 1;
        }
    }

  return GetSerializedSize ();
}

string
SvValue::ToString() const
{
  uint32_t i = 0;
  stringstream ss;
  // only show the first 128 bit, in case the length of bitset m_value is
  // too long
  if (k_NUM_BITS <= 128)
    {
      ss << m_value;
    }
  else
    {
      NS_ASSERT_MSG(m_value.size() > 128,
                    "bitset m_value should have more than 128 bits");
      for (i = 0; i < 128; i++)
        {
          ss << m_value[i];
        }
    }
  return ss.str();
}

Ptr<Value>
SvValue::Clone () const
{
  return SvValue::New (m_value);
}

bool
SvValue::Equals (const Ptr<Value> vPtr) const
{
  NS_ASSERT_MSG (vPtr != 0,
                 "Must not be NULL pointer");

  if(GetType () != vPtr->GetType ())
    {
      return false;
    }

  Ptr<SvValue> other = DynamicCast<SvValue, Value> (vPtr);

  for (uint32_t i = 0; i < k_NUM_BITS; i++)
    {
      if (m_value[i] != other->m_value[i])
        {
          return false;
        }
    }
  return true;
}

bool
SvValue::Less (const Ptr<Value> vPtr) const
{
  NS_ASSERT_MSG (vPtr != 0,
                 "Must not be NULL pointer");

  if (GetType () < vPtr->GetType ())
    {
      return true;
    }
  else if (GetType () > vPtr->GetType ())
    {
      return false;
    }

  Ptr<SvValue> other = DynamicCast<SvValue, Value> (vPtr);

  for (uint32_t i = 0; i < k_NUM_BITS; i++)
    {
      if (m_value[i] == other->m_value[i])
        {
          continue;
        }
      else
        {
          return m_value[i] < other->m_value[i];
        }
    }

  return false;
}

Ptr<Value>
SvValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;

  switch (op)
  {
    // currently SvValue does not support any operator
    default:
      {
        AssertUnhandledOperator (op);
        retval = Ptr<Value> (NULL);
      }
  }
  return retval;
}

Ptr<Value>
SvValue::New (const bitset<k_NUM_BITS> & value)
{
  Ptr<SvValue> retval = Create<SvValue>();
  retval->m_value = value;
  return retval;
}

uint32_t
SvValue::Hash (const string & id)
{
  locale loc; // the "C" locale
  const collate<char>& coll = use_facet<collate<char> >(loc);
  long value = coll.hash (id.data (), id.data () + id.length ());
  return value;
}

Ptr<Value>
SvValue::AndNot (const Ptr<Value> svVPtr_1, const Ptr<Value> svVPtr_2)
{
  NS_ASSERT_MSG (svVPtr_1 != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (svVPtr_2 != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (svVPtr_1->GetType () == SV,
                 "svVPtr_1 is not SV type");
  NS_ASSERT_MSG (svVPtr_2->GetType () == SV,
                 "svVPtr_2 is not SV type");

  Ptr<SvValue> svPtr_1 = DynamicCast<SvValue, Value> (svVPtr_1);
  Ptr<SvValue> svPtr_2 = DynamicCast<SvValue, Value> (svVPtr_2);

  Ptr<SvValue> resultSvPtr = Create<SvValue> ();
  resultSvPtr->m_value =
    (svPtr_1->m_value) & (~(svPtr_2->m_value));

  return resultSvPtr;
}

bool
SvValue::HashIn (const Ptr<Value> svVPtr, const Ptr<Value> strVPtr)
{
  NS_ASSERT_MSG (svVPtr != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (svVPtr->GetType () == SV,
                 "svVPtr is not SV type");
  NS_ASSERT_MSG (strVPtr != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (strVPtr->GetType () == STR,
                 "strVPtr is not STR type");

  Ptr<SvValue> svPtr = DynamicCast<SvValue, Value> (svVPtr);
  Ptr<StrValue> strPtr = DynamicCast<StrValue, Value> (strVPtr);
  string id = strPtr->GetStrValue();

  string s = id;
  uint32_t hashValue = 0;
  for (uint32_t i = 0; i < k_NUM_HASH_FUNCS; i++)
    {
      hashValue = SvValue::Hash (s);
      if (!svPtr->m_value.test (hashValue % k_NUM_BITS))
        {
          return false;
        }
      s = s + id;
    }
  return true;
}

Ptr<Value>
SvValue::HashInsert (const Ptr<Value> svVPtr, const Ptr<Value> strVPtr)
{
  NS_ASSERT_MSG (svVPtr != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (svVPtr->GetType () == SV,
                 "svVPtr is not SV type");
  NS_ASSERT_MSG (strVPtr != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (strVPtr->GetType () == STR,
                 "strVPtr is not STR type");

  Ptr<SvValue> svPtr = DynamicCast<SvValue, Value> (svVPtr);
  Ptr<StrValue> strPtr = DynamicCast<StrValue, Value> (strVPtr);
  string id = strPtr->GetStrValue();

  Ptr<SvValue> resultSvPtr = Create<SvValue>();
  resultSvPtr->m_value = svPtr->m_value;

  string s = id;
  uint32_t hashValue = 0;
  for (uint32_t i = 0; i < k_NUM_HASH_FUNCS; i++)
    {
      hashValue = SvValue::Hash (s);
      resultSvPtr->m_value.set (hashValue % k_NUM_BITS, true);
      s = s + id;
    }
  return resultSvPtr;
}

Ptr<Value>
SvValue::HashRemove (const Ptr<Value> svVPtr, const Ptr<Value> strVPtr)
{
  NS_ASSERT_MSG (svVPtr != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (svVPtr->GetType () == SV,
                 "svVPtr is not SV type");
  NS_ASSERT_MSG (strVPtr != 0,
                 "Must not be NULL pointer");
  NS_ASSERT_MSG (strVPtr->GetType () == STR,
                 "strVPtr is not STR type");

  Ptr<SvValue> svPtr = DynamicCast<SvValue, Value> (svVPtr);
  Ptr<StrValue> strPtr = DynamicCast<StrValue, Value> (strVPtr);
  string id = strPtr->GetStrValue();

  Ptr<SvValue> resultSvPtr = Create<SvValue>();
  resultSvPtr->m_value = svPtr->m_value;

  string s = id;
  uint32_t hashValue = 0;
  for (uint32_t i = 0; i < k_NUM_HASH_FUNCS; i++)
    {
      hashValue = SvValue::Hash (s);
      resultSvPtr->m_value.set (hashValue % k_NUM_BITS, false);
      s = s + id;
    }
  return resultSvPtr;
}

/*
uint64_t
SvValue::Str2uint64 (unsigned char * s, uint32_t begin, uint32_t length)
{
  NS_ASSERT_MSG (length <= 8, " 8 bytes for uint64_t");
  NS_ASSERT_MSG (length == k_SEG_LEN, "length should equal to k_SEG_LEN");
  uint64_t retvl = 0;
  uint32_t i = 0;
  for (i = begin; i < begin + length; i++)
    { // byte by byte
      retvl = (retvl << 8) | s[i];
    }
  return retvl;
}
*/
