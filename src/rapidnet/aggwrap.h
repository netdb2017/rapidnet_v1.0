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

#ifndef AGGWRAP_H
#define AGGWRAP_H

#include "ns3/ref-count-base.h"
#include "ns3/assert.h"
#include "relation-base.h"
#include "relation.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief A base class for all aggregate-wrap implementation.
 *
 * An aggregate wrap is an aggregate that wraps around a rule strand. Its life
 * time is only until the rule is executing. It produces an aggregate view of
 * a materialized or computed relation. This view may be used to compute the
 * result of the rule that is fired and after that the aggregate wrap is destroyed.
 *
 * Note that AggWrapCount works a bit differently.
 */
class AggWrap: public RefCountBase
{
public:
  virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result) = 0;

  /**
   * \brief A template based Factory method.
   *
   * Subtype should either be AggWrapMin or AggWrapMax. Not AggWrapCount
   * since it works differently.
   *
   * @param allAttrs The list of all attributes in the input relation.
   * @param aggrPos The position number (1-based) of the attribute over which
   *                the aggregate is defined.
   *
   */
  template<class SubType>
  static Ptr<AggWrap> New (list<attrdeftype> allAttrs, uint32_t aggrPos);

protected:

  /**
   * Create and return a new relation for computing the aggregate results.
   */
  Ptr<Relation> GetNewAggrRelation (string name);

  int m_type;

  list<attrdeftype> m_allAttrs;

  uint32_t m_aggrPos;

  string m_aggrAttrName;
};

template <class SubType>
Ptr<AggWrap>
AggWrap::New (list<attrdeftype> allAttrs, uint32_t aggrPos)
{
  Ptr<SubType> retval = Create<SubType> ();
  retval->m_allAttrs = allAttrs;
  retval->m_aggrPos = aggrPos;

  NS_ASSERT_MSG (aggrPos <= allAttrs.size (),
    "AggWrap::New: Aggregate position greater than number of attributes.");

  return retval;
};

class AggWrapList: public AggWrap
{
public:
  const static string LIST_ATTR;

  virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result);

  virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event, const string attrName);

  static Ptr<AggWrapList> New ();
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Aggregate wrap implementation for the count aggregate.
 *
 * \internal For now, only count<*> is implemented.
 */
class AggWrapCount: public AggWrap
{
public:
  const static string COUNT_ATTR;

  /**
   * \brief Compute method as inherited from AggWrap that is not
   *        implemented. @see Compute() which implements the actual
   *        functionality.
   */
  virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result);

  /**
   * \brief Computes the aggregate result.
   *
   * The count aggregate wrap is also required to execute the last join
   * operation with the event tuple in a series of joins that may be involved
   * in the execution of the rule. This is because, if that last join result
   * is empty, then the count value is set to 0 and all other attributes are
   * retrieved from the event that fired the rule. This is precisely the reason
   * why count aggwrap is implemented differently.
   *
   * @param result The input relation on which this aggregate is applied.
   * @param event The event that triggered the rule that instantiated this
   *              aggwrap.

   */
  virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event);

  static Ptr<AggWrapCount> New ();
};

/**
 * \ingroup rapidnet_library
 *
 * \brief A single aggregate wrap for both Min/Max. We use the template
 *        method pattern to let the base classes implement the @see
 *        Compare() method.
 *
 * @see AggWrapMin and @see AggWrapMax
 */
class AggWrapMinMax: public AggWrap
{
public:

  virtual Ptr<RelationBase> Compute (Ptr<RelationBase> reln);

protected:

  virtual bool Compare (Ptr<Tuple> newTuple, Ptr<Tuple> currentTuple) = 0;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Aggregate wrap for the min aggregate operation.
 */
class AggWrapMin: public AggWrapMinMax
{
protected:
  virtual bool Compare (Ptr<Tuple> newTuple, Ptr<Tuple> currentTuple);
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Aggregate wrap for the max aggregate operation.
 */
class AggWrapMax: public AggWrapMinMax
{
protected:
  virtual bool Compare (Ptr<Tuple> newTuple, Ptr<Tuple> currentTuple);
};

} // namespace rapidnet
} // namespace ns3

#endif // AGGWRAP_H
