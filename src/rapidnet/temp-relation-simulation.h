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
#ifndef TEMP_RELATION_H
#define TEMP_RELATION_H

#include "relation-base.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
* \brief A relation that is implemented as a list of tuples.
*
* It is used to return results of database operations like join, select,
* project, etc. It defines no key attributes.
*/
class TempRelation : public RelationBase
{
public:

  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId ()
  {
    return TempRelation::GetTypeId ();
  }

  TempRelation (string name = "no-name");

  virtual ~TempRelation() {}

  /**
   * \brief This method is not supported. An error is thrown.
   */
  virtual void AddKeyAttribute (attrdeftype attribute);

  /**
   * \brief Adds a tuple to the relation.
   */
  virtual int Insert (Ptr<Tuple> tuple);

  /**
   * \brief Deletes the tuple from the relation.
   */
  virtual void Delete (Ptr<Tuple> tuple);

  /**
   * \brief Returns all the tuples as a list.
   */
  virtual list<Ptr<Tuple> > GetAllTuples ();

  /**
   * \brief Returns the number of tuples in the relation.
   */
  virtual uint32_t Count ();

  /**
   * \brief Factory method to create a new TempRelation.
   */
  static Ptr<RelationBase> New (string name);

protected:

  list<Ptr<Tuple> > m_tuples;

  /**
   * \brief Used by the static factory method only.
   */
  void SetName (string name)
  {
    m_name = name;
  }
};

} //namespace rapidnet
} //namepsace ns3

#endif // TEMP_RELATION_H
