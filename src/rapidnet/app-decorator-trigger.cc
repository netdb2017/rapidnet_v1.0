/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */
#include <sstream>
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/rapidnet-types.h"
#include "app-decorator-trigger.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

const string AppDecoratorTrigger::UNSPECIFIED = "unspecified";

void
AppDecoratorTrigger::SetApplication (Ptr<RapidNetApplicationBase> app)
{
  NS_ASSERT (PeekPointer (app->GetDecoratorFrontend()) != 0);
  AppTrigger::SetApplication (app);
  m_node = app->GetNode ();
  m_decorator = app->GetDecoratorFrontend();
}

Ptr<AppDecoratorTrigger>
AppNodeStateTrigger::New(Ptr<RapidNetApplicationBase> app, string name,
  string state, bool add)
{
  Ptr<AppNodeStateTrigger> retval = Create<AppNodeStateTrigger> ();
  retval->SetApplication (app);
  retval->m_name = name;
  retval->m_state = state;
  retval->m_add = add;
  return Ptr<AppDecoratorTrigger> (dynamic_cast <AppDecoratorTrigger*>
    (PeekPointer (retval)));
}

void
AppNodeStateTrigger::Invoke(Ptr<Tuple> tuple)
{
  if (tuple->GetName() != m_name)
    {
      return;
    }
  if (m_add)
    {
      m_decorator->AddNodeState (m_node, m_state);
    }
  else
    {
      m_decorator->RemoveNodeState (m_node, m_state);
    }
}

Ptr<AppDecoratorTrigger>
AppLinkStateTrigger::New(Ptr<RapidNetApplicationBase> app, string name,
  string state, bool add, string targetAttr)
{
  Ptr<AppLinkStateTrigger> retval = Create<AppLinkStateTrigger> ();
  retval->SetApplication (app);
  retval->m_name = name;
  retval->m_state = state;
  retval->m_add = add;
  retval->m_targetAttr = targetAttr;
  return Ptr<AppDecoratorTrigger> (dynamic_cast <AppDecoratorTrigger*>
    (PeekPointer (retval)));
}

void
AppLinkStateTrigger::Invoke(Ptr<Tuple> tuple)
{
  if (tuple->GetName() != m_name)
    {
      return;
    }
  Ipv4Address target = ipv4_value (tuple->GetAttribute(
    m_targetAttr));

  if (m_add)
    {
      m_decorator->AddLinkState (m_node, target, m_state);
    }
  else
    {
      m_decorator->RemoveLinkState (m_node, target, m_state);
    }
}

Ptr<AppDecoratorTrigger>
AppTupleStateTrigger::New(Ptr<RapidNetApplicationBase> app, string name,
  bool add, list<string> attrs)
{
  Ptr<AppTupleStateTrigger> retval = Create<AppTupleStateTrigger> ();
  retval->SetApplication (app);
  retval->m_name = name;
  retval->m_state = name;
  retval->m_add = add;
  retval->m_attrs = attrs;
  return Ptr<AppDecoratorTrigger> (dynamic_cast <AppDecoratorTrigger*>
    (PeekPointer (retval)));
}

void
AppTupleStateTrigger::Invoke(Ptr<Tuple> tuple)
{
  if (tuple->GetName() != m_name)
    {
      return;
    }
  if (m_add)
    {
      m_decorator->AddTupleState (m_node, tuple, m_attrs);
    }
  else
    {
      m_decorator->RemoveTupleState (m_node, tuple, m_attrs);
    }
}

} // namespace rapidnet
} // namespace ns3
