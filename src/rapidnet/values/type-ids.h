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

#ifndef TYPEID_H
#define TYPEID_H

#include <string>
#include "ns3/ptr.h"
#include "ns3/assert.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

class Value;

/**
 * \ingroup rapidnet_values
 *
 * \brief An enumeration of all the value types defined in RapidNet.
 */
enum ValueTypeId
{
  NIL,
  ANYTYPE,
  INT32,
  REAL,
  STR,
  IPV4,
  BOOL,
  LIST,
  ID,
  SV,
  BYTE_ARRAY,
};

/**
 * \ingroup rapidnet_values
 *
 * \brief Returns the string name of the value type of the given ValueTypeId
 *        enumeration.
 */
string
GetTypeName (ValueTypeId type);

/**
 * \ingroup rapidnet_values
 *
 * \brief Creates and returns a new object for the given ValueTypeId enumeration
 *        with the default value as specified by the value type class.
 */
Ptr<Value>
NewValueForType (ValueTypeId type);

} // namespace rapidnet
} // namespace ns3

#endif // TYPEID_H
