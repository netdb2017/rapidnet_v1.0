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
#include "ns3/tuple-attribute.h"
#include "ns3/rapidnet-types.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet TupleAttribute.
 *
 * @see TupleAttribute
 */
class TupleAttributeTest : public Test
{
public:

  TupleAttributeTest () : Test ("Rapidnet-TupleAttributeTest") {}

  virtual ~TupleAttributeTest () {}

  virtual bool RunTests (void);

protected:

  bool TestEquals ();
  bool TestMap ();
};

bool
TupleAttributeTest::RunTests ()
{
  bool result = true;
  result = TestEquals ()
    && TestMap ();

  return result;
}

map<Ptr<TupleAttribute>, string, bool (*) (Ptr<TupleAttribute>,
    Ptr<TupleAttribute>)> testmap (&TupleAttribute::Less);

bool
TupleAttributeTest::TestEquals ()
{
  bool result = true;
  /* int32 tests */
  Ptr<TupleAttribute> a1 = TupleAttribute::New ("a1", Int32Value::New (54));
  Ptr<TupleAttribute> a2 = TupleAttribute::New ("a2", Int32Value::New (-988));
  Ptr<TupleAttribute> a1_1 = TupleAttribute::New ("a1", Int32Value::New (54));
  Ptr<TupleAttribute> a3 = TupleAttribute::New ("a2", Int32Value::New (54));
  Ptr<TupleAttribute> a3img = CopyObject<TupleAttribute> (a3);

  NS_TEST_ASSERT (!a1->Equals (a2));
  NS_TEST_ASSERT (a1->Equals (a1_1));
  NS_TEST_ASSERT (a2->Equals (a2));
  NS_TEST_ASSERT (!a1->Equals (a3));
  NS_TEST_ASSERT (a3->Equals (a3img));
  NS_TEST_ASSERT (PeekPointer (a3img) != PeekPointer (a3));

  /* double tests */
  Ptr<TupleAttribute> d1 = TupleAttribute::New ("d1", RealValue::New (9.01));
  Ptr<TupleAttribute> d2 = TupleAttribute::New ("d2", RealValue::New (-56.9));
  Ptr<TupleAttribute> d1_1 = TupleAttribute::New ("d1", RealValue::New (9.01));
  Ptr<TupleAttribute> d3 = TupleAttribute::New ("d2", RealValue::New (9.01));
  Ptr<TupleAttribute> d3img = CopyObject<TupleAttribute> (d3);

  NS_TEST_ASSERT (!d1->Equals (d2));
  NS_TEST_ASSERT (d1->Equals (d1_1));
  NS_TEST_ASSERT (d2->Equals (d2));
  NS_TEST_ASSERT (!d1->Equals (d3));
  NS_TEST_ASSERT (d3->Equals (d3img));
  NS_TEST_ASSERT (PeekPointer (d3img) != PeekPointer (d3));

  /* string tests */
  Ptr<TupleAttribute> s1 = TupleAttribute::New ("s1", StrValue::New ("str 123"));
  Ptr<TupleAttribute> s2 = TupleAttribute::New ("s2", StrValue::New ("hello"));
  Ptr<TupleAttribute> s1_1 = TupleAttribute::New ("s1", StrValue::New ("str 123"));
  Ptr<TupleAttribute> s3 = TupleAttribute::New ("s2", StrValue::New ("str 123"));
  Ptr<TupleAttribute> s3img = CopyObject<TupleAttribute> (s3);

  NS_TEST_ASSERT (!s1->Equals (s2));
  NS_TEST_ASSERT (s1->Equals (s1_1));
  NS_TEST_ASSERT (s2->Equals (s2));
  NS_TEST_ASSERT (!s1->Equals (s3));
  NS_TEST_ASSERT (s3->Equals (s3img));
  NS_TEST_ASSERT (PeekPointer (s3img) != PeekPointer (s3));


  /* ipv4 tests */
  Ptr<TupleAttribute> ip1 = TupleAttribute::New ("ip1", Ipv4Value::New ("192.168.0.1"));
  Ptr<TupleAttribute> ip2 = TupleAttribute::New ("ip2", Ipv4Value::New ("128.111.0.255"));
  Ptr<TupleAttribute> ip1_1 = TupleAttribute::New ("ip1", Ipv4Value::New ("192.168.0.1"));
  Ptr<TupleAttribute> ip3 = TupleAttribute::New ("ip2", Ipv4Value::New ("192.168.0.1"));
  Ptr<TupleAttribute> ip3img = CopyObject<TupleAttribute> (ip3);

  NS_TEST_ASSERT (!ip1->Equals (ip2));
  NS_TEST_ASSERT (ip1->Equals (ip1_1));
  NS_TEST_ASSERT (ip2->Equals (ip2));
  NS_TEST_ASSERT (!ip1->Equals (ip3));
  NS_TEST_ASSERT (ip3->Equals (ip3img));
  NS_TEST_ASSERT (PeekPointer (ip3img) != PeekPointer (ip3));
  return result;
}

bool
TupleAttributeTest::TestMap ()
{
  bool result = true;
  /** int32 tests */
  Ptr<TupleAttribute> int1 = TupleAttribute::New ("int1", Int32Value::New (10));
  Ptr<TupleAttribute> int2 = TupleAttribute::New ("int2", Int32Value::New (100));
  Ptr<TupleAttribute> int3 = TupleAttribute::New ("int3", Int32Value::New (110));

  testmap[int1] = "10";
  testmap[int2] = "100";
  testmap[int3] = "110";

  NS_TEST_ASSERT (testmap[TupleAttribute::New("int1", Int32Value::New (10))] == "10");
  NS_TEST_ASSERT (testmap[TupleAttribute::New("int2", Int32Value::New (111))] != "100");
  NS_TEST_ASSERT (testmap[int3] == "110");

  NS_TEST_ASSERT (testmap[CopyObject<TupleAttribute> (int2)] == "100");

  /** int32 tests */
  Ptr<TupleAttribute> d1 = TupleAttribute::New ("d1", Int32Value::New (10.0));
  Ptr<TupleAttribute> d2 = TupleAttribute::New ("d2", Int32Value::New (3.14));
  Ptr<TupleAttribute> d3 = TupleAttribute::New ("d3", Int32Value::New (6.023e+3));

  testmap[d1] = "d1";
  testmap[d2] = "d2";
  testmap[d3] = "d3";

  NS_TEST_ASSERT (testmap[TupleAttribute::New("d1", Int32Value::New (10.0))] == "d1");
  NS_TEST_ASSERT (testmap[TupleAttribute::New("d2", Int32Value::New (55))] != "d2");
  NS_TEST_ASSERT (testmap[d3] == "d3");

  NS_TEST_ASSERT (testmap[CopyObject<TupleAttribute> (d2)] == "d2");

  /** string tests **/
  Ptr<TupleAttribute> str1 = TupleAttribute::New ("str1", StrValue::New ("s100"));
  Ptr<TupleAttribute> str2 = TupleAttribute::New ("str2", StrValue::New ("s103"));
  Ptr<TupleAttribute> str3 = TupleAttribute::New ("str3", StrValue::New ("a"));

  testmap[str1] = "str1";
  testmap[str2] = "str2";
  testmap[str3] = "str3";

  NS_TEST_ASSERT (testmap[TupleAttribute::New("str1", StrValue::New ("s100"))] == "str1");
  NS_TEST_ASSERT (testmap[TupleAttribute::New("str2", StrValue::New ("s103_"))] != "str2");
  NS_TEST_ASSERT (testmap[str3] == "str3");

  NS_TEST_ASSERT (testmap[CopyObject<TupleAttribute> (str2)] == "str2");

  /** ipv4 tests **/
  Ptr<TupleAttribute> ip1 = TupleAttribute::New ("ip1", Ipv4Value::New ("192.0.0.1"));
  Ptr<TupleAttribute> ip2 = TupleAttribute::New ("ip2", Ipv4Value::New ("192.0.0.2"));
  Ptr<TupleAttribute> ip3 = TupleAttribute::New ("ip3", Ipv4Value::New ("127.0.0.1"));

  testmap[ip1] = "ip1";
  testmap[ip2] = "ip2";
  testmap[ip3] = "ip3";


  NS_TEST_ASSERT (testmap[TupleAttribute::New("ip1", Ipv4Value::New ("192.0.0.1"))] == "ip1");
  NS_TEST_ASSERT (testmap[TupleAttribute::New("ip2", Ipv4Value::New ("192.0.0.123"))] != "ip2");
  NS_TEST_ASSERT (testmap[ip3] == "ip3");

  NS_TEST_ASSERT (testmap[CopyObject<TupleAttribute> (ip2)] == "ip2");

  return result;
}

static TupleAttributeTest g_tupleAttributeTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
