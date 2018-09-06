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
#include "type-ids.h"
#include "rapidnet-types.h"

namespace ns3 {
namespace rapidnet {

string
GetTypeName (ValueTypeId type)
{
  switch (type)
  {
    case NIL: return "nil";
    case ANYTYPE: return "anytype";
    case INT32: return "int32";
    case REAL: return "real";
    case STR: return "str";
    case IPV4: return "ipv4";
    case BOOL: return "bool";
    case LIST: return "list";
    case ID:  return "id";
    case SV: return "sv";
    case BYTE_ARRAY: return "byte-array";
    default: return "unknown";
  }
}

Ptr<Value>
NewValueForType (ValueTypeId type)
{
  switch (type)
  {
  case NIL: return NilValue::New ();
  case INT32: return Int32Value::New ();
  case REAL: return RealValue::New ();
  case STR: return StrValue::New ();
  case IPV4: return Ipv4Value::New ();
  case BOOL: return BoolValue::New ();
  case LIST: return ListValue::New ();
  case ID: return IdValue::New ();
  case SV: return SvValue::New();
  case BYTE_ARRAY: return ByteArrayValue::New();
  default:
    NS_ASSERT_MSG (false, "Unknown type-id " << type);
    return Ptr<Value> (NULL);
  }
}

} // namespace rapidnet
} // namespace ns3
