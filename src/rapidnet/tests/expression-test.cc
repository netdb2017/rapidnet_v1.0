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
#include "ns3/tuple.h"
#include "ns3/expression.h"
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
 * \brief Tests RapidNet Relation.
 *
 * @see Expression
 */
class ExpressionTest : public Test
{
public:

  ExpressionTest () : Test ("Rapidnet-ExpressionTest") {}

  virtual ~ExpressionTest () {}

  virtual bool RunTests (void);

protected:

  bool AssertEquals (Ptr<Expression> expr, Ptr<Value> value,
    Ptr<Tuple> tuple = Tuple::New ("dummy"));

  bool TestInt32 ();
  bool TestReal ();
  bool TestStr ();
  bool TestBool ();
  bool TestIpv4 ();
  bool TestList ();
  bool TestId ();
  bool TestRange ();
};

bool
ExpressionTest::RunTests ()
{
  bool result = true;
  result = TestInt32 ()
    && TestReal ()
    && TestStr ()
    && TestBool ()
    && TestIpv4 ()
    && TestList ()
    && TestId ()
    && TestRange ();

  return result;
}

bool
ExpressionTest::AssertEquals (Ptr<Expression> expr, Ptr<Value> value,
  Ptr<Tuple> tuple)
{
  bool result = true;

  //cout << expr << " = " << value << endl;
  Ptr<Value> resultValue = expr->Eval (tuple);
  NS_TEST_ASSERT (resultValue->Equals (value));
  //  "Expected " << value->GetTypeName () << ":" << value <<
  //  " but got " << result->GetTypeName () << ":" << result);
  return result;
}

bool
ExpressionTest::TestInt32 ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (Int32Value::New (10)),
    ValueExpr::New (Int32Value::New (7)));
  value = Int32Value::New (17);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (Int32Value::New (8)),
    ValueExpr::New (RealValue::New (5.023)));
  value = RealValue::New (13.023);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MINUS,
    ValueExpr::New (Int32Value::New (33)),
    ValueExpr::New (Int32Value::New (77)));
  value = Int32Value::New (-44);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MINUS,
    ValueExpr::New (Int32Value::New (8)),
    ValueExpr::New (RealValue::New (5.05)));
  value = RealValue::New (8 - 5.05);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_TIMES,
    ValueExpr::New (Int32Value::New (343)),
    ValueExpr::New (Int32Value::New (45)));
  value = Int32Value::New (343 * 45);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_TIMES,
    ValueExpr::New (Int32Value::New (45)),
    ValueExpr::New (RealValue::New (32.09)));
  value = RealValue::New (45 * 32.09);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_DIVIDE,
    ValueExpr::New (Int32Value::New (22)),
    ValueExpr::New (Int32Value::New (4)));
  value = Int32Value::New (22 / 4);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_DIVIDE,
    ValueExpr::New (Int32Value::New (8)),
    ValueExpr::New (RealValue::New (5.023)));
  value = RealValue::New (8 / 5.023);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MODULUS,
    ValueExpr::New (Int32Value::New (33)),
    ValueExpr::New (Int32Value::New (5)));
  value = Int32Value::New (3);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LSHIFT,
    ValueExpr::New (Int32Value::New (2)),
    ValueExpr::New (Int32Value::New (5)));
  value = Int32Value::New (2 << 5);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_RSHIFT,
    ValueExpr::New (Int32Value::New (1 << 5)),
    ValueExpr::New (Int32Value::New (5)));
  value = Int32Value::New (1);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (Int32Value::New (84)),
    ValueExpr::New (Int32Value::New (84)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (Int32Value::New (1)),
    ValueExpr::New (Int32Value::New (8)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GT,
    ValueExpr::New (Int32Value::New (33)),
    ValueExpr::New (Int32Value::New (-88)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (Int32Value::New (7006)),
    ValueExpr::New (Int32Value::New (7005)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (Int32Value::New (44)),
    ValueExpr::New (Int32Value::New (44)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LT,
    ValueExpr::New (Int32Value::New (1)),
    ValueExpr::New (Int32Value::New (6)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (Int32Value::New (0)),
    ValueExpr::New (Int32Value::New (2)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (Int32Value::New (123)),
    ValueExpr::New (Int32Value::New (123)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_BIT_AND,
    ValueExpr::New (Int32Value::New (10)),
    ValueExpr::New (Int32Value::New (73)));
  value = Int32Value::New (10 & 73);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_BIT_OR,
    ValueExpr::New (Int32Value::New (8)),
    ValueExpr::New (Int32Value::New (523)));
  value = Int32Value::New (8 | 523);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_BIT_XOR,
    ValueExpr::New (Int32Value::New (33)),
    ValueExpr::New (Int32Value::New (77)));
  value = Int32Value::New (33 ^ 77);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_BIT_NOT,
    ValueExpr::New (Int32Value::New (8)));
  value = Int32Value::New (~8);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    Operation::New (RN_MODULUS,
      ValueExpr::New (Int32Value::New (104)),
      ValueExpr::New (Int32Value::New (9))),
    Operation::New (RN_TIMES,
      Operation::New (RN_DIVIDE,
        ValueExpr::New (Int32Value::New (34)),
        ValueExpr::New (RealValue::New (3.0))),
      Operation::New (RN_LSHIFT,
        ValueExpr::New (Int32Value::New (1)),
        ValueExpr::New (Int32Value::New (4)))));
  value = RealValue::New ((104 % 9) + ((34 / 3.0) * (1 << 4)));
  result = result && AssertEquals (expr, value);

  //cout << "All int32 expression tests passed!" << endl;
  return result;
}

bool
ExpressionTest::TestReal ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (RealValue::New (8.05)),
    ValueExpr::New (RealValue::New (5.023)));
  value = RealValue::New (8.05 + 5.023);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (RealValue::New (10.89)),
    ValueExpr::New (Int32Value::New (-7)));
  value = RealValue::New (10.89 - 7);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MINUS,
    ValueExpr::New (RealValue::New (789.098324)),
    ValueExpr::New (RealValue::New (77.234)));
  value = RealValue::New (789.098324 - 77.234);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MINUS,
    ValueExpr::New (RealValue::New (8.09)),
    ValueExpr::New (Int32Value::New (5)));
  value = RealValue::New (8.09 - 5);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_TIMES,
    ValueExpr::New (RealValue::New (34.3)),
    ValueExpr::New (RealValue::New (4.5)));
  value = RealValue::New (34.3 * 4.5);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_TIMES,
    ValueExpr::New (RealValue::New (45.52)),
    ValueExpr::New (Int32Value::New (32)));
  value = RealValue::New (45.52 * 32);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_DIVIDE,
    ValueExpr::New (RealValue::New (22.1)),
    ValueExpr::New (RealValue::New (4.3)));
  value = RealValue::New (22.1 / 4.3);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_DIVIDE,
    ValueExpr::New (RealValue::New (8.56)),
    ValueExpr::New (Int32Value::New (51)));
  value = RealValue::New (8.56 / 51);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (RealValue::New (84.02)),
    ValueExpr::New (RealValue::New (84.02)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (RealValue::New (1)),
    ValueExpr::New (RealValue::New (8)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GT,
    ValueExpr::New (RealValue::New (-33.0)),
    ValueExpr::New (RealValue::New (-33.001)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (RealValue::New (700.6)),
    ValueExpr::New (RealValue::New (700.5)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (RealValue::New (44.44)),
    ValueExpr::New (RealValue::New (44.4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LT,
    ValueExpr::New (RealValue::New (0.01)),
    ValueExpr::New (RealValue::New (0.06)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (RealValue::New (0)),
    ValueExpr::New (RealValue::New (2.0)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (RealValue::New (1.23)),
    ValueExpr::New (RealValue::New (1.23)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    Operation::New (RN_DIVIDE,
      ValueExpr::New (RealValue::New (1.04)),
      ValueExpr::New (Int32Value::New (9))),
    Operation::New (RN_TIMES,
      Operation::New (RN_DIVIDE,
        ValueExpr::New (Int32Value::New (34)),
        ValueExpr::New (RealValue::New (3.0))),
      Operation::New (RN_LSHIFT,
        ValueExpr::New (Int32Value::New (1)),
        ValueExpr::New (Int32Value::New (4)))));
  value = RealValue::New ((1.04 / 9) + ((34 / 3.0) * (1 << 4)));
  result = result && AssertEquals (expr, value);

  //cout << "All real expression tests passed!" << endl;
  return result;
}

bool
ExpressionTest::TestStr ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (StrValue::New ("hello, ")),
    ValueExpr::New (StrValue::New ("world!")));
  value = StrValue::New ("hello, world!");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (StrValue::New ("test int")),
    ValueExpr::New (Int32Value::New (-7)));
  value = StrValue::New ("test int-7");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (StrValue::New ("homeip=")),
    ValueExpr::New (Ipv4Value::New ("127.0.0.1")));
  value = StrValue::New ("homeip=127.0.0.1");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (StrValue::New ("junk string")),
    ValueExpr::New (StrValue::New ("junk" " string")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (StrValue::New ("blah")),
    ValueExpr::New (StrValue::New ("blah blah")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GT,
    ValueExpr::New (StrValue::New ("string")),
    ValueExpr::New (StrValue::New ("str")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (StrValue::New ("string")),
    ValueExpr::New (StrValue::New ("str")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (StrValue::New ("string")),
    ValueExpr::New (StrValue::New ("string")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LT,
    ValueExpr::New (StrValue::New ("str")),
    ValueExpr::New (StrValue::New ("str123")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (StrValue::New ("str")),
    ValueExpr::New (StrValue::New ("str123")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (StrValue::New ("str123")),
    ValueExpr::New (StrValue::New ("str123")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  //cout << "All str expression tests passed!" << endl;
  return result;
}

bool
ExpressionTest::TestBool ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_AND,
    ValueExpr::New (BoolValue::New (true)),
    ValueExpr::New (BoolValue::New (false)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_AND,
    ValueExpr::New (BoolValue::New (true)),
    ValueExpr::New (BoolValue::New (true)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_OR,
    ValueExpr::New (BoolValue::New (true)),
    ValueExpr::New (BoolValue::New (false)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_OR,
    ValueExpr::New (BoolValue::New (true)),
    ValueExpr::New (BoolValue::New (true)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NOT,
    ValueExpr::New (BoolValue::New (true)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NOT,
    ValueExpr::New (BoolValue::New (false)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (BoolValue::New (true)),
    ValueExpr::New (BoolValue::New (true)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (BoolValue::New (false)),
    ValueExpr::New (BoolValue::New (false)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (BoolValue::New (true)),
    ValueExpr::New (BoolValue::New (true)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (BoolValue::New (false)),
    ValueExpr::New (BoolValue::New (true)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_OR,
    Operation::New (RN_AND,
      Operation::New (RN_EQ,
        ValueExpr::New (Int32Value::New (44)),
        ValueExpr::New (RealValue::New (44.0001))),
      Operation::New (RN_GT,
        ValueExpr::New (StrValue::New ("test123")),
        ValueExpr::New (StrValue::New ("test1234")))),
      Operation::New (RN_OR,
        Operation::New (RN_LT,
          ValueExpr::New (RealValue::New (55.0001)),
          ValueExpr::New (Int32Value::New (55))),
        Operation::New (RN_NEQ,
          ValueExpr::New (Ipv4Value::New ("127.0.0.1")),
          ValueExpr::New (Ipv4Value::New ("127.0.1.1")))));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  //cout << "All bool expression tests passed!" << endl;
  return result;
}

bool
ExpressionTest::TestIpv4 ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_EQ,
    ValueExpr::New (Ipv4Value::New ("192.168.0.123")),
    ValueExpr::New (Ipv4Value::New ("192.168.0.123")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (Ipv4Value::New ("55.1.0.10")),
    ValueExpr::New (Ipv4Value::New ("55.1.1.10")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  // Comparing Ipv4 with Str
  expr = Operation::New (RN_EQ,
    ValueExpr::New (Ipv4Value::New ("55.1.0.10")),
    ValueExpr::New (StrValue::New ("55.1.0.10")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    ValueExpr::New (Ipv4Value::New ("55.1.0.10")),
    ValueExpr::New (StrValue::New ("bummer")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (Ipv4Value::New ("128.1.0.1")),
    ValueExpr::New (StrValue::New ("33.1.1.0")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (Ipv4Value::New ("127.0.0.1")),
    ValueExpr::New (StrValue::New ("127.0.0.1")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (Ipv4Value::New ("128.1.0.1")),
    ValueExpr::New (StrValue::New ("128.1.0.1")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    ValueExpr::New (Ipv4Value::New ("10.0.0.2")),
    ValueExpr::New (StrValue::New ("NIL")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  //cout << "All ipv4 expression tests passed!" << endl;
  return result;
}

bool
ExpressionTest::TestList ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_PLUS,
    Operation::New (RN_PLUS,
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            ValueExpr::New (ListValue::New ()),
            ValueExpr::New (Int32Value::New (7))),
          ValueExpr::New (StrValue::New ("test123"))),
        ValueExpr::New (RealValue::New (3.14))),
      ValueExpr::New (BoolValue::New (true))),
    ValueExpr::New (Ipv4Value::New ("128.0.1.1")));

  list<Ptr<Value> > l1;
  l1.push_back (Int32Value::New (7));
  l1.push_back (StrValue::New ("test123"));
  l1.push_back (RealValue::New (3.14));
  l1.push_back (BoolValue::New (true));
  l1.push_back (Ipv4Value::New ("128.0.1.1"));
  value = ListValue::New (l1);
  result = result && AssertEquals (expr, value);
  return result;
}

bool
ExpressionTest::TestId ()
{
  bool result = true;
  Ptr<Expression> expr;
  Ptr<Value> value;

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (IdValue::New ("101011110101010101010101010101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = IdValue::New ("101110010010101100110000000010");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_PLUS,
    ValueExpr::New (IdValue::New ("101110010010101100110000000010")),
    ValueExpr::New (Int32Value::New (98724)));
  value = IdValue::New ("101110010011000100110110100110");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MINUS,
    ValueExpr::New (IdValue::New ("101110010010101100110000000010")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = IdValue::New ("101011110101010101010101010101");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_MINUS,
    ValueExpr::New (IdValue::New ("101110010011000100110110100110")),
    ValueExpr::New (Int32Value::New (98724)));
  value = IdValue::New ("101110010010101100110000000010");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LSHIFT,
    ValueExpr::New (IdValue::New ("101110010011000100110110100110")),
    ValueExpr::New (Int32Value::New (7)));
  value = IdValue::New ("1011100100110001001101101001100000000");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_RSHIFT,
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")),
    ValueExpr::New (Int32Value::New (7)));
  value = IdValue::New ("1001110101011101101");
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    Operation::New (RN_RSHIFT,
      Operation::New (RN_LSHIFT,
        ValueExpr::New (IdValue::New ("101110010011000100110110100110")),
        ValueExpr::New (Int32Value::New (7))),
      ValueExpr::New (Int32Value::New (7))),
    ValueExpr::New (IdValue::New ("101110010011000100110110100110")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_NEQ,
    Operation::New (RN_LSHIFT,
      Operation::New (RN_RSHIFT,
        ValueExpr::New (IdValue::New ("101110010011000100110110100110")),
        ValueExpr::New (Int32Value::New (7))),
      ValueExpr::New (Int32Value::New (7))),
    ValueExpr::New (IdValue::New ("101110010011000100110110100110")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_EQ,
    Operation::New (RN_PLUS,
      ValueExpr::New (IdValue::New ("101110010011000100110110100110")),
      ValueExpr::New (IdValue::New ("101110010011000100110110100110"))),
    Operation::New (RN_LSHIFT,
      ValueExpr::New (IdValue::New ("101110010011000100110110100110")),
      ValueExpr::New (Int32Value::New (1))));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  // LT
  expr = Operation::New (RN_LT,
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LT,
    ValueExpr::New (IdValue::New ("000000011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LT,
    ValueExpr::New (IdValue::New ("000110011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  // GT
  expr = Operation::New (RN_GT,
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GT,
    ValueExpr::New (IdValue::New ("000000011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GT,
    ValueExpr::New (IdValue::New ("000110011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  // LTE
  expr = Operation::New (RN_LTE,
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (IdValue::New ("000000011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_LTE,
    ValueExpr::New (IdValue::New ("000110011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  // GTE
  expr = Operation::New (RN_GTE,
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (IdValue::New ("000000011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = Operation::New (RN_GTE,
    ValueExpr::New (IdValue::New ("000110011101010111011010101101")),
    ValueExpr::New (IdValue::New ("000010011101010111011010101101")));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  //cout << "All id expression tests passed!" << endl;
  return result;
}

bool
ExpressionTest::TestRange ()
{
  bool result = true;

  Ptr<Expression> expr;
  Ptr<Value> value;

  // RANGECC Tests
  expr = RangeExpr::New (RangeExpr::RANGECC,
      ValueExpr::New (Int32Value::New (4)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECC,
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECC,
      ValueExpr::New (Int32Value::New (2)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECC,
      ValueExpr::New (Int32Value::New (0)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECC,
      ValueExpr::New (Int32Value::New (100)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  // RANGEOC Tests
  expr = RangeExpr::New (RangeExpr::RANGEOC,
      ValueExpr::New (RealValue::New (4)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOC,
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOC,
      ValueExpr::New (RealValue::New (2)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOC,
      ValueExpr::New (RealValue::New (0)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOC,
      ValueExpr::New (RealValue::New (4.001)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);


  // RANGECO Tests
  expr = RangeExpr::New (RangeExpr::RANGECO,
      ValueExpr::New (RealValue::New (4)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECO,
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECO,
      ValueExpr::New (RealValue::New (2)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGECO,
      ValueExpr::New (RealValue::New (0)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOC,
      ValueExpr::New (RealValue::New (4.001)),
      ValueExpr::New (RealValue::New (1)),
      ValueExpr::New (RealValue::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);


  // RANGEOO Tests
  expr = RangeExpr::New (RangeExpr::RANGEOO,
      ValueExpr::New (Int32Value::New (4)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOO,
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOO,
      ValueExpr::New (Int32Value::New (2)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (true);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOO,
      ValueExpr::New (Int32Value::New (0)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  expr = RangeExpr::New (RangeExpr::RANGEOO,
      ValueExpr::New (Int32Value::New (4.001)),
      ValueExpr::New (Int32Value::New (1)),
      ValueExpr::New (Int32Value::New (4)));
  value = BoolValue::New (false);
  result = result && AssertEquals (expr, value);

  //cout << "All range expression tests passed!" << endl;
  return result;
}


static ExpressionTest g_expressionTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3

