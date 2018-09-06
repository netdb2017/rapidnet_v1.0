/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Pennsylvania
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
#include "rapidnet-application-helper.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-address.h"
#include "ns3/net-device.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include <netdb.h>

namespace ns3 {
namespace rapidnet {

void
RapidNetApplicationHelper::SetAttribute (
  std::string name,
  const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
RapidNetApplicationHelper::Install (NodeContainer container)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = container.Begin (); i != container.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<RapidNetApplicationBase> application = CreateNewApplication ();
      if(m_l4Platform)
        application->SetL4Platform(true);
      
      SetAddress (node, application);
      node->AddApplication (application);
      application->SetNode(node);
      apps.Add (application);
    }
  return apps;
}

ApplicationContainer
RapidNetApplicationHelper::Install (Node node)
{
  ApplicationContainer apps;
  Ptr<RapidNetApplicationBase> application = CreateNewApplication ();
  if(m_l4Platform)
    application->SetL4Platform(true);
  SetAddress (Ptr<Node> (&node), application);
  node.AddApplication (application);
  apps.Add (application);
  return apps;
}

void
RapidNetApplicationHelper::SetAddress (Ptr<Node> node,
  Ptr<RapidNetApplicationBase> application)
{
  if (m_l4Platform == true)
  {

    if (m_localAddress != Ipv4Address::GetAny())
    {
      application->SetAddress (m_localAddress);
      std::clog << "localAddress: " << m_localAddress << endl;
      return;
    }	
    Ipv4Address local;
    char name[80];
    gethostname (name, 80);
    struct hostent *hp;  
    struct in_addr **addr_list;
    hp = gethostbyname (name);
    addr_list = (struct in_addr**)hp->h_addr_list;
    for (int i = 0; addr_list[i] != NULL; i++)
    {
      std::clog << "Available addresses: " << Ipv4Address (inet_ntoa (*addr_list[i])) << endl;    
    }
    for (int i = 0; addr_list[i] != NULL; i++)
      {        
         if (Ipv4Address (inet_ntoa (*addr_list[i])) != Ipv4Address::GetLoopback() ) 
            {
              // hp->addr_list[i] is a non-loopback address
              // bcopy (hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
              local = Ipv4Address (inet_ntoa (*addr_list[i]));
              application -> SetAddress (local);
              std::clog << "localAddress: " << local << endl;
              break;
            }
      }

   }
  else 
  {
    Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
    NS_ASSERT_MSG (ipv4 != 0,
      "RapidNetApplicationHelper: Ipv4 object not found in node!");

    Ipv4InterfaceAddress loopbackAddress (Ipv4Address::GetLoopback (),
      Ipv4Mask::GetLoopback ());

    for (uint32_t i = 0; i < node->GetNDevices (); i++)
      {
        Ptr<NetDevice> device = node->GetDevice (i);
        uint32_t id = ipv4->GetInterfaceForDevice (device);
        Ipv4InterfaceAddress ipv4InterfaceAddress = ipv4->GetAddress (id, 0);
        if (ipv4InterfaceAddress != loopbackAddress)
          {
            application->SetAddress (Ipv4Address (
              ipv4InterfaceAddress.GetLocal ()));
              return;
        }
      }
  }
}

void
RapidNetApplicationHelper::SetL4Platform (bool l4Platform)
{
  m_l4Platform = l4Platform;
}

void
RapidNetApplicationHelper::SetLocalAddress (Ipv4Address localAddress)
{
  m_localAddress = localAddress;
}

} // namespace rapidnet
} // namespace ns3

