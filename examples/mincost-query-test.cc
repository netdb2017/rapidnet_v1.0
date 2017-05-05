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
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/mincost-prov-module.h"
#include "ns3/mincost-query-module.h"
#include <time.h>



#define link(src, next, cost) \
tuple (MincostProv::LINK, \
attr ("link_attr1", Ipv4Value, src), \
attr ("link_attr2", Ipv4Value, next), \
attr ("link_attr3", Int32Value, cost))

#define insertlink(from, to, cost) \
app(from)->Insert (link (addr (from), addr (to), cost)); \
app(to)->Insert (link (addr (to), addr (from), cost));

#define deletelink(from, to, cost) \
app(from)->Delete (link (addr (from), addr (to), cost)); \
app(to)->Delete (link (addr (to), addr (from), cost));

//define the tuple you would like to query and how to insert it
#define tupleQuery(loc, name, attr1, attr2, attr3)         \
tuple (MincostQuery::TUPLE, \
attr ("tuple_attr1", Ipv4Value, loc), \
attr ("tuple_attr2", StrValue, name), \
attr ("tuple_attr3", Ipv4Value, attr1), \
attr ("tuple_attr4", Ipv4Value, attr2), \
attr ("tuple_attr5", Int32Value, attr3))

#define inserttuple(loc, name, attr1, attr2, attr3)     \
queryNode->Insert (tupleQuery(queryNode->GetAddress(), name, addr(attr1), addr(attr2), attr3)); 


using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::mincostprov;
using namespace ns3::rapidnet::mincostquery;

ApplicationContainer apps;
ApplicationContainer queryapps;

void 
initApps()
{
	NodeContainer mainAppNodes;
	mainAppNodes.Create (3);

	NodeContainer queryAppNodes;
	queryAppNodes.Create (1);

  NodeContainer csmaNodes;
  csmaNodes.Add(mainAppNodes);
  csmaNodes.Add(queryAppNodes);

  CsmaHelper csma;

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;
  Ipv4Address base = "10.1.1.0";

  address.SetBase (base, "255.255.255.0");
  address.Assign (csmaDevices);

  apps.Add(Create<MincostProvHelper>()->Install(mainAppNodes));
  queryapps.Add(Create<MincostQueryHelper>()->Install(queryAppNodes));

  SetMaxJitter (apps, 0.001);
  SetMaxJitter (queryapps, 0.001);
}

void
UpdateLinks1 ()
{
  insertlink (1, 2, 3);
  insertlink (2, 3, 2);
  insertlink (1, 3, 5);
}


void
TupleToQuery ()
{
  Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
  inserttuple(1, "bestPath", 3, 2, 2); 
  inserttuple(1, "path", 1, 3, 5); 
  inserttuple(1, "link", 1, 2, 3); 
}

void
Print ()
{
  PrintRelation (apps, MincostProv::BESTPATH);
  PrintRelation (apps, MincostProv::PROV);
  PrintRelation (apps, MincostProv::RULEEXEC);

  PrintRelation (queryapps, MincostQuery::TUPLE);
  PrintRelation (queryapps, MincostQuery::RECORDS);
}


int
main (int argc, char *argv[])
{
  LogComponentEnable("MincostProv", LOG_LEVEL_INFO);
  LogComponentEnable("MincostQuery", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  initApps();

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (5.0));
  queryapps.Start (Seconds (0.0));
  queryapps.Stop (Seconds (5.0));

  schedule (1.0, TupleToQuery);
  schedule (2.0, UpdateLinks1);
  schedule (5.0, Print);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

