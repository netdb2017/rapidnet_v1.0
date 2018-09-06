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

#include "selector.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/rapidnet-types.h"

NS_LOG_COMPONENT_DEFINE ("Selector");

using namespace ns3;
using namespace rapidnet;

Selector::Selector ()
{
}

Selector::~Selector ()
{
}

bool
Selector::Select (Ptr<Tuple> tuple)
{
  if (tuple->IsEmpty ())
    {
      NS_LOG_INFO ("P2Selector::Select : Skipping empty tuple");
      return false;
    }

  Ptr<Value> result = m_expr->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (result, BoolValue),
    "Selector expression must evaluate to a boolean value");
  return rn_bool (result);
}

Ptr<Selector>
Selector::New (Ptr<Expression> expr)
{
  Ptr<Selector> selector = Create<Selector> ();
  selector->m_expr = expr;
  return selector;
}
