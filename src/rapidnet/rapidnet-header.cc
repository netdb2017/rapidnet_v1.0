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
#include "rapidnet-header.h"

using namespace ns3;
using namespace ns3::rapidnet;

TypeId
RapidNetHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::RapidNetHeader")
    .SetParent<Header> ()
    .AddConstructor<RapidNetHeader>();
    ;
  return tid;
}

TypeId
RapidNetHeader::GetInstanceTypeId (void) const
{
  return GetTypeId();
}

RapidNetHeader::RapidNetHeader (Ptr<Tuple> tuple)
{
  SetTuple(tuple);
}

RapidNetHeader::~RapidNetHeader ()
{}

void
RapidNetHeader::SetTuple (Ptr<Tuple> tuple)
{
  m_tuple = tuple;
}

Ptr<Tuple>
RapidNetHeader::GetTuple (void) const
{
  return m_tuple;
}

uint32_t
RapidNetHeader::GetSerializedSize (void) const
{
  return m_tuple->GetSerializedSize ();
}

void
RapidNetHeader::Serialize (Buffer::Iterator start) const
{
  m_tuple->Serialize (start);
}

uint32_t
RapidNetHeader::Deserialize (Buffer::Iterator end)
{
  m_tuple = CreateObject<Tuple> ();
  return m_tuple->Deserialize (end);
}

void
RapidNetHeader::Print (ostream &os) const
{
  os << m_tuple;
}
