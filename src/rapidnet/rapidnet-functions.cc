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

#include "rapidnet-functions.h"
#include <cstdlib>
#include "ns3/simulator.h"
#include "ns3/rapidnet-types.h"
#include "rapidnet-application-base.h"
#include "rapidnet-utils.h"
#include "expression.h"

using namespace ns3;
using namespace rapidnet;

Ptr<Value>
FAppend::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  result.push_back (m_source->Eval (tuple));
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FAppend::New (Ptr<Expression> source)
{
  Ptr<FAppend> retval = Create<FAppend> ();
  retval->m_source = source;
  return retval;
}

Ptr<Value>
FEmpty::Eval (Ptr<Tuple> tuple)
{
  return ListValue::New ();
}

Ptr<FunctionExpr>
FEmpty::New ()
{
  return Create<FEmpty> ();
}

Ptr<Value>
FConcat::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  Ptr<ListValue> headVal = DynamicCast<ListValue, Value> (
      m_head->Eval (tuple));
  Ptr<ListValue> tailVal = DynamicCast<ListValue, Value> (
      m_tail->Eval (tuple));
  for (rn_list_iterator it = headVal->Begin(); it != headVal->End (); ++it)
    {
      result.push_back (*it);
    }
  for (rn_list_iterator it = tailVal->Begin(); it != tailVal->End (); ++it)
    {
      result.push_back (*it);
    }
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FConcat::New (Ptr<Expression> head, Ptr<Expression> tail)
{
  Ptr<FConcat> retval = Create<FConcat> ();
  retval->m_head = head;
  retval->m_tail = tail;
  return retval;
}

Ptr<Value>
FItem::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > lst = rn_list (m_lst->Eval (tuple));
  uint32_t index = rn_int32 (m_index->Eval (tuple));

  rn_list_iterator it = lst.begin ();

  if (index > lst.size () || index < 1)
    {
      return NilValue::New ();
    }

  for (uint32_t i = 0; i < index - 1; i++)
    {
      it++;
    }

  return (*it)->Clone ();
}

Ptr<FunctionExpr>
FItem::New (Ptr<Expression> lst, Ptr<Expression> index)
{
  Ptr<FItem> retval = Create<FItem> ();
  retval->m_lst = lst;
  retval->m_index = index;
  return retval;
}

Ptr<Value>
FMember::Eval (Ptr<Tuple> tuple)
{
  Ptr<ListValue> lstVal = DynamicCast<ListValue, Value> (m_lst->Eval (tuple));
  // Seems P2 does not support "true" and "false" as keywords. So use int32 for now
  return Int32Value::New (lstVal->Contains (m_item->Eval (tuple)));
}

Ptr<FunctionExpr>
FMember::New (Ptr<Expression> lst, Ptr<Expression> item)
{
  Ptr<FMember> retval = Create<FMember> ();
  retval->m_lst = lst;
  retval->m_item = item;
  return retval;
}

Ptr<Value>
FNow::Eval (Ptr<Tuple> tuple)
{
	return RealValue::New (Simulator::Now ().GetSeconds ());
}

Ptr<FunctionExpr>
FNow::New ()
{
  return Create<FNow> ();
}

Ptr<Value>
FDiffTime::Eval (Ptr<Tuple> tuple)
{
  return Operation::New (RN_MINUS, m_time2, m_time1)->Eval (tuple);
}

Ptr<FunctionExpr>
FDiffTime::New (Ptr<Expression> time2, Ptr<Expression> time1)
{
  Ptr<FDiffTime> retval = Create<FDiffTime> ();
  retval->m_time2 = time2;
  retval->m_time1 = time1;
  return retval;
}

Ptr<Value>
FHslsTtl::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> now = FNow::New ()->Eval (tuple);
  Ptr<Value> normTimeDiff = Operation::New (RN_PLUS,
  Operation::New (RN_DIVIDE, ValueExpr::New (now), m_periodAttrName),
  ValueExpr::New (RealValue::New (1.0)))->Eval (tuple);
  int32_t n = int32_t (rn_real (normTimeDiff));
  int32_t ttl = n % 2 == 1 ? 2 : (n % 4 == 2 ? 4 : (n % 8 == 4 ? 8 : 16));
  return Int32Value::New (ttl);
}

Ptr<FunctionExpr>
FHslsTtl::New (Ptr<Expression> timeAttrName, Ptr<Expression> periodAttrName)
{
  Ptr<FHslsTtl> retval = Create<FHslsTtl> ();
  retval->m_timeAttrName = timeAttrName;
  retval->m_periodAttrName = periodAttrName;
  return retval;
}

Ptr<Value>
FSize::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_last can only be applied to a list type attribute.");

  return Int32Value::New ((DynamicCast<ListValue, Value> (lstVal))->Size ());
}

Ptr<FunctionExpr>
FSize::New (Ptr<Expression> listAttrName)
{
  Ptr<FSize> retval = Create<FSize> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FLast::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_last can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
    {
      return Ptr<Value> (NULL);
    }

  rn_list_iterator it = lst.end ();
  return (*--it)->Clone ();
}

Ptr<FunctionExpr>
FLast::New (Ptr<Expression> listAttrName)
{
  Ptr<FLast> retval = Create<FLast> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FRemoveLast::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_removeLast can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
    {
      return Ptr<Value> (NULL);
    }

  rn_list_iterator it = lst.end ();
  --it; // move one step back
  list<Ptr<Value> > newlst;
  for (rn_list_iterator jt = lst.begin (); jt != it; jt++)
    {
      newlst.push_back (*jt);
    }
  return ListValue::New (newlst);
}

Ptr<FunctionExpr>
FRemoveLast::New (Ptr<Expression> listAttrName)
{
  Ptr<FRemoveLast> retval = Create<FRemoveLast> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FTypeOf::Eval (Ptr<Tuple> tuple)
{
  return StrValue::New (m_arg->Eval (tuple)->GetTypeName ());
}

Ptr<FunctionExpr>
FTypeOf::New (Ptr<Expression> arg)
{
  Ptr<FTypeOf> retval = Create<FTypeOf> ();
  retval->m_arg = arg;
  return retval;
}

Ptr<Value>
FRand::Eval (Ptr<Tuple> tuple)
{
  stringstream ss;
  ss << rand ();
  return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FRand::New ()
{
  return Create<FRand> ();
}

Ptr<Value>
FSha1::Eval (Ptr<Tuple> tuple)
{
  string str = m_arg->Eval (tuple)->ToString ();
  return IdValue::New (GetSHA1Digest (str), 16);
}

Ptr<FunctionExpr>
FSha1::New (Ptr<Expression> arg)
{
  Ptr<FSha1> retval = Create<FSha1> ();
  retval->m_arg = arg;
  return retval;
}

Ptr<Value>
FSvCreate::Eval (Ptr<Tuple> tuple)
{
  return SvValue::New();
}

Ptr<FunctionExpr>
FSvCreate::New ()
{
  Ptr<FSvCreate> retval = Create<FSvCreate> ();
  return retval;
}

Ptr<Value>
FSvIn::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  int32_t result = SvValue::HashIn(svVPtr, strVPtr) ? 1 : 0;

  return Int32Value::New(result);
}

Ptr<FunctionExpr>
FSvIn::New (Ptr<Expression> svExpr,
            Ptr<Expression> strExpr)
{
  Ptr<FSvIn> retval = Create<FSvIn>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

Ptr<Value>
FSvAndNot::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr_1 = m_svExpr_1->Eval (tuple);
  Ptr<Value> svVPtr_2 = m_svExpr_2->Eval (tuple);

  return SvValue::AndNot(svVPtr_1, svVPtr_2);
}

Ptr<FunctionExpr>
FSvAndNot::New (Ptr<Expression> svExpr_1,
                Ptr<Expression> svExpr_2)
{
  Ptr<FSvAndNot> retval = Create<FSvAndNot>();
  retval->m_svExpr_1 = svExpr_1;
  retval->m_svExpr_2 = svExpr_2;
  return retval;
}

Ptr<Value>
FSvAppend::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  return  SvValue::HashInsert(svVPtr, strVPtr);
}

Ptr<FunctionExpr>
FSvAppend::New (Ptr<Expression> svExpr,
            Ptr<Expression> strExpr)
{
  Ptr<FSvAppend> retval = Create<FSvAppend>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

Ptr<Value>
FSvRemove::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  return SvValue::HashRemove(svVPtr, strVPtr);
}

Ptr<FunctionExpr>
FSvRemove::New (Ptr<Expression> svExpr,
            Ptr<Expression> strExpr)
{
  Ptr<FSvRemove> retval = Create<FSvRemove>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

Ptr<Value>
FPEdb::Eval(Ptr<Tuple> tuple)
{
  string prov = m_prov->Eval (tuple)-> ToString();

  return StrValue::New (prov);
}

Ptr<FunctionExpr>
FPEdb::New (Ptr<Expression> prov, Ptr<Expression> id)
{
  Ptr<FPEdb> retval = Create<FPEdb>();
  retval->m_prov = prov;
  retval->m_id = id;
  return retval;
}

Ptr<Value>
FPIdb::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  if (provList.size() != 1)
    {
	  ss << "(";
    }

  int index = 0;

  for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
    {
      if (index++!=0) ss << "+";
      ss << (*it)->ToString ();
    }

//  string loc = m_loc->Eval (tuple)->ToString ();
//  ss << ")@[" << loc << "]";

  if (provList.size() != 1) 
    {
	  ss << ")";
    }

  return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPIdb::New (Ptr<Expression> provList, Ptr<Expression> loc)
{
  Ptr<FPIdb> retval = Create<FPIdb>();
  retval->m_provList = provList;
  retval->m_loc = loc;
  return retval;
}

Ptr<Value>
FPRule::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  string rule = m_rule->Eval (tuple)->ToString ();

  uint32_t ipaddr = (rn_ipv4 (m_rloc->Eval (tuple))).Get ();
  ipaddr = (ipaddr / 256) % 65536;
  ss << rule << "@n" << ipaddr << "(";

  int index = 0;

  for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
    {
      if (index++!=0) ss << "*";
      ss << (*it)->ToString ();
    }

  ss << ")";

  return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPRule::New (Ptr<Expression> provList, Ptr<Expression> rloc, Ptr<Expression> rule)
{
  Ptr<FPRule> retval = Create<FPRule>();
  retval->m_provList = provList;
  retval->m_rloc = rloc;
  retval->m_rule = rule;

  return retval;
}

