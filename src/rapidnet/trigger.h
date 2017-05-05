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

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/export.hpp>


using namespace ns3;

//Forward declaration of class boost::serialization::access
namespace boost{
  namespace serialization{
    class access;
  }
}

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

  Trigger(){};
   
  virtual void Invoke (Ptr<Tuple> tuple) = 0;

  virtual ~Trigger () {}

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Process ref count base!" << endl;    
    ar & boost::serialization::base_object<RefCountBase>(*this);
    std::cout << "ref count base done!" << endl;    
  }
};



/**
 * \ingroup rapidnet_library
 *
* \brief A list of triggers
*/
class TriggerList
{
public:

  TriggerList(){};

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

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Process triggers!" << endl;    
    ar & m_list;
    std::cout << "Triggers done!" << endl;    
  }

  list<Ptr<Trigger> > m_list;
};


} //namespace rapidnet
} //namepsace ns3


BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::Trigger)
BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::TriggerList)

#endif // TRIGGER_H
