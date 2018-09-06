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
#ifndef APP_DECORATOR_TRIGGERS_H
#define APP_DECORATOR_TRIGGERS_H

#include <string>
#include <map>
#include <list>
#include "ns3/application-container.h"
#include "tuple.h"
#include "trigger.h"
#include "rapidnet-application-base.h"
#include "rapidnet-decorator-frontend.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief A generic trigger defined for emitting decorator-
 * frontend events for visualization.
 *
 * This trigger is attached to the OnRecv, OnSend, OnInsert
 * or OnDelete trigger list of the application. It can be extended
 * to define application specific triggers.
 */
class AppDecoratorTrigger: public AppTrigger
{
public:

  const static string UNSPECIFIED;

  /**
   * \brief Links to a RapidNet application.
   */
  virtual void SetApplication (Ptr<RapidNetApplicationBase> app);

  virtual void Invoke (Ptr<Tuple> tuple) = 0;

  virtual ~AppDecoratorTrigger () { }

protected:

  string m_name;

  string m_state;

  bool m_add;

  Ptr<RapidNetDecoratorFrontend> m_decorator;

  Ptr<Node> m_node;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief A trigger to record a node specific event.
 */
class AppNodeStateTrigger: public AppDecoratorTrigger
{
public:

  /**
   * \brief Factory method to create a new object.
   *
   * @param app Associated RapidNet application.
   * @param name Name of the tuple that triggers the event.
   * @param state Name of the node state that is recorded.
   * @param add If true, then the node state is added, removed otherwise.
   */
  static Ptr<AppDecoratorTrigger> New (Ptr<RapidNetApplicationBase> app,
    string name, string state, bool add);

  virtual void Invoke (Ptr<Tuple> tuple);

  virtual ~AppNodeStateTrigger () { }
};

/**
 * \ingroup rapidnet_library
 *
 * \brief A trigger to record a link specific event.
 */
class AppLinkStateTrigger: public AppDecoratorTrigger
{
public:

  /**
   * \brief Factory method to create a new object.
   *
   * @param app Associated RapidNet application.
   * @param name Name of the tuple that triggers the event.
   * @param state Name of the link state that is recorded.
   * @param add If true, then the link state is added, removed otherwise.
   * @param targetAttr The name of the attribute the value of which is used
   *                   to determine the other node on the tuple. The value
   *                   should be of Ipv4Value type.
   */
  static Ptr<AppDecoratorTrigger> New (Ptr<RapidNetApplicationBase> app,
    string name, string state, bool add, string targetAttr);

  virtual void Invoke (Ptr<Tuple> tuple);

  virtual ~AppLinkStateTrigger () { }

protected:

  string m_targetAttr;
};

/**
 * \ingroup rapidnet_library
 *
 * \brief A trigger to record a tuple event at a node.
 */
class AppTupleStateTrigger: public AppDecoratorTrigger
{
public:

  /**
   * \brief Factory method to create a new object.
   *
   * @param app Associated RapidNet application.
   * @param name Name of the tuple that triggers the event.
   * @param add If true, then the link state is added, removed otherwise.
   * @param attrs The names of the attributes in the tuple that are to be
   *              recorded.
   */
  static Ptr<AppDecoratorTrigger> New (Ptr<RapidNetApplicationBase> app,
    string name, bool add, list<string> attrs);

  virtual void Invoke (Ptr<Tuple> tuple);

  virtual ~AppTupleStateTrigger () { }

protected:

  list<string> m_attrs;
};

} // namespace rapidnet
} // namespace ns3

#endif // APP_DECORATOR_TRIGGERS_H
