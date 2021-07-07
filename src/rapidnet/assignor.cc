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

#include "assignor.h"

using namespace ns3;
using namespace rapidnet;

Assignor::Assignor ()
{
}

Assignor::~Assignor ()
{
}

void
Assignor::Assign (Ptr<Tuple> tuple)
{
  tuple->OverwriteAttribute (TupleAttribute::New (
    m_target, m_expr->Eval (tuple)));
}

Ptr<Assignor>
Assignor::New (string target, Ptr<Expression> expr)
{
  Ptr<Assignor> assignor = Create<Assignor> ();
  assignor->m_target = target;
  assignor->m_expr = expr;
  return assignor;
}
