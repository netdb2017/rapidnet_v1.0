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
#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <map>
#include <list>
#include "ns3/assert.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/value.h"
#include "ns3/str-value.h"
#include "ns3/int32-value.h"
#include "tuple-attribute.h"

#define QUAL(qual, name) qual + ":" + name
#define EMPTY "EMPTY"

using namespace std;

namespace ns3 {
namespace rapidnet {

class Selector;
class Assignor;

/**
 * \ingroup rapidnet_library
 *
* \brief A RapidNet tuple. A RapidNet tuple is a collection of @see
* TupleAttribute objects that are mapped by their name.
*
* Each TupleAttribute object is a name-value pair. The RapidNet tuple is like
* a row in a database table. Tuple objects are either stored in the @see
* Database or they are sent over the network by serializing them within @see
* RapidNetHeader objects.
*
* Tuples also have names. If stored in a database, the tuple has the same name
* as the table object. See @see Relation.
*
*/
class Tuple : public Object
{
public:

  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId ()
  {
    return Tuple::GetTypeId();
  }

  Tuple (string name = "no-name");

  Tuple (Tuple& attrList);

  virtual ~Tuple ();

  /**
   * \brief Returns the size in bytes when serialized.
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serializes the object for transmitting over the
   *        network inside a RapidNet header.
   */
  virtual void Serialize (Buffer::Iterator& start) const;

  /**
   * \brief De-serializes the object that is embedded in a
   *         RapidNet header received from the network.
   */
  virtual uint32_t Deserialize (Buffer::Iterator& end);

  /**
   * \brief Returns the string representation of the TupleAttribute.
   */
  virtual string ToString () const;

  virtual void SetName(string name);

  virtual string GetName(void) const;

  /**
   * \brief Adds a new attribute if it does not already exists. Throws
   * an error otherwise.
   *
   */
  virtual void AddAttribute (Ptr<TupleAttribute> attr);

  /**
   * \brief Adds the attribute if it does not already exists. If it already
   *        exists, then it is overwritten with the given attribute object.
   */
  virtual void OverwriteAttribute (Ptr<TupleAttribute> attr);

  /**
   * \brief Adds the attribute objects in the given list to the tuple.
   */
  virtual void AddAttributes (list<Ptr<TupleAttribute> > attributes);

  /**
   * \brief Returns true if an attribute is found and removed, false
   * otherwise.
   *
   */
  virtual bool RemoveAttribute (string name);

  /**
   * \brief Returns the attribute with the given name if one exists. Throws
   *        an error otherwise.
   */
  virtual Ptr<TupleAttribute> GetAttribute (string name);

  /**
   * \brief Returns true if the tuple has an attribute with the given name.
   *        Returns false otherwise.
   */
  virtual bool HasAttribute (string name);

  /**
   * \brief Returns all the attributes as a map, keyed by their names.
   */
  virtual map<string, Ptr<TupleAttribute> >& GetAllAttributes ();

  /**
   * \brief Checks if the given list attribute values match with
   * the current tuple.
   *
   */
  virtual bool Matches (Ptr<Tuple> tuple);

  /*
   * \brief Adds all attributes of another attribute list to itself.
   * The attribute names are qualified based on the second argument.
   *
   */
  virtual void AddAllAttributes (Ptr<Tuple> attrList, bool qualified = false);

  /**
   * \brief Performs a project operation and returns a new tuple with the
   * given name and the requested attributes that are renamed correspondingly
   * with the given new names.
   *
   */
  virtual Ptr<Tuple> Project (string newTupleName, list<string> attrNames,
    list<string> newNames = list<string> ());

  /**
   * \brief Invokes the given Assignor with this tuple as the argument.
   *
   */
  virtual void Assign (Ptr<Assignor> assignor);

  /**
   * \brief Invokes the given Selector with this tuple as the argument.
   *
   * If the selector evaluates to true then this tuple is returned. Otherwise,
   * an "empty" tuple is returned. See NewEmpty().
   */
  virtual Ptr<Tuple> Select (Ptr<Selector> selector);

  /**
   * \brief Returns true if this tuple is equal to the given tuple.
   */
  virtual bool Equals (Ptr<Tuple> tuple);

  /**
   * \brief Returns true if this is an empty tuple.
   */
  virtual bool IsEmpty ()
  {
    return GetName () == EMPTY;
  }

  void SetTimestampNow ()
  {
    m_timestamp = Simulator::Now ();
  }

  Time GetTimestamp ()
  {
    return m_timestamp;
  }

  void IncRefCount ()
  {
    m_refCount++;
  }

  unsigned int DecRefCount ()
  {
    if (m_refCount > 0) 
      {
        m_refCount--;
      }	
    return m_refCount;
  }

  unsigned int GetRefCount ()
  {
    return m_refCount;
  }

  static bool Less (Ptr<Tuple> l1, Ptr<Tuple> l2);

  /**
   * \brief Factory method to create a new tuple with the given name or
   *        a default name and no attributes.
   */
  static Ptr<Tuple> New (string name = "no-name");

  /**
   * \brief Factory method to create a new tuple with the given name and
   *        the attributes in the given list added to it.
   */
  static Ptr<Tuple> New (string name, list<Ptr<TupleAttribute> > attributes);

  /**
   * \brief Creates and returns a new special tuple called the empty tuple.
   *
   * This tuple has a dummy name and no attributes. If a Selector
   * condition evaluates to false, then an empty tuple is returned.
   */
  static Ptr<Tuple> NewEmpty ()
  {
    return New (EMPTY);
  }

protected:

  map<string, Ptr<TupleAttribute> > m_attributes;

  Ptr<StrValue> m_name;

  /**
   * \brief Timestamp used for soft-state timeout.
   */
  Time m_timestamp;

  /**
   * \brief Reference counter used for hard-state.
   */
  uint32_t m_refCount;
};

ostream& operator << (ostream& os, const Ptr<Tuple>& tuple);

} //namespace rapidnet
} //namepsace ns3

#endif // TUPLE_H
