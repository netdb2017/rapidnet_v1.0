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
#include "aggwrap.h"
#include <list>
#include <cstdlib>
#include <sstream>
#include "ns3/int32-value.h"
#include "ns3/bool-value.h"
#include "ns3/list-value.h"
#include "ns3/tuple.h"
#include "temp-relation.h"

using namespace ns3;
using namespace ns3::rapidnet;

const string AggWrapCount::COUNT_ATTR = "count";
const string AggWrapList::LIST_ATTR = "list";

Ptr<Relation>
AggWrap::GetNewAggrRelation (string name)
{
  list<attrdeftype>::iterator it;
  uint32_t i;

  Ptr<Relation> aggrResults = Relation::New (name);
  for (it = m_allAttrs.begin (), i = 1; i < m_aggrPos &&
    it != m_allAttrs.end (); ++it, ++i)
    {
      aggrResults->AddKeyAttribute (*it);
    }
  // Name of the attribute on which aggregate is applied
  m_aggrAttrName = it->first;

  return aggrResults;
}

Ptr<RelationBase>
AggWrapList::Compute (Ptr<RelationBase> result)
{
  NS_ASSERT_MSG (false, "AggWrapList::Compute (Ptr<RelationBase>) "
      "is not supported.");

  return Ptr<RelationBase> (NULL);
}

Ptr<RelationBase>
AggWrapList::Compute (Ptr<RelationBase> result, Ptr<Tuple> event, const string attrName)
{
  Ptr<Tuple> clone = CopyObject<Tuple> (event);

  list<Ptr<Tuple> > allTuples = result->GetAllTuples();

  list<Ptr<Value> > tmpResult;

  for (list<Ptr<Tuple> >::iterator it = allTuples.begin ();
    it != allTuples.end (); ++it)
    {
      //      cout<<"current tuple: "<<(*it)->ToString()<<endl;

      // pick the value of the attribute whose name is attrName
      Ptr<Value> current = (*it)->GetAttribute(attrName)->GetValue();

      // add the values into the list
      tmpResult.push_back (current);
    }

  clone->AddAttribute(TupleAttribute::New (LIST_ATTR, 
    ListValue::New (tmpResult)));

  Ptr<RelationBase> retval = TempRelation::New (result->GetName ());
  retval->Insert (clone);

  return retval;
}

Ptr<AggWrapList>
AggWrapList::New ()
{
  return Create<AggWrapList> ();
}

Ptr<RelationBase>
AggWrapCount::Compute (Ptr<RelationBase> result)
{
  NS_ASSERT_MSG (false, "AggWrapCount::Compute (Ptr<RelationBase>) "
      "is not supported.");

  return Ptr<RelationBase> (NULL);
}

Ptr<RelationBase>
AggWrapCount::Compute (Ptr<RelationBase> result, Ptr<Tuple> event)
{
  Ptr<Tuple> clone = CopyObject<Tuple> (event);
  clone->AddAttribute(TupleAttribute::New (COUNT_ATTR,
    Int32Value::New (result->Count())));

  Ptr<RelationBase> retval = TempRelation::New (result->GetName ());
  retval->Insert (clone);
  return retval;
}

Ptr<AggWrapCount>
AggWrapCount::New ()
{
  return Create<AggWrapCount> ();
}

Ptr<RelationBase>
AggWrapMinMax::Compute (Ptr<RelationBase> result)
{
  Ptr<Relation> aggrResults = GetNewAggrRelation (result->GetName ());
  aggrResults->SetRelaxed (true);

  // Add a dummy key if aggrPos is 1
  string dummyKey;
  string dummyKeyValue = "same-for-all";
  if (m_aggrPos == 1)
    {
      stringstream ss;
      ss << "aggrwapminmax-dummy-key-" << rand ();
      dummyKey = ss.str ();
      aggrResults->AddKeyAttribute (attrdeftype (dummyKey, ANYTYPE));
    }

  list<Ptr<Tuple> > allTuples = result->GetAllTuples();
  Ptr<TupleAttribute> dummyKeyAttr = TupleAttribute::New (dummyKey,
    StrValue::New (dummyKeyValue));

  for (list<Ptr<Tuple> >::iterator it = allTuples.begin ();
    it != allTuples.end (); ++it)
    {
      if (m_aggrPos == 1)
        {
          (*it)->AddAttribute (dummyKeyAttr);
        }
      Ptr<Tuple> tupleKey = aggrResults->GetKey (*it);
      //cout << "got: " << *it << endl;
      if (!aggrResults->KeyExists (tupleKey))
        {
          //cout << "inserting: " << *it << endl;
          aggrResults->Insert (*it);
        }
      else
        {
          Ptr<Tuple> currentTuple = aggrResults->GetTupleForKey (tupleKey);
          if (Compare (*it, currentTuple))
            {
              //cout << "overwriting: " << *it << endl;
              aggrResults->Insert (*it);
            }
        }
    }

  return aggrResults;
}

bool
AggWrapMin::Compare (Ptr<Tuple> newTuple, Ptr<Tuple> currentTuple)
{
  Ptr<Expression> var = VarExpr::New (m_aggrAttrName);

  return rn_bool (var->Eval (newTuple)->Eval (RN_LT, currentTuple, var));
}

bool
AggWrapMax::Compare (Ptr<Tuple> newTuple,  Ptr<Tuple> currentTuple)
{
  Ptr<Expression> var = VarExpr::New (m_aggrAttrName);

  return rn_bool (var->Eval (newTuple)->Eval (RN_GT, currentTuple, var));
}
