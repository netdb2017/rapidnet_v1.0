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
#include <list>
#include "ns3/ipv4-address.h"
#include "ns3/test.h"
#include "ns3/rapidnet-types.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

/**
 * \ingroup rapidnet_library
 * \defgroup rapidnet_tests Unit Tests
 *
 * \internal Documentation is not generated because this is not a header?
 */

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet value types.
 *
 * @see Value
 */
class ValuesTest : public Test
{
public:

  ValuesTest () : Test ("Rapidnet-ValuesTest") {}

  virtual ~ValuesTest () {}

  virtual bool RunTests (void);

protected:

  bool Int32ValueTest ();
  bool RealValueTest ();
  bool StrValueTest ();
  bool Ipv4ValueTest ();
  bool BoolValueTest ();
  bool NilValueTest ();
  bool SvValueTest ();
  bool ListValueTest ();
  bool IdValueTest ();
  bool ByteArrayValueTest ();
  bool EqualsTest ();
  bool LessTest ();
};

bool
ValuesTest::RunTests ()
{
  bool result = true;
  result = Int32ValueTest ()
    && RealValueTest ()
    && StrValueTest ()
    && NilValueTest ()
    && SvValueTest ()
    && ListValueTest ()
    && IdValueTest ()
    && ByteArrayValueTest ()
    && EqualsTest ()
    && LessTest ();

  return result;
}

bool
ValuesTest::Int32ValueTest ()
{
  bool result = true;
  Ptr<Value> value = Int32Value::New (51);
  NS_TEST_ASSERT (value->Equals (value->Clone ()));
  //cout << value << endl;

  return result;
}

bool
ValuesTest::RealValueTest ()
{
  bool result = true;
  Ptr<Value> value = RealValue::New (3.14);
  NS_TEST_ASSERT (value->Equals (value->Clone ()));
  //cout << value << endl;

  return result;
}

bool
ValuesTest::StrValueTest ()
{
  bool result = true;
  Ptr<Value> value = StrValue::New ("test-string");
  NS_TEST_ASSERT (value->Equals (value->Clone ()));
  //cout << value << endl;

  return result;
}

bool
ValuesTest::Ipv4ValueTest ()
{
  bool result = true;
  Ptr<Value> value = Ipv4Value::New (Ipv4Address ("192.168.0.2"));
  NS_TEST_ASSERT (value->Equals (value->Clone ()));
  //cout << value << endl;

  return result;
}

bool
ValuesTest::BoolValueTest ()
{
  bool result = true;
  Ptr<Value> value = BoolValue::New (true);
  NS_TEST_ASSERT (value->Equals (value->Clone ()));
  //cout << value << endl;

  return result;
}

bool
ValuesTest::NilValueTest ()
{
  bool result = true;
  Ptr<Value> value = NilValue::New ();
  NS_TEST_ASSERT (value->Equals (value->Clone ()));
  //cout << value << endl;

  return result;
}

bool
ValuesTest::SvValueTest ()
{
  bool result = true;
  Ptr<Value> vPtr_1 = SvValue::New ();
  Ptr<Value> vPtr_2 = SvValue::New ();

  NS_TEST_ASSERT (vPtr_1->Equals (vPtr_2));
  //cout << vPtr_1 << endl;
  //cout << vPtr_2 <<endl;

  Ptr<Value> strVPtr_1 = StrValue::New("hello");
  Ptr<Value> strVPtr_2 = StrValue::New("world");
  Ptr<Value> strVPtr_3 = StrValue::New("is");
  Ptr<Value> strVPtr_4 = StrValue::New("perfect.");

  Ptr<Value> vPtr_3 = SvValue::HashInsert(vPtr_1, strVPtr_1);
  Ptr<Value> vPtr_4 = SvValue::HashInsert(vPtr_2, strVPtr_1);
  Ptr<Value> vPtr_5 = SvValue::HashInsert(vPtr_2, strVPtr_2);

  NS_TEST_ASSERT (vPtr_3->Equals (vPtr_4));
  NS_TEST_ASSERT (!vPtr_3->Equals (vPtr_5));
  //cout << vPtr_3 <<endl;
  //cout << vPtr_4 << endl;
  //cout << vPtr_5 << endl;

  Ptr<Value> vPtr_6 = SvValue::HashInsert(vPtr_3, strVPtr_2);
  Ptr<Value> vPtr_7 = SvValue::HashInsert(vPtr_6, strVPtr_3);
  Ptr<Value> vPtr_8 = SvValue::HashInsert(vPtr_7, strVPtr_4);
  //cout << vPtr_6 << endl;
  //cout << vPtr_7 << endl;
  //cout << vPtr_8 << endl;

  NS_TEST_ASSERT(SvValue::HashIn(vPtr_8, strVPtr_4));
  NS_TEST_ASSERT(SvValue::HashIn(vPtr_8, strVPtr_3));
  NS_TEST_ASSERT(SvValue::HashIn(vPtr_8, strVPtr_2));
  NS_TEST_ASSERT(SvValue::HashIn(vPtr_8, strVPtr_1));

  Ptr<Value> vPtr_9 = SvValue::HashRemove(vPtr_8, strVPtr_3);
  Ptr<Value> vPtr_10 = SvValue::HashRemove(vPtr_9, strVPtr_4);
  Ptr<Value> vPtr_11 = SvValue::HashRemove(vPtr_10, strVPtr_2);
  Ptr<Value> vPtr_12 = SvValue::HashRemove(vPtr_11, strVPtr_1);

  NS_TEST_ASSERT(!SvValue::HashIn(vPtr_9, strVPtr_3));
  NS_TEST_ASSERT(!SvValue::HashIn(vPtr_10, strVPtr_4));
  NS_TEST_ASSERT(vPtr_12->Equals(vPtr_1));
  //cout << vPtr_9 << endl;
  //cout << vPtr_10 << endl;
  //cout << vPtr_11 << endl;
  //cout << vPtr_12 << endl;

  Ptr<Value> vPtr_13 = SvValue::AndNot(vPtr_8, vPtr_10);
  //cout << vPtr_8 << endl;
  //cout << vPtr_10 << endl;
  //cout << vPtr_13 << endl;

  uint32_t i = 0;
  bitset<SvValue::k_NUM_BITS> bitset_1;
  for (i = 0; i < SvValue::k_NUM_BITS; i++)
    {
      bitset_1[i] = false;
    }

  bitset<SvValue::k_NUM_BITS> bitset_2;
  for (i = 0; i < SvValue::k_NUM_BITS - 1; i++)
    {
      bitset_2[i] = false;
    }
  bitset_2[i] = true;
  
  Ptr<Value> vPtr_14 = SvValue::New(bitset_1);
  Ptr<Value> vPtr_15 = SvValue::New(bitset_2);
  Ptr<Value> vPtr_16 = SvValue::New(bitset_1);

  NS_TEST_ASSERT(vPtr_14->Equals(vPtr_16));
  NS_TEST_ASSERT(vPtr_14->Less(vPtr_15));
  NS_TEST_ASSERT(!vPtr_15->Less(vPtr_14));  

  bitset_1[SvValue::k_NUM_BITS - 2] = true;
  
  Ptr<Value> vPtr_17 = SvValue::New(bitset_1);

  NS_TEST_ASSERT(!vPtr_17->Less(vPtr_15));
  NS_TEST_ASSERT(vPtr_15->Less(vPtr_17));  
  NS_TEST_ASSERT(!vPtr_17->Equals(vPtr_14));

  //cout << "SvValue tests passed!" << endl;

  return result;
}

bool
ValuesTest::ListValueTest ()
{
  bool result = true;
  Ptr<Value> i = Int32Value::New (7);
  Ptr<Value> s = StrValue::New ("test123");
  Ptr<Value> r = RealValue::New (3.14);
  Ptr<Value> b = BoolValue::New (true);
  Ptr<Value> ip = Ipv4Value::New ("128.0.1.1");

  list<Ptr<Value> > l1;
  l1.push_back (i);
  l1.push_back (s);
  l1.push_back (r);
  l1.push_back (b);
  l1.push_back (ip);
  Ptr<Value> lv1 = ListValue::New (l1);

  //cout << lv1 << endl;
  NS_TEST_ASSERT (lv1->Equals (lv1->Clone ()));
  NS_TEST_ASSERT ((DynamicCast<ListValue, Value> (lv1))->Contains (i));

  list<Ptr<Value> > l2;
  l2.push_back (i);
  l2.push_back (s);
  l2.push_back (r);
  l2.push_back (b);
  Ptr<Value> lv2 = ListValue::New (l2);

  NS_TEST_ASSERT (!lv1->Less (lv2));
  NS_TEST_ASSERT (lv2->Less (lv1));
  NS_TEST_ASSERT (!lv1->Equals (lv2));
  NS_TEST_ASSERT (!lv2->Equals (lv1));
  NS_TEST_ASSERT (!(DynamicCast<ListValue, Value> (lv2))->Contains (ip));

  list<Ptr<Value> > l3;
  l3.push_back (i);
  l3.push_back (s);
  l3.push_back (RealValue::New (3));
  l3.push_back (b);
  Ptr<Value> lv3 = ListValue::New (l3);

  NS_TEST_ASSERT (lv3->Less (lv2));
  NS_TEST_ASSERT (!lv3->Equals (lv2));

  list<Ptr<Value> > l4;
  l4.push_back (lv1);
  l4.push_back (lv2);
  Ptr<Value> lv4 = ListValue::New (l4);
  Ptr<Value> lv5 = ListValue::New (l4);
  //cout << lv4 << endl;

  NS_TEST_ASSERT (lv4->Equals (lv5));
  NS_TEST_ASSERT (!lv4->Less (lv1));

  NS_TEST_ASSERT ((DynamicCast<ListValue, Value> (lv4))->Contains (lv1));

  return result;
}

bool
ValuesTest::IdValueTest ()
{
  bool result = true;
  Ptr<Value> value = IdValue::New ("00000000000001010101010101010"
    "101010101010101010101010101010101010101010101010101010101010"
    "101010101010101010101111111101000010010101110101110101010101");
  NS_TEST_ASSERT (value->Equals (value->Clone ()));

  value = IdValue::New ("aaaaaaaaaaaaaaaaaaaaaaaaff42575d55", 16);
  NS_TEST_ASSERT (value->Equals (value->Clone ()));

  //cout << value << endl;
  return result;
}

bool
ValuesTest::ByteArrayValueTest ()
{
  bool result = true;
  uint8_t * ba = new uint8_t[4];
  ba[0] = 'a';
  ba[1] = 'b';
  ba[2] = 'c';
  ba[3] = 't';
  Ptr<Value> value = ByteArrayValue::New (ba, 4);
  NS_TEST_ASSERT (value->Equals (value->Clone ()));

  return result;
}

bool
ValuesTest::EqualsTest ()
{
  bool result = true;
  Ptr<Value> i1 = Int32Value::New (7);
  Ptr<Value> i2 = Int32Value::New (903);
  Ptr<Value> i3 = Int32Value::New (7);
  NS_TEST_ASSERT (!i1->Equals (i2));
  NS_TEST_ASSERT (i1->Equals (i3));
  NS_TEST_ASSERT (i1->Equals (i1));

  Ptr<Value> d1 = RealValue::New (8093.1934);
  Ptr<Value> d2 = RealValue::New (903);
  Ptr<Value> d3 = RealValue::New (8093.1934);
  NS_TEST_ASSERT (!d1->Equals (d2));
  NS_TEST_ASSERT (d1->Equals (d3));
  NS_TEST_ASSERT (d1->Equals (d1));

  Ptr<Value> s1 = StrValue::New ("hello, world!");
  Ptr<Value> s2 = StrValue::New ("hi");
  Ptr<Value> s3 = StrValue::New ("hello, world!");
  NS_TEST_ASSERT (!s1->Equals (s2));
  NS_TEST_ASSERT (s1->Equals (s3));
  NS_TEST_ASSERT (s1->Equals (s1));

  Ptr<Value> ip1 = Ipv4Value::New (Ipv4Address ("192.168.0.255"));
  Ptr<Value> ip2 = Ipv4Value::New (Ipv4Address ("127.0.0.255"));
  Ptr<Value> ip3 = Ipv4Value::New (Ipv4Address ("192.168.0.255"));
  NS_TEST_ASSERT (!ip1->Equals (ip2));
  NS_TEST_ASSERT (ip1->Equals (ip3));
  NS_TEST_ASSERT (ip1->Equals (ip1));

  Ptr<Value> b1 = BoolValue::New (true);
  Ptr<Value> b2 = BoolValue::New (false);
  Ptr<Value> b3 = BoolValue::New (true);
  NS_TEST_ASSERT (!b1->Equals (b2));
  NS_TEST_ASSERT (b1->Equals (b3));
  NS_TEST_ASSERT (b1->Equals (b1));


  Ptr<Value> id1 = IdValue::New ("0101010101010101010111111101010101010101001010101");
  Ptr<Value> id2 = IdValue::New ("0101010101011010101111011");
  Ptr<Value> id3 = IdValue::New ("0101010101010101010111111101010101010101001010101");
  NS_TEST_ASSERT (!id1->Equals (id2));
  NS_TEST_ASSERT (id1->Equals (id3));
  NS_TEST_ASSERT (id1->Equals (id1));


  uint8_t* c1 = new uint8_t[3];
  c1[0] = 'a';
  c1[1] = 'b';
  c1[2] = 'c';
  Ptr<Value> ba1 = ByteArrayValue::New (c1, 3);

  uint8_t* c2 = new uint8_t[4];
  c2[0] = 'a';
  c2[1] = 'b';
  c2[2] = 'c';
  c2[3] = 'd';
  Ptr<Value> ba2 = ByteArrayValue::New (c2, 4);

  uint8_t* c3 = new uint8_t[3];
  c3[0] = 'a';
  c3[1] = 'b';
  c3[2] = 'c';
  Ptr<Value> ba3 = ByteArrayValue::New (c3, 3);

  NS_TEST_ASSERT (!ba1->Equals (ba2));
  NS_TEST_ASSERT (ba1->Equals (ba3));
  NS_TEST_ASSERT (ba1->Equals (ba1));

  map<Ptr<Value>, string, bool (*) (Ptr<Value>, Ptr<Value>)> testmap (
    &Value::Less);

  testmap [i1] = "i1";
  testmap [i2] = "i2";
  testmap [i3] = "i3";

  testmap [d1] = "d1";
  testmap [d2] = "d2";
  testmap [d3] = "d3";

  testmap [s1] = "s1";
  testmap [s2] = "s2";
  testmap [s3] = "s3";

  testmap [ip1] = "ip1";
  testmap [ip2] = "ip2";
  testmap [ip3] = "ip3";

  testmap [b1] = "b1";
  testmap [b2] = "b2";
  testmap [b3] = "b3";

  testmap [id1] = "id1";
  testmap [id2] = "id2";
  testmap [id3] = "id3";

//  cout << "id1: " << testmap[id1] << endl;
//  cout << "id2: " << testmap[id2] << endl;
//  cout << "id3: " << testmap[id3] << endl;


  NS_TEST_ASSERT (testmap[i1] == "i3");
  NS_TEST_ASSERT (testmap[d1] == "d3");
  NS_TEST_ASSERT (testmap[s1] == "s3");
  NS_TEST_ASSERT (testmap[ip1] == "ip3");
  NS_TEST_ASSERT (testmap[b1] == "b3");
  NS_TEST_ASSERT (testmap[id1] == "id3");
//  NS_TEST_ASSERT (testmap[ba1] == "ba3");

  NS_TEST_ASSERT (testmap[i2] == "i2");
  NS_TEST_ASSERT (testmap[d2] == "d2");
  NS_TEST_ASSERT (testmap[s2] == "s2");
  NS_TEST_ASSERT (testmap[ip2] == "ip2");
  NS_TEST_ASSERT (testmap[b2] == "b2");
  NS_TEST_ASSERT (testmap[id2] == "id2");
//  NS_TEST_ASSERT (testmap[ba2] == "ba2");

  NS_TEST_ASSERT (testmap.size () == 12);

//  cout << "Equals test passed!" << endl;

  return result;
}

bool
ValuesTest::LessTest ()
{
  bool result = true;
  Ptr<Value> i1 = Int32Value::New (7);
  Ptr<Value> i2 = Int32Value::New (903);
  Ptr<Value> i3 = Int32Value::New (7);
  NS_TEST_ASSERT (i1->Less (i2));
  NS_TEST_ASSERT (!i2->Less (i3));
  NS_TEST_ASSERT (!i1->Less (i1));
  NS_TEST_ASSERT (!i1->Less (i3));

  Ptr<Value> d1 = RealValue::New (8093.1934);
  Ptr<Value> d2 = RealValue::New (903);
  Ptr<Value> d3 = RealValue::New (8093.1934);
  NS_TEST_ASSERT (!d1->Less (d2));
  NS_TEST_ASSERT (d2->Less (d3));
  NS_TEST_ASSERT (!d1->Less (d1));
  NS_TEST_ASSERT (!d1->Less (d3));

  Ptr<Value> s1 = StrValue::New ("hello, world!");
  Ptr<Value> s2 = StrValue::New ("hi");
  Ptr<Value> s3 = StrValue::New ("hello, world!");
  NS_TEST_ASSERT (s1->Less (s2));
  NS_TEST_ASSERT (!s2->Less (s1));
  NS_TEST_ASSERT (!s1->Less (s1));
  NS_TEST_ASSERT (!s1->Less (s3));

  Ptr<Value> ip1 = Ipv4Value::New (Ipv4Address ("192.168.0.255"));
  Ptr<Value> ip2 = Ipv4Value::New (Ipv4Address ("127.0.0.255"));
  Ptr<Value> ip3 = Ipv4Value::New (Ipv4Address ("192.168.0.255"));
  NS_TEST_ASSERT (!ip1->Less (ip2));
  NS_TEST_ASSERT (ip2->Less (ip1));
  NS_TEST_ASSERT (!ip1->Less (ip1));
  NS_TEST_ASSERT (!ip1->Less (ip3));

  Ptr<Value> b1 = BoolValue::New (true);
  Ptr<Value> b2 = BoolValue::New (false);
  Ptr<Value> b3 = BoolValue::New (true);
  NS_TEST_ASSERT (!b1->Less (b2));
  NS_TEST_ASSERT (b2->Less (b1));
  NS_TEST_ASSERT (!b1->Less (b1));
  NS_TEST_ASSERT (!b1->Less (b3));

  Ptr<Value> id1 = IdValue::New ("010101010101111110101010101010111010"
    "10101010100000000000111111111000001111111101011101010100000010101");
  Ptr<Value> id2 = IdValue::New ("001010110101001010101010101010111010"
    "10101010100000000000111111111000001111111101011101010100000010101");
  Ptr<Value> id3 = IdValue::New ("010101010101111110101010101010111010"
    "10101010100000000000111111111000001111111101011101010100000010101");
  NS_TEST_ASSERT (!id1->Less (id2));
  NS_TEST_ASSERT (id2->Less (id1));
  NS_TEST_ASSERT (!id1->Less (id1));
  NS_TEST_ASSERT (!id1->Less (id3));

  //cout << "Less test passed!" << endl;

  return result;
}

static ValuesTest g_valuesTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3

