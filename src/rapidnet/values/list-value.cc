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

#include "list-value.h"
#include <sstream>
#include "ns3/tuple.h"

using namespace ns3;
using namespace rapidnet;

ListValue::ListValue (list<Ptr<Value> > value)
  :Value (LIST), m_value (value)
{
}

ListValue::~ListValue ()
{
}


uint32_t
ListValue::GetSerializedSize (void) const
{
  // Add 4 for the size of the list
  uint32_t retval = Value::GetSerializedSize () + 4;

  for (rn_list_iterator it = Begin (); it != End (); ++it)
    {
      retval += (*it)->GetSerializedSize ();
    }
  return retval;
}

void
ListValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
  start.WriteHtonU32 (m_value.size ());
  for (rn_list_iterator it = Begin (); it != End (); ++it)
    {
      (*it)->Serialize (start);
    }
}

uint32_t
ListValue::Deserialize (Buffer::Iterator& end)
{
  int32_t size = end.ReadNtohU32 ();
  for (int32_t i = 0; i < size; ++i)
    {
      // Read and discard the type id
      ValueTypeId type = (ValueTypeId) end.ReadNtohU16 ();
      // Create the object using the factory
      Ptr<Value> item = Value::GetInstanceOfType (type);
      // Now it is safe to Deserialize
      item->Deserialize (end);

      m_value.push_back (item);
    }
  return GetSerializedSize ();
}

string
ListValue::ToString () const
{
  stringstream ss;
  ss << "(";
  for (rn_list_iterator it = Begin (); it != End (); ++it)
    {
      ss << (*it)->GetTypeName () << ":" << *it << ",";
    }

  string result = ss.str ();
  if (result.size () > 0)
    {
      result.replace (result.end () - 1, result.end (), ")");
    }
  return result;
}

Ptr<Value>
ListValue::Clone () const
{
  list<Ptr<Value> > clone;
  for (rn_list_iterator it = Begin (); it != End (); ++it)
    {
      clone.push_back ((*it)->Clone ());
    }
  return ListValue::New (clone);
}

bool
ListValue::Equals (const Ptr<Value> v) const
{
  if(GetType () != v->GetType ())
    {
      return false;
    }

  Ptr<ListValue> other = DynamicCast<ListValue, Value> (v);
  if (Size () != other->Size ())
    {
      return false;
    }

  for (rn_list_iterator it = Begin (), jt = other->Begin ();
    it != End () && jt != other->End (); ++it, jt++)
    {
      if ((*it)->Equals (*jt))
        {
          continue;
        }
      else
        {
          return false;
        }
    }
  return true;
}

bool
ListValue::Less (const Ptr<Value> v) const
{
  if (GetType () < v->GetType ())
    {
      return true;
    }
  else if (GetType () > v->GetType ())
    {
      return false;
    }

  Ptr<ListValue> other = DynamicCast<ListValue, Value> (v);

  // Compare as though a string
  rn_list_iterator it = Begin (), jt = other->Begin ();
  for (;it != End () && jt != other->End (); ++it, jt++)
    {
      if ((*it)->Equals (*jt))
        {
          continue;
        }
      else
        {
          return (*it)->Less (*jt);
        }
    }

  if (it == End () && jt == other->End ())
    {
      return false;
    }
  else if (it == End ())
    {
      return true;
    }
  else
    {
      return false;
    }
}

Ptr<Value>
ListValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  Ptr<Value> retval;
  Ptr<Value> operand = expr->Eval (tuple);

  switch (op)
  {
    case RN_PLUS:
      {
        list<Ptr<Value> > newlist;
        for (rn_list_iterator it = Begin (); it != End (); ++it)
          {
            newlist.push_back ((*it)->Clone ());
          }
        newlist.push_back (operand);
        retval = ListValue::New (newlist);
        break;
      }
    default:
      {
        AssertUnhandledOperator (op);
        retval = Ptr<Value> (NULL);
      }
  }
  return retval;
}

bool
ListValue::Contains (Ptr<Value> value) const
{
  for (rn_list_iterator it = Begin (); it != End (); ++it)
    {
      if ((*it)->Equals (value))
        {
          return true;
        }
    }
  return false;
}

Ptr<Value>
ListValue::New (list<Ptr<Value> > value)
{
  Ptr<ListValue> listValue = Create<ListValue> ();
  listValue->m_value = value;
  return listValue;
}
