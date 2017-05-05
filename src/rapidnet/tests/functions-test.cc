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
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/rapidnet-functions.h"
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
 * \brief Tests RapidNet functions.
 *
 */
class FunctionsTest : public Test
{
public:

  FunctionsTest () : Test ("Rapidnet-FunctionsTest") {}

  virtual ~FunctionsTest () {}

  virtual bool RunTests (void);

protected:

  bool FAppendTest ();
  bool FConcatTest ();
  bool FMemberTest ();
  bool FSha1Test ();
};

bool
FunctionsTest::RunTests ()
{
  bool result = true;
  result = FAppendTest ()
    && FConcatTest ()
    && FMemberTest ()
    && FSha1Test ();

  return result;
}


bool
FunctionsTest::FAppendTest ()
{
  bool result = true;
  Ptr<Tuple> tuple = Tuple::New ("fappend_test");
  tuple->AddAttribute (TupleAttribute::New ("a1", Int32Value::New (5)));
  Ptr<Expression> func = FAppend::New (VarExpr::New ("a1"));
  Ptr<Assignor> assignor = Assignor::New ("a2", func);
  assignor->Assign (tuple);
  NS_TEST_ASSERT (tuple->HasAttribute ("a2"));
  //cout << tuple << endl;
  list<Ptr<Value> > results;
  results.push_back (Int32Value::New (5));
  NS_TEST_ASSERT (tuple->GetAttribute ("a2")->GetValue ()->Equals (
    ListValue::New (results)));

  //cout << "FAppend tests passed!" << endl;
  return result;
}

bool
FunctionsTest::FConcatTest ()
{
  bool result = true;
  list<Ptr<Value> > a1;
  a1.push_back (Ipv4Value::New ("10.1.1.1"));
  a1.push_back (Ipv4Value::New ("10.1.1.2"));

  list<Ptr<Value> > a2;
  a2.push_back (Ipv4Value::New ("10.1.1.3"));

  list<Ptr<Value> > a3;
  a3.push_back (Ipv4Value::New ("10.1.1.1"));
  a3.push_back (Ipv4Value::New ("10.1.1.2"));
  a3.push_back (Ipv4Value::New ("10.1.1.3"));


  Ptr<Tuple> tuple = Tuple::New ("fconcat_test");
  tuple->AddAttribute (TupleAttribute::New ("a1", ListValue::New (a1)));
  tuple->AddAttribute (TupleAttribute::New ("a2", ListValue::New (a2)));
  Ptr<Expression> func = FConcat::New (VarExpr::New ("a1"),
    VarExpr::New ("a2"));
  Ptr<Assignor> assignor = Assignor::New ("a3", func);
  assignor->Assign (tuple);
  NS_TEST_ASSERT (tuple->HasAttribute ("a3"));
  //cout << tuple << endl;
  NS_TEST_ASSERT (tuple->GetAttribute ("a3")->GetValue ()->Equals (
    ListValue::New (a3)));

  //cout << "FConcat tests passed!" << endl;
  return result;
}


bool
FunctionsTest::FMemberTest ()
{
  bool result = true;
  list<Ptr<Value> > a1;
  a1.push_back (Ipv4Value::New ("10.1.1.1"));
  a1.push_back (Ipv4Value::New ("10.1.1.2"));
  a1.push_back (Ipv4Value::New ("10.1.1.3"));

  Ptr<Tuple> tuple = Tuple::New ("fmember_test");
  tuple->AddAttribute (TupleAttribute::New ("a1", ListValue::New (a1)));
  tuple->AddAttribute (TupleAttribute::New ("a2", Ipv4Value::New ("10.1.1.2")));

  Ptr<Expression> func = FMember::New (VarExpr::New ("a1"),
    ValueExpr::New (Ipv4Value::New ("10.1.1.2")));
  Ptr<Selector> selector = Selector::New (Operation::New (RN_EQ,
    func,
    ValueExpr::New (Int32Value::New (true))));
  //cout << tuple << endl;
  NS_TEST_ASSERT (selector->Select (tuple));

  selector = Selector::New (Operation::New (RN_EQ,
    FMember::New (VarExpr::New ("a1"),
      VarExpr::New ("a2")),
    ValueExpr::New (Int32Value::New (true))));
  NS_TEST_ASSERT (selector->Select (tuple));

  selector = Selector::New (Operation::New (RN_EQ,
    FMember::New (VarExpr::New ("a1"),
      ValueExpr::New (Ipv4Value::New ("10.1.1.4"))),
    ValueExpr::New (Int32Value::New (true))));
  NS_TEST_ASSERT (!selector->Select (tuple));

  //cout << "FMember tests passed!" << endl;
  return result;
}

bool
FunctionsTest::FSha1Test ()
{
  bool result = true;
  Ptr<Expression> expr1, expr2;
  Ptr<Value> value;
  Ptr<Tuple> dummy = tuple ("dummy");

  value = StrValue::New ("Hello, World!");
  expr1 = FSha1::New (ValueExpr::New (value));
  expr2 = FSha1::New (ValueExpr::New (value->Clone ()));
  NS_TEST_ASSERT (expr1->Eval (dummy)->Equals (expr2->Eval (dummy)));
  //cout << expr1->Eval (dummy) << endl;

  value = Ipv4Value::New ("192.168.0.1");
  expr1 = FSha1::New (ValueExpr::New (value));
  expr2 = FSha1::New (ValueExpr::New (value->Clone ()));
  NS_TEST_ASSERT (expr1->Eval (dummy)->Equals (expr2->Eval (dummy)));

  //cout << "FSha1 tests passed!" << endl;
  return result;
}

static FunctionsTest g_functionsTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
