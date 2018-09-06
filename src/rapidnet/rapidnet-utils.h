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
#ifndef RAPIDNET_UTILS_H
#define RAPIDNET_UTILS_H

#include <stdio.h>
#include <string>
#include <list>
#include <time.h>
#include "ns3/nstime.h"
#include "tuple.h"
#include "ns3/byte-array-value.h"

#define strlist MakeList<string>
#define intlist MakeList<int>
#define attrdeftype pair<string, ValueTypeId>
#define attrdef(name, type) attrdeftype (name, type)
#define attrdeflist MakeList<attrdeftype>

#define attr(name, type, value) \
  TupleAttribute::New (name, type::New (value))
#define attrlist MakeList<Ptr<TupleAttribute> >
#define TIME_INFINITY -1

#define LOCAL_ADDRESS ValueExpr::New (Ipv4Value::New (GetAddress ()))
#define BROADCAST_ADDRESS ValueExpr::New (Ipv4Value::New ("255.255.255.255"))

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \brief Returns true if the tuple with given time-stamp and TTL
 *        has timed out.
 *
 * @param timestamp Timestamp on the tuple
 * @param ttl The TTL defined for the corresponding relation.
 * @param now The current time.
 */
bool HasTimedout (Time timestamp, Time ttl, Time now);

/**
 * \brief A string representation for the given insert status code.
 */
string GetInsertStatusAsString (int statusCode);

/**
 * \brief Creates a new tuple with given name and no attributes.
 */
Ptr<Tuple> tuple (string name);

/**
 * \brief Creates a new tuple with given name and the attributes
 *        from the given list added to it.
 */
Ptr<Tuple> tuple (string name, list<Ptr<TupleAttribute> > attributes);

/**
 * \brief Creates a new tuple with given name and adds the given
 *        attribute(s) to it.
 */
Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1);

/**
 * \brief Creates a new tuple with given name and adds the given
 *        attribute(s) to it.
 */
Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2);

/**
 * \brief Creates a new tuple with given name and adds the given
 *        attribute(s) to it.
 */
Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3);

/**
 * \brief Creates a new tuple with given name and adds the given
 *        attribute(s) to it.
 */
Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3,
  Ptr<TupleAttribute> attr4);

/**
 * \brief Creates a new tuple with given name and adds the given
 *        attribute(s) to it.
 */
Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3,
  Ptr<TupleAttribute> attr4, Ptr<TupleAttribute> attr5);

/**
 * \brief Creates a new tuple with given name and adds the given
 *        attribute(s) to it.
 */
Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3,
  Ptr<TupleAttribute> attr4, Ptr<TupleAttribute> attr5,
  Ptr<TupleAttribute> attr6);

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1)
{
  list<T> retval;
  retval.push_back (t1);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4, T t5)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  retval.push_back (t5);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4, T t5, T t6)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  retval.push_back (t5);
  retval.push_back (t6);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4, T t5, T t6, T t7)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  retval.push_back (t5);
  retval.push_back (t6);
  retval.push_back (t7);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  retval.push_back (t5);
  retval.push_back (t6);
  retval.push_back (t7);
  retval.push_back (t8);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  retval.push_back (t5);
  retval.push_back (t6);
  retval.push_back (t7);
  retval.push_back (t8);
  retval.push_back (t9);
  return retval;
}

/**
 * \brief Creates a list with object(s) of given type inserted.
 */
template<class T>
list<T> MakeList (T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10)
{
  list<T> retval;
  retval.push_back (t1);
  retval.push_back (t2);
  retval.push_back (t3);
  retval.push_back (t4);
  retval.push_back (t5);
  retval.push_back (t6);
  retval.push_back (t7);
  retval.push_back (t8);
  retval.push_back (t9);
  retval.push_back (t10);
  return retval;
}

/**
 * \brief A utility C++ wrapper over openssl's SHA1 function.
 */
string GetSHA1Digest (string message);

/**
 * \brief Utility method to strip a tuple of all RapidNet specific
 *        indicator attributes (names that begin with "rn-") and
 *        returns them as a separate tuple. This method is used
 *        before signing or encrypting tuples.
 */
Ptr<Tuple> RemoveAllRapidNetAttributes (Ptr<Tuple> tuple);

/**
 * \brief Utility method to serialize the contents of a tuple
 *        into a byte array using standard serialize primitives
 *        of constituent attributes.
 */
Ptr<ByteArrayValue> SerializeTupleToByteArray (Ptr<Tuple> tuple);

/**
 * \brief Utility method to deserialize a byte array value returned
 *        by @see SerializeTupleToByteArray method back to the
 *        original tuple.
 */
Ptr<Tuple> DeserializeTupleFromByteArray (Ptr<ByteArrayValue> byteArray);


} //namespace rapidnet
} //namepsace ns3

#endif // RAPIDNET_UTILS_H
