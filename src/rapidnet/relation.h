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
#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <list>
#include "ns3/assert.h"
#include "ns3/nstime.h"
#include "relation-base.h"
#include "rapidnet-utils.h"

#include <boost/serialization/map.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

using namespace std;

//Forward declaration of class boost::serialization::access
namespace boost{
  namespace serialization{
    class access;
  }
}

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief A database relation with key attributes defined.
 *
 * The RapidNet @see Database contains relations of this type. It maps
 * tuples by their keys. Methods @see AddKeyAttribute() and @see
 * AddKeyAttributes() are used to define the keys. A key attribute is defined
 * by the name and type of the attribute. A relation must have at least one
 * key attribute. All tuples inserted must have those attributes of matching
 * types and any other additional attributes.
 *
 * Usually, tuples in a relation will have the same set of non-key attributes
 * as well. But there is no way to enforce this. The schema of the relation is
 * defined only by the name and type of the key attributes.
 *
 * Implementation note: This class maintains a map in which the value is the tuple
 * and the key is another tuple that is contains only the key attributes of the
 * corresponding tuple.
 *
 */
class Relation : public RelationBase
{
public:

  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId ()
  {
    return Relation::GetTypeId ();
  }

  Relation (string name = "no-name");

  virtual ~Relation () {}

  /**
   * \brief Adds a key attribute to the schema.
   */
  virtual void AddKeyAttribute (attrdeftype attrdef);

  virtual void AddKeyAttributes (list<attrdeftype> attributes);

  /**
   * \brief Verifies if the tuple has all the key attributes of the given type
   *        and returns the key attributes as a Tuple.
   */
  virtual Ptr<Tuple> GetKey (Ptr<Tuple> tuple);

  /**
   * \brief Returns true if a tuple with the given key exists in the relation.
   */
  virtual bool KeyExists (Ptr<Tuple> tupleKey);

  /**
   * \brief Returns the tuple corresponding to the given key in the relation.
   */
  virtual Ptr<Tuple> GetTupleForKey (Ptr<Tuple> tupleKey);

  /**
   * \brief Adds a tuple to the relation.
   */
  virtual int Insert (Ptr<Tuple> tuple);

  /**
   * \brief Deletes a tuple from the relation.
   */
  virtual void Delete (Ptr<Tuple> tuple);

  /**
   * \brief Returns all the tuples as a list.
   */
  virtual list<Ptr<Tuple> > GetAllTuples ();

  /**
   * \brief Clear all tuples
   */
  virtual void ClearAllTuples ();

  /**
   * \brief Returns the number of tuples in the relation.
   */  virtual uint32_t Count ();
  virtual void SetRelaxed (bool value = true);

  /**
   * \brief Factory method to create a new relation with
   *        the given name.
   */
  static Ptr<Relation> New (string name);

protected:

  /* If set to true, means that type checking be relaxed. */
  bool m_relaxed;

  /**
   * \brief Map of key attributes
   */
  map<string, ValueTypeId> m_keyAttributes;

  typedef map<Ptr<Tuple>, Ptr<Tuple>, bool (*) (Ptr<Tuple>, Ptr<Tuple>)>
    TupleMap;

  /**
   * \brief Map of tuples against key attributes
   */
  TupleMap m_tuples;
};

} //namespace rapidnet
} //namepsace ns3

#endif // RELATION_H
