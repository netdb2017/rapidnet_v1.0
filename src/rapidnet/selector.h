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

#ifndef SELECTOR_H
#define SELECTOR_H

#include "ns3/ref-count-base.h"
#include "ns3/tuple.h"

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief Implements the database select operation. It evaluates
 *        an expression against the given tuple and returns true if
 *        the tuple should be selected, false otherwise.
 */
class Selector: public RefCountBase
{
public:

  Selector ();

  /**
   * \brief Evaluates the selection expression and returns true
   *        if the given tuple should be selected. False otherwise.
   */
  bool Select (Ptr<Tuple> tuple);

  /**
   * \brief Factory method to create a new selector object.
   *
   * @param expr The expression that is evaluated against the given
   *             tuple to determine whether it should be selected or
   *             not.
   */
  static Ptr<Selector> New (Ptr<Expression> expr);

  virtual ~Selector ();

public:

  Ptr<Expression> m_expr;
};

} // namespace rapidnet
} // namespace ns3

#endif // SELECTOR_H
