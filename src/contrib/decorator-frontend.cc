/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Drexel University
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *
 * Author: Joe Kopena (tjkopena@cs.drexel.edu)
 */

#include <vector>

#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/node-list.h"

#include "ns3/config.h"
#include "ns3/simulator.h"

#include "ns3/ipv4.h"

#include "ns3/mobility-model.h"
#include "ns3/rectangle.h"
#include "ns3/rapidnet-application-base.h"
#include "decorator-frontend.h"

using namespace ns3;
using namespace std; 
using namespace ns3::rapidnet;

NS_LOG_COMPONENT_DEFINE("Decorator");

//--------------------------------------------------------------
//----------------------------------------------
DecoratorFrontend::DecoratorFrontend() :
  m_ostream(0),
  m_bounds(0, 0, 0, 0)
{
}
DecoratorFrontend::~DecoratorFrontend()
{
}
void
DecoratorFrontend::DoDispose(void)
{

  Object::DoDispose();
  // DecoratorFrontend::DoDispose
}

void
DecoratorFrontend::SetStream(std::ostream *os)
{
  m_ostream = os;
  // end SetStream
}

void
DecoratorFrontend::SetArena(Rectangle r)
{
  NS_LOG_INFO(r);
  m_bounds = r;
  // end DecoratorFrontend::SetArena
}
Rectangle
DecoratorFrontend::GetArena()
{
  return m_bounds;
  // end DecoratorFrontend::GetArena
}

//--------------------------------------------------------------
//----------------------------------------------
void
DecoratorFrontend::Init()
{

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback(&DecoratorFrontend::CourseChange, this));

  Ipv4Address loopback("127.0.0.1");

  (*m_ostream) << "arena " <<
    m_bounds.xMin << " " <<
    m_bounds.yMin << " " <<
    m_bounds.xMax << " " <<
    m_bounds.yMax << std::endl;

  Time now = Simulator::Now();

  for (NodeList::Iterator iter = NodeList::Begin();
       iter != NodeList::End();
       iter++) {
    Ptr<Node> node = (*iter);
    
    Ptr<RapidNetApplicationBase> rapidNetApplication = node->GetApplication (0)->GetObject<RapidNetApplicationBase> ();
    vector<Ipv4Address> addressList = rapidNetApplication->GetAddressList ();
      for (vector<Ipv4Address>::iterator iterator = addressList.begin (); iterator != addressList.end (); iterator++)
      {
        Ipv4Address ipv4Address = *iterator;
       if (ipv4Address != loopback) {
          (*m_ostream) << now << " ip " << node->GetId() << " " <<
            ipv4Address << std::endl;
      }
    }
 
	
    Ptr<MobilityModel> mobility = node->GetObject<MobilityModel>();

    if (mobility != 0) {
      Vector pos = mobility->GetPosition();
      Vector vel = mobility->GetVelocity ();

      (*m_ostream) << now << " position " << node->GetId() << " " <<
        pos.x << " " << pos.y << " " << pos.z << " " <<
        vel.x << " " << vel.y << " " << vel.z << std::endl;
    }

    // end iterate over nodes
  }

  // end DecoratorFrontend::Init
}

//----------------------------------------------
void
DecoratorFrontend::CourseChange(std::string foo,
                                Ptr<const MobilityModel> mobility)
{

  Vector pos = mobility->GetPosition ();
  Vector vel = mobility->GetVelocity ();

  Ptr<Node> node = mobility->GetObject<Node>();
  NS_ASSERT(node != 0);

  (*m_ostream) << Simulator::Now () << " position " << node->GetId() << " " <<
    pos.x << " " << pos.y << " " << pos.z << " " <<
    vel.x << " " << vel.y << " " << vel.z << std::endl;

  // end DecoratorFrontend::CourseChange
}

//--------------------------------------------------------------
//----------------------------------------------
void
DecoratorFrontend::AddLinkState(Ptr<Node> node, Ipv4Address target,
                                std::string state)
{

  NS_ASSERT(node != 0);
  NS_ASSERT(state != "");

  (*m_ostream) << Simulator::Now () << " link " << node->GetId() <<
    " " << target << " +" << state << std::endl;

  // end DecoratorFrontend::Link
}

void
DecoratorFrontend::RemoveLinkState(Ptr<Node> node, Ipv4Address target,
                                   std::string state)
{

  NS_ASSERT(node != 0);
  NS_ASSERT(state != "");

  (*m_ostream) << Simulator::Now () << " link " << node->GetId() <<
    " " << target << " -" << state << std::endl;

  // end DecoratorFrontend::Link
}

//--------------------------------------------------------------
//----------------------------------------------
void
DecoratorFrontend::AddNodeState(Ptr<Node> node,
                                std::string state)
{

  NS_ASSERT(node != 0);
  NS_ASSERT(state != "");

  (*m_ostream) << Simulator::Now () << " state " << node->GetId() <<
    " +" << state << std::endl;

  // end DecoratorFrontend::Link
}
void
DecoratorFrontend::RemoveNodeState(Ptr<Node> node,
                                   std::string state)
{

  NS_ASSERT(node != 0);
  NS_ASSERT(state != "");

  (*m_ostream) << Simulator::Now () << " state " << node->GetId() <<
    " -" << state << std::endl;

  // end DecoratorFrontend::Link
}
