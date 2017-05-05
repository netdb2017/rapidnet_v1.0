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

#ifndef BYTEARRAYVALUE_H_
#define BYTEARRAYVALUE_H_

#include "value.h"
#define rn_byte_array(arr) \
  DynamicCast<ByteArrayValue, Value> (arr)->GetByteArrayPtr ()


using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for uint8_t array (byte buffer).
 */
class ByteArrayValue: public ns3::rapidnet::Value
{
public:
  ByteArrayValue (uint8_t* buf = NULL, uint32_t len = 0);

  virtual ~ByteArrayValue ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the pointer to the byte array
   */
  inline virtual uint8_t* GetByteArrayPtr () const;

  /**
   * \brief Returns the size of the byte array
   */
  inline virtual uint32_t GetByteArrayLen () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * \brief Factory method to create a new ByteArrayValue object. The default
   *        value is an empty char vector.
   */
  static Ptr<Value> New (uint8_t* buf = NULL, uint32_t len = 0);

protected:

  uint8_t* m_array;

  uint32_t m_len;
};


uint8_t*
ByteArrayValue::GetByteArrayPtr  () const
{
  return m_array;
}

uint32_t
ByteArrayValue::GetByteArrayLen () const
{
  return m_len;
}

} // namespace rapidnet
} // namespace ns3

#endif /* BYTEARRAYVALUE_H_ */
