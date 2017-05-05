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
#include "ns3/pingpong-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define tlink(src, next) \
  tuple (Pingpong::TLINK, \
    attr ("tLink_attr1", Ipv4Value, src), \
    attr ("tLink_attr2", Ipv4Value, next))

#define insertlink(from, to) \
  app(from)->Insert (tlink (addr (from), addr (to))); \
  app(to)->Insert (tlink (addr (to), addr (from)));

#define deletelink(from, to) \
  app(from)->Delete (tlink (addr (from), addr (to))); \
  app(to)->Delete (tlink (addr (to), addr (from)));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pingpong;

ApplicationContainer apps;

/** Create a 2 nodes */
void
UpdateLinks1 ()
{
  insertlink (1, 2);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Pingpong", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (2, Create<PingpongHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (10.0));

  schedule (0.0001, UpdateLinks1);
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

