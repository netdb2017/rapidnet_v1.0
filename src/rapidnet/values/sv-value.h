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

#ifndef SV_VALUE_H
#define	SV_VALUE_H

#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <locale>

#include "value.h"
#include "ns3/assert.h"

#define rn_sv(ptr) \
  DynamicCast<SvValue, Value> (ptr)->GetSvValue ()

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for summary vector. This value type is used
 *        by the Epidemic protocol for delay tolerant networks.
 */
class SvValue : public Value
{
public:
  /*
   * \brief num of bits for SV, dividable by 32 (=> 4 byte serialize and deserialize)
   */
  static const uint32_t k_NUM_BITS = 512;
  /*
   * \brief num of hash function used for SV
   */
  static const uint32_t k_NUM_HASH_FUNCS = 4;

  SvValue (void);
  SvValue (const string & s);
  SvValue (const bitset<k_NUM_BITS>& bs);

  virtual ~SvValue (void){}

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the bitset object that represents the summary
   *        vector value.
   */
  inline bitset<k_NUM_BITS> GetSvValue () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> vPtr) const;

  virtual bool Less (const Ptr<Value> vPtr) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  static Ptr<Value> New (const bitset<k_NUM_BITS> & value = bitset<k_NUM_BITS>());

  /*
   *  \brief hash a string to a uint32_t
   */
  static uint32_t Hash (const string & id);

  /*
   * \brief Bitwise: s1 AND (Not s2), produce a new SvValue
   */
  static Ptr<Value> AndNot (const Ptr<Value> svVPtr_1, const Ptr<Value> svVPtr_2);

  /*
   * \brief Whether id is in s by hashing, yes: 1, no: 0
   */
  //static bool HashIn (const Ptr<Value> vPtr, const string & id);
  static bool HashIn (const Ptr<Value> svVPtr, const Ptr<Value> strVPtr);

  /*
   * \brief Hash Insert an ID into sumamry vector and create a new SV
   */
  //static Ptr<Value> HashInsert (const Ptr<Value> vPtr, const string & id);
  static Ptr<Value> HashInsert (const Ptr<Value> svVPtr, const Ptr<Value> strVPtr);

  /*
   * \brief Hash Remove an ID from summary vector and return the result (new)
   */
  //static Ptr<Value> HashRemove (const Ptr<Value> vPtr, const string & id);
  static Ptr<Value> HashRemove (const Ptr<Value> svVPtr, const Ptr<Value> strVPtr);

  /*
   * \brief Convert string to uint64 (bit by bit)
   *  depracated
   */
  //static uint64_t Str2uint64 (unsigned char * s, uint32_t begin, uint32_t length);

  /*
   * \brief num of bytes for the result of SHA-1
   * deprecated
   */
  //static const uint32_t k_SHA_DIGEST_LEN = 20;

  /*
   * \brief kSegLen must <= 8 (uint64_t is 8 bytes)
   * deprecated
   */
  //static const uint32_t k_SEG_LEN = k_SHA_DIGEST_LEN / k_NUM_HASH_FUNCS;

protected:

  bitset<k_NUM_BITS> m_value;
};

bitset<SvValue::k_NUM_BITS>
SvValue::GetSvValue () const
{
  return m_value;
}

} //namespace rapidnet
} //namepsace ns3

#endif	// SV_VALUE_H
