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

#ifndef BOOLVALUE_H_
#define BOOLVALUE_H_

#include "value.h"
#define rn_bool(ptr) \
  DynamicCast<BoolValue, Value> (ptr)->GetBoolValue ()

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for booleans.
 */
class BoolValue: public Value
{
public:

  BoolValue (bool value = false);

  virtual ~BoolValue ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  inline bool GetBoolValue () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  static Ptr<Value> New (bool value = false);

protected:

  bool m_value;
};


bool
BoolValue::GetBoolValue () const
{
  return m_value;
}

} // namespace rapidnet
} // namespace ns3

#endif /* BOOLVALUE_H_ */
