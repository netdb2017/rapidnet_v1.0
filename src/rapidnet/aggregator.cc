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
#include "aggregator.h"
#include "ns3/int32-value.h"
#include "ns3/bool-value.h"
#define int32_value(attr) \
  rn_int32 (attr->GetValue ())


using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

void
Aggregator::Init (string name, Ptr<RapidNetApplicationBase> app,
  list<attrdeftype> allAttrs, uint32_t aggrPos)
{
  list<attrdeftype>::iterator it;
  uint32_t i;

  NS_ASSERT_MSG (aggrPos <= allAttrs.size (),
    "Aggregator::Init '" << name << "': Not enough attributes.");

  m_application = app;
  m_aggrResults = Relation::New (name);
  m_aggrResults->SetRelaxed (true);
  for (it = allAttrs.begin (), i = 1; i < aggrPos && it != allAttrs.end ();
    ++it, ++i)
    {
      m_aggrResults->AddKeyAttribute (*it);
    }

  Ptr<InsertOnInsertTrigger> insTrigger = Create<InsertOnInsertTrigger> ();
  insTrigger->SetApplication (app);
  m_aggrResults->OnInsert += insTrigger;

  Ptr<DeleteOnDeleteTrigger> delTrigger = Create<DeleteOnDeleteTrigger> ();
  delTrigger->SetApplication (app);
  m_aggrResults->OnDelete += delTrigger;

  m_aggrAttrName = it->first;

  m_tempReln = Relation::New (name);
  m_tempReln->SetRelaxed (true);
  m_tempReln->AddKeyAttributes (allAttrs);
}

void
Aggregator::Insert (Ptr<Tuple> tuple)
{
  
  m_tempReln->Insert (tuple);

  Ptr<Tuple> tupleKey = m_aggrResults->GetKey (tuple);
  //clog << m_application->GetAddress () << " [Insert] Tuple key " <<
  //  tupleKey << endl;

  if (!m_aggrResults->KeyExists (tupleKey))
    {
      m_aggrResults->Insert (CopyObject(tuple));
      //clog << m_application->GetAddress () <<
      //  " [Insert] Adding new key to aggregate " << tuple << endl;
    }
  else
    {
      Ptr<Tuple> currentTuple = m_aggrResults->GetTupleForKey (tupleKey);
      //int currentValue = currentTuple->GetAttribute (m_aggrAttrName)->GetInt32Value ();
      //int newValue = tuple->GetAttribute (m_aggrAttrName)->GetInt32Value ();
      //clog << m_application->GetAddress () << " currentValue: " <<
      //  currentValue << " newValue: " << newValue << endl;
      if (Compare (tuple, currentTuple))
        {
          m_aggrResults->Insert (CopyObject(tuple));
          //clog << m_application->GetAddress () <<
          //  " [Insert] Replacing key to aggregate " << tuple << endl;
        }
    }
}

void
Aggregator::Delete (Ptr<Tuple> tuple)
{

  m_tempReln->Delete (tuple);

  Ptr<Tuple> tupleKey = m_aggrResults->GetKey (tuple);
  Ptr<Tuple> current = m_aggrResults->GetTupleForKey (tupleKey);
  if (!current->Equals (tuple))
    {
      return;
    }
  //clog << m_application->GetAddress () <<
  //  "[Delete] Deleting current from aggregate " << current << endl;
  m_aggrResults->Delete (current);

  list<Ptr<Tuple> > groupTuples = m_tempReln->Select (Selector::New (
    FMatches::New (tupleKey)))->GetAllTuples ();
  if (groupTuples.size () == 0)
    {
      //clog << m_application->GetAddress () <<
      //  "[Delete] No replacement found for aggregate " << endl;
      return;
    }

  list<Ptr<Tuple> >::iterator it = groupTuples.begin (),
    optTuple = groupTuples.begin ();
  //int minValue, iValue;

  for (it++; it != groupTuples.end (); ++it)
    {
      //minValue = (*optTuple)->GetAttribute (m_aggrAttrName)->GetInt32Value ();
      //iValue = (*it)->GetAttribute (m_aggrAttrName)->GetInt32Value ();
      if (Compare (*it, *optTuple))
        {
          optTuple = it;
        }
    }

  //clog << m_application->GetAddress () <<
  //  "[Delete] Replacing in aggregate with " << *optTuple << endl;
  m_aggrResults->Insert (*optTuple);
}

bool
AggrMin::Compare (Ptr<Tuple> tuple1, Ptr<Tuple> tuple2)
{
  int value1 = int32_value (tuple1->GetAttribute (m_aggrAttrName));
  int value2 = int32_value (tuple2->GetAttribute (m_aggrAttrName));
  return value1 < value2;
}

Ptr<Aggregator>
AggrMin::New (string name, Ptr<RapidNetApplicationBase> app,
    list<attrdeftype> allAttrs, uint32_t aggrPos)
{
  Ptr<AggrMin> retval = Create<AggrMin> ();
  retval->Init (name, app, allAttrs, aggrPos);
  return Ptr<Aggregator> (
    dynamic_cast<Aggregator *> (PeekPointer (retval)));
}

bool
AggrMax::Compare (Ptr<Tuple> tuple1, Ptr<Tuple> tuple2)
{
  int value1 = int32_value (tuple1->GetAttribute (m_aggrAttrName));
  int value2 = int32_value (tuple2->GetAttribute (m_aggrAttrName));
  return value1 > value2;
}

Ptr<Aggregator>
AggrMax::New (string name, Ptr<RapidNetApplicationBase> app,
    list<attrdeftype> allAttrs, uint32_t aggrPos)
{
  Ptr<AggrMax> retval = Create<AggrMax> ();
  retval->Init (name, app, allAttrs, aggrPos);
  return Ptr<Aggregator> (
    dynamic_cast<Aggregator *> (PeekPointer (retval)));
}


Ptr<Value>
FMatches::Eval (Ptr<Tuple> tuple)
{
  return BoolValue::New (tuple->Matches (m_tuple));
}

Ptr<FunctionExpr>
FMatches::New (Ptr<Tuple> tuple)
{
  Ptr<FMatches> retval = Create<FMatches> ();
  retval->m_tuple = tuple;
  return retval;
}
