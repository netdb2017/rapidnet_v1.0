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

#ifndef ASSIGNOR_H_
#define ASSIGNOR_H_

#include "ns3/ref-count-base.h"
#include "tuple.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief Implements the database assignment operation. An assignor assigns
 *        an attribute with a given name to a tuple by evaluating an
 *        @see Expression.
 */
class Assignor : public RefCountBase
{
public:

  Assignor ();

  virtual ~Assignor ();

  /**
   * \brief Assigns the attribute to the given tuple.
   */
  virtual void Assign (Ptr<Tuple> tuple);

  /**
   * \brief Factory method for creating a new assignor.
   *
   * @param target Name of the attribute to be assigned.
   * @param expr The @see Expression object that is used to evaluate
   *        the value to be assigned.
   */
  static Ptr<Assignor> New (string target, Ptr<Expression> expr);

protected:

  /**
   * \brief Name of the target attribute
   */
  string m_target;

  Ptr<Expression> m_expr;
};

} // namespace rapidnet
} // namespace ns3

#endif /* ASSIGNOR_H_ */
