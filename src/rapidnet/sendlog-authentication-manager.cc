/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2009 University of Pennsylvania
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
 */

#include "sendlog-authentication-manager.h"
#include "rapidnet-application-base.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

SendlogAuthenticationManager::SendlogAuthenticationManager ()
{
}

SendlogAuthenticationManager::~SendlogAuthenticationManager ()
{
}

void SendlogAuthenticationManager::SetPrincipal (
  Ptr<RapidNetApplicationBase> principal)
{
  m_principal = principal;
}
