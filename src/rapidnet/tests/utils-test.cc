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
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet utils.
 *
 */
class UtilsTest : public Test
{
public:

  UtilsTest () : Test ("Rapidnet-UtilsTest") {}

  virtual ~UtilsTest () {}

  virtual bool RunTests (void);

protected:

  Ptr<Tuple> GetTuple ();

  bool TestStringList1 ();
  bool TestStringList2 ();
  bool TestStringList3 ();

  bool TupleSerializeTest ();
};

bool
UtilsTest::RunTests ()
{
  bool result = true;
  result = TestStringList1 ()
    && TestStringList2 ()
    && TestStringList3 ()
    && TupleSerializeTest ();

  return result;
}

Ptr<Tuple>
UtilsTest::GetTuple ()
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

  uint8_t* buf = new uint8_t[3];
  buf[0] = 'a'; buf[1] = 'b'; buf[2] = 'c';
  Ptr<Value> ba = ByteArrayValue::New (buf, 3);
  tuple->AddAttribute (TupleAttribute::New ("byte array", ba));

  return tuple;
}

bool
UtilsTest::TestStringList1 ()
{
  bool result = true;

  list<string> stringlist = strlist (string ("test1"));
  NS_TEST_ASSERT (stringlist.size () == 1);
  NS_TEST_ASSERT (*stringlist.begin () == "test1");

  return result;
}

bool
UtilsTest::TestStringList2 ()
{
  bool result = true;

  list<string> stringlist = MakeList<string> ("test1", "test2");
  NS_TEST_ASSERT (stringlist.size () == 2);

  list<string>::iterator it = stringlist.begin ();
  NS_TEST_ASSERT (*it == "test1"); ++it;
  NS_TEST_ASSERT (*it == "test2"); ++it;

  return result;
}

bool
UtilsTest::TestStringList3 ()
{
  bool result = true;
  list<string> stringlist = MakeList<string> ("test1", "test2", "test3");
  NS_TEST_ASSERT (stringlist.size () == 3);

  list<string>::iterator it = stringlist.begin ();
  NS_TEST_ASSERT (*it == "test1"); ++it;
  NS_TEST_ASSERT (*it == "test2"); ++it;
  NS_TEST_ASSERT (*it == "test3"); ++it;

  return result;
}

bool
UtilsTest::TupleSerializeTest()
{
  Ptr<Tuple> tuple = GetTuple ();

  return tuple->Equals (DeserializeTupleFromByteArray(
    SerializeTupleToByteArray (tuple)));

}

static UtilsTest g_utilsTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3

