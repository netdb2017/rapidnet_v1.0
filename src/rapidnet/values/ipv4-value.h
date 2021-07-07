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

#ifndef IPV4VALUE_H
#define IPV4VALUE_H

#include "ns3/ipv4-address.h"
#include "value.h"
#define rn_ipv4(ptr) \
  DynamicCast<Ipv4Value, Value> (ptr)->GetIpv4Value ()

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for IP v4 addresses.
 */
class Ipv4Value: public Value
{
public:
  Ipv4Value (Ipv4Address value = Ipv4Address ());

  virtual ~Ipv4Value ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the Ipv4Address value.
   */
  inline Ipv4Address GetIpv4Value () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * \brief Factory method that creates a new Ipv4Value object.
   *        The default value is defined by ns-3. @see Ipv4Address
   */
  static Ptr<Value> New (Ipv4Address value = Ipv4Address ());

protected:

  Ipv4Address m_value;
};

Ipv4Address
Ipv4Value::GetIpv4Value () const
{
  return m_value;
}

} // namespace rapidnet
} // namespace ns3

#endif // IPV4VALUE_H
