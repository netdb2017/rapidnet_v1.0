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
#include "rapidnet-utils.h"
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <openssl/sha.h>
#include "ns3/buffer.h"
#include "relation-base.h"

namespace ns3 {
namespace rapidnet {

bool HasTimedout (Time timestamp, Time ttl, Time now)
{
  if (ttl.GetSeconds () == TIME_INFINITY)
    {
      return false;
    }
  Time diff = now - timestamp;
  return diff > ttl;
}

string
GetInsertStatusAsString (int statusCode)
{
  switch (statusCode)
  {
    case RelationBase::INSERTED: return "Inserted";
    case RelationBase::REFRESHED: return "Refreshed";
    case RelationBase::UPDATED: return "Updated";
    default: return "Default inserted";
  }
}

Ptr<Tuple>
tuple (string name)
{
  return Tuple::New (name);
}

Ptr<Tuple>
tuple (string name, list<Ptr<TupleAttribute> > attributes)
{
  return Tuple::New (name, attributes);
}

Ptr<Tuple> tuple (string name, Ptr<TupleAttribute> attr1)
{
  return tuple (name, attrlist (
    attr1));
}

Ptr<Tuple>
tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2)
{
  return tuple (name, attrlist (
    attr1, attr2));
}

Ptr<Tuple>
tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3)
{
  return tuple (name, attrlist (
    attr1, attr2, attr3));
}

Ptr<Tuple>
tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3,
  Ptr<TupleAttribute> attr4)
{
  return tuple (name, attrlist (
    attr1, attr2, attr3, attr4));
}

Ptr<Tuple>
tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3,
  Ptr<TupleAttribute> attr4, Ptr<TupleAttribute> attr5)
{
  return tuple (name, attrlist (
    attr1, attr2, attr3, attr4, attr5));
}

Ptr<Tuple>
tuple (string name, Ptr<TupleAttribute> attr1,
  Ptr<TupleAttribute> attr2, Ptr<TupleAttribute> attr3,
  Ptr<TupleAttribute> attr4, Ptr<TupleAttribute> attr5,
  Ptr<TupleAttribute> attr6)
{
  return tuple (name, attrlist (
    attr1, attr2, attr3, attr4, attr5, attr6));
}

string
GetSHA1Digest (string message)
{
  unsigned char obuf[SHA_DIGEST_LENGTH];
  SHA1(reinterpret_cast<const unsigned char*>(message.c_str ()),
    message.length (), obuf);

  char temp[3];
  stringstream digest;

  for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(temp, "%02x", obuf[i]);
        digest << temp;
    }
  return digest.str ();
}

Ptr<Tuple>
RemoveAllRapidNetAttributes (Ptr<Tuple> tuple)
{
  Ptr<Tuple> retval = Tuple::New (tuple->GetName ());
  map<string, Ptr<TupleAttribute> > attrMap = tuple->GetAllAttributes ();
  map<string, Ptr<TupleAttribute> >::iterator it;

  for (it = attrMap.begin (); it != attrMap.end (); ++it)
    {
      if ((it->first).find ("rn-") == 0)
        {
          //clog << "Removing: " << it->second << endl;
          retval->AddAttribute (it->second);
          tuple->RemoveAttribute (it->first);
        }
    }

  return retval;
}

Ptr<ByteArrayValue>
SerializeTupleToByteArray (Ptr<Tuple> tuple)
{
  uint32_t numBytes = tuple->GetSerializedSize ();

  Buffer buffer;
  buffer.AddAtStart (numBytes);

  Buffer::Iterator it = buffer.Begin ();
  tuple->Serialize (it);

  uint8_t *arr = new uint8_t[numBytes];
  buffer.Begin ().Read (arr, numBytes);

  return DynamicCast<ByteArrayValue, Value> (ByteArrayValue::New(
    arr, numBytes));
}

Ptr<Tuple>
DeserializeTupleFromByteArray (Ptr<ByteArrayValue> byteArray)
{
  Ptr<Tuple> tuple = Tuple::New ();
  uint32_t numBytes = byteArray->GetByteArrayLen ();

  Buffer buffer;
  buffer.AddAtEnd (numBytes);
  buffer.Begin ().Write (byteArray->GetByteArrayPtr (), numBytes);

  Buffer::Iterator it = buffer.Begin ();
  tuple->Deserialize (it);

  return tuple;
}

} // namespace rapidnet
} // namespace ns3
