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
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <map>
#include "ns3/assert.h"
#include "relation.h"
#include "rapidnet-application-base.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

class RapidNetApplicationBase;

/**
 * \ingroup rapidnet_library
 *
 * \brief A RapidNet database.
 *
 * It is implemented as a map of @see Relation objects keyed
 * by their name. The database object is associated with one
 * @see RapidNetApplicationBase object to which it belongs.
 */
class Database : public Object
{
public:

  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId () { return Database::GetTypeId(); }

  Database ();

  virtual ~Database();

  /**
   * \brief Returns true if a relation with the given name exists in this
   *        database.
   */
  virtual bool HasRelation (string name);

  /**
   * \brief Adds a relation to the database if it does not already exist.
   *        Throws an error otherwise.
   */
  virtual void AddRelation (Ptr<RelationBase> reln);

  /**
   * \brief Removes the relation with the given name from this database.
   */
  virtual bool RemoveRelation (string name);

  /**
   * \brief Returns the relation with the given name, throws an error otherwise.
   */
  virtual Ptr<RelationBase> GetRelation (string name);

  /**
   * \brief Inserts the given tuple into the relation with the same name as the
   *        tuple. If no relation with this name exists, an error is thrown.
   */
  int Insert (Ptr<Tuple> tuple);

  void Delete (Ptr<Tuple> tuple);

  /**
   * \brief Returns the associated @see RapidNet application object.
   */
  Ptr<RapidNetApplicationBase> GetApplication ()
  {
    return m_application;
  }

  /**
   * \brief Returns name-relation map in this database.
   */
  map<string, Ptr<RelationBase> > GetAllRelations ()
  {
    return m_relations;
  }

  /**
   * \brief Factory method to create a new database.
   */
  static Ptr<Database> New (Ptr<RapidNetApplicationBase> application);

protected:

  /**
  * \brief Removes RapidNet specific attributes from the tuple before
  *        insertion into the database relation.
  */
  virtual void CleanTupleBeforeInsert (Ptr<Tuple> &tuple);

  map<string, Ptr<RelationBase> > m_relations;

  Ptr<RapidNetApplicationBase> m_application;

};

} //namespace rapidnet
} //namepsace ns3

#endif // DATABASE_H
