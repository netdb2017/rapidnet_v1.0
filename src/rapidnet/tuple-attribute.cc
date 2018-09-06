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

#include "tuple-attribute.h"
#include <sstream>

using namespace ns3;
using namespace ns3::rapidnet;


TupleAttribute::TupleAttribute (string name, Ptr<Value> value)
  : m_value (value)
{
  SetName (name);
}

TupleAttribute::TupleAttribute (TupleAttribute &attr)
{
  SetName (attr.GetName ());
  m_value = attr.GetValue ()->Clone ();
}

TupleAttribute::~TupleAttribute ()
{
}

TypeId
TupleAttribute::GetTypeId(void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::TupleAttribute")
    .SetParent<Object> ()
    .AddConstructor<TupleAttribute>();
    ;

  return tid;
}

uint32_t
TupleAttribute::GetSerializedSize (void) const
{
  return m_name->GetSerializedSize () + m_value->GetSerializedSize ();
}

void
TupleAttribute::Serialize (Buffer::Iterator& start) const
{
  m_name->Serialize (start);
  m_value->Serialize (start);
}

uint32_t
TupleAttribute::Deserialize (Buffer::Iterator& end)
{
  SetName ("dummy"); // Initialize the name object first
  end.ReadNtohU16 (); // Do a dummy type-id read to discard it
  m_name->Deserialize (end); // Now it is safe to De-serialize

  // Read and discard the type id
  ValueTypeId type = (ValueTypeId) end.ReadNtohU16 ();
  // Create the object using the factory
  m_value = Value::GetInstanceOfType (type);
  // Now it is safe to De-serialize
  m_value->Deserialize (end);

  return GetSerializedSize ();
}

string
TupleAttribute::ToString ()
{
  stringstream ss;
  ss << m_name->GetStrValue () << "(" << GetTypeName(m_value->GetType()) <<
    ":" << m_value->ToString () << ")";
  return ss.str ();
}

bool
TupleAttribute::ValueEquals (const Ptr<TupleAttribute> attr) const
{
  return m_value->Equals (attr->GetValue ());
}

bool
TupleAttribute::Equals (const Ptr<TupleAttribute> attr) const
{
  return GetName () == attr->GetName () && m_value->Equals (attr->GetValue ());
}

bool
TupleAttribute::Less (const Ptr<TupleAttribute> a1,
  const Ptr<TupleAttribute> a2)
{
  int32_t nameComparison = a1->GetName ().compare (a2->GetName ());

  if (nameComparison == 0)
    {
      return a1->GetValue ()->Less (a2->GetValue ());
    }
  else
    {
      return nameComparison < 0;
    }
}

Ptr<TupleAttribute>
TupleAttribute::New (string name, Ptr<Value> value)
{
  Ptr<TupleAttribute> retval = Create<TupleAttribute> ();
  retval->SetName (name);
  retval->SetValue (value);
  return retval;
}

Ptr<TupleAttribute>
TupleAttribute::New (string name, Ptr<TupleAttribute> attr)
{
  return New (name, attr->GetValue ());
}

ostream& ns3::rapidnet::operator << (ostream& os, const Ptr<TupleAttribute>&
  attr)
{
  os << attr->ToString ();
  return os;
}
