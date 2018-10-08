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

#ifndef IDVALUE_H_
#define IDVALUE_H_

#include <bitset>
#include <gmpxx.h>
#include "value.h"
#define rn_id(ptr) \
  DynamicCast<IdValue, Value> (ptr)->GetIdValue ()

namespace ns3 {
namespace rapidnet {

const uint32_t ID_LEN = 160;
const string MAX_ID_HEX_STRING = "ffffffffffffffffffffffffffffffffffffffff";
const mpz_class MAX_ID (MAX_ID_HEX_STRING, 16);

typedef bitset<ID_LEN> id_t;

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for 160-bit SHA1 hash values used by chord.
 */
class IdValue: public Value
{
public:

  IdValue (string value = "0", uint32_t base = 2);

  virtual ~IdValue ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  virtual string ToFractionString () const;

  inline id_t GetIdValue () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * Return mpz_class instance corresponding to the bit string value.
   */
  virtual mpz_class GetMpz () const;

  static Ptr<Value> New (string value = "0", uint32_t base = 2);

protected:

  static Ptr<Value> New (mpz_class value);

  static Ptr<Value> New (bitset<ID_LEN> value);

  id_t m_value;
};

} // namespace rapidnet
} // namespace ns3

#endif /* IDVALUE_H_ */
