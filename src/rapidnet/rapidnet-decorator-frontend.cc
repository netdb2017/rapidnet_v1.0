/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2009 University of Pennsylvania
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
 */

#include "rapidnet-decorator-frontend.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/simulator.h"

using namespace ns3;
using namespace ns3::rapidnet;

NS_LOG_COMPONENT_DEFINE("RapidNet-Decorator");

RapidNetDecoratorFrontend::RapidNetDecoratorFrontend ()
{
}

RapidNetDecoratorFrontend::~RapidNetDecoratorFrontend ()
{
}

void
RapidNetDecoratorFrontend::AddTupleState (Ptr<Node> node, Ptr<Tuple> tuple,
  list<string>& attrs)
{
  NS_ASSERT (node != 0);
  NS_ASSERT (PeekPointer (tuple) != 0);

  (*m_ostream) << Simulator::Now () << " tuple " << node->GetId() <<
    " +" << tuple->GetName () << " ";

  for (list<string>::iterator  it = attrs.begin (); it != attrs.end (); ++it)
    {
      (*m_ostream) << *it << ":" << tuple->GetAttribute(
        *it)->GetValue ()->ToString () << " ";
    }
  (*m_ostream) << endl;
}

void
RapidNetDecoratorFrontend::RemoveTupleState (Ptr<Node> node, Ptr<Tuple> tuple,
  list<string>& attrs)
{
  NS_ASSERT (node != 0);
  NS_ASSERT (PeekPointer (tuple) != 0);

  (*m_ostream) << Simulator::Now () << " tuple " << node->GetId() <<
    " -" << tuple->GetName () << " ";

  for (list<string>::iterator  it = attrs.begin (); it != attrs.end (); ++it)
    {
      (*m_ostream) << *it << ":" << tuple->GetAttribute(
        *it)->GetValue ()->ToString () << " ";
    }
  (*m_ostream) << endl;
}
