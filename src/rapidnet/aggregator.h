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
#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include "ns3/ref-count-base.h"
#include "tuple.h"
#include "relation.h"
#include "selector.h"
#include "rapidnet-application-base.h"
#include "rapidnet-functions.h"

using namespace ns3;

namespace ns3 {
namespace rapidnet {

class Tuple;

/**
 * \ingroup rapidnet_library
 *
* \brief An base class for all database aggregate operations.
*
*/
class Aggregator: public RefCountBase
{
public:

  /**
   * \brief Initializes the aggregate implementation object.
   *
   * It creates an internal table with all tuples and a results table
   * with the aggregated results. It sets the all attributes for the
   * internal table and also specifies the position number of the attribute
   * over which the aggregate is defined. The attributes before this
   * in position are used to define the group (as in the GROUP BY command
   * in SQL)
   */
  virtual void Init (string name, Ptr<RapidNetApplicationBase> app,
    list<attrdeftype> allAttrs, uint32_t aggrPos);

  virtual ~Aggregator () {}

  /**
   * \brief Inserts a tuple into the aggregate.
   */
  virtual void Insert (Ptr<Tuple> tuple);

  /**
   * \brief Deletes a tuple from the aggregate.
   */
  virtual void Delete (Ptr<Tuple> tuple);

protected:

  /**
   * \brief Comparison of tuples to be implemented by sub-classes.
   */
  virtual bool Compare (Ptr<Tuple> tuple1, Ptr<Tuple> tuple2) = 0;

  string m_aggrAttrName;

  Ptr<RapidNetApplicationBase> m_application;

  /**
   * \brief Relation with the aggregated results
   */
  Ptr<Relation> m_aggrResults;

  /**
   * \brief Temporary relation that contains all tuples.
   */
  Ptr<Relation> m_tempReln;

  friend class InsertOnInsertTrigger;
  friend class DeleteOnDeleteTrigger;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Implements the database aggregate operation MIN.
 */
class AggrMin : public Aggregator
{
public:

  virtual ~AggrMin () {}

  /**
   * \brief Factory method to create a new AggrMin instance.
   *
   * @param name Name of the aggregate result relation.
   * @param app Associated RapidNet application
   * @param allAttrs List of all attributes in the tuples.
   * @param aggrPos The 1-based position of the attribute in allAttrs
   *                over which the aggregate is defined.
   */
  static Ptr<Aggregator> New (string name, Ptr<RapidNetApplicationBase> app,
    list<attrdeftype> allAttrs, uint32_t aggrPos);

protected:

  virtual bool Compare (Ptr<Tuple> tuple1, Ptr<Tuple> tuple2);
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Implements the database aggregate operation MAX.
 */
class AggrMax : public Aggregator
{
public:

  virtual ~AggrMax () {}

  /**
   * \brief Factory method to create a new AggrMax instance.
   *
   * @param name Name of the aggregate result relation.
   * @param app Associated RapidNet application
   * @param allAttrs List of all attributes in the tuples.
   * @param aggrPos The 1-based position of the attribute in allAttrs
   *                over which the aggregate is defined.
   */
  static Ptr<Aggregator> New (string name, Ptr<RapidNetApplicationBase> app,
    list<attrdeftype> allAttrs, uint32_t aggrPos);

protected:

  virtual bool Compare (Ptr<Tuple> tuple1, Ptr<Tuple> tuple2);
};

class InsertOnInsertTrigger : public AppTrigger
{
public:

  virtual void Invoke (Ptr<Tuple> tuple)
  {
    GetApplication ()->Insert (tuple);
  }

  ~InsertOnInsertTrigger () {}
};

class DeleteOnDeleteTrigger : public AppTrigger
{
public:

  virtual void Invoke (Ptr<Tuple> tuple)
  {
    GetApplication ()->Delete (tuple);
  }

  ~DeleteOnDeleteTrigger () {}
};

class FMatches : public FunctionExpr
{
public:

  virtual ~FMatches () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Tuple> tuple);

protected:

  Ptr<Tuple> m_tuple;
};

} //namespace rapidnet
} //namepsace ns3

#endif // AGGREGATOR_H
