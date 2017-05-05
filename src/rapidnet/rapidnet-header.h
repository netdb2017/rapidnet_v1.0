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
#ifndef RAPIDNET_HEADER_H
#define RAPIDNET_HEADER_H

#include "ns3/assert.h"
#include "ns3/header.h"
#include "tuple.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
* \brief RapidNet extension of @see Header. RapidNet tuples are serialized
*        on to a RapidNet header for sending over the network in a @see
*        Packet.
*/
class RapidNetHeader : public Header
{
public:
  static TypeId GetTypeId (void);

  RapidNetHeader (Ptr<Tuple> tuple = Tuple::New ("unnamed"));

  virtual ~RapidNetHeader ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual TypeId GetInstanceTypeId(void) const;

  virtual void Serialize (Buffer::Iterator start) const;

  virtual uint32_t Deserialize (Buffer::Iterator end);

  virtual void Print (std::ostream &os) const;

  virtual void SetTuple (Ptr<Tuple> tuple);

  virtual Ptr<Tuple> GetTuple (void) const;

protected:

  Ptr<Tuple> m_tuple;
};

} //namespace rapidnet
} //namepsace ns3

#endif // RAPIDNET_HEADER_H
