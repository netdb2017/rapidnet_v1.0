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

#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <assert.h>

#include "ns3/ptr.h"
#include "ns3/ref-count-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet_compiler {

class ParsedValue;
typedef Ptr<ParsedValue> ValuePtr;

class ParsedValue : public RefCountBase
{
public:

  ParsedValue () {}

  virtual ~ParsedValue () {}

  enum TypeCode
  {
      NULLV = 0,
      STR,
      INT32,
      UINT32,
      DOUBLE,
      ID,
      IP_ADDR,
      /* NEW TYPE DEFINITIONS GO UNDER HERE */
      LIST,
      BYTE_ARRAY,
      /* NEW TYPE DEFINITIONS GO ABOVE HERE */
      SV,
      TYPES,
      VAR,
  };

  TypeCode GetTypeCode () const
  {
    return m_typeCode;
  }

  string GetTypeName () const
  {
    return m_typeName;
  }

  virtual string ToString () const = 0;

protected:

  ParsedValue (TypeCode typeCode, string typeName);

  TypeCode m_typeCode;

  string m_typeName;
};


class ValNull: public ParsedValue
{
public:

  string ToString () const
  {
    return "NULL";
  }

  static ValuePtr New ();

};


class ValStr: public ParsedValue
{
public:

  string ToString () const
  {
    return s;
  }

  ValStr (string theString) : ParsedValue (ParsedValue::STR, "str"),
    s (theString)
  {
  }

  virtual ~ValStr ()
  {
  }

  static ValuePtr New (string s)
  {
    ValuePtr p (new ValStr (s));
    return p;
  }

private:
  string s;
};


class ValInt32: public ParsedValue
{
public:

  virtual string ToString () const
  {
    ostringstream s;
    s << i;
    return s.str ();
  }

  ValInt32 (int32_t theInt) : ParsedValue (ParsedValue::INT32, "int32"),
    i (theInt)
  {
  }

  static ValuePtr New (int32_t i)
  {
    ValuePtr p (new ValInt32 (i));
    return p;
  }

  static ValuePtr ZERO;

  int32_t GetInt32Value ()
  {
    return i;
  }

private:
  int32_t i;
};


class ValUInt32: public ParsedValue
{
public:

  virtual string ToString () const
  {
    ostringstream s;
    s << i;
    return s.str ();
  }

  ValUInt32 (uint32_t theInt) : ParsedValue (ParsedValue::UINT32, "uint32"),
    i (theInt)
  {
  }

  static ValuePtr New (uint32_t i)
  {
    return ValuePtr (new ValUInt32 (i));
  }

  uint32_t GetUInt32Value ()
  {
    return i;
  }

private:
  uint32_t i;
};


class ValDouble: public ParsedValue
{
public:

  virtual string ToString () const;

  ValDouble (double theFloat) : ParsedValue (ParsedValue::DOUBLE, "double"),
    d (theFloat)
  {
  }

  static ValuePtr New (double d)
  {
    ValuePtr p (new ValDouble (d));
    return p;
  }

  double GetDoubleValue()
  {
    return d;
  }
  
private:
  double d;
};

class ValId: public ParsedValue
{
public:

  static const unsigned WORDS = 5;

  virtual string ToString () const;

  ValId (string);

  virtual ~ValId ()
  {
  }

  static ValuePtr New (string theID)
  {
    ValuePtr p (new ValId (theID));
    return p;
  }

protected:

  uint32_t words[WORDS];
};


class ValVar: public ParsedValue
{
public:

  string ToString () const
  {
    return s;
  }

  ValVar (string str) :
    ParsedValue (ParsedValue::VAR, "var"), s (str)
  {
  }

  virtual ~ValVar ()
  {
  }

  static ValuePtr New (string s)
  {
    ValuePtr p (new ValVar (s));
    return p;
  }

private:
  string s;
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif /* __VALUE_H_ */
