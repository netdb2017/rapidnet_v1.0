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
#include "ns3/pktfwd-prov-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"

#define link(src, next, cost) \
  tuple (PktfwdProv::LINK, \
    attr ("link_attr1", Ipv4Value, src), \
    attr ("link_attr2", Ipv4Value, next), \
    attr ("link_attr3", Int32Value, cost))

#define status(node) \
  tuple (PktfwdProv::STATUS, \
    attr ("status_attr1", Ipv4Value, node))

#define table(src, index, dest) \
  tuple (PktfwdProv::TABLE, \
    attr ("table_attr1", Ipv4Value, src), \
    attr ("table_attr2", Int32Value, index), \
    attr ("table_attr3", Ipv4Value, dest))

#define insertlink(from, to, cost) \
  app(from)->Insert (link (addr (from), addr (to), cost)); \
  app(to)->Insert (link (addr (to), addr (from), cost));

#define deletelink(from, to, cost) \
  app(from)->Delete (link (addr (from), addr (to), cost)); \
  app(to)->Delete (link (addr (to), addr (from), cost));

#define inserttable(src, index, dest) \
  app(src)->Insert (table (addr (src), index, addr (dest)));

#define insertstatus(node) \
  app(node)->Insert (status (addr (node)));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdprov;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, PktfwdProv::PATH);
  PrintRelation (apps, PktfwdProv::BESTPATH);
  PrintRelation (apps, PktfwdProv::PROV);
  PrintRelation (apps, PktfwdProv::RULEEXEC);
}

void
UpdateLinks1 ()
{
  insertlink (1, 2, 1);
  insertlink (2, 3, 2);
  insertlink (3, 4, 3);
  insertlink (4, 5, 4);
  insertlink (5, 1, 5);
}

void
UpdateTable ()
{
  srand(37);
  for (int i = 1; i<=5; i++) 
    for (int j = 0; j<40; j++) {
      int dest = rand()%5 + 1;
      inserttable (i, j, dest);
    }
}

void
UpdateStatus ()
{
  for (int i = 1; i<=5; i++)
    insertstatus (i);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdProv", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<PktfwdProvHelper> ());
  SetMaxJitter (apps, 0.001);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (6.0));

  schedule (0.0001, UpdateLinks1);
  schedule (2.0000, Print);
  schedule (3.0000, UpdateTable);
  schedule (4.0000, UpdateStatus);
  schedule (6.0000, Print);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

