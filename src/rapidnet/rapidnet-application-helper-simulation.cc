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
  SetAddress (Ptr<Node> (&node), application);
  node.AddApplication (application);
  apps.Add (application);
  return apps;
}

void
RapidNetApplicationHelper::SetAddress (Ptr<Node> node,
  Ptr<RapidNetApplicationBase> application)
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

} // namespace rapidnet
} // namespace ns3

