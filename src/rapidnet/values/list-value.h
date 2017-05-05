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

#ifndef LISTVALUE_H
#define LISTVALUE_H

#include <list>
#include "value.h"

#define rn_list(ptr) \
  DynamicCast<ListValue, Value> (ptr)->GetListValue ()

#define rn_list_iterator list<Ptr<Value> >::const_iterator

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type that is a list of RapidNet value types.
 */
class ListValue: public Value
{
public:

  ListValue (list<Ptr<Value> > value = list<Ptr<Value> > ());

  virtual ~ListValue ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the list of value objects.
   */
  inline list<Ptr<Value> > GetListValue () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * \brief Returns the number of elements in the list.
   */
  inline uint32_t Size () const;

  /**
   * \brief Returns an iterator pointing to the first value object
   *        in this list.
   */
  inline rn_list_iterator Begin () const;

  /**
   * \brief Returns an iterator pointing to the end of this list.
   */
  inline rn_list_iterator End () const;

  bool Contains (Ptr<Value> value) const;

  /**
   * \brief Factory method that creates a new ListValue object with
   *        an empty list.
   */
  static Ptr<Value> New (list<Ptr<Value> > value = list<Ptr<Value> > ());

protected:
  list<Ptr<Value> > m_value;
};

list<Ptr<Value> >
ListValue::GetListValue () const
{
  return m_value;
}

uint32_t
ListValue::Size () const
{
  return m_value.size ();
}

rn_list_iterator
ListValue::Begin () const
{
  return m_value.begin ();
}

rn_list_iterator
ListValue::End () const
{
  return m_value.end ();
}

} // namespace rapidnet
} // namespace ns3

#endif // LISTVALUE_H
