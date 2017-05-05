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

#include "heap-relation.h"
#include "expression.h"
#include "ns3/bool-value.h"

using namespace ns3;
using namespace ns3::rapidnet;

HeapRelation::HeapRelation ()
{
  m_type = MIN;
  m_size = 0;
  m_heapAttr = "none";
}

HeapRelation::~HeapRelation ()
{
}

void
HeapRelation::SetType (HeapRelation::Type type)
{
  m_type = type;
}

void
HeapRelation::SetHeapAttribute (string attr)
{
  m_heapAttr = attr;
}

void
HeapRelation::SetHeapSize (uint32_t size)
{
  m_size = size;
}

int
HeapRelation::Insert (Ptr<Tuple> tuple)
{
  if (Count () < m_size)
    {
      Relation::Insert (tuple);
    }
  else
    {
      Operator evictPolicy = m_type == MIN ? RN_GT: RN_LT;
      TupleMap::iterator it = m_tuples.begin ();
      Ptr<Tuple> evictCandidate = it->second;
      Ptr<Expression> heapVar = VarExpr::New (m_heapAttr);

      for (++it; it != m_tuples.end (); ++it)
        {
          bool evict = rn_bool (heapVar->Eval (it->second)->Eval (
            evictPolicy, evictCandidate, heapVar));
          if (evict)
            {
              evictCandidate = it->second;
            }
        }
      Relation::Delete (evictCandidate);
      Relation::Insert (tuple);
    }
  return INSERTED;
}

Ptr<HeapRelation>
HeapRelation::New (string name, string heapAttr,
  HeapRelation::Type type, uint32_t size)
{
  Ptr<HeapRelation> retval = CreateObject<HeapRelation> ();
  retval->m_name = name;
  retval->m_heapAttr = heapAttr;
  retval->m_type = type;
  retval->m_size = size;
  return retval;
}
