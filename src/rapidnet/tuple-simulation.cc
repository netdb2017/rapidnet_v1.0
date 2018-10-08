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
#include "tuple.h"
#include <sstream>
#include "selector.h"
#include "assignor.h"

using namespace ns3;
using namespace ns3::rapidnet;

TypeId
Tuple::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::Tuple")
    .SetParent<Object> ()
    .AddConstructor<Tuple>();
    ;
  return tid;
}

Tuple::Tuple (string name)
{
  SetName (name);
  m_refCount = 1;
}

Tuple::Tuple (Tuple& tuple)
{
  SetName (tuple.GetName ());

  for (map<string, Ptr<TupleAttribute> >::const_iterator it =
    tuple.m_attributes.begin (); it != tuple.m_attributes.end (); it++)
    {
      m_attributes[it->first] = CopyObject<TupleAttribute> (it->second);
    }
  m_timestamp = tuple.m_timestamp;
  m_refCount = tuple.m_refCount;
}


Tuple::~Tuple ()
{}

void
Tuple::SetName (string name)
{
  m_name = DynamicCast<StrValue, Value> (StrValue::New (name));
}

string
Tuple::GetName (void) const
{
  return m_name->GetStrValue();
}


bool
Tuple::HasAttribute (string name)
{
  return m_attributes.find (name) != m_attributes.end ();
}

void
Tuple::AddAttribute (Ptr<TupleAttribute> attr)
{
  NS_ASSERT_MSG (!HasAttribute (attr->GetName ()),
    "Attribute with name '" << attr->GetName () <<
    "' already exists in '" << GetName () << "'.");
  m_attributes[attr->GetName()] = attr;
}

void
Tuple::OverwriteAttribute (Ptr<TupleAttribute> attr)
{
  RemoveAttribute (attr->GetName ());
  AddAttribute (attr);
}

void
Tuple::AddAttributes (list<Ptr<TupleAttribute> > attributes)
{
  list<Ptr<TupleAttribute> >::iterator it;
  for (it = attributes.begin (); it != attributes.end (); ++it)
    {
      AddAttribute (*it);
    }
}

bool
Tuple::RemoveAttribute (string name)
{
  return m_attributes.erase (name) == 1;
}

Ptr<TupleAttribute>
Tuple::GetAttribute (string name)
{
  NS_ASSERT_MSG (HasAttribute (name),
    "No attribute found with name '" << name <<
    "' in '" << GetName () << "'.");
  return m_attributes[name];
}

map<string, Ptr<TupleAttribute> >&
Tuple::GetAllAttributes ()
{
  return m_attributes;
}

uint32_t
Tuple::GetSerializedSize (void) const
{
  map<string, Ptr<TupleAttribute> >::const_iterator it;

  //Size of the name
  uint32_t retval = m_name->GetSerializedSize ();

  //Create a dummy INT32 attribute for the number of attributes
  Ptr<Value> numAttr = Int32Value::New ();
  retval += numAttr->GetSerializedSize ();

  for(it = m_attributes.begin (); it != m_attributes.end (); it++)
    {
      retval += it->second->GetSerializedSize ();
    }
  return retval;
}

void
Tuple::Serialize (Buffer::Iterator& start) const
{
  map<string, Ptr<TupleAttribute> >::const_iterator it;

  //Serialize the name
  m_name->Serialize (start);

  //Serialize the number of attributes as an INT32 attribute
  Ptr<Value> numAttrs = Int32Value::New (m_attributes.size ());
  numAttrs->Serialize (start);

  //Serialize the attributes
  for(it = m_attributes.begin (); it != m_attributes.end (); it++)
    {
      it->second->Serialize (start);
    }
}

uint32_t
Tuple::Deserialize (Buffer::Iterator& end)
{
  //Deserialize the name
  end.ReadNtohU16 ();
  uint32_t retval = m_name->Deserialize (end);

  //Deserialize the number of attributes
  Ptr<Value> numAttrs = Int32Value::New ();
  end.ReadNtohU16 ();
  retval += numAttrs->Deserialize (end);
  int32_t count = DynamicCast<Int32Value, Value> (numAttrs)->GetInt32Value ();

  Ptr<TupleAttribute> attr;
  for(int i = 0; i < count; ++i)
    {
      attr = TupleAttribute::New ();
      retval += attr->Deserialize (end);
      AddAttribute (attr);
    }
  return retval;
}

string
Tuple::ToString () const
{
  stringstream ss;
  map<string, Ptr<TupleAttribute> >::const_iterator it;

  ss << m_name->GetStrValue () << '[';

  for(it = m_attributes.begin (); it != m_attributes.end (); it++)
    {
      ss << it->second << " ";
    }
  ss << ']';

  return ss.str ();
}


void
Tuple::AddAllAttributes (Ptr<Tuple> tuple, bool qualified)
{
  map<string, Ptr<TupleAttribute> > attrMap = tuple->GetAllAttributes ();
  map<string, Ptr<TupleAttribute> >::iterator it;

  for (it = attrMap.begin (); it != attrMap.end (); it ++)
    {
      AddAttribute (qualified ?
        TupleAttribute::New (QUAL (tuple->GetName (),
          it->second->GetName ()), it->second) :
        CopyObject<TupleAttribute> (it->second));
    }
}

bool
Tuple::Matches (Ptr<Tuple> tuple)
{
  map<string, Ptr<TupleAttribute> > attrMap = tuple->GetAllAttributes ();
  map<string, Ptr<TupleAttribute> >::iterator it;

  for (it = attrMap.begin (); it != attrMap.end (); it ++)
    {
      if (!GetAttribute (it->first)->Equals (it->second))
        {
          return false;
        }
    }
  return true;
}

Ptr<Tuple>
Tuple::Project (string newTupleName, list<string> attrNames,
  list<string> newNames)
{
  // Return with no change if empty
  if (IsEmpty ())
    {
      return Ptr<Tuple> (this);
    }

  Ptr<Tuple> retval = Tuple::New (newTupleName);
  list<string>::iterator it, jt;
  bool rename = newNames.size () != 0;

  for (it = attrNames.begin (), jt = newNames.begin ();
    it != attrNames.end (); ++it)
    {
      NS_ASSERT_MSG (m_attributes.find(*it) != m_attributes.end(),
          "P2Tuple::Project: Attribute with name '" << *it << "' not found in tuple '" <<
          GetName() << "'.");

      if (rename)
        {
           retval->AddAttribute (TupleAttribute::New (*jt, m_attributes[*it]));
           jt++;
        }
      else
        {
           retval->AddAttribute (CopyObject<TupleAttribute> (m_attributes[*it]));
        }
    }
  return retval;
}

Ptr<Tuple>
Tuple::Select (Ptr<Selector> selector)
{
  // If selector returns true then return pointer to self
  if (selector->Select (this))
    {
      return Ptr<Tuple> (this);
    }
  // else return pointer to a dummy empty tuple
  else
    {
      return Tuple::NewEmpty ();
    }
}

void
Tuple::Assign (Ptr<Assignor> assignor)
{
  assignor->Assign (Ptr<Tuple> (this));
}

bool
Tuple::Equals (Ptr<Tuple> attrList)
{
  if (!m_name->Equals (attrList->m_name))
    {
      return false;
    }

  if(m_attributes.size () != attrList->m_attributes.size ())
    {
      return false;
    }

  map<string, Ptr<TupleAttribute> >::iterator it;
  map<string, Ptr<TupleAttribute> >::iterator found;
  for (it = m_attributes.begin (); it != m_attributes.end (); ++it)
    {
      found = attrList->GetAllAttributes ().find (it->first);
      if (!(it->second)->Equals (found->second))
        {
          return false;
        }
    }
  return true;
}

bool
Tuple::Less (Ptr<Tuple> l1, Ptr<Tuple> l2)
{
  int32_t comp = l1->GetName ().compare (l2->GetName ());
  if (comp < 0)
    {
      return true;
    }
  else if (comp > 0)
    {
      return false;
    }

  comp = l1->m_attributes.size () - l2->m_attributes.size ();
  if (comp < 0)
    {
      return true;
    }
  else if (comp > 0)
    {
      return false;
    }

  map<string, Ptr<TupleAttribute> >::iterator it;
  Ptr<TupleAttribute> a1, a2;
  for (it = l1->m_attributes.begin (); it != l1->m_attributes.end (); ++it)
    {
      a1 = it->second;
      a2 = l2->GetAttribute (it->first);
      if (TupleAttribute::Less (a1, a2))
        {
          return true;
        }
      else if (TupleAttribute::Less (a2, a1))
        {
          return false;
        }
    }
    return false;
}

ostream& ns3::rapidnet::operator << (ostream& os, const Ptr<Tuple>& tuple)
{
  os << tuple->ToString ();
  return os;
}

Ptr<Tuple>
Tuple::New (string name)
{
  Ptr<Tuple> tuple = CreateObject<Tuple> ();
  tuple->SetName (name);
  return tuple;
}

Ptr<Tuple>
Tuple::New (string name, list<Ptr<TupleAttribute> > attributes)
{
  Ptr<Tuple> tuple = New (name);
  tuple->AddAttributes (attributes);
  return tuple;
}
