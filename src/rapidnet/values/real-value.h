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

#ifndef REALVALUE_H
#define REALVALUE_H

#include "value.h"
#define rn_real(ptr) \
  DynamicCast<RealValue, Value> (ptr)->GetRealValue ()

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for a floating point value.
 */
class RealValue: public Value
{
public:

  RealValue (double value = 0.0);

  virtual ~RealValue ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the floating point value.
   */
  inline double GetRealValue () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * \brief Factory method to create a new RealValue object. The default
   *        value is 0.0.
   */
  static Ptr<Value> New (double value = 0.0);

protected:

  double m_value;
};

double
RealValue::GetRealValue () const
{
  return m_value;
}

} // namespace rapidnet
} // namespace ns3

#endif // REALVALUE_H
