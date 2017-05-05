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
#include "ns3/database.h"
#include "ns3/rapidnet-types.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

const string STUDENT = "STUDENT";
const string NAME = "NAME";
const string STR_ID = "ID";
const string COURSE = "COURSE";
const string GRADE = "GRADE";

class CountTrigger : public Trigger
{
public:
  static int count;
  virtual void Invoke (Ptr<Tuple> tuple)
  {
    //cout << tuple << endl;
    count++;
  }
};

int CountTrigger::count = 0;

void AddStudents (Ptr<Database> db)
{
  Ptr<Tuple> s1 = Tuple::New (STUDENT);
  s1->AddAttribute (TupleAttribute::New (NAME, StrValue::New ("Jack")));
  s1->AddAttribute (TupleAttribute::New (STR_ID, Int32Value::New (100)));

  Ptr<Tuple> s2 = Tuple::New (STUDENT);
  s2->AddAttribute (TupleAttribute::New (NAME, StrValue::New ("Jill")));
  s2->AddAttribute (TupleAttribute::New (STR_ID, Int32Value::New (101)));

  db->Insert (s1);
  db->Insert (s2);
}

void AddCourses (Ptr<Database> db)
{
  Ptr<Tuple> c1 = Tuple::New (COURSE);
  c1->AddAttribute (TupleAttribute::New (NAME, StrValue::New ("CIS-100")));
  c1->AddAttribute (TupleAttribute::New (STR_ID, Int32Value::New (100)));

  Ptr<Tuple> c2 = Tuple::New (COURSE);
  c2->AddAttribute (TupleAttribute::New (NAME, StrValue::New ("CIS-110")));
  c2->AddAttribute (TupleAttribute::New (STR_ID, Int32Value::New (101)));

  db->Insert (c1);
  db->Insert (c2);
}

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet Relation.
 *
 * @see Database
 */
class DatabaseTest : public Test
{
public:

  DatabaseTest () : Test ("Rapidnet-DatabaseTest") {}

  virtual ~DatabaseTest () {}

  virtual bool RunTests (void);

};

bool
DatabaseTest::RunTests ()
{
  bool result = true;

  Ptr<Database> db = CreateObject<Database> ();

  Ptr<Relation> reln_student = Relation::New (STUDENT);
  reln_student->AddKeyAttribute (attrdeftype (STR_ID, INT32));
  reln_student->OnInsert += Create<CountTrigger> ();
  db->AddRelation (reln_student);

  Ptr<Relation> reln_course = Relation::New (COURSE);
  reln_course->AddKeyAttribute (attrdeftype (STR_ID, INT32));
  reln_course->OnInsert += Create<CountTrigger> ();
  db->AddRelation (reln_course);

  AddStudents (db);
  NS_TEST_ASSERT (CountTrigger::count = 2);

  CountTrigger::count = 0;
  AddCourses (db);
  NS_TEST_ASSERT (CountTrigger::count = 2);

  //db->AddRelation (Relation::New (STUDENT));

  NS_TEST_ASSERT (db->HasRelation (STUDENT));
  NS_TEST_ASSERT (db->RemoveRelation (STUDENT) == true);

  //cout << "All database tests passed!" << endl;
  return result;
}

static DatabaseTest g_databaseTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3

