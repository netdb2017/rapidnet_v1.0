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
#include "relation.h"
#include "ns3/log.h"

using namespace ns3;
using namespace ns3::rapidnet;

TypeId
Relation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::Relation")
    .SetParent<RelationBase> ()
  ;
  return tid;
}

Relation::Relation (string name)
  : RelationBase (name), m_tuples (Tuple::Less)
{
  m_relaxed = false;
  m_timeToLive = Seconds (TIME_INFINITY);
}

void
Relation::AddKeyAttribute (attrdeftype attribute)
{
  m_keyAttributes[attribute.first] = attribute.second;
}

void
Relation::AddKeyAttributes (list<attrdeftype> attributes)
{
  list<attrdeftype>::iterator it;
  for (it = attributes.begin (); it != attributes.end (); ++it)
    {
      AddKeyAttribute (*it);
    }
}

bool
Relation::KeyExists (Ptr<Tuple> tupleKey)
{
  int retval = m_tuples.find (tupleKey) != m_tuples.end ();
  return retval;
}


Ptr<Tuple>
Relation::GetTupleForKey (Ptr<Tuple> tupleKey)
{
  NS_ASSERT_MSG (KeyExists (tupleKey),
    "Tuple for key " << tupleKey << " does not exist in" <<
    " relation '" << m_name << "'");
  return m_tuples.find (tupleKey)->second;
}

int
Relation::Insert (Ptr<Tuple> tuple)
{
  Ptr<Tuple> tupleKey = GetKey (tuple);
  tuple->SetTimestampNow ();

  if (!KeyExists (tupleKey))
    {
      //New Key
      //Insert Case
      m_tuples [tupleKey] = tuple;
      OnInsert.Invoke (tuple);
      //OnInsert.Invoke (CopyObject<Tuple> (tuple));
      return INSERTED;
    }
  else if (m_tuples [tupleKey]->Equals (tuple))
    {
      //Key exists but tuples exactly
      //Refresh Case
      //Note: check for hard-state/soft-state
      if (IsSoftState ())
        {
          m_tuples [tupleKey]->SetTimestampNow ();
        }
      else
        {
          m_tuples [tupleKey]->IncRefCount ();
        }
      OnRefresh.Invoke (m_tuples [tupleKey]);
      //OnRefresh.Invoke (CopyObject<Tuple> (m_tuples [tupleKey]));
      return REFRESHED;
    }
  else
    {
      //Key exist - tuples don't match
      //Update Case
      Delete (GetTupleForKey (tupleKey));
      Insert (tuple);
      return UPDATED;
    }
}

void
Relation::Delete (Ptr<Tuple> tuple)
{
  Ptr<Tuple> tupleKey = GetKey (tuple);

  if (m_tuples.find (tupleKey) == m_tuples.end ())
    {
      clog << "WARNING: Relation::Delete: Tuple not found " << tuple << endl;
      // NS_ASSERT_MSG (false, "Relation::Delete: Tuple not found " << tuple);
    }
  else
    {
      m_tuples.erase (tupleKey);
     // OnDelete.Invoke (CopyObject<Tuple> (tuple));
      OnDelete.Invoke (tuple);
    }
}

list<Ptr<Tuple> >
Relation::GetAllTuples ()
{
  map<Ptr<Tuple>, Ptr<Tuple> >::iterator it;
  list<Ptr<Tuple> > retval;

  for (it = m_tuples.begin (); it != m_tuples.end (); it++)
    {
      retval.push_back (it->second);
    }
  return retval;
}

void
Relation::ClearAllTuples ()
{
  m_tuples.clear ();
}

uint32_t
Relation::Count ()
{
  return m_tuples.size ();
}

Ptr<Relation>
Relation::New (string name)
{
  Ptr<Relation> relation = CreateObject<Relation> ();
  relation->m_name = name;
  return relation;
}

Ptr<Tuple>
Relation::GetKey (Ptr<Tuple> tuple)
{
  //Check for at least one key to be defined.
  NS_ASSERT_MSG (m_keyAttributes.size () != 0, "No keys defined for relation '" << m_name << "'.");

  Ptr<Tuple> key = Tuple::New (m_name + "-key");

  // Check if the tuple name matches
  NS_ASSERT_MSG (tuple->GetName () == m_name, "Tuple name '" << tuple->GetName () <<
    "' does not match with relation name '" << m_name << "'.");

  map<string, ValueTypeId>::iterator it;
  Ptr<TupleAttribute> attr;
  ValueTypeId type;

  for (it = m_keyAttributes.begin (); it != m_keyAttributes.end (); ++it)
    {
      //Check for attribute
      NS_ASSERT_MSG (tuple->HasAttribute (it->first),
        "Tuple does not have key attribute '" << it->first << "'");

      attr = tuple->GetAttribute (it->first);
      type = attr->GetType ();

      //Check for attribute type
      NS_ASSERT_MSG (m_relaxed || type == it->second || type != ANYTYPE, "Expected type '" <<
        GetTypeName (it->second) << "' for key attribute '" <<
        it->first << "' but found '" << GetTypeName (type) << "'.");

      key->AddAttribute (attr);
    }
  return key;
}

void
Relation::SetRelaxed (bool value)
{
  m_relaxed = value;
}

BOOST_CLASS_EXPORT_IMPLEMENT(ns3::rapidnet::Relation)
