/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Pennsylvania
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free SoftwFoundation;
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
const string STR_ID = "STUD_ID";

const string COURSE = "COURSE";
const string STUDENT_ID = "ID";
const string COURSE_ID = "COURSE_ID";
const string GRADE = "GRADE";

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests join and project operations in RapidNet.
 *
 * @see Relation
 */
class JoinProjectTest : public Test
{
public:

  JoinProjectTest () : Test ("Rapidnet-JoinProjectTest") {}

  virtual ~JoinProjectTest () {}

  virtual bool RunTests (void);

protected:

  Ptr<Relation> GetStudentRelation ();
  Ptr<Relation> GetCourseRelation ();

  bool TestJoinWithTuple ();
  bool TestJoinStudentWithCourse ();
  bool TestJoinCourseWithStudent ();
  bool TestProject ();
  bool TestProjectWithRename ();
};

bool
JoinProjectTest::RunTests ()
{
  bool result = true;
  result = TestJoinWithTuple ()
    && TestJoinStudentWithCourse ()
    && TestJoinCourseWithStudent ()
    && TestProject ()
    && TestProjectWithRename ();

  return result;
}

Ptr<Relation>
JoinProjectTest::GetStudentRelation (void)
{
  Ptr<Relation> reln_student = Relation::New (STUDENT);
  reln_student->AddKeyAttribute (attrdeftype (STR_ID, STR));
  Ptr<Tuple> s;

  s = Tuple::New (STUDENT);
  s->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("Jake")));
  s->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Jarmel")));
  s->AddAttribute (TupleAttribute::New (STR_ID, StrValue::New ("jakej")));
  reln_student->Insert (s);

  s = Tuple::New (STUDENT);
  s->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("George")));
  s->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Costanza")));
  s->AddAttribute (TupleAttribute::New (STR_ID, StrValue::New ("georgec")));
  reln_student->Insert (s);

  s = Tuple::New (STUDENT);
  s->AddAttribute (TupleAttribute::New (FNAME, StrValue::New ("George")));
  s->AddAttribute (TupleAttribute::New (LNAME, StrValue::New ("Steinbrenner")));
  s->AddAttribute (TupleAttribute::New (STR_ID, StrValue::New ("steinb")));
  reln_student->Insert (s);

  return reln_student;
}

Ptr<Relation>
JoinProjectTest::GetCourseRelation ()
{
  Ptr<Relation> reln_course = Relation::New (COURSE);
  reln_course->AddKeyAttribute (attrdeftype (STUDENT_ID, STR));
  reln_course->AddKeyAttribute (attrdeftype (COURSE_ID, STR));
  Ptr<Tuple> c;

  c = Tuple::New (COURSE);
  c->AddAttribute (TupleAttribute::New (STUDENT_ID, StrValue::New ("jakej")));
  c->AddAttribute (TupleAttribute::New (COURSE_ID, StrValue::New ("cis500")));
  c->AddAttribute (TupleAttribute::New (GRADE, Int32Value::New (75)));
  reln_course->Insert (c);

  c = Tuple::New (COURSE);
  c->AddAttribute (TupleAttribute::New (STUDENT_ID, StrValue::New ("georgec")));
  c->AddAttribute (TupleAttribute::New (COURSE_ID, StrValue::New ("cis505")));
  c->AddAttribute (TupleAttribute::New (GRADE, Int32Value::New (50)));
  reln_course->Insert (c);

  c = Tuple::New (COURSE);
  c->AddAttribute (TupleAttribute::New (STUDENT_ID, StrValue::New ("jakej")));
  c->AddAttribute (TupleAttribute::New (COURSE_ID, StrValue::New ("cis505")));
  c->AddAttribute (TupleAttribute::New (GRADE, Int32Value::New (80)));
  reln_course->Insert (c);

  c = Tuple::New (COURSE);
  c->AddAttribute (TupleAttribute::New (STUDENT_ID, StrValue::New ("jerrys")));
  c->AddAttribute (TupleAttribute::New (COURSE_ID, StrValue::New ("cis505")));
  c->AddAttribute (TupleAttribute::New (GRADE, Int32Value::New (50)));
  reln_course->Insert (c);

  c = Tuple::New (COURSE);
  c->AddAttribute (TupleAttribute::New (STUDENT_ID, StrValue::New ("jakej")));
  c->AddAttribute (TupleAttribute::New (COURSE_ID, StrValue::New ("cis502")));
  c->AddAttribute (TupleAttribute::New (GRADE, Int32Value::New (60)));
  reln_course->Insert (c);

  return reln_course;
}

bool
JoinProjectTest::TestJoinWithTuple ()
{
  bool result = true;
  Ptr<RelationBase> reln_student = GetStudentRelation ();
  Ptr<RelationBase> reln_course = GetCourseRelation ();

  list<Ptr<Tuple> > students = reln_student->GetAllTuples (), joinTuples;
  Ptr<RelationBase> joinResult;
  list<Ptr<Tuple> >::iterator it, jt;

  list<string> student_attrs, course_attrs;
  student_attrs.push_back (STR_ID);
  course_attrs.push_back (STUDENT_ID);

  for (it = students.begin (); it != students.end (); ++it)
    {
      //cout << "Student " << *it << " matches with: " << endl;
      joinResult = reln_course->Join (*it, course_attrs, student_attrs);
      joinTuples = joinResult->GetAllTuples ();

      //for (jt = joinTuples.begin (); jt != joinTuples.end (); jt++)
      //  {
      //    cout << '\t' << (*jt) << endl;
      //  }
      //cout << endl;

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "georgec")
        {
          NS_TEST_ASSERT (joinTuples.size () == 1);
          //"Student with id 'georgec' should match with only 1 course(s). Please verify data!");
        }

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "jakej")
        {
          NS_TEST_ASSERT (joinTuples.size () == 3);
          //"Student with id 'jakej' should match with only 3 course(s). Please verify data!");
        }

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "steinb")
        {
          NS_TEST_ASSERT (joinTuples.size () == 0);
          //"Student with id 'steinb' should match with only 0 course(s). Please verify data!");
        }
    }
  //cout << "Test for joining course relation with each student tuple passed!" << endl;
  return result;
}

bool
JoinProjectTest::TestJoinStudentWithCourse ()
{
  bool result = true;

  Ptr<RelationBase> reln_student = GetStudentRelation ();
  Ptr<RelationBase> reln_course = GetCourseRelation ();

  list<Ptr<Tuple> > joinTuples;
  Ptr<RelationBase> joinResult;
  list<Ptr<Tuple> >::iterator it;

  list<string> student_attrs, course_attrs;
  student_attrs.push_back (STR_ID);
  course_attrs.push_back (STUDENT_ID);

  joinResult = reln_student->Join (reln_course, student_attrs, course_attrs, false);
  joinTuples = joinResult->GetAllTuples ();

  int georgec_count = 0, jakej_count = 0, steinb_count = 0;
  for (it = joinTuples.begin (); it != joinTuples.end (); ++it)
    {
      //cout << '\t' << (*it) << endl;
      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "georgec")
        {
          georgec_count++;
        }

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "jakej")
        {
          jakej_count++;
        }

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "steinb")
        {
          steinb_count++;
        }
    }
  NS_TEST_ASSERT (georgec_count == 1);
  //"Student with id 'georgec' should match with only 1 course(s). Please verify data!");

  NS_TEST_ASSERT (jakej_count == 3);
  //"Student with id 'jakej' should match with only 3 course(s). Please verify data!");

  NS_TEST_ASSERT (steinb_count == 0);
  //"Student with id 'steinb' should match with only 0 course(s). Please verify data!");

  //cout << "Test for joining student with course passed!" << endl;
  return result;
}

bool
JoinProjectTest::TestJoinCourseWithStudent ()
{
  bool result = true;

  Ptr<RelationBase> reln_student = GetStudentRelation ();
  Ptr<RelationBase> reln_course = GetCourseRelation ();

  list<Ptr<Tuple> > joinTuples;
  Ptr<RelationBase> joinResult;
  list<Ptr<Tuple> >::iterator it;

  list<string> student_attrs, course_attrs;
  student_attrs.push_back (STR_ID);
  course_attrs.push_back (STUDENT_ID);

  joinResult = reln_course->Join (reln_student, course_attrs, student_attrs);
  joinTuples = joinResult->GetAllTuples ();

  int georgec_count = 0, jakej_count = 0, steinb_count = 0;
  for (it = joinTuples.begin (); it != joinTuples.end (); ++it)
    {
      //cout << '\t' << (*it) << endl;
      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "georgec")
        {
          georgec_count++;
        }

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "jakej")
        {
          jakej_count++;
        }

      if ((*it)->GetAttribute (STR_ID)->GetValue ()->ToString () == "steinb")
        {
          steinb_count++;
        }
    }
  NS_TEST_ASSERT (georgec_count == 1);
  // "Student with id 'georgec' should match with only 1 course(s). Please verify data!");

  NS_TEST_ASSERT (jakej_count == 3);
  // "Student with id 'jakej' should match with only 3 course(s). Please verify data!");

  NS_TEST_ASSERT (steinb_count == 0);
  //"Student with id 'steinb' should match with only 0 course(s). Please verify data!");

  //cout << "Test for joining course with student passed!" << endl;
  return result;
}

bool
JoinProjectTest::TestProject ()
{
  bool result = true;
  Ptr<RelationBase> reln_student = GetStudentRelation ();
  Ptr<RelationBase> reln_course = GetCourseRelation ();

  list<Ptr<Tuple> > joinTuples;
  Ptr<RelationBase> joinResult;
  list<Ptr<Tuple> >::iterator it;

  list<string> student_attrs, course_attrs;
  student_attrs.push_back (STR_ID);
  course_attrs.push_back (STUDENT_ID);

  list<string> attrs;
  attrs.push_back (STUDENT_ID);
  attrs.push_back (COURSE_ID);
  attrs.push_back (GRADE);

  joinResult = reln_course->Join (reln_student, course_attrs, student_attrs, false);
  joinResult = joinResult->Project ("cis-students", attrs);
  joinTuples = joinResult->GetAllTuples ();

  NS_TEST_ASSERT (joinResult->GetName () == "cis-students");

  for (it = joinTuples.begin (); it != joinTuples.end (); ++it)
    {
      //cout << '\t' << (*it) << endl;
      NS_TEST_ASSERT ((*it)->HasAttribute (FNAME) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (LNAME) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (STR_ID) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (COURSE) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (COURSE_ID) == true);
      NS_TEST_ASSERT ((*it)->HasAttribute (STUDENT_ID) == true);
      NS_TEST_ASSERT ((*it)->HasAttribute (GRADE) == true);
    }
  //cout << "Test for projecting relations (and tuples) passed!" << endl;
  return result;
}

bool
JoinProjectTest::TestProjectWithRename ()
{
  bool result = true;

  Ptr<RelationBase> reln_student = GetStudentRelation ();
  Ptr<RelationBase> reln_course = GetCourseRelation ();

  list<Ptr<Tuple> > joinTuples;
  Ptr<RelationBase> joinResult;
  list<Ptr<Tuple> >::iterator it;

  list<string> student_attrs, course_attrs;
  student_attrs.push_back (STR_ID);
  course_attrs.push_back (STUDENT_ID);

  list<string> attrs;
  attrs.push_back (STUDENT_ID);
  attrs.push_back (COURSE_ID);
  attrs.push_back (GRADE);

  list<string> newNames;
  newNames.push_back ("penn-id");
  newNames.push_back ("cis-course");
  newNames.push_back ("number-grade");

  joinResult = reln_course->Join (reln_student, course_attrs, student_attrs, false);
  joinResult = joinResult->Project ("cis-students", attrs, newNames);
  joinTuples = joinResult->GetAllTuples ();

  for (it = joinTuples.begin (); it != joinTuples.end (); ++it)
    {
      //cout << '\t' << (*it) << endl;
      NS_TEST_ASSERT ((*it)->HasAttribute (FNAME) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (LNAME) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (STR_ID) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (COURSE) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (COURSE_ID) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (STUDENT_ID) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute (GRADE) == false);
      NS_TEST_ASSERT ((*it)->HasAttribute ("penn-id") == true);
      NS_TEST_ASSERT ((*it)->HasAttribute ("cis-course") == true);
      NS_TEST_ASSERT ((*it)->HasAttribute ("number-grade") == true);
    }
  //cout << "Test for projecting relations (and tuples) with renames passed!" << endl;
  return result;
}

static JoinProjectTest g_joinProjectTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
