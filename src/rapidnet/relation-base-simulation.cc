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
#include "relation-base.h"
#include "temp-relation.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

TypeId
RelationBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::RelationBase")
    .SetParent<Object> ()
  ;
  return tid;
}

RelationBase::RelationBase (string name)
{
  m_name = name;
}

string
RelationBase::GetName ()
{
  return m_name;
}

void
RelationBase::InsertAll (list<Ptr<Tuple> > tuples)
{
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      Insert (*it);
    }
}

void
RelationBase::PrintAllTuples (ostream& os)
{
  os << " :Printing all tuples in relation '" << m_name << "'" << endl;

  list<Ptr<Tuple> > tuples = GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); it++)
    {
     os << "    " << *it << endl;
    }
}

Ptr<RelationBase>
RelationBase::Project (string newRelnName, list<string> attrNames,
  list<string> newNames)
{
  Ptr<RelationBase> result = TempRelation::New (newRelnName);
  list<Ptr<Tuple> > tuples = GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      result->Insert ((*it)->Project (newRelnName, attrNames, newNames));
    }
  return result;
}

Ptr<RelationBase>
RelationBase::Select (Ptr<Selector> selector)
{
  Ptr<RelationBase> result = TempRelation::New (m_name);
  list<Ptr<Tuple> > tuples = GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      if (selector->Select (*it))
        {
          result->Insert (CopyObject<Tuple> (*it));
        }
    }
  return result;
}

Ptr<RelationBase>
RelationBase::Join (Ptr<Tuple> rTuple, list<string> lAttrNames,
  list<string> rAttrNames, bool qualify)
{
  Ptr<RelationBase> retval = TempRelation::New (JOIN_NAMES (
    GetName (), rTuple->GetName ())) ;
  list<Ptr<TupleAttribute> > rAttrs = _GetAttributesAsList (
    rTuple, rAttrNames);
  list<Ptr<Tuple> > result = _DoJoin (lAttrNames, rTuple, rAttrs, qualify);
  retval->InsertAll (result);
  return retval;
}

/**
 * \brief Used to get the attributes from the right tuple as a list.
 *
 */
list<Ptr<TupleAttribute> >
RelationBase::_GetAttributesAsList (Ptr<Tuple> tuple, list<string> attrNames)
{
  list<Ptr<TupleAttribute> > retval;
  list<string>::iterator it;

  for (it = attrNames.begin (); it != attrNames.end (); it++)
    {
      retval.push_back (tuple->GetAttribute (*it));
    }

  return retval;
}

/**
 * \brief Joins the right tuple with all tuples in the current relation
 * on the left.
 *
 */
list<Ptr<Tuple> >
RelationBase::_DoJoin (list<string> lAttrNames, Ptr<Tuple> rTuple,
  list<Ptr<TupleAttribute> > rAttrs, bool qualify)
{
  list<Ptr<Tuple> > retval;
  list<Ptr<Tuple> > lTuples = GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;
  Ptr<Tuple> lTuple, tempTuple;

  for (it = lTuples.begin (); it != lTuples.end (); it++)
    {
      lTuple = *it;
      if (_IsMatch (lTuple, lAttrNames, rAttrs))
        {
          tempTuple = Tuple::New (JOIN_NAMES (lTuple->GetName (),
            rTuple-> GetName ()));
          tempTuple->AddAllAttributes (lTuple, qualify);
          tempTuple->AddAllAttributes (rTuple, qualify);
          retval.push_back (tempTuple);
        }
    }
  return retval;
}

/**
 * \brief Determines if a given left tuple from the current relation matches with
 * the right tuple attributes.
 *
 */
bool
RelationBase::_IsMatch (Ptr<Tuple> lTuple, list<string> lAttrNames,
  list<Ptr<TupleAttribute> > rAttrs)
{
  list<string>::iterator lan_it;
  list<Ptr<TupleAttribute> >::iterator ra_it;
  Ptr<TupleAttribute> leftAttr, rightAttr;

  for (lan_it = lAttrNames.begin (), ra_it = rAttrs.begin ();
    lan_it != lAttrNames.end () && ra_it != rAttrs.end (); lan_it++, ra_it++)
    {
      leftAttr = lTuple->GetAttribute (*lan_it);
      rightAttr = *ra_it;
      if (leftAttr->ValueEquals (rightAttr) == false)
        {
          return false;
        }
    }
  return true;
}

Ptr<RelationBase>
RelationBase::Join (Ptr<RelationBase> reln, list<string> lAttrNames,
  list<string> rAttrNames, bool qualify)
{
  Ptr<RelationBase> retval = TempRelation::New (JOIN_NAMES (
    GetName (), reln->GetName ()));
  list<Ptr<Tuple> > rTuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;
  list<Ptr<TupleAttribute> > rAttrs;
  list<Ptr<Tuple> > result;

  for (it = rTuples.begin (); it != rTuples.end (); ++it)
    {
      rAttrs = _GetAttributesAsList (*it, rAttrNames);
      result = _DoJoin (lAttrNames, *it, rAttrs, qualify);
      retval->InsertAll (result);
    }

  return retval;
}

void
RelationBase::Assign (Ptr<Assignor> assignor)
{
  list<Ptr<Tuple> > tuples = GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      assignor->Assign (*it);
    }
}
