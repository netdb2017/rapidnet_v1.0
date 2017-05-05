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
#include "ns3/reachable-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define link(src, next, cost) \
  tuple (Reachable::LINK, \
    attr ("link_attr1", Ipv4Value, src), \
    attr ("link_attr2", Ipv4Value, next), \
    attr ("link_attr3", Int32Value, cost))

#define insertlink(from, to, cost) \
  app(from)->Insert (link (addr (from), addr (to), cost)); \
  app(to)->Insert (link (addr (to), addr (from), cost));

#define deletelink(from, to, cost) \
  app(from)->Delete (link (addr (from), addr (to), cost)); \
  app(to)->Delete (link (addr (to), addr (from), cost));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::reachable;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, Reachable::PATH);
}

void
UpdateLinks1 ()
{
  insertlink (1, 2, 7);
  insertlink (2, 3, 4)
  insertlink (3, 4, 2);
  insertlink (4, 5, 6);
}

void
UpdateLinks2 ()
{
  deletelink (2, 3, 4);
}

void
UpdateLinks3 ()
{
  insertlink (2, 3, 10);
  insertlink (1, 5, 3);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Reachable", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<ReachableHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (10.0));

  schedule (0.0001, UpdateLinks1);
  schedule (2.0000, Print);
  schedule (2.0001, UpdateLinks2);
  schedule (4.0000, Print);
  schedule (4.0001, UpdateLinks3);
  schedule (6.0000, Print);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

