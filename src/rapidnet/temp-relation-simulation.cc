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
#include "temp-relation.h"
#include "relation-base.h"

using namespace ns3;
using namespace ns3::rapidnet;

TypeId
TempRelation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::TempRelation")
    .SetParent<RelationBase> ()
  ;
  return tid;
}

TempRelation::TempRelation (string name)
  : RelationBase (name)
{
}

void
TempRelation::AddKeyAttribute (attrdeftype attribute)
{
  NS_ASSERT_MSG (false, "Temp relation '" << m_name <<
    "' does not support key attributes.");
}

int
TempRelation::Insert (Ptr<Tuple> tuple)
{
  m_tuples.push_back (tuple);
  OnInsert.Invoke (tuple);
  return DONTCARE;
}

void
TempRelation::Delete (Ptr<Tuple> tuple)
{
  list<Ptr<Tuple> >::iterator pos = find (m_tuples.begin (), m_tuples.end (), tuple);

  if (pos == m_tuples.end ())
    {
      NS_ASSERT_MSG (false, "TempRelation::Delete: Tuple not found " << tuple);
    }
  else
    {
      m_tuples.erase (pos);
      OnDelete.Invoke (tuple);
    }
}

list<Ptr<Tuple> >
TempRelation::GetAllTuples ()
{
  return m_tuples;
}

uint32_t
TempRelation::Count ()
{
  return m_tuples.size ();
}

Ptr<RelationBase>
TempRelation::New (string name)
{
  Ptr<TempRelation> relation = CreateObject<TempRelation> ();
  relation->SetName (name);
  return Ptr<RelationBase> (dynamic_cast<RelationBase*> (PeekPointer (
    relation)));
}

