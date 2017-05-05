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
#ifndef RAPIDNET_APPLICATION_HELPER_H
#define RAPIDNET_APPLICATION_HELPER_H

#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/ref-count-base.h"
#include "rapidnet-application-base.h"

namespace ns3 {
namespace rapidnet {

/**
 * \brief A base helper for all RapidNet applications.
 */
class RapidNetApplicationHelper : public RefCountBase
{
public:
  RapidNetApplicationHelper () {}

  virtual ~RapidNetApplicationHelper () {}

  void SetAttribute (std::string name, const AttributeValue &value);

  /**
   * \brief Installs the RapidNet application to the nodes in the given
   *        node container.
   */
  ApplicationContainer Install (NodeContainer container);

  /**
   * \brief Install the RapidNet application to the given node.
   */
  ApplicationContainer Install (Node node);

protected:

  /**
   * \brief Sets the address of the RapidNet application to the IP
   *        address of the node.
   */
  void SetAddress (Ptr<Node> node, Ptr<RapidNetApplicationBase> application);

  /**
  * \brief Factory method for creating P2 applications
  *
  */
  virtual Ptr<RapidNetApplicationBase> CreateNewApplication () = 0;

  ObjectFactory m_factory;
};

} // namespace rapidnet
} // namespace ns3

#endif // RAPIDNET_APPLICATION_HELPER_H

