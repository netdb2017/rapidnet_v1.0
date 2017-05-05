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
#include "ns3/relation.h"
#include "ns3/rapidnet-types.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

const string STUDENT = "STUDENT";
const string FNAME = "FNAME";
const string LNAME = "LNAME";
const string STR_ID = "ID";
const string GRADE = "GRADE";

const string LINK = "LINK";
const string STR_SRC = "STR_SRC";
const string SRC_DEST = "SRC_DEST";
const string COST = "COST";

class EchoTrigger : public Trigger
{
public:

  virtual void Invoke (Ptr<Tuple> tuple)
  {
    //cout << tuple << endl;
  }
};

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet Relation.
 *
 * @see Relation
 */
class RelationTest : public Test
{
public:

  RelationTest () : Test ("Rapidnet-RelationTest") {}

  virtual ~RelationTest () {}

  virtual bool RunTests (void);

protected:

  bool Test1 ();
  bool Test2 ();
};

bool
RelationTest::RunTests ()
{
  bool result = true;
  result = Test1 ()
    && Test2 ();

  return result;
}

bool
RelationTest::Test1 (void)
{
  bool result = true;

  Ptr<Relation> reln_student = Relation::New (STUDENT);
  reln_student->OnInsert += Create<EchoTrigger> ();
  //reln_student->OnInsert += Create<ListNamesTrigger> ();

  reln_student->AddKeyAttribute (attrdeftype (STR_ID, INT32));

  Ptr<Tuple> s1 = Tuple::New (STUDENT);
  s1->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("Jake")));
  s1->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Jarmel")));
  s1->AddAttribute (TupleAttribute::New (STR_ID, Int32Value::New (100)));

  Ptr<Tuple> s2 = Tuple::New (STUDENT);
  s2->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("Art")));
  s2->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Vandelier")));
  s2->AddAttribute (TupleAttribute::New (STR_ID, Int32Value::New (101)));

  reln_student->Insert (s1);
  reln_student->Insert (s2);

  NS_TEST_ASSERT (reln_student->Count () == 2);
  return result;
}


bool
RelationTest::Test2 (void)
{
  bool result = true;

  Ptr<Relation> links = Relation::New (LINK);
  links->OnInsert += Create<EchoTrigger> ();
  links->AddKeyAttribute (attrdeftype (STR_SRC, IPV4));
  links->AddKeyAttribute (attrdeftype (SRC_DEST, IPV4));

  Ptr<Tuple> l1 = Tuple::New (LINK);
  l1->AddAttribute (TupleAttribute::New (STR_SRC, Ipv4Value::New ("192.168.0.1")));
  l1->AddAttribute (TupleAttribute::New (SRC_DEST, Ipv4Value::New ("192.168.0.250")));
  l1->AddAttribute (TupleAttribute::New (COST, Int32Value::New (100)));

  Ptr<Tuple> l2 = Tuple::New (LINK);
  l2->AddAttribute (TupleAttribute::New (STR_SRC, Ipv4Value::New ("192.168.0.1")));
  l2->AddAttribute (TupleAttribute::New (SRC_DEST, Ipv4Value::New ("192.168.0.0")));
  l2->AddAttribute (TupleAttribute::New (COST, Int32Value::New (150)));

  links->Insert (l1);
  links->Insert (l2);

  NS_TEST_ASSERT (links->Count () == 2);

  return result;
}

static RelationTest g_relationTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3

