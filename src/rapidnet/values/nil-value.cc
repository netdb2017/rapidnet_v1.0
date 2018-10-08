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

#include "nil-value.h"

using namespace ns3;
using namespace ns3::rapidnet;


NilValue::NilValue ()
  : Value (NIL)
{
}

NilValue::~NilValue ()
{
}

uint32_t
NilValue::GetSerializedSize (void) const
{
  return Value::GetSerializedSize ();
}

void
NilValue::Serialize (Buffer::Iterator& start) const
{
  Value::Serialize (start);
}

uint32_t
NilValue::Deserialize (Buffer::Iterator& end)
{
  return GetSerializedSize ();
}

string
NilValue::ToString () const
{
  return "nil";
}

Ptr<Value>
NilValue::Clone () const
{
  return NilValue::New ();
}

bool
NilValue::Equals (const Ptr<Value> v) const
{
  return GetType () == v->GetType ();
}

bool
NilValue::Less (const Ptr<Value> v) const
{
  return GetType () < v->GetType ();
}

Ptr<Value>
NilValue::Eval (Operator op, Ptr<Tuple> tuple, Ptr<Expression> expr)
{
  AssertUnhandledOperator (op);
  return Ptr<Value> (NULL);
}

Ptr<Value>
NilValue::New ()
{
  return Create<NilValue> ();
}


