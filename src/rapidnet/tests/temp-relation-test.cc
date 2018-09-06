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

#include "ns3/temp-relation.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

const string STUDENT = "STUDENT";
const string FNAME = "FNAME";
const string LNAME = "LNAME";

class EchoTrigger : public Trigger
{
public:

  static int count;

  virtual void Invoke (Ptr<Tuple> tuple)
  {
    //cout << tuple << endl;
    count++;
  }
};

int EchoTrigger::count = 0;

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet TempRelation.
 *
 * @see TempRelation
 */
class TempRelationTest : public Test
{
public:

  TempRelationTest () : Test ("Rapidnet-TempRelationTest") {}

  virtual ~TempRelationTest () {}

  virtual bool RunTests (void);

protected:

  bool TestStringList1 ();
  bool TestStringList2 ();
  bool TestStringList3 ();
};

bool
TempRelationTest::RunTests ()
{
  bool result = true;

  Ptr<RelationBase> reln_student = TempRelation::New (STUDENT);
  reln_student->OnInsert += Create<EchoTrigger> ();

  Ptr<Tuple> s1 = Tuple::New (STUDENT);
  s1->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("Jake")));
  s1->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Jarmel")));

  Ptr<Tuple> s2 = Tuple::New (STUDENT);
  s2->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("Art")));
  s2->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Vandelier")));

  reln_student->Insert (s1);
  reln_student->Insert (s2);

  NS_TEST_ASSERT (EchoTrigger::count == 2);

  return result;
}

static TempRelationTest g_tempRelationTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3


