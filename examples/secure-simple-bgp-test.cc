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

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/secure-simple-bgp-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define neighbor(src, next) \
  tuple(SecureSimpleBgp::NEIGHBOR,\
    attr ("neighbor_attr1", Ipv4Value, src), \
    attr ("neighbor_attr2", Ipv4Value, next))

#define insertneighbor(from, to) \
  app(from)->Insert (neighbor (addr (from), addr (to))); \
  app(to)->Insert (neighbor (addr (to), addr (from)));

#define deleteneighbor(from, to) \
  app(from)->Delete (neighbor (addr (from), addr (to))); \
  app(to)->Delete (neighbor (addr (to), addr (from)));


#define acceptRoute(src, via, to) \
  tuple(SecureSimpleBgp::ACCEPTROUTE,\
    attr ("acceptRoute_attr1", Ipv4Value, src), \
    attr ("acceptRoute_attr2", Ipv4Value, via), \
    attr ("acceptRoute_attr3", Ipv4Value, to))

#define insertacceptRoute(src, via, to) \
  app(src)->Insert (acceptRoute (addr (src), addr(via), addr (to)));

#define carryTraffic(src, from, to) \
  tuple(SecureSimpleBgp::CARRYTRAFFIC,\
    attr ("carryTraffic_attr1", Ipv4Value, src), \
    attr ("carryTraffic_attr2", Ipv4Value, from), \
    attr ("carryTraffic_attr3", Ipv4Value, to))

#define insertcarryTraffic(src, from, to) \
  app(src)->Insert (carryTraffic (addr (src), addr(from), addr (to))); \
  app(src)->Insert (carryTraffic (addr (src), addr(to), addr (from)));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::securesimplebgp;

ApplicationContainer apps;


void
UpdateNeighbors1 ()
{
  insertneighbor (1, 2);
  insertneighbor (1, 3);
  insertneighbor (2, 3);
  insertneighbor (3, 4);
  insertneighbor (2, 4);
}

void
UpdateCarryTraffic1 ()
{
  insertcarryTraffic (2, 1, 4);
  insertcarryTraffic (2, 3, 4);
  insertcarryTraffic (2, 2, 3);
  insertcarryTraffic (1, 1, 3);
  insertcarryTraffic (3, 1, 3);
  insertcarryTraffic (3, 2, 3);
}

void
UpdateAcceptRoute1 ()
{
  insertacceptRoute (1, 2, 3);
  insertacceptRoute (1, 2, 4);
  insertacceptRoute (1, 3, 4);
  insertacceptRoute (3, 2, 4);
  insertacceptRoute (3, 2, 1);
  insertacceptRoute (4, 2, 1);
  insertacceptRoute (4, 3, 1);
  insertacceptRoute (4, 2, 3);
}

void Print ()
{
  PrintRelation (apps, SecureSimpleBgp::ROUTE);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("SecureSimpleBgp", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (4, Create<SecureSimpleBgpHelper> ());

  //  SetMaxJitter (apps, 0);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (1.5));

  schedule (0.0, UpdateNeighbors1);
  schedule (0.0, UpdateAcceptRoute1);
  schedule (0.0, UpdateCarryTraffic1);
  schedule (10.0, Print);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


