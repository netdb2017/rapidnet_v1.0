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

#include "value.h"
#include "type-ids.h"

using namespace ns3;
using namespace ns3::rapidnet;

NS_LOG_COMPONENT_DEFINE ("Value");

const uint32_t Value::SIZE_TYPEID = 2;

Value::Value (ValueTypeId type)
  : m_type (type)
{
}

Value::~Value ()
{
}

uint32_t
Value::GetSerializedSize (void) const
{
  // For the type-id
  return SIZE_TYPEID;
}

void
Value::Serialize (Buffer::Iterator& start) const
{
  // Serialize the type
  start.WriteHtonU16 ((uint16_t) m_type);
}

bool
Value::Less (const Ptr<Value> v1, const Ptr<Value> v2)
{
  return v1->Less (v2);
}

Ptr<Value>
Value::GetInstanceOfType (ValueTypeId type)
{
  return NewValueForType (type);
}

void
Value::AssertUnknownExpression (Operator op, Ptr<Value> operand)
{
  NS_ASSERT_MSG (false, "Unknown expression " << GetTypeName () << " '" <<
    OpString (op) << "' " << operand->GetTypeName ());
}

void
Value::AssertUnhandledOperator (Operator op)
{
  NS_ASSERT_MSG (false, "Unhandled operator for '" << OpString (op) <<
    "' for type " << GetTypeName ());
}

ostream&
ns3::rapidnet::operator << (ostream& os, const Ptr<Value>& value)
{
  os << value->ToString ();
  return os;
}
