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
#include <list>

#include "ns3/test.h"
#include "ns3/heap-relation.h"
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

class EchoInsertTrigger : public Trigger
{
public:
  virtual void Invoke (Ptr<Tuple> tuple)
  {
    cout << "Inserting: " << tuple << endl;
  }
};

class EchoDeleteTrigger : public Trigger
{
public:
  virtual void Invoke (Ptr<Tuple> tuple)
  {
    cout << "Deleting : " << tuple << endl;
  }
};

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet HeapRelation.
 *
 * @see HeapRelation
 */
class HeapRelationTest : public Test
{
public:

  HeapRelationTest () : Test ("Rapidnet-HeapRelationTest") {}

  virtual ~HeapRelationTest () {}

  virtual bool RunTests (void);

protected:

  bool Test1 ();

  bool Test2 ();
};

bool
HeapRelationTest::RunTests ()
{
  bool result = true;
  result = Test1 ()
    && Test2 ();

  return result;
}

Ptr<RelationBase> reln_student1, reln_student2;

void Init (Ptr<RelationBase>& reln)
{
  reln->AddKeyAttribute (attrdeftype (STR_ID, INT32));
  //reln->OnInsert += Create<EchoInsertTrigger> ();
  //reln->OnDelete += Create<EchoDeleteTrigger> ();
}

void
InitAllRelations ()
{
  // Top 3 grades
  reln_student1 = HeapRelation::New (STUDENT, GRADE,
    HeapRelation::MAX, 3);
  Init (reln_student1);

  // First 3 alphabetically
  reln_student2 = HeapRelation::New (STUDENT, FNAME,
    HeapRelation::MIN, 3);
  Init (reln_student2);
}

void
SetupData (Ptr<RelationBase>& reln)
{
  InitAllRelations ();

  reln->Insert (tuple (STUDENT,
    attr (STR_ID, Int32Value, 1),
    attr (FNAME, StrValue, "Jim"),
    attr (GRADE, Int32Value, 75)));

  reln->Insert (tuple (STUDENT,
    attr (STR_ID, Int32Value, 2),
    attr (FNAME, StrValue, "Joe"),
    attr (GRADE, Int32Value, 85)));

  reln->Insert (tuple (STUDENT,
    attr (STR_ID, Int32Value, 3),
    attr (FNAME, StrValue, "Jack"),
    attr (GRADE, Int32Value, 60)));

  reln->Insert (tuple (STUDENT,
    attr (STR_ID, Int32Value, 4),
    attr (FNAME, StrValue, "Jake"),
    attr (GRADE, Int32Value, 90)));

  reln->Insert (tuple (STUDENT,
    attr (STR_ID, Int32Value, 5),
    attr (FNAME, StrValue, "James"),
    attr (GRADE, Int32Value, 80)));

}

bool
HeapRelationTest::Test1 ()
{
  bool result = true;

  SetupData(reln_student1);
  int count = 0;
  list<Ptr<Tuple> > tuples = reln_student1->GetAllTuples();
  for (list<Ptr<Tuple> >::iterator it = tuples.begin (); it != tuples.end ();
    ++it)
    {
      uint32_t id = rn_int32 ((*it)->GetAttribute (STR_ID)->GetValue ());
      if (id == 2 || id == 4 || id == 5)
        {
          ++count;
        }
    }

  //reln_student1->PrintAllTuples (cout);
  NS_TEST_ASSERT (count == 3);

  SetupData(reln_student2);
  count = 0;
  tuples = reln_student2->GetAllTuples();
  for (list<Ptr<Tuple> >::iterator it = tuples.begin (); it != tuples.end ();
    ++it)
    {
      uint32_t id = rn_int32 ((*it)->GetAttribute (STR_ID)->GetValue ());
      if (id == 3 || id == 4 || id == 5)
        {
          ++count;
        }
    }

  //reln_student2->PrintAllTuples (cout);
  NS_TEST_ASSERT (count == 3);

  return result;
}

bool
HeapRelationTest::Test2 ()
{
  bool result = true;

  Ptr<HeapRelation> reln = HeapRelation::New ("NODE", "DIST",
    HeapRelation::MIN, 4);
  reln->AddKeyAttribute (attrdeftype (STR_ID, INT32));
  //reln->OnInsert += Create<EchoInsertTrigger> ();
  //reln->OnDelete += Create<EchoDeleteTrigger> ();

  reln->Insert (tuple ("NODE",
    attr (STR_ID, Int32Value, 3),
    TupleAttribute::New ("DIST", IdValue::New(
      "4367bdf8fcbf738f6501fc1fdb70cf23a5ac229a", 16))));

  reln->Insert (tuple ("NODE",
    attr (STR_ID, Int32Value, 5),
    TupleAttribute::New ("DIST", IdValue::New(
      "5912b1947e96201ad3f1ba11d1b69112089e71af", 16))));

  reln->Insert (tuple ("NODE",
    attr (STR_ID, Int32Value, 7),
    TupleAttribute::New ("DIST", IdValue::New(
      "6cd5c68d23fa9601ea881421cc623667da0823f3", 16))));

  reln->Insert (tuple ("NODE",
    attr (STR_ID, Int32Value, 6),
    TupleAttribute::New ("DIST", IdValue::New(
      "371b3e631649f0c0b9f9d94c28fd93d0a39621d8", 16))));

  reln->Insert (tuple ("NODE",
    attr (STR_ID, Int32Value, 8),
    TupleAttribute::New ("DIST", IdValue::New(
      "9b6285353cdd27e24e743350e1937229f3926584", 16))));

  NS_TEST_ASSERT (reln->Count () == 4);

  return result;
}

static HeapRelationTest g_heapRelationTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
