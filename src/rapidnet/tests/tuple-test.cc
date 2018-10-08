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

#include "ns3/test.h"

#include "ns3/tuple.h"
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
 * \brief Tests RapidNet's Tuple class.
 *
 * @see Tuple
 */
class TupleTest : public Test
{
public:

  TupleTest () : Test ("Rapidnet-TupleTest") {}

  virtual ~TupleTest () {}

  virtual bool RunTests (void);

protected:

  bool AttributeListEquals (Ptr<Tuple> t, Ptr<Tuple> s);

  bool EqualsTest ();
  bool MapTest ();
  bool OtherTests ();
};

bool
TupleTest::RunTests ()
{
  bool result = true;
  result = EqualsTest ()
    && MapTest ()
    && OtherTests ();

  return result;
}

bool
TupleTest::AttributeListEquals (Ptr<Tuple> t, Ptr<Tuple> s)
{
  bool result = true;
  NS_TEST_ASSERT(s->Equals(t));
  NS_TEST_ASSERT(PeekPointer(s) != PeekPointer(t));
  map<string, Ptr<TupleAttribute> >::const_iterator it;
  for (it = t->GetAllAttributes().begin(); it != t->GetAllAttributes().end(); it++)
    {
      NS_TEST_ASSERT(PeekPointer(it->second) != PeekPointer(s->GetAttribute(it->first)));
    }
  return result;
}

bool
TupleTest::EqualsTest ()
{
  bool result = true;

  Ptr<Tuple> t1 = Tuple::New();
  Ptr<Tuple> t2 = Tuple::New("no-name");
  NS_TEST_ASSERT (t1->Equals(t2));

  Ptr<Tuple> t3 = Tuple::New("t3");
  NS_TEST_ASSERT (!t1->Equals(t3));

  t3->AddAttribute (TupleAttribute::New("a1", Int32Value::New (2)));
  NS_TEST_ASSERT (!t1->Equals(t3));

  t1->SetName ("t3");
  NS_TEST_ASSERT (!t1->Equals(t3));

  t1->AddAttribute (TupleAttribute::New("a1", Int32Value::New (2)));
  NS_TEST_ASSERT (t1->Equals(t3));

  t1->AddAttribute (TupleAttribute::New("a2", Ipv4Value::New ("10.0.0.1")));
  NS_TEST_ASSERT(!t1->Equals(t3));

  //t3->AddAttribute(TupleAttribute::New("a2", Ipv4Address("10.0.0.0")));
  //NS_TEST_ASSERT(!t1->Equals(t3));

  //t3->AddAttribute(TupleAttribute::New("a2", Ipv4Address("10.0.0.1")));
  //NS_TEST_ASSERT(t1->Equals(t3));

  Ptr<Tuple> t = Tuple::New("test-tuple");
  t->AddAttribute(TupleAttribute::New("int-attr", Int32Value::New (78)));
  t->AddAttribute(TupleAttribute::New("str-attr", StrValue::New ("string theory")));
  t->AddAttribute(TupleAttribute::New("ipv4-attr", Ipv4Value::New ("10.1.2.4")));

  Ptr<Tuple> s = CopyObject<Tuple> (t);
  AttributeListEquals (s, t);

  s->RemoveAttribute ("int-attr");
  NS_TEST_ASSERT (!(s->Equals(t)));

  s->AddAttribute (TupleAttribute::New("int-attr", Int32Value::New (78)));
  AttributeListEquals (s, t);

  return result;
}

bool
TupleTest::MapTest ()
{
  bool result = true;

  Ptr<Tuple> list1 = Tuple::New ("list");
  list1->AddAttribute (TupleAttribute::New ("a1", Int32Value::New (45)));
  list1->AddAttribute (TupleAttribute::New ("a2", StrValue::New ("abc")));
  list1->AddAttribute (TupleAttribute::New ("a3", Ipv4Value::New ("192.168.1.250")));

  map<Ptr<Tuple>, string, bool (*) (Ptr<Tuple>, Ptr<Tuple>)>
    testmap (Tuple::Less);

  testmap[list1]  = "list1";
  NS_TEST_ASSERT(testmap[list1] == testmap[CopyObject<Tuple> (list1)]);

  Ptr<Tuple> list2 = CopyObject<Tuple> (list1);
  list2->AddAttribute (TupleAttribute::New ("a4", Int32Value::New (100)));
  testmap [list2] = "list2";
  NS_TEST_ASSERT (testmap[list2] == "list2");

  Ptr<Tuple> list3 = Tuple::New ("list3");
  testmap [list3] = "list3";
  NS_TEST_ASSERT (testmap[list3] == "list3");

  return result;
}

bool
TupleTest::OtherTests ()
{
  bool result = true;
  Ptr<TupleAttribute> a1, a2, a3, b1, b2, b3;

  Ptr<Tuple> list1 = Tuple::New ("lista");
  list1->AddAttribute (a1 = TupleAttribute::New ("a1", Int32Value::New (45)));
  list1->AddAttribute (a2 = TupleAttribute::New ("a2", StrValue::New ("abc")));
  list1->AddAttribute (a3 = TupleAttribute::New ("a3", Ipv4Value::New ("192.168.1.250")));

  //cout << list1->GetAttribute ("a1") << endl;
  //cout << list1->GetAttribute ("xyz");

  Ptr<Tuple> list2 = Tuple::New ("listb");
  list2->AddAttribute (b1 = TupleAttribute::New ("b1", Int32Value::New (45)));
  list2->AddAttribute (b2 = TupleAttribute::New ("b2", StrValue::New ("abc")));
  list2->AddAttribute (b3 = TupleAttribute::New ("b3", Ipv4Value::New ("192.168.1.250")));

  Ptr<Tuple> list3 = Tuple::New ("listab");

  list3->AddAllAttributes (list1);
  list3->AddAllAttributes (list2);

  NS_TEST_ASSERT (list3->HasAttribute ("a1"));
  NS_TEST_ASSERT (list3->HasAttribute ("a2"));
  NS_TEST_ASSERT (list3->HasAttribute ("a3"));
  NS_TEST_ASSERT (list3->HasAttribute ("b1"));
  NS_TEST_ASSERT (list3->HasAttribute ("b2"));
  NS_TEST_ASSERT (list3->HasAttribute ("b3"));

  //cout << list3 << endl;
  return result;
}


static TupleTest g_tupleTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3

