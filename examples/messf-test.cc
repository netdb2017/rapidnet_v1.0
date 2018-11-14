/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */


#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/messf-module.h"
#include <time.h>
#include <fstream>
#include <list>
#include <string>

#define source(node, content, score) \
ns3::rapidnet::tuple (Messf::SOURCE, \
attr ("source_attr1", Ipv4Value, node), \
attr ("source_attr2", StrValue, content), \
attr ("source_attr3", RealValue, score))

#define msg(node, content, score,ttl) \
ns3::rapidnet::tuple (Messf::MSG, \
attr ("msg_attr1", Ipv4Value, node), \
attr ("msg_attr2", StrValue, content), \
attr ("msg_attr3", RealValue, score), \
attr ("msg_attr4", Int32Value, ttl))

   
#define link(node1, node2,score) \
ns3::rapidnet::tuple (Messf::LINK, \
attr ("link_attr1", Ipv4Value, node1), \
attr ("link_attr2", Ipv4Value, node2), \
attr ("link_attr3", RealValue, score))

#define insertsource(node, content,score) \
app(node)->Insert (source(addr(node), content,score));


#define insertlink(node1, node2, score) \
app(node1)->Insert (link(addr(node1), addr(node2), score)); 



using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::messf;

ApplicationContainer apps;

void Print()
{
  //PrintRelation (apps, Messf::PROV);
  //PrintRelation (apps, Messf::RULEEXEC);
  PrintRelation (apps, Messf::LINK);
  
  

}

void
UpdateTables1 ()
{
  //insertsource(1,"Hello",1.0);
}

void 
UpdateTables2()
{
  /*insertsource(0,"Hello",1.0);
  insertlink(1,2,1.0);
  insertlink(1,3,1.0);
  insertlink(1,4,1.0);
  insertlink(2,5,1.0);
  insertlink(2,6,0.7);
  insertlink(3,7,0.8);
  insertlink(4,7,1.0);
  insertlink(7,6,0.8);
  insertlink(5,8,1.0);
  insertlink(6,8,0.5);*/
  ifstream myfile;
  myfile.open("examples/messf_tables/link.csv");
  string store;
  int node1=0;
  int node2=0;
  int lossy=10;
  float lossy1=1.0;
  insertlink(1,2,1.0);
  while(getline(myfile,store,',')){
    node1 = store[0]-'0';
    getline(myfile,store,',');
    node2 = store[0]-'0';
    getline(myfile,store,',');
    lossy = store[0]-'0';
    lossy1 = float(lossy)/10;
    insertlink(node1+1,node2+1,lossy1);
  }
  
  
  
}




int 
main (int argc, char *arhv[])
{
  //LogComponentEnable("Messf", LOG_LEVEL_INFO); 
  //LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO); 
  clock_t begintime = clock();
  apps = InitRapidNetApps (10, Create<MessfHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (2000.0));

  //schedule (1.0, UpdateTables1);
  schedule (30.0, UpdateTables2);
  

  schedule (1000.0,Print);

  Simulator::Run ();
  Simulator::Destroy ();
  clock_t t = clock() - begintime;
  cout<<((float)t)/CLOCKS_PER_SEC<<endl;  

  return 0;
}











