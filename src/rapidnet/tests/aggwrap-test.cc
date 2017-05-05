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
#include "ns3/aggwrap.h"
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
 * \brief Tests RapidNet Aggwraps
 *
 * @see AggWrap
 */
class AggWrapTest : public Test
{
public:

  AggWrapTest () : Test ("Rapidnet-AggWrapTest") {}

  virtual ~AggWrapTest () {}

  virtual bool RunTests (void);

protected:

  bool AggrMinTest1 ();
  bool AggrMinTest2 ();
  bool AggrMaxTest1 ();
  bool AggrMaxTest2 ();
};

bool
AggWrapTest::RunTests ()
{
  bool result = true;
  result = AggrMinTest1 ()
    && AggrMinTest2 ()
    && AggrMaxTest1 ()
    && AggrMaxTest2 ();

  return result;
}

//materialize(link, infinity, 1, keys(1,2)).

Ptr<RelationBase>
GetRelation ()
{
  Ptr<Relation> linkReln = Relation::New ("link");
  linkReln->AddKeyAttribute (attrdeftype ("id", STR));

  linkReln->Insert (tuple ("link",
    attr ("id", StrValue, "T1"),
    attr ("src", Ipv4Value, "10.0.0.1"),
    attr ("dest", Ipv4Value, "10.0.0.2"),
    attr ("cost", Int32Value, 5),
    attr ("timestamp", StrValue, "1pm")));

  linkReln->Insert (tuple ("link",
    attr ("id", StrValue, "T2"),
    attr ("src", Ipv4Value, "10.0.0.1"),
    attr ("dest", Ipv4Value, "10.0.0.2"),
    attr ("cost", Int32Value, 3),
    attr ("timestamp", StrValue, "2pm")));

  linkReln->Insert (tuple ("link",
    attr ("id", StrValue, "T3"),
    attr ("src", Ipv4Value, "10.0.0.1"),
    attr ("dest", Ipv4Value, "10.0.0.2"),
    attr ("cost", Int32Value, 10),
    attr ("timestamp", StrValue, "3pm")));

  linkReln->Insert (tuple ("link",
    attr ("id", StrValue, "T4"),
    attr ("src", Ipv4Value, "10.0.0.1"),
    attr ("dest", Ipv4Value, "10.0.0.3"),
    attr ("cost", Int32Value, 11),
    attr ("timestamp", StrValue, "4pm")));

  linkReln->Insert (tuple ("link",
    attr ("id", StrValue, "T5"),
    attr ("src", Ipv4Value, "10.0.0.1"),
    attr ("dest", Ipv4Value, "10.0.0.3"),
    attr ("cost", Int32Value, 5),
    attr ("timestamp", StrValue, "5pm")));

  linkReln->Insert (tuple ("link",
    attr ("id", StrValue, "T6"),
    attr ("src", Ipv4Value, "10.0.0.1"),
    attr ("dest", Ipv4Value, "10.0.0.4"),
    attr ("cost", Int32Value, 15),
    attr ("timestamp", StrValue, "6pm")));

  return linkReln;
}

// Test Min for every src/dest pair
bool
AggWrapTest::AggrMinTest1 ()
{
  bool result = true;

  Ptr<AggWrap> aggwrap1 = AggWrap::New<AggWrapMin> (
    attrdeflist (
      attrdeftype ("src", ANYTYPE),
      attrdeftype ("dest", ANYTYPE),
      attrdeftype ("cost", ANYTYPE),
      attrdeftype ("id", ANYTYPE),
      attrdeftype ("timestamp", ANYTYPE)),
    3);

  // Assume we get this as the result
  Ptr<RelationBase> resultReln = GetRelation ();

  // Run aggwrap over the result
  resultReln = aggwrap1->Compute (resultReln);
  // result->PrintAllTuples (cout);
  NS_TEST_ASSERT (resultReln->Count() == 3);
  list<Ptr<Tuple> > tuples = resultReln->GetAllTuples ();

  int correct_match = 0;
  for (list<Ptr<Tuple> >::iterator it = tuples.begin ();
    it != tuples.end (); ++it)
    {
      string id = rn_str ((*it)->GetAttribute ("id")->GetValue ());
      if (id == "T2" || id == "T5" || id == "T6")
        {
          correct_match++;
        }
    }
  NS_TEST_ASSERT (correct_match == 3);
  //cout << "AggrMinTest1 is passed!" << endl;
  return result;
}

// Test Max for every src/dest pair
bool
AggWrapTest::AggrMaxTest1 ()
{
  bool result = true;

  Ptr<AggWrap> aggwrap1 = AggWrap::New<AggWrapMax> (
    attrdeflist (
      attrdeftype ("src", ANYTYPE),
      attrdeftype ("dest", ANYTYPE),
      attrdeftype ("cost", ANYTYPE),
      attrdeftype ("id", ANYTYPE),
      attrdeftype ("timestamp", ANYTYPE)),
    3);

  // Assume we get this as the result
  Ptr<RelationBase> resultReln = GetRelation ();

  // Run aggwrap over the result
  resultReln = aggwrap1->Compute (resultReln);
  // result->PrintAllTuples (cout);
  NS_TEST_ASSERT (resultReln->Count() == 3);
  list<Ptr<Tuple> > tuples = resultReln->GetAllTuples ();

  int correct_match = 0;
  for (list<Ptr<Tuple> >::iterator it = tuples.begin ();
    it != tuples.end (); ++it)
    {
      string id = rn_str ((*it)->GetAttribute ("id")->GetValue ());
      if (id == "T3" || id == "T4" || id == "T6")
        {
          correct_match++;
        }
    }
  NS_TEST_ASSERT (correct_match == 3);
  //cout << "AggrMaxTest1 is passed!" << endl;
  return result;
}

// Test Min over all
bool
AggWrapTest::AggrMinTest2 ()
{
  bool result = true;

  Ptr<AggWrap> aggwrap1 = AggWrap::New<AggWrapMin> (
    attrdeflist (
      attrdeftype ("cost", ANYTYPE),
      attrdeftype ("src", ANYTYPE),
      attrdeftype ("dest", ANYTYPE),
      attrdeftype ("id", ANYTYPE)),
    1);

  // Assume we get this as the result
  Ptr<RelationBase> resultReln = GetRelation ();

  // Run aggwrap over the result
  resultReln = aggwrap1->Compute (resultReln);
  //result->PrintAllTuples (cout);
  NS_TEST_ASSERT (resultReln->Count() == 1);
  list<Ptr<Tuple> > tuples = resultReln->GetAllTuples ();

  int correct_match = 0;
  for (list<Ptr<Tuple> >::iterator it = tuples.begin ();
    it != tuples.end (); ++it)
    {
      string id = rn_str ((*it)->GetAttribute ("id")->GetValue ());
      if (id == "T2")
        {
          correct_match++;
        }
    }
  NS_TEST_ASSERT (correct_match == 1);
  //cout << "AggrMinTest2 is passed!" << endl;
  return result;
}

bool
AggWrapTest::AggrMaxTest2 ()
{
  bool result = true;

  Ptr<AggWrap> aggwrap1 = AggWrap::New<AggWrapMax> (
    attrdeflist (
      attrdeftype ("cost", ANYTYPE),
      attrdeftype ("src", ANYTYPE),
      attrdeftype ("dest", ANYTYPE),
      attrdeftype ("id", ANYTYPE)),
    1);

  // Assume we get this as the result
  Ptr<RelationBase> resultReln = GetRelation ();

  // Run aggwrap over the result
  resultReln = aggwrap1->Compute (resultReln);
  //result->PrintAllTuples (cout);
  NS_TEST_ASSERT (resultReln->Count() == 1);
  list<Ptr<Tuple> > tuples = resultReln->GetAllTuples ();

  int correct_match = 0;
  for (list<Ptr<Tuple> >::iterator it = tuples.begin ();
    it != tuples.end (); ++it)
    {
      string id = rn_str ((*it)->GetAttribute ("id")->GetValue ());
      if (id == "T6")
        {
          correct_match++;
        }
    }
  NS_TEST_ASSERT (correct_match == 1);
  //cout << "AggrMaxTest2 is passed!" << endl;
  return result;
}

static AggWrapTest g_aggWrapTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
