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

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include "ns3/ptr.h"
#include "ns3/ref-count-base.h"
#include "ns3/buffer.h"
#include "ns3/expression.h"
#include "ns3/log.h"
#include "type-ids.h"

#define V_InstanceOf(ptr, type) \
  (PeekPointer (DynamicCast<type, Value> (ptr)) != NULL)

using namespace std;

namespace ns3 {
namespace rapidnet {


/**
 * \ingroup rapidnet_library
 * \defgroup rapidnet_values RapidNet Value Types
 */

/**
 * \ingroup rapidnet_values
 *
 * \brief The base for all RapidNet value types.
 *
 * All RapidNet value types must extend Value and implement all the
 * abstract methods.
 */
class Value : public RefCountBase
{
public:

  /**
   * \brief Number of bytes occupied by the type id when serialized
   *         into a buffer.
   */
  static const uint32_t SIZE_TYPEID;

  /**
   * \brief Value constructor that initializes the type of the value
   *         should be a member of the @see ValueTypeId enum.
   */
  Value (ValueTypeId value);

  virtual ~Value ();

  /**
   * \brief Returns the size in bytes when serialized.
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serializes the object for transmitting over the
   *        network inside a RapidNet header.
   */
  virtual void Serialize (Buffer::Iterator& start) const;

  /**
   * \brief De-serializes the object that is embedded in a
   *         RapidNet header received from the network.
   */
  virtual uint32_t Deserialize (Buffer::Iterator& end) = 0;

  /**
   * \brief Returns the type of the value object. See @see ValueTypeId.
   */
  inline ValueTypeId GetType () const;

  /**
   * \brief Returns the type name as a string.
   */
  inline string GetTypeName () const;

  /**
   * \brief Returns the string representation of the value object.
   */
  virtual string ToString () const = 0;

  /**
   * \brief Create and return a deep copy or clone of this object.
   */
  virtual Ptr<Value> Clone () const = 0;

  /**
   * \brief To compare any two value objects.
   */
  virtual bool Equals (const Ptr<Value> v) const = 0;

  /**
   * \brief This method is used by the hash function of std::map when the
   *         value objects are inserted as keys.
   */
  virtual bool Less (const Ptr<Value> v) const = 0;

  /**
   * \brief Compares the given two value objects by invoking the @see Less()
   *         method on them.
   */
  static bool Less (const Ptr<Value> v1, const Ptr<Value> v2);

  /**
   * \brief Pass argument expression and tuple instead of argument value
   *         to allow lazy evaluation in case of short circuit operations.
   */
  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL)) = 0;

  /**
   * \brief Returns a default instance of given type. Used for
   *         de-serialization.
   */
  static Ptr<Value> GetInstanceOfType (ValueTypeId type);

protected:

  void AssertUnknownExpression (Operator op, Ptr<Value> operand);

  void AssertUnhandledOperator (Operator op);

  ValueTypeId m_type;
};

ostream& operator << (ostream& os, const Ptr<Value>& value);

ValueTypeId
Value::GetType () const
{
  return m_type;
}

string
Value::GetTypeName () const
{
  return ns3::rapidnet::GetTypeName (m_type);
}

} // namespace rapidnet
} // namespace ns3

#endif // VALUE_H
