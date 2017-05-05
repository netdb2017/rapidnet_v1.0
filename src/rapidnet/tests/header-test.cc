/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Pennsylvania
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

#include <iostream>
#include <string>
#include <map>

#include "ns3/test.h"
#include "ns3/packet.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-header.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {


/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet Relation.
 *
 * @see Relation
 */
class HeaderTest : public Test
{
public:

  HeaderTest () : Test ("Rapidnet-HeaderTest") {}

  virtual ~HeaderTest () {}

  virtual bool RunTests (void);

protected:

};

Ptr<Value>
GetListValue ()
{
  Ptr<Value> i = Int32Value::New (7);
  Ptr<Value> s = StrValue::New ("test123");
  Ptr<Value> r = RealValue::New (3.14);
  Ptr<Value> b = BoolValue::New (true);
  Ptr<Value> ip = Ipv4Value::New ("128.0.1.1");

  Ptr<Value> strVPtr_1 = StrValue::New("hello");
  Ptr<Value> strVPtr_2 = StrValue::New("world");
  Ptr<Value> svPtr_1 = SvValue::New();
  Ptr<Value> svPtr_2 = SvValue::HashInsert(svPtr_1, strVPtr_1);
  Ptr<Value> svPtr_3 = SvValue::HashInsert(svPtr_2, strVPtr_2);

  list<Ptr<Value> > l1;
  l1.push_back (i);
  l1.push_back (s);
  l1.push_back (r);
  l1.push_back (b);
  l1.push_back (ip);
  l1.push_back(svPtr_3);
  return ListValue::New (l1);
}

Ptr<Tuple>
GetTuple ()
{
  Ptr<Tuple> tuple = Tuple::New ("test-tuple");

  tuple->AddAttribute (TupleAttribute::New ("int32", Int32Value::New (78)));;
  tuple->AddAttribute (TupleAttribute::New ("double",
    RealValue::New (6.023)));
  tuple->AddAttribute (TupleAttribute::New ("string",
    StrValue::New ("test string")));
  tuple->AddAttribute (TupleAttribute::New ("ipv4",
    Ipv4Value::New ("101.10.0.1")));
  tuple->AddAttribute (TupleAttribute::New ("bool",
    BoolValue::New (true)));

  Ptr<Value> strVPtr_1 = StrValue::New("hello");
  Ptr<Value> strVPtr_2 = StrValue::New("world");
  Ptr<Value> svPtr_1 = SvValue::New();
  Ptr<Value> svPtr_2 = SvValue::HashInsert(svPtr_1, strVPtr_1);
  Ptr<Value> svPtr_3 = SvValue::HashInsert(svPtr_2, strVPtr_2);
  tuple->AddAttribute (TupleAttribute::New ("sv",
    svPtr_3));
  tuple->AddAttribute (TupleAttribute::New ("list",
    GetListValue ()));
  tuple->AddAttribute (TupleAttribute::New ("id",
    IdValue::New ("1010101010101010101010101010101"
      "1010101010101010101011111111010000100101011"
      "10101110101010101010101010101010101010101010"
      "10001001010111101011010001")));

  uint8_t* buf = new uint8_t[3];
  buf[0] = 'a';
  buf[1] = 'b';
  buf[2] = 'c';
  Ptr<Value> ba = ByteArrayValue::New (buf, 3);
  tuple->AddAttribute (TupleAttribute::New ("byte array", ba));

  return tuple;
}

bool
HeaderTest::RunTests ()
{
  bool result = true;
  RapidNetHeader header;
  header.SetTuple (GetTuple ());

  //cout << "Tuple sent: " << header.GetTuple () << endl;

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (header);

  RapidNetHeader newHeader;
  packet->RemoveHeader (newHeader);
  //cout<< "Tuple received: " << newHeader.GetTuple () << endl;

  NS_TEST_ASSERT (header.GetTuple ()->Equals (newHeader.GetTuple ()));

  //cout << "All rapidnet-header tests passed!" << endl;

  return result;
}

static HeaderTest g_headerTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
