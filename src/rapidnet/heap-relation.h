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

#ifndef HEAP_RELATION_H
#define HEAP_RELATION_H

#include "relation.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief A special type of relation that is similar to a min-heap or
 *        a max-heap.
 *
 * A heap attribute is specified that value of which is compared across all
 * tuples inserted in the relation. And a maximum size is defined and a @see
 * Type, either MIN or MAX is defined. In case of MIN, for example, the
 * relation keeps utmost the maximum size number of tuples with lowest values
 * for the heap attribute. If more tuples are added, the ones with the highest
 * values are removed before inserting the new one. Similarly, for the MAX
 * type.
 */
class HeapRelation: public Relation
{
public:

  /**
   * \brief Type of the heap relation.
   */
  enum Type {MIN, MAX};

  HeapRelation ();

  virtual ~HeapRelation ();

  /**
   * \brief Sets the @see Type.
   */
  void SetType (Type type);

  /**
   * \brief Sets the name of the heap attribute.
   */
  void SetHeapAttribute (string attr);

  /**
   * \brief Sets the heap size.
   */
  void SetHeapSize (uint32_t size);

  /**
   * \brief Inserts a tuple in the relation.
   */
  virtual int Insert (Ptr<Tuple> tuple);

  /**
   * \brief Factory method to create a new heap relation.
   *
   * @param name Name of the relation
   * @param heapAttr Name of the heap attribute
   * @param type Type of the heap, MIN or MAX
   * @param size Size of the heap
   */
  static Ptr<HeapRelation> New (string name, string heapAttr,
    Type type, uint32_t size);

protected:

  Type m_type;

  string m_heapAttr;

  uint32_t m_size;
};

} // namespace rapidnet
} // namespace ns3

#endif // HEAP_RELATION_H
