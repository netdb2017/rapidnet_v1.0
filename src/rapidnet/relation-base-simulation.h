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
#ifndef RELATION_BASE_H
#define RELATION_BASE_H

#include <string>
#include <list>
#include <algorithm>

#include "ns3/assert.h"
#include "tuple.h"
#include "selector.h"
#include "trigger.h"
#include "assignor.h"
#include "rapidnet-utils.h"

#define JOIN_NAMES(lname, rname) lname + "-join-" + rname
#define COUNT_STAR "COUNT_STAR"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet {

class Tuple;
class Trigger;

const bool IS_KEY = true;
const bool IS_NOT_KEY = false;

/**
 * \ingroup rapidnet_library
 *
 * \brief A base class for relation objects. A relation has a name and
 *        is a collection of @see Tuple objects.
 *
 * All tuples in the relation have the same name as the relation itself.
 * Two common relation objects are @see Relation that is defines a key
 * to map the tuples and @see TempRelation is simply a list of tuples
 * that is generated on applying a database operation like join, select,
 * etc.
 */
class RelationBase : public Object
{
public:

  enum { DONTCARE, INSERTED, UPDATED, REFRESHED };

  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId ()
  {
    return RelationBase::GetTypeId();
  }

  RelationBase(string name = "no-name");

  virtual ~RelationBase() {}

  virtual string GetName ();

  /**
   * \brief Adds a key attribute to the schema. This method can
   *        be used by sub-classes for reading the list of key
   *        attributes. Sub-classes may also choose to not
   *        implement it by throwing an error.
   */
  virtual void AddKeyAttribute (attrdeftype attribute) = 0;

  /**
   * \brief Adds a tuple to the relation.
   */
  virtual int Insert (Ptr<Tuple> tuple) = 0;

  /**
   * \brief Deletes a tuple from the relation.
   */
  virtual void Delete (Ptr<Tuple> tuple) = 0;

  /**
   * \brief Adds all tuples in the list to the relation.
   */
  virtual void InsertAll (list<Ptr<Tuple> > tuples);

  /**
   * \brief The list of triggers that are invoked when a tuple is inserted.
   */
  TriggerList OnInsert;

  /**
   * \brief The list of triggers that are invoked when a tuple is deleted.
   *
   */
  TriggerList OnDelete;

  /**
   * \brief The list of triggers that are invoked when a tuple is refreshed.
   *
   */
  TriggerList OnRefresh;

  /**
   * \brief Returns all the tuples as a list.
   */
  virtual list<Ptr<Tuple> > GetAllTuples () = 0;

  /**
   * \brief Returns the number of tuples in the relation.
   */
  virtual uint32_t Count () = 0;

  /**
   * \brief Prints all the tuples to the given stream.
   */
  virtual void PrintAllTuples (ostream& os);

  /**
   * \brief Implements a database project operation.
   *
   * It creates and returns a new TempRelation object and inserts all
   * the tuples in this relation to it by renaming them and their attributes
   *
   * @param newRelnName Name of the new project relation
   * @param attrNames Names of the attributes that need to be projects
   * @param newNames Corresponding new names for the projected attributes
   *                 If empty, the project attributes have the same name.
   *
   */
  virtual Ptr<RelationBase> Project (string newRelnName,
    list<string> attrNames, list<string> newNames = list<string> ());

  /**
   * \brief Implements a database select operation.
   *
   * Returns a new relation with only those tuples in this relation
   * that satisfy the given Selector.
   */
  virtual Ptr<RelationBase> Select (Ptr<Selector> selector);

  /**
   * \brief Implements a database join operation.
   *
   * Joins this relation with the given tuple.
   *
   * @param rTuple The tuple to be joined with (on the right).
   * @param lAttrs The join attributes for this relation (on the left).
   * @param rAttrs The corresponding join attributes for the tuple.
   * @param qualify If true, the attribute names in the resultant relation
   *        are qualified by their respective relation/tuple names.
   *
   */
  virtual Ptr<RelationBase> Join (Ptr<Tuple> rTuple, list<string> lAttrs,
    list<string> rAttrs, bool qualify = false);

  /**
   * \brief Implements a database join operation.
   *
   * Joins this relation with the given relation.
   *
   * @param reln The relation to be joined with (on the right).
   * @param lAttrNames The join attributes for this relation (on the left).
   * @param rAttrNames The corresponding join attributes for the tuple.
   * @param qualify If true, the attribute names in the resultant relation
   *        are qualified by their respective relation names.
   *
   * \internal Keep default qualification to false since all attributes are
   *           named as <tablename>_attr<pos>, example link_attr1, link_attr2.
   *           So they are implicitly qualified as of now.
   */
  virtual Ptr<RelationBase> Join (Ptr<RelationBase> reln,
    list<string> lAttrNames, list<string> rAttrNames, bool qualify = false);

  /**
   * \brief Invokes the given Assignor on all tuples of this relation.
   *
   */
  virtual void Assign (Ptr<Assignor> assignor);

  /**
   * \brief Returns the time-to-live, i.e., the soft-state timeout duration for
   *        this relation.
   */
  Time GetTimeToLive ()
  {
    return m_timeToLive;
  }

  /**
   * \brief Sets the time-to-live, i.e., the soft-state timeout duration for
   *        this relation.
   */
  void SetTimeToLive (Time ttl)
  {
    m_timeToLive = ttl;
  }

  /**
   * \brief Returns true if this is a soft-state relation, i.e. the time-to-live
   *        is finite.
   */
  bool IsSoftState ()
  {
    return m_timeToLive.GetSeconds () != TIME_INFINITY;
  }

protected:

  string m_name;

  /**
   * \brief Time-to-live (or TTL) for soft-state timeout.
   */
  Time m_timeToLive;

private:

  list<Ptr<TupleAttribute> > _GetAttributesAsList (Ptr<Tuple> tuple,
    list<string> attrNames);

  list<Ptr<Tuple> > _DoJoin (list<string> lAttrNames,
    Ptr<Tuple> rTuple, list<Ptr<TupleAttribute> > rAttrs, bool qualify);

  bool _IsMatch (Ptr<Tuple> lTuple, list<string> lAttrNames,
    list<Ptr<TupleAttribute> > rAttrs);
};

} //namespace rapidnet
} //namepsace ns3

#endif // RELATION_BASE_H
