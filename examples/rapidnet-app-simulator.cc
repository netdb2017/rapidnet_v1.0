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
#define DEFAULT_BASE_IP "192.168.0.0"
#define DEFAULT_XPOS "0,0,25"
#define DEFAULT_YPOS "0,25,0"
#define DEFAULT_MOBILITY_MODEL MOBILITY_MODEL_CONSTANT_POSITION
#define DEFAULT_XBOUND 2000
#define DEFAULT_YBOUND 2000
#define DEFAULT_SPEED 10
#define DEFAULT_DISTANCE 100
#define DEFAULT_QUERYNUM 0

#define __OnOff(boolean) \
  (boolean? "On": "Off")

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::discovery;

NS_LOG_COMPONENT_DEFINE ("Main");

/**
 * This is a generic runner script to run various RapidNet applications.
 * It receives as arguments several parameters that define the experimental
 * conditions under which the application is run. For example: the number of
 * nodes, the duration of the experiment, arena size, mobility model and
 * parameters etc.
 *
 */

// Duration of the simulation in seconds.
double g_duration = DEFAULT_DURATION;

// Number of nodes.
uint32_t g_numNodes = DEFAULT_NUM_NODES;

// The physical layer to be used.
string g_phy = DEFAULT_PHY;

// The print stream for printing tuple.
string g_printStream = DEFAULT_PRINT_STREAM_NAME;

// The base IP address
Ipv4Address g_baseIp = DEFAULT_BASE_IP;

// Application helper name
string g_appName = DEFAULT_RAPIDNET_APP_HELPER_NAME;

// Comma separated list of x-axis positions of nodes at start
string g_xpos = DEFAULT_XPOS;

// Comma separated list of y-axis positions of nodes at start
string g_ypos = DEFAULT_YPOS;

// Mobility model
string g_mobilityModel = DEFAULT_MOBILITY_MODEL;

// Mobility model attribute bound
double g_xbound = DEFAULT_XBOUND;

// Mobility model attribute bound
double g_ybound = DEFAULT_YBOUND;

// Mobility model attribute speed
double g_speed_low = DEFAULT_SPEED;
double g_speed_up = DEFAULT_SPEED;

// Mobility model attribute distance
double g_distance = DEFAULT_DISTANCE;

// Number of DSR queries
int g_queryNum = DEFAULT_QUERYNUM;
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

// Period for printing, 0 => INFINITY or print only once
double g_printPeriod = 0;

// Maximum jitter in milliseconds.
int g_maxJitter = RapidNetApplicationBase::MAX_JITTER;

int main(int argc, char *argv[])
{
  NodeContainer nodes;
  NetDeviceContainer netDevices;
  ApplicationContainer apps;

  LogComponentEnable ("Main", LOG_LEVEL_INFO);
  LogComponentEnable ("ScriptUtils", LOG_LEVEL_INFO);

  CommandLine cmd;
  cmd.AddValue ("duration", "Duration of simulation in seconds (double)", g_duration);
  cmd.AddValue ("nodes", "Number of nodes (integer)", g_numNodes);
  cmd.AddValue ("phy", "The physical layer (wifi/csma)", g_phy);
  cmd.AddValue ("stream", "The print stream (cout/clog)", g_printStream);
  cmd.AddValue ("print-period", "Period for printing, 0 if only once", g_printPeriod);
  cmd.AddValue ("print-reln", "Names of relations to be printed (comma separated list of strings)", g_printReln);
  cmd.AddValue ("baseIp", "Base IP address", g_baseIp);
  cmd.AddValue ("app", "RapidNet Application", g_appName);
  cmd.AddValue ("log-discovery", "Set discovery application logging (0=Off/1=On)", g_logDiscovery);
  cmd.AddValue ("log-app", "Set application logging (0=Off/1=On)", g_logApp);
  cmd.AddValue ("pcap", "Set pcap tracing (0=Off/1=On)", g_tracePcap);
  cmd.AddValue ("xpos", "X-axis positions of the nodes (comma separated list of double values)", g_xpos);
  cmd.AddValue ("ypos", "Y-axis positions of the nodes (comma separated list of double values)", g_ypos);
  cmd.AddValue ("mm", "Mobility model", g_mobilityModel);
  cmd.AddValue ("xbound", "X-axis bound for nodes to move", g_xbound);
  cmd.AddValue ("ybound", "Y-axis bound for nodes to move", g_ybound);
  cmd.AddValue ("speed_low", "Mobility model speed attribute lower bound", g_speed_low);
  cmd.AddValue ("speed_up", "Mobility model speed attribute upper bound", g_speed_up);
  cmd.AddValue ("distance", "Mobility model distance attribute", g_distance);
  cmd.AddValue ("queryNum", "Number of DSR/Epidemic queries", g_queryNum);
  cmd.AddValue ("maxJitter", "Maximum value of jitter (in milliseconds)", g_maxJitter);
  cmd.AddValue ("dir", "Directory where the simulation results are dumped", g_dir);
  cmd.Parse(argc, argv);

  NS_LOG_INFO ("Duration                             : " << g_duration << " sec");
  NS_LOG_INFO ("Number of nodes                      : " << g_numNodes);
  NS_LOG_INFO ("Physical layer                       : " << g_phy);
  NS_LOG_INFO ("Print stream                         : " << g_printStream);
  NS_LOG_INFO ("Print Relations                      : " << g_printReln);
  NS_LOG_INFO ("Print Period                         : " << g_printPeriod);
  NS_LOG_INFO ("Base IP address                      : " << g_baseIp);
  NS_LOG_INFO ("RapidNet Application                 : " << g_appName);
  NS_LOG_INFO ("Discovery application logging        : " << __OnOff (g_logDiscovery));
  NS_LOG_INFO ("Application logging                  : " << __OnOff (g_logApp));
  NS_LOG_INFO ("Pcap tracing                         : " << __OnOff (g_tracePcap));
  NS_LOG_INFO ("Mobility model                       : " << g_mobilityModel);
  NS_LOG_INFO ("Mobility model bounds                : Rectangle (0,0) to (" << g_xbound << ", " << g_ybound << ")");
  NS_LOG_INFO ("Mobility model speed attribute (low) : " << g_speed_low);
  NS_LOG_INFO ("Mobility model speed attribute (up)  : " << g_speed_up);
  NS_LOG_INFO ("Mobility model distance attribute    : " << g_distance);
  NS_LOG_INFO ("Maximum jitter (milliseconds)        : " << g_maxJitter);
  NS_LOG_INFO ("Number of DSR/Epidemic queries       : " << g_queryNum);


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

  // Create directories if they do not exist
  system (("mkdir " + g_dir).c_str ());
  system (("mkdir " + g_dir + "/pcaps/").c_str ());

  // Create the nodes
  nodes.Create(g_numNodes);

  // Install physical layer
  string pcapFilename = g_dir + "/pcaps/" + g_appName;
  netDevices = g_phy == WIFI ? InstallWifi (nodes, g_tracePcap, pcapFilename)
    : InstallCsma (nodes, g_tracePcap, pcapFilename);

  // Install IPv4 stack
  InstallIpv4 (nodes, netDevices, g_baseIp);

  if (g_tracePcap)
    {
      CsmaHelper::EnablePcapAll (pcapFilename, true);
    }

  // Position the nodes and set mobility model and its attributes
  InstallMobility (nodes, g_xpos, g_ypos, g_xbound, g_ybound, g_mobilityModel,
    RandomVariableValue (UniformVariable(g_speed_low, g_speed_up)),
    DoubleValue (g_distance));

  // Install RapidNet Application
  apps = GetRapidNetAppHelper (g_appName)->Install (nodes);

  // Set maximum value of jitter
  SetMaxJitter (apps, g_maxJitter);

  // Install Decorator for visualization
  ofstream* decorator_out = InstallDecorator (apps, g_dir, g_xbound, g_ybound);

  // Install application specific decorator triggers
  InstallDecoratorTriggers (g_appName, apps);

  // Schedule application specific network events
  ScheduleEvents (g_appName, apps, g_queryNum, g_duration);

  // Initialize and start all applications
  StartApps (g_appName, apps);

  apps.Stop (Seconds (g_duration));

  if (g_printPeriod == 0)
    {
      g_printPeriod = g_duration;
    }

  // Schedule PrintRelations
  Simulator::Schedule (Seconds (0), &PrintRelations, apps, g_printReln, g_printStream,
    g_printPeriod, (int) g_duration / g_printPeriod + 1);

  // Schedule simulation duration
  Simulator::Stop (Seconds (g_duration));

  // Run simulator
  Simulator::Run ();
  Simulator::Destroy ();

  decorator_out->close();
  delete decorator_out;
}
