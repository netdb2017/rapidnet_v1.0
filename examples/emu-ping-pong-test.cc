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

#define DEFAULT_DURATION 100

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pingpong;

ApplicationContainer apps;
string myaddr, dstaddr;
double duration = DEFAULT_DURATION;

void
UpdateLinks1 ()
{
  Ptr<RapidNetApplicationBase> app = apps.Get(0)->GetObject<RapidNetApplicationBase>();
  app->Insert (tlink (Ipv4Address (myaddr.c_str()), Ipv4Address (dstaddr.c_str())));
}

NS_LOG_COMPONENT_DEFINE ("EmulationPingPongExample");

int
main (int argc, char *argv[])
{
  LogComponentEnable("Pingpong", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  ns3::Mac48Address macAddress;
  std::string deviceName;
  std::string IPnet;
  std::string IPmask;
  std::string IPbase;

  CommandLine cmd;
  cmd.AddValue("duration", "Duration of simulation in seconds (double)", duration);
  cmd.AddValue("deviceName", "device name", deviceName);
  cmd.AddValue("macAddress", "mac address", macAddress);
  cmd.AddValue("IPnet", "IP network", IPnet);
  cmd.AddValue("IPmask", "IP mask", IPmask);
  cmd.AddValue("IPbase", "IP base", IPbase);
  cmd.AddValue("myaddr", "my IP address", myaddr);
  cmd.AddValue("dstaddr", "dst IP address", dstaddr);
  cmd.Parse (argc, argv);

  GlobalValue::Bind ("SimulatorImplementationType",
  StringValue ("ns3::RealtimeSimulatorImpl"));

  NodeContainer n;
  n.Create (1);

  InternetStackHelper internet;
  internet.Install (n);

  EmuHelper emu;
  emu.SetAttribute ("DeviceName", StringValue (deviceName));
  NetDeviceContainer d = emu.Install (n);

  Ptr<NetDevice> nd = d.Get (0);
  Ptr<EmuNetDevice> ed = nd->GetObject<EmuNetDevice> ();
  ed->SetAddress (Mac48Address(macAddress));

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase (Ipv4Address (IPnet.c_str()), Ipv4Mask (IPmask.c_str()),
    Ipv4Address (IPbase.c_str()));
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  PingpongHelper app;

  apps = app.Install(n);
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (duration));

  Simulator::Schedule (Seconds (0.0), UpdateLinks1);

  string  pacpname="emu-ping-pong-"+myaddr;
  EmuHelper::EnablePcapAll (pacpname, true);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

