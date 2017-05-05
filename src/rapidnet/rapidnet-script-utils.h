/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
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
#ifndef RAPIDNET_SCRIPT_UTILS_H
#define RAPIDNET_SCRIPT_UTILS_H

#include <list>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>

#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/vector.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/wifi-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-address.h"
#include "ns3/mobility-helper.h"
#include "ns3/random-walk-2d-mobility-model.h"
#include "ns3/application-container.h"

#include "tuple.h"
#include "rapidnet-application-base.h"
#include "rapidnet-application-helper.h"
#include "rapidnet-utils.h"
#include "app-decorator-trigger.h"
#include "rapidnet-decorator-frontend.h"

//Scripts should use 1-based indexing
#define app(i) (apps.Get(i-1)->GetObject<RapidNetApplicationBase>())
#define addr(i) (app(i)->GetAddress ())
#define schedule(seconds, function) Simulator::Schedule (Seconds (seconds), function)
#define DEFAULT_MASK "255.255.255.0"

#define COUT "cout"
#define CLOG "clog"

#define MOBILITY_MODEL_CONSTANT_POSITION "ns3::ConstantPositionMobilityModel"
#define MOBILITY_MODEL_RANDOM_WALK_2D "ns3::RandomWalk2dMobilityModel"
#define MOBILITY_MODEL_RANDOM_WAYPOINT "ns3::RandomWaypointMobilityModel"
#define MOBILITY_MODEL_HIERARCHICAL "ns3::HierarchicalMobilityModel"

#define __GetStream(stream_name) \
    (stream_name==COUT? cout: \
    (stream_name==CLOG? clog: \
     clog))

#define APPTABLE_NAME "apptable"
#define APPTABLE_SRC "apptable_attr1"
#define APPTABLE_DST "apptable_attr2"
#define APPTABLE_COST "apptable_attr3"

#define DISCOVERY_LINK "link"
#define DISCOVERY_LINK_NEXT "link_attr2"

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \brief Initializes RapidNet applications using the given
 * helper on given the given number of nodes and sets
 * addresses with the given base.
 */
ApplicationContainer
InitRapidNetApps (int numNodes, Ptr<RapidNetApplicationHelper> appHelper,
  Ipv4Address base = "10.1.1.0", string pcapLogFileName = "test");

/**
 * \brief Prints the given relation for all applications
 * in the container.
 */
void
PrintRelation (ApplicationContainer apps, string relnName, ostream& os = cout);

/**
 * \brief Installs Wifi NetDevices on the the nodes and returns the container.
 */
NetDeviceContainer
InstallWifi (NodeContainer nodes, bool enablePcap, string pcapFilename);

/**
 * \brief Installs Ethernet NetDevices on the the nodes and returns the container.
 */
NetDeviceContainer
InstallCsma (NodeContainer nodes, bool enablePcap, string pcapFilename);

/**
 * \brief Installs the IP stack on the nodes and assigns IP addresses.
 */
void
InstallIpv4 (NodeContainer nodes, NetDeviceContainer netDevices,
  Ipv4Address network = "192.168.1.0", Ipv4Mask mask = DEFAULT_MASK,
  Ipv4Address base = "0.0.0.1");

/**
 * \brief Installs the mobility model to the nodes and positions them.
 */
void
InstallMobility (NodeContainer nodes, string xpos, string ypos,
  double xbound, double ybound, string modelName,
  const AttributeValue& speed=EmptyAttributeValue(),
  const AttributeValue& distance=EmptyAttributeValue());

/**
 * \brief Installs the decorator frontend to all applications.
 */
ofstream*
InstallDecorator (ApplicationContainer& apps, string dir, double xbound, double ybound);

/**
 * \brief Installs the decorator frontend to all applications for emulation.
 */
ofstream*
InstallDecoratorEmu (ApplicationContainer& apps, string dir, string ipaddr);

/**
 * \brief For printing relations, printPeriod: Period for printing, numTimes:
 * Number of times to execute
 */
void
PrintRelations (ApplicationContainer& apps, string relations,
  string streamName = CLOG, int printPeriod = 0, int numTimes = 2);

/**
 * \brief Imports app-table into all nodes for application level filtering. This
 * function is used for emulation.
 *
 * The topology string should have all the necessary IP address pairs. They are
 * just inserted as it is.
 */
void
ImportAppTable (ApplicationContainer& apps, ifstream* apptableStream,
  double apptablePeriod, double duration);

/**
 * \brief Installs frontend-decorator triggers specific to the DSR
 *        protocol.
 */
void
InstallDsrDecoratorTriggers (ApplicationContainer& apps);

/**
 * \brief Installs frontend-decorator triggers specific to the Epidemic
 *        protocol.
 */
void
InstallEpidemicDecoratorTriggers (ApplicationContainer& apps);

/**
 * \brief Installs frontend-decorator triggers specific to the link-state
 *        and HSLS protocols.
 */
void
InstallLSUDecoratorTriggers (ApplicationContainer& apps);

/**
 * \brief Schedules DSR route queries at periodic intervals.
 *
 * @param apps The collection of applications.
 * @param count The number of queries to be scheduled.
 * @param duration The period between each query.
 */
void
ScheduleDsrQueries (ApplicationContainer& apps, int count, double duration);

/**
 * \brief Schedules Epidemic messages at periodic intervals.
 *
 * @param apps The collection of applications.
 * @param count The number of queries to be scheduled.
 * @param duration The period between each query.
 */
void
ScheduleEpidemicMessages (ApplicationContainer& apps, int count,
  double duration);

/**
 * \brief Utility method to set maximum jitter value on all applications
 *        in the given ApplicationContainer.
 */
void
SetMaxJitter (ApplicationContainer& apps, uint32_t maxJitter);

/**
 * \brief Initializes the chord applications.
 */
void
InitChordApps (ApplicationContainer& apps);

/**
 * \brief Installs frontend-decorator triggers specific to the link-state
 *        and HSLS protocols.
 */
void
InstallChordDecoratorTriggers (ApplicationContainer& apps);

/**
 * \brief Install application specific decorator triggers.
 */
void
InstallDecoratorTriggers (string appName, ApplicationContainer& apps);

/**
 * \brief Schedule application specific network events.
 */
void
ScheduleEvents (string appName, ApplicationContainer& apps, int queryNum,
  double duration);

/**
 * \brief Initialize and start all applications
 */
void
StartApps (string appName, ApplicationContainer& apps);

} //namespace rapidnet
} //namepsace ns3

#endif // RAPIDNET_SCRIPT_UTILS_H
