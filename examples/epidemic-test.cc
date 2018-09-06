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
#include "ns3/helper-module.h"
#include "ns3/epidemic-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/rapidnet-script-utils.h"

#define eLinkAdd(src, next, cost) \
  tuple (Epidemic::ELINKADD, \
    attr ("eLinkAdd_attr1", Ipv4Value, src), \
    attr ("eLinkAdd_attr2", Ipv4Value, next), \
    attr ("eLinkAdd_attr3", Int32Value, cost))

#define insertlink(from, to, cost) \
  app(from)->Inject (eLinkAdd (addr (from), addr (to), cost)); \
  app(to)->Inject (eLinkAdd (addr (to), addr (from), cost));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::epidemic;

ApplicationContainer apps;

// dynamic topology
void
UpdateLink1()
{
  insertlink(1, 2, 1);
  insertlink(1, 3, 1);
  insertlink(4, 5, 1);
}

void
UpdateLink2()
{
  insertlink(3, 5, 1);
}

void
UpdateLink3()
{
  insertlink(4, 6, 1);
}

void
InjectMessage1 ()
{
  app(1)->Inject (tuple (Epidemic::EMESSAGEINJECTORIGINAL,
    attr ("eMessageInjectOriginal_attr1", Ipv4Value, Ipv4Address ("192.168.0.1")),
    attr ("eMessageInjectOriginal_attr2", Ipv4Value, Ipv4Address ("192.168.0.6"))));
}

void
InjectMessage2 ()
{
  app(2)->Inject (tuple (Epidemic::EMESSAGEINJECTORIGINAL,
    attr ("eMessageInjectOriginal_attr1", Ipv4Value, Ipv4Address ("192.168.0.2")),
    attr ("eMessageInjectOriginal_attr2", Ipv4Value, Ipv4Address ("192.168.0.6"))));
}


void
PrintTables ()
{
  PrintRelation(apps, Epidemic::TMESSAGE, std::clog);
  PrintRelation(apps, Epidemic::TSUMMARYVEC, std::clog);
}

int
main (int argc, char *argv[])
{
  srand((unsigned)time(0));   // seed should be set in main function

  LogComponentEnable("Epidemic", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  double TOTAL_TIME = 15.0;
  apps = InitRapidNetApps (6, Create<EpidemicHelper> (), "192.168.0.0");
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (TOTAL_TIME));
  schedule (0.0, UpdateLink1);
  schedule (1.0, InjectMessage1);
  schedule (1.1, InjectMessage2);
  schedule (5.0, UpdateLink2);
  schedule (10.0, UpdateLink3);
  schedule (TOTAL_TIME, PrintTables);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
