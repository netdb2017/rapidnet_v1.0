/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ns3/ptr.h"
#include "ns3/ref-count-base.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief Enumeration of all supported operators.
 */
enum Operator
{
  RN_NO_OP = 0, // No operation
  RN_PLUS,
  RN_MINUS,
  RN_TIMES,
  RN_DIVIDE,
  RN_MODULUS,
  RN_LSHIFT,
  RN_RSHIFT,
  RN_NOT,
  RN_AND,
  RN_OR,
  RN_EQ,
  RN_NEQ,
  RN_GT,
  RN_LT,
  RN_GTE,
  RN_LTE,
  RN_IN_RANGE,
  RN_BIT_XOR,
  RN_BIT_AND,
  RN_BIT_OR,
  RN_BIT_NOT,
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Returns a string representation for the operator.
 */
string OpString (Operator op);

class Tuple;
class Value;

/**
 * \ingroup rapidnet_library
 *
 * \brief Abstract base class for all expressions.
 */
class Expression: public RefCountBase
{
public:

  Expression ();

  virtual ~Expression ();

  /**
   * \brief Evaluates the expression against the given tuple.
   *        Note that all expressions are evaluated only against
   *        a tuple. The expression may use attributes of the tuple
   *        by using a VarExpr object. Or it may not use any
   *        attributes at all.
   *
   * @see VarExpr
   */
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple) = 0;

  /**
   * \brief String representation of the expression.
   */
  virtual string ToString () const = 0;
};

ostream& operator << (ostream& os, const Ptr<Expression>& expr);

/**
 * \ingroup rapidnet_library
 *
 * \brief An operation over one (unary) or two (binary) operands
 *        which are in turn expressions.
 */
class Operation: public Expression
{
public:

  Operation ();

  virtual ~Operation ();

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  virtual string ToString () const;

  /**
   * \brief Factory method to create a unary expression.
   */
  static Ptr<Expression> New (Operator op, Ptr<Expression> left);

  /**
   * \brief Factory method to create a unary expression.
   */
  static Ptr<Expression> New (Operator op, Ptr<Expression> left,
    Ptr<Expression> right);

protected:

  enum Type { UNARY, BINARY } m_type;

  Operator m_op;

  Ptr<Expression> m_left, m_right;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief A value expression. Evaluates to the value itself.
 */
class ValueExpr : public Expression
{
public:

  ValueExpr ();

  virtual ~ValueExpr ();

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  virtual string ToString () const;

  /**
   * \brief Creates a new ValueExpr object.
   */
  static Ptr<Expression> New (Ptr<Value> value);

protected:

  Ptr<Value> m_value;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief A variable expression. The variable name is the name of a
 * tuple attribute. It evaluates to the value of that attribute in the
 * given tuple.
 */
class VarExpr : public Expression
{
public:
  VarExpr ();

  virtual ~VarExpr ();

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  virtual string ToString () const;

  /**
   * \brief Factory method to create a new VarExpr object.
   */
  static Ptr<Expression> New (string name);

protected:

  string m_name;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Range expression. It is one of the 4 types
 *        as defined by Interval.
 *
 * It uses operators LT, LTE, GT and GTE on the arguments for evaluation.
 *
 * @see Interval.
 */
class RangeExpr : public Expression
{
public:

  virtual ~RangeExpr () {}

  /**
   * \brief The type of the RangeExpr
   */
  enum Interval
  {
    /**
     * \brief Open on both sides: ()
     */
    RANGEOO,

    /**
     * \brief Open on the left, closed on the right: (]
     */
    RANGEOC,

    /**
     * \brief Closed on the left, open on the right: [)
     */
    RANGECO,

    /**
     * \brief Closed on both sides: []
     */
    RANGECC
  };

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  virtual string ToString () const;

  /**
   * \brief Factory method to create a new range expression.
   *
   * @param type The type of range expression.
   * @param value The value to be tested for being in range.
   * @param start The lower limit of the range.
   * @param end The upper limit of the range.
   */
  static Ptr<Expression> New (Interval type, Ptr<Expression> value,
    Ptr<Expression> start, Ptr<Expression> end);

protected:

  Interval m_type;

  Ptr<Expression> m_testExpr, m_start, m_end;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief Function call as an expression. This class is merely a
 * a marker interface for all functions, it does not provide any
 * functionality.
 */
class FunctionExpr : public Expression
{
public:
  FunctionExpr ();

  virtual ~FunctionExpr ();

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple) = 0;

  virtual string ToString () const;
};

} // namespace rapidnet
} // namespace ns3

#endif // EXPRESSION_H
