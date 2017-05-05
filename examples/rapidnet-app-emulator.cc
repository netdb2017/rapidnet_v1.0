/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Pennsylvania
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
 */
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "ns3/core-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/simulator-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "rapidnet-applications.h"
#define WIFI "wifi"
#define CSMA "csma"
#define LINKSTATE "Linkstate"
#define HSLS "Hsls"
#define LINKSTATEFILTERED "Linkstatefiltered"
#define HSLSFILTERED "Hslsfiltered"

#define DEFAULT_DURATION 100
#define DEFAULT_NUM_NODES 3
#define DEFAULT_PHY CSMA
#define DEFAULT_PRINT_STREAM_NAME CLOG
#define DEFAULT_PRINT_STREAM clog
#define DEFAULT_XPOS "0,0,25"
#define DEFAULT_YPOS "0,25,0"
#define DEFAULT_MOBILITY_MODEL MOBILITY_MODEL_CONSTANT_POSITION
#define DEFAULT_XBOUND 2000
#define DEFAULT_YBOUND 2000
#define DEFAULT_SPEED 10
#define DEFAULT_DISTANCE 100
#define DEFAULT_QUERYNUM 0

#define DEFAULT_MAC_ADDRESS "00:00:00:00:00:00"
#define DEFAULT_DEVICE_NAME "eth1"
#define DEFAULT_NETWORK_IP "192.168.0.0"
#define DEFAULT_BASE_IP "0.0.1.1"
#define DEFAULT_IP_MASK "255.255.0.0"
#define DEFAULT_CURRENT_NODE_IP "192.168.1.1"

#define __Bounds(xbound, ybound) \
  RectangleValue (Rectangle (Rectangle (0, xbound, 0, ybound)))

#define __OnOff(boolean) \
  (boolean? "On": "Off")

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::discovery;

NS_LOG_COMPONENT_DEFINE ("Main");

// Duration of the simulation in seconds.
double g_duration = DEFAULT_DURATION;

// The print stream for printing tuple.
string g_printStream = DEFAULT_PRINT_STREAM_NAME;

// P2 Application helper name
string g_appName = DEFAULT_RAPIDNET_APP_HELPER_NAME;

// Turn on/off discovery logging
bool g_logDiscovery = true;

// Turn on/off application logging
bool g_logApp = true;

// Turn on/off pcap traces
bool g_tracePcap = true;

// Relations to print
string g_printReln = "tLink,tLSU";

// Directory where to dump all the pcap files
string g_dir=".";

// Apptable file name
string g_apptableFile = "";

// Apptable file stream
ifstream g_apptableStream;

// Period for printing, 0 = INFINITY or print only once
double g_printPeriod = 0;

// Application start time in seconds.
double g_appStartTime = 0.0;

// Mobility model attribute bound
double g_xbound = DEFAULT_XBOUND;

// Mobility model attribute bound
double g_ybound = DEFAULT_YBOUND;

// Number of DSR queries
int g_queryNum = DEFAULT_QUERYNUM;


// Maximum jitter in milliseconds.
int g_maxJitter = RapidNetApplicationBase::MAX_JITTER;

/* Emulation parameters */

// MAC Address
Mac48Address g_mac (DEFAULT_MAC_ADDRESS);

// Device name
string g_device (DEFAULT_DEVICE_NAME);

// Network IP
Ipv4Address g_ipnet (DEFAULT_NETWORK_IP);

// Base IP
Ipv4Address g_ipbase (DEFAULT_BASE_IP);

// IP Mask
Ipv4Mask g_ipmask (DEFAULT_IP_MASK);

// IP address
Ipv4Address g_ipaddr (DEFAULT_CURRENT_NODE_IP);

NetDeviceContainer
InstallEmuNetDevices (NodeContainer nodes, string deviceName, Mac48Address macAddress)
{
  EmuHelper emu;
  emu.SetAttribute ("DeviceName", StringValue (deviceName));
  NetDeviceContainer devices = emu.Install (nodes);

  Ptr<NetDevice> netDevice = devices.Get (0);
  Ptr<EmuNetDevice> emuNetDevice = netDevice->GetObject<EmuNetDevice> ();
  emuNetDevice->SetAddress (macAddress);

  return devices;
}

int
main(int argc, char *argv[])
{
  NodeContainer nodes;
  NetDeviceContainer netDevices;
  ApplicationContainer apps;

  LogComponentEnable("Main", LOG_LEVEL_INFO);
  LogComponentEnable("ScriptUtils", LOG_LEVEL_INFO);

  CommandLine cmd;
  cmd.AddValue ("duration", "Duration of simulation in seconds (double)", g_duration);
  cmd.AddValue ("stream", "The print stream (cout/clog)", g_printStream);
  cmd.AddValue ("print-reln", "Names of relations to be printed (comma separated list of strings)", g_printReln);
  cmd.AddValue ("print-period", "Period for printing, 0 if only once", g_printPeriod);
  cmd.AddValue ("app", "RapidNet Application", g_appName);
  cmd.AddValue ("log-discovery", "Set discovery application logging (0=Off/1=On)", g_logDiscovery);
  cmd.AddValue ("log-app", "Set application logging (0=Off/1=On)", g_logApp);
  cmd.AddValue ("pcap", "Set pcap tracing (0=Off/1=On)", g_tracePcap);
  cmd.AddValue ("dir", "Directory where the simulation results are dumped", g_dir);
  cmd.AddValue ("topo", "The apptable file name", g_apptableFile);
  cmd.AddValue ("queryNum", "Number of DSR/Epidemic queries", g_queryNum);
  cmd.AddValue ("maxJitter", "Maximum value of jitter (in milliseconds)", g_maxJitter);
  cmd.AddValue ("start-time", "The application start time", g_appStartTime);

  /* Emulation parameters */
  cmd.AddValue("device", "Device name", g_device);
  cmd.AddValue("mac", "MAC address", g_mac);
  cmd.AddValue("ipnet", "Network IP address", g_ipnet);
  cmd.AddValue("ipbase", "Base IP address", g_ipbase);
  cmd.AddValue("ipmask", "IP Mask", g_ipmask);
  cmd.AddValue("ipaddr", "IP address", g_ipaddr);

  cmd.Parse(argc, argv);

  NS_LOG_INFO ("Duration                           : " << g_duration << " sec");
  NS_LOG_INFO ("Print stream                       : " << g_printStream);
  NS_LOG_INFO ("Print Relations                    : " << g_printReln);
  NS_LOG_INFO ("Print Period                       : " << g_printPeriod);
  NS_LOG_INFO ("RapidNet Application               : " << g_appName);
  NS_LOG_INFO ("Discovery application logging      : " << __OnOff (g_logDiscovery));
  NS_LOG_INFO ("Application logging                : " << __OnOff (g_logApp));
  NS_LOG_INFO ("Pcap tracing                       : " << __OnOff (g_tracePcap));
  NS_LOG_INFO ("Emulation directory                : " << g_dir);
  NS_LOG_INFO ("Using apptable file                : " << g_apptableFile);
  NS_LOG_INFO ("Application start time             : " << g_appStartTime);



  /* Emulation parameters */
  NS_LOG_INFO ("Device name                        : " << g_device);
  NS_LOG_INFO ("MAC address                        : " << g_mac);
  NS_LOG_INFO ("Network IP address                 : " << g_ipnet);
  NS_LOG_INFO ("Base IP address                    : " << g_ipbase);
  NS_LOG_INFO ("IP Mask                            : " << g_ipmask);
  NS_LOG_INFO ("IP address                         : " << g_ipaddr);
  NS_LOG_INFO ("Maximum jitter (milliseconds)      : " << g_maxJitter);
  NS_LOG_INFO ("Number of DSR/Epidemic queries     : " << g_queryNum);

  //Turn on the logs
  if (g_logDiscovery)
    {
      LogComponentEnable("Discovery", LOG_LEVEL_INFO);
    }
  if (g_logApp)
    {
      LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
      LogComponentEnable(g_appName.c_str(), LOG_LEVEL_INFO);
    }

  GlobalValue::Bind ("SimulatorImplementationType",
  StringValue ("ns3::RealtimeSimulatorImpl"));

  // Create directories if they do not exist

  system (("mkdir " + g_dir).c_str ());
  system (("mkdir " + g_dir + "/pcaps/").c_str ());

  // Create the nodes
  nodes.Create(1);

  // Install physical layer
  netDevices = InstallEmuNetDevices (nodes, g_device, g_mac);

  // Install IPv4 stack
  InstallIpv4 (nodes, netDevices, g_ipnet, g_ipmask, g_ipbase);

  // Install RapidNet Application
  apps = GetRapidNetAppHelper (g_appName)->Install (nodes);

  // Set maximum value of jitter
  SetMaxJitter (apps, g_maxJitter);

  // Import apptable for application-level filtering, if emulation
  NS_LOG_INFO ("Using apptable for emulation.");
  ifstream apptableStream (g_apptableFile.c_str());
  Simulator::Schedule(MilliSeconds (1), &ImportAppTable, apps, &apptableStream, 5.0, g_duration);

  // Turn on pcap traces
  if (g_tracePcap)
    {
      stringstream strstr;
      strstr << g_dir << "/pcaps/" << g_appName << "_" << g_ipaddr;
      string pcapfile = strstr.str ();
      EmuHelper::EnablePcapAll (pcapfile, true);
    }

  // Schedule the application, give some time for the
  // apptable tuples to be fully inserted
  apps.Start (Seconds (g_appStartTime));
  apps.Stop (Seconds (g_duration));

  // Initialize random seed
  srand ( time(NULL) );

  stringstream ipaddrstream;
  ipaddrstream << g_ipaddr;
  string ipaddr = ipaddrstream.str();
  ofstream* decorator_out = InstallDecoratorEmu (apps, g_dir, ipaddr);

  // Install application specific decorator triggers
  InstallDecoratorTriggers (g_appName, apps);

   // Schedule application specific network events
  ScheduleEvents (g_appName, apps, g_queryNum, g_duration);
   // Initialize and start all applications
//   StartApps (g_appName, apps);

//   apps.Stop (Seconds (g_duration));

   if (g_printPeriod == 0)
     {
       g_printPeriod = g_duration;
     }

  // Set simulation time
  Simulator::Schedule (Seconds (0.001), &PrintRelations, apps, g_printReln, g_printStream, g_printPeriod,
    (int) g_duration / g_printPeriod + 1);
  Simulator::Stop (Seconds(g_duration));

  // Run simulator
  Simulator::Run ();
  Simulator::Destroy ();
  decorator_out->close();
  delete decorator_out;
  apptableStream.close ();

  return 0;
}
