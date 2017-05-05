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
#include "ns3/secure-pathvector-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define link(src, next, cost) \
  tuple (SecurePathvector::LINK, \
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
using namespace ns3::rapidnet::securepathvector;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, SecurePathvector::PATH);
  PrintRelation (apps, SecurePathvector::BESTPATH);
}


// Create a chain topology
void
UpdateLinks1 ()
{
  insertlink (1, 2, 2);
  insertlink (1, 3, 3);
  insertlink (1, 4, 5);
  insertlink (2, 3, 6);
  insertlink (3, 4, 2);
  insertlink (4, 5, 10);
}

void
UpdateLinks2 ()
{
  deletelink (1, 2, 2);
}

void
UpdateLinks3 ()
{
  insertlink (1, 2, 3);
  insertlink (2, 5, 1);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("SecurePathvector", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<SecurePathvectorHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (40.0));

  Simulator::Schedule (Seconds(0.0001), &UpdateLinks1);
  Simulator::Schedule (Seconds(10.0000), &Print);
  Simulator::Schedule (Seconds(10.0001), &UpdateLinks2);
  Simulator::Schedule (Seconds(20.0000), &Print);
  Simulator::Schedule (Seconds(20.0001), &UpdateLinks3);
  Simulator::Schedule (Seconds(40.0000), &Print);

  SetMaxJitter (apps, 30);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
