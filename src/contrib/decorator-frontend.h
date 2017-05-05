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

#ifndef __DECORATOR_H__
#define __DECORATOR_H__

#include "ns3/object.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/rectangle.h"
#include "ns3/mobility-model.h"

namespace ns3 {

  class DecoratorFrontend : public Object {
  public:
    DecoratorFrontend();
    ~DecoratorFrontend();

    void SetStream(std::ostream *os);
    void SetArena(Rectangle r);
    Rectangle GetArena();

    void Init();

    void AddLinkState(Ptr<Node> node, Ipv4Address target,
                      std::string state="default");
    void RemoveLinkState(Ptr<Node> node, Ipv4Address target,
                         std::string state="default");

    void AddNodeState(Ptr<Node> node, std::string state="default");
    void RemoveNodeState(Ptr<Node> node, std::string state="default");

  protected:
    virtual void DoDispose(void);
    std::ostream *m_ostream;

  private:
    Rectangle m_bounds;

    void CourseChange (std::string foo, Ptr<const MobilityModel> mobility);

    // end class DecoratorFrontend
  };


  // end namespace ns3
};

#endif // __DECORATOR_H__
