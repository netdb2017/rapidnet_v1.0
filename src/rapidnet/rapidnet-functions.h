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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <list>
#include "expression.h"
#include "tuple.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 * \defgroup rapidnet_functions RapidNet Functions
 */

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns a list with the given
 *        value(s) appended in it.
 */
class FAppend : public FunctionExpr
{
public:

  virtual ~FAppend () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> source);

protected:

  Ptr<Expression> m_source;
};

class FEmpty : public FunctionExpr
{
public:

  virtual ~FEmpty () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New ();

};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that concatenates two list values.
 */
class FConcat : public FunctionExpr
{
public:

  virtual ~FConcat () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> head, Ptr<Expression> tail);

protected:

  Ptr<Expression> m_head, m_tail;
};

class FItem : public FunctionExpr
{
public:

  virtual ~FItem () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> lst, Ptr<Expression> index);

protected:

  Ptr<Expression> m_lst, m_index;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that tests if a given value is the member of
 *        a list.
 */
class FMember : public FunctionExpr
{
public:

  virtual ~FMember () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> lst, Ptr<Expression> item);

protected:

  Ptr<Expression> m_lst, m_item;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the current time as a string.
 */
class FNow : public FunctionExpr
{
public:

  virtual ~FNow () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New ();
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RadidNet function that returns the difference of two time
 * strings (in seconds) as an integer.
 */
class FDiffTime : public FunctionExpr
{
public:

  virtual ~FDiffTime () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> time2, Ptr<Expression> time1);

protected:

  Ptr<Expression> m_time2, m_time1;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RadidNet function that returns the TTL or the number of hops that
 * a triggered update should propogate in triggered HSLS implementation.
 */
class FHslsTtl : public FunctionExpr
{
public:

  virtual ~FHslsTtl () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> time,
    Ptr<Expression> hslsPeriod);

protected:

  Ptr<Expression> m_timeAttrName, m_periodAttrName;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the size of a list.
 */
class FSize : public FunctionExpr
{
public:

  virtual ~FSize () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName);

protected:

  Ptr<Expression> m_listAttrName;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the last element of a list.
 */
class FLast : public FunctionExpr
{
public:

  virtual ~FLast () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName);

protected:

  Ptr<Expression> m_listAttrName;
};


/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that removes and returns last element of a list.
 */
class FRemoveLast : public FunctionExpr
{
public:

  virtual ~FRemoveLast () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName);

protected:

  Ptr<Expression> m_listAttrName;
};


/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the string representation of the
 *        type of the given expression.
 */
class FTypeOf : public FunctionExpr
{
public:

  virtual ~FTypeOf () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> arg);

protected:

  Ptr<Expression> m_arg;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns a random number as a string.
 */
class FRand : public FunctionExpr
{
public:

  virtual ~FRand () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New ();
};


/**
 * \ingroup rapidnet_functions
 *
 * \brief A RadpidNet function that returns the SHA1 hash of the string
 * representation of the given value.
 */
class FSha1 : public FunctionExpr
{
public:

  virtual ~FSha1 () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> arg);

protected:

  Ptr<Expression> m_arg;
};

/*
 * \ingroup rapidnet_functions
 *
 * \brief A RadpidNet function that creates a new and empty summary vector.
 *
 * Example: SV := f_svcreate()
 */
class FSvCreate : public FunctionExpr
{
public:

  virtual ~FSvCreate () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New ();
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that tests whether a given ID (string) is
 * hashed in a summary vector.
 *
 * @return 0:no or 1:yes, type:INT32
 *
 * Example: Result := f_svin(SV, ID)
 */
class FSvIn : public FunctionExpr
{
public:

  virtual ~FSvIn () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr,
                                Ptr<Expression> strExpr);
protected:

  Ptr<Expression> m_svExpr, m_strExpr;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that computes the andNot result of
 * two summary vectors: (SV_1 & (~ SV_2)).
 *
 * @return The result summary vector, type:SvValue
 *
 * Example: ResultSV := f_svandnot(SV_1, SV_2), SV_1 and SV_2 are not affected
 */
class FSvAndNot : public FunctionExpr
{
public:

  virtual ~FSvAndNot () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr_1,
                                Ptr<Expression> svExpr_2);
protected:

  Ptr<Expression> m_svExpr_1, m_svExpr_2;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that hashes a given ID (string) into a new
 * summary vector.
 *
 * @return Resultant summary vector, type: SV
 *
 * examples: ResultSV := f_svappend(SV, ID), SV is not affected
 */
class FSvAppend : public FunctionExpr
{
public:
  virtual ~FSvAppend () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr,
                                Ptr<Expression> strExpr);
private:
  Ptr<Expression> m_svExpr, m_strExpr;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief FA RapidNet function that hashes a given ID (string) out from
 * a new summary vector.
 *
 * @return The resultant summary vector, type:SV
 *
 * Example: ResultSV := f_svremove(SV, ID), SV is not affected
 */
class FSvRemove : public FunctionExpr
{
public:
  virtual ~FSvRemove () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr,
                                Ptr<Expression> strExpr);
private:
  Ptr<Expression> m_svExpr, m_strExpr;
};

class FPEdb : public FunctionExpr
{
public:
  virtual ~FPEdb () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> prov, Ptr<Expression> id);

private:
  Ptr<Expression> m_prov, m_id;
};

class FPIdb : public FunctionExpr
{
public:
  virtual ~FPIdb () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> provList, Ptr<Expression> loc);

private:
  Ptr<Expression> m_provList, m_loc;
};

class FPRule : public FunctionExpr
{
public:
  virtual ~FPRule () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> provList, Ptr<Expression> rloc, Ptr<Expression> rule, Ptr<Expression> rwght);

private:
  Ptr<Expression> m_provList, m_rloc, m_rule, m_rwght;
};

class FPCal : public FunctionExpr
{
public:
  virtual ~FPCal () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> m_cal);

protected:
  Ptr<Expression> m_cal;
};

} // namespace rapidnet
} // namespace ns3


#endif // FUNCTIONS_H
