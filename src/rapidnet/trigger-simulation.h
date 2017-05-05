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
#ifndef TRIGGER_H
#define TRIGGER_H

#include "ns3/ref-count-base.h"
#include "tuple.h"

using namespace ns3;

namespace ns3 {
namespace rapidnet {

class Tuple;

/**
 * \ingroup rapidnet_library
 *
* \brief A callback function object that serves as a database trigger.
*/
class Trigger : public RefCountBase
{
public:

  virtual void Invoke (Ptr<Tuple> tuple) = 0;

  virtual ~Trigger () {}
};

/**
 * \ingroup rapidnet_library
 *
* \brief A list of triggers
*/
class TriggerList
{
public:

  void operator += (Ptr<Trigger> trig)
  {
    m_list.push_back (trig);
  }

  void Invoke (Ptr<Tuple> tuple)
  {
    for (list<Ptr<Trigger> >::iterator it = m_list.begin ();
      it != m_list.end(); it++)
      {
        (*it) -> Invoke (tuple);
      }
  }

protected:

  list<Ptr<Trigger> > m_list;
};

} //namespace rapidnet
} //namepsace ns3

#endif // TRIGGER_H
