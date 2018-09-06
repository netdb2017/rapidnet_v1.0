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

#ifndef RAPIDNET_DECORATOR_FRONTEND_H
#define RAPIDNET_DECORATOR_FRONTEND_H

#include <string>
#include <iostream>
#include <list>
#include <map>

#include "ns3/decorator-frontend.h"
#include "ns3/tuple.h"
#include "ns3/rapidnet-utils.h"

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief A RapidNet extension to the decorator frontend used
 *        for visualization.
 */
class RapidNetDecoratorFrontend: public DecoratorFrontend
{
public:
  RapidNetDecoratorFrontend ();
  ~RapidNetDecoratorFrontend ();

  /**
   * \brief Add a tuple state.
   *
   * @param node The node at which the tuple state is to be added.
   * @param tuple The tuple in context
   * @param attrs The attributes of the tuple to be recorded
   */
  void AddTupleState (Ptr<Node> node, Ptr<Tuple> tuple,
    list<string>& attrs);

  /**
   * \brief Remove a tuple state.
   *
   * @param node The node at which the tuple state is to be removed.
   * @param tuple The tuple in context
   * @param attrs The attributes of the tuple to be recorded
   */
  void RemoveTupleState (Ptr<Node> node, Ptr<Tuple> tuple,
    list<string>& attrs);
};

} // namespace rapidnet
} // namespace ns3


#endif // RAPIDNET_DECORATOR_FRONTEND_H
