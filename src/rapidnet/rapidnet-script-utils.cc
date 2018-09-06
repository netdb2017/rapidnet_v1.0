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
#include "rapidnet-script-utils.h"
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/rapidnet-types.h"
#include "ns3/pointer.h"
#include "ns3/chord.h"

using namespace std;

namespace ns3 {
namespace rapidnet {

NS_LOG_COMPONENT_DEFINE ("ScriptUtils");

ApplicationContainer
InitRapidNetApps (int numNodes, Ptr<RapidNetApplicationHelper> appHelper,
  Ipv4Address base, string pcapLogFileName)
{
  NodeContainer csmaNodes;
  csmaNodes.Create (numNodes);

  CsmaHelper csma;

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;

  address.SetBase (base, DEFAULT_MASK);
  address.Assign (csmaDevices);

  CsmaHelper::EnablePcapAll (pcapLogFileName, true);

  return appHelper->Install (csmaNodes);
}

void
PrintRelation (ApplicationContainer apps, string relnName, ostream& os)
{
  int now = round (Simulator::Now ().GetSeconds ());

  for (vector<Ptr<Application> >::const_iterator it = apps.Begin ();
    it != apps.End (); it++)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
        (RapidNetApplicationBase*) PeekPointer (*it));

      if (!app->GetDatabase ()->HasRelation (relnName))
        {
          continue;
        }
      Ipv4Address nodeAddr = app->GetAddress ();
      list<Ptr<Tuple> > tuples = app->GetRelation (
        relnName)->GetAllTuples ();
      for (list<Ptr<Tuple> >::iterator jt = tuples.begin ();
        jt != tuples.end (); ++jt)
        {
          os << "time=" << now << " node=" << nodeAddr << " tuple=" <<
            (*jt)->GetName () << " ";
          map<string, Ptr<TupleAttribute> > attributes =
            (*jt)->GetAllAttributes ();
          for (map<string, Ptr<TupleAttribute> >::const_iterator kt =
            attributes.begin (); kt != attributes.end (); ++kt)
          {
            os << kt->first << "=" << kt->second->GetValue ()->ToString () << " ";
          }
          os << endl;
        }
    }
}

NetDeviceContainer
InstallWifi (NodeContainer nodes, bool enablePcap, string pcapFilename)
{
  NS_LOG_INFO ("Installing Wifi");
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  NetDeviceContainer retval = wifi.Install(wifiPhy, wifiMac, nodes);
  if (enablePcap)
    {
      wifiPhy.EnablePcapAll (pcapFilename);
    }
  return retval;
}

NetDeviceContainer
InstallCsma (NodeContainer nodes, bool enablePcap, string pcapFilename)
{
  NS_LOG_INFO ("Installing CSMA");
  CsmaHelper csma;
  if (enablePcap)
    {
      CsmaHelper::EnablePcapAll (pcapFilename, true);
    }
  return csma.Install (nodes);
}

void
InstallIpv4 (NodeContainer nodes, NetDeviceContainer netDevices,
  Ipv4Address network, Ipv4Mask mask, Ipv4Address base)
{
  InternetStackHelper internet;
  internet.Install(nodes);

  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase(network, mask, base);
  ipAddrs.Assign(netDevices);
}

/**
 * Tokenizes string with comma separated double values into a list of
 * double values.
 */
list<double>
tokenize (string pos, int count)
{
  list<double> retval;
  istringstream strstr (pos);
  double value;
  char comma;
  for (int i=0; i<count; ++i)
    {
      strstr >> value;
      retval.push_back(value);
      strstr >> comma;
    }
  return retval;
}

Ptr<ListPositionAllocator>
GetPositions (string xpos, string ypos, int count)
{
  Ptr<ListPositionAllocator> positionAllocator =
    CreateObject<ListPositionAllocator>();
  list<double> x = tokenize (xpos, count);
  list<double> y = tokenize (ypos, count);
  double z = 0;
  Vector vector;
  int i = 1;

  for (list<double>::iterator xit = x.begin (), yit = y.begin ();
    xit != x.end () && yit != y.end ();
    ++xit, ++yit)
    {
      vector = Vector (*xit, *yit, z);
      positionAllocator->Add (vector);
      NS_LOG_INFO ("Positioning node " << i++ << " at [" << vector << "]");
    }
  return positionAllocator;
}

void
InstallMobility (NodeContainer nodes, string xpos, string ypos,
  double xbound, double ybound, string modelName,
  const AttributeValue& speed, const AttributeValue& distance)
{
  NS_LOG_INFO ("Installing mobility model: " << modelName);
    MobilityHelper mobility;
  /*  if (modelName == MOBILITY_MODEL_CONSTANT_POSITION ||
        modelName == MOBILITY_MODEL_RANDOM_WALK_2D)
        {
          Ptr<ListPositionAllocator> positionAllocator = GetPositions (
            xpos, ypos, nodes.GetN());
          mobility.SetPositionAllocator(positionAllocator);
        }
  */
    Ptr<RandomRectanglePositionAllocator> positionAllocator =
      CreateObject<RandomRectanglePositionAllocator>();
    positionAllocator->SetX (UniformVariable (0.0, xbound));
    positionAllocator->SetY (UniformVariable (0.0, ybound));
    mobility.SetPositionAllocator(positionAllocator);

    if (modelName == MOBILITY_MODEL_CONSTANT_POSITION)
      {
        mobility.SetMobilityModel (modelName);
      }
    else if (modelName == MOBILITY_MODEL_RANDOM_WALK_2D)
      {
        RectangleValue bounds = RectangleValue (Rectangle (
          Rectangle (0, xbound, 0, ybound)));
        mobility.SetMobilityModel (modelName, "Bounds", bounds, "Speed", speed,
          "Distance", distance);
      }
    else if (modelName == MOBILITY_MODEL_RANDOM_WAYPOINT)
      {
        mobility.SetMobilityModel (modelName, "Speed", speed,
          "PositionAllocator", PointerValue(positionAllocator));
      }
    else if (modelName == MOBILITY_MODEL_HIERARCHICAL)
      {
        mobility.SetMobilityModel (modelName, "Parent", PointerValue (),
          "Child", PointerValue());
      }
    mobility.Install(nodes);
}

ofstream*
InstallDecorator (ApplicationContainer& apps, string dir,
  double xbound, double ybound)
{
  Ptr<RapidNetDecoratorFrontend> decorator =
    CreateObject<RapidNetDecoratorFrontend> ();
  decorator->SetArena(Rectangle(0, xbound, 0, ybound));
  ofstream* out = new ofstream((dir + "/decorator.log").c_str ());
  decorator->SetStream(out);
  decorator->Init();
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->SetDecoratorFrontend (decorator);

      // Discovery link
      app->OnInsert += AppLinkStateTrigger::New (app, DISCOVERY_LINK,
        DISCOVERY_LINK, true, DISCOVERY_LINK_NEXT);
      app->OnDelete += AppLinkStateTrigger::New (app, DISCOVERY_LINK,
        DISCOVERY_LINK, false, DISCOVERY_LINK_NEXT);

      // tLSU
      //app->OnInsert += AppTupleStateTrigger::New (app, "tLSU", true,
      //  strlist ("tLSU_attr2", "tLSU_attr3"));
      //app->OnDelete += AppTupleStateTrigger::New (app, "tLSU", false,
      //  strlist ("tLSU_attr2", "tLSU_attr3"));
    }
  return out;
}

ofstream*
InstallDecoratorEmu (ApplicationContainer& apps, string dir, string ipaddr)
{
  Ptr<RapidNetDecoratorFrontend> decorator =
    CreateObject<RapidNetDecoratorFrontend> ();
  ofstream* out = new ofstream((dir + "/decorator-"+ipaddr+".log" ).c_str ());
  decorator->SetStream(out);
  decorator->Init();
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->SetDecoratorFrontend (decorator);
    }
  return out;
}

list<string>
Tokenize (string names, char delim=',')
{
  list<string> retval;
  uint32_t lastPos = 0, pos = 0;
  do
    {
      pos = names.find_first_of (delim, lastPos);
      retval.push_back (names.substr (lastPos, pos-lastPos));
      lastPos = pos+1;
    }
  while (pos != string::npos);
  return retval;
}

void
PrintRelations (ApplicationContainer& apps, string relations,
  string streamName, int printPeriod, int numTimes)
{
  ostream& os = __GetStream(streamName);
  list<string> namesList = Tokenize (relations);
  for (list<string>::iterator it = namesList.begin (); it != namesList.end ();
    ++it)
    {
      PrintRelation (apps, *it, os);
    }
  //clog << numTimes << " printPeriod" << printPeriod << endl;
  --numTimes;
  if (numTimes > 0)
    {
      //clog << "scheduling again" << endl;
      Simulator::Schedule (Seconds (printPeriod - 0.001),
        &PrintRelations, apps, relations, streamName, printPeriod, numTimes);
    }
}

list<pair<int, int> >
ParseTopology (string topology)
{
  list<string> links = Tokenize (topology, ':');

  list<pair<int, int> > intlinks;
  int from, to;
  char delim;
  for (list<string>::iterator it = links.begin (); it != links.end (); ++it)
    {
      stringstream strstr (*it);
      strstr >> from >> delim >> to;
      intlinks.push_back (pair<int, int>(from, to));
      intlinks.push_back (pair<int, int>(to, from));
      intlinks.push_back (pair<int, int>(from, from));
      intlinks.push_back (pair<int, int>(to, to));
    }
  return intlinks;
}

string
GetAddressAsString (Ptr<RapidNetApplicationBase> app)
{
  Ipv4Address addr = app->GetAddress ();
  stringstream strstr;
  strstr << addr;
  return strstr.str ();
}

Ptr<Tuple> GetApptableTuple (string srcAddr, string destAddr, int cost)
{
  return tuple (APPTABLE_NAME, attrlist (
    attr (APPTABLE_SRC, Ipv4Value, srcAddr.c_str ()),
    attr (APPTABLE_DST, Ipv4Value, destAddr.c_str ()),
    attr (APPTABLE_COST, Int32Value, cost)));
}

void
ImportIntoApps (ApplicationContainer& apps, stringstream& strstr)
{
  for (ApplicationContainer::Iterator it = apps.Begin(); it != apps.End(); it++)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (*it)));
      string addr = GetAddressAsString (app);
      string line;

      // Skip the application if it does not have apptable
      // This is possible if the application has not started
      //
      if (!app->GetDatabase ()->HasRelation (APPTABLE_NAME))
        {
          continue;
        }

      //Loopback
      app->Insert (GetApptableTuple (addr, addr, 1));

      while (!strstr.eof ())
        {
          getline (strstr, line);
          list<string> words = Tokenize (line, ' ');
          list<string>::iterator it = words.begin ();
          string action = *it++;
          string srcAddr = *it++;
          string destAddr = *it;
          if (srcAddr == addr)
            {
              Ptr<Tuple> tuple;
              if (action == "INSERT")
                {
                  tuple = GetApptableTuple (srcAddr, destAddr, 1);
                  // NS_LOG_INFO ("Inserting apptable entry: " << tuple);
                  app->Insert (tuple);
                }
              else if (action == "DELETE")
                {
                  tuple = GetApptableTuple (srcAddr, destAddr, 1);
                  // NS_LOG_INFO ("Deleting apptable entry: " << tuple);
                  app->Delete (tuple);
                }
              else
                {
                  NS_LOG_ERROR ("Unknown action: " << action);
                }
            }
        }
    }
}

void
ImportAppTable (ApplicationContainer& apps, ifstream* apptableStream,
  double apptablePeriod, double duration)
{
  static double time = 0.0;

  string apptableString;
  if (!apptableStream->eof ())
    {
      getline (*apptableStream, apptableString, '&');
    }
  stringstream strstr (apptableString);

  // read and discard the first 2 lines which has a newline and time
  string temp;
  getline (strstr, temp); //newline
  getline (strstr, temp); //time

  ImportIntoApps (apps, strstr);

  time += apptablePeriod;
  if (time <= duration)
    {
      Simulator::Schedule (Seconds (apptablePeriod), &ImportAppTable,
        apps, apptableStream, apptablePeriod, duration);
    }
}

void
InstallDsrDecoratorTriggers (ApplicationContainer& apps)
{
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->OnRecv += AppNodeStateTrigger::New (app, "eQuery",
        "eQuery", true);
      app->OnRecv += AppNodeStateTrigger::New (app, "eQuery_Dst",
        "eQuery_Dst", true);
      app->OnInsert += AppNodeStateTrigger::New (app, "tBestPath",
    	  "tBestPath", true);
      app->OnSend += AppNodeStateTrigger::New (app, "ePathDst",
    	  "ePathDst", true);
      app->OnSend += AppNodeStateTrigger::New (app, "ePathSrc",
    	  "ePathSrc", true);
      app->OnInsert += AppTupleStateTrigger::New (app, "tBestPath",
        true, strlist ("tBestPath_attr4"));
      app->OnRecv += AppNodeStateTrigger::New (app, "clear_node_state",
        "Clear", true);
      app->OnSend += AppLinkStateTrigger::New (app, "ePathSrc",
        "linkSrc", true, "ePathSrc_attr1");
    }
}

void
InstallEpidemicDecoratorTriggers (ApplicationContainer& apps)
{
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->OnRecv += AppNodeStateTrigger::New (app, "eMessageInjectOriginal",
        "eMessageInjectOriginal", true);
      app->OnRecv += AppNodeStateTrigger::New (app, "eMessageInjectDst",
         "eMessageInjectDst", true);
      app->OnInsert += AppNodeStateTrigger::New (app, "tMessage",
        "tMessage", true);
      app->OnRecv += AppNodeStateTrigger::New (app, "eMessageEnd",
        "eMessageEnd", true);
      app->OnRecv += AppNodeStateTrigger::New (app, "clear_node_state",
        "Clear", true);
      app->OnRecv += AppLinkStateTrigger::New (app, "eBitVectorReply",
        "eBitVectorReply", true, "eBitVectorReply_attr2");
      app->OnRecv += AppTupleStateTrigger::New (app, "eMessage",
         true, strlist (RN_SRC));
      app->OnInsert += AppTupleStateTrigger::New (app, "tMessage",
         true, strlist ("tMessage_attr4"));
    }
}

void
InstallLSUDecoratorTriggers (ApplicationContainer& apps)
{
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->OnRecv += AppTupleStateTrigger::New (app, "eLSU",
          true, strlist (RN_SRC));
    }
}

Ptr<RapidNetApplicationBase>
GetRapidNetApp (ApplicationContainer& apps, int id)
{
  return Ptr<RapidNetApplicationBase> (
    dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(id))));
  //apps.Get (id)->GetObject<ApplicationBase> ();
}

void InjectDsrQuery (Ptr<RapidNetApplicationBase> src,
  Ptr<RapidNetApplicationBase> dst, string id)
{
  Ipv4Address from = src->GetAddress();
  Ipv4Address to = dst->GetAddress();
  src->Inject (tuple ("eQuery", attr ("eQuery_attr1", Ipv4Value, from),
    attr ("eQuery_attr2", Ipv4Value, to),
    attr ("eQuery_attr3", StrValue, id)));
  dst->Inject (tuple ("eQuery_Dst", attr ("eQuery_attr1", Ipv4Value, from),
    attr ("eQuery_attr2", Ipv4Value, to),
    attr ("eQuery_attr3", StrValue, id)));
}

void
InjectEpidemicMessage (Ptr<RapidNetApplicationBase> src,
  Ptr<RapidNetApplicationBase> dst, string id)
{
  Ipv4Address from = src->GetAddress();
  Ipv4Address to = dst->GetAddress();
  src->Inject (tuple ("eMessageInjectOriginal",
    attr ("eMessageInjectOriginal_attr1", Ipv4Value, from),
    attr ("eMessageInjectOriginal_attr2", Ipv4Value, to)));
  dst->Inject (tuple ("eMessageInjectDst",
    attr ("eMessageInjectOriginal_attr1", Ipv4Value, from),
    attr ("eMessageInjectOriginal_attr2", Ipv4Value, to)));
}


void
ClearNodeStates (ApplicationContainer& apps)
{
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
        dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->Inject (tuple ("clear_node_state"));
    }
}


void
ScheduleDsrQueries (ApplicationContainer& apps, int count, double duration)
{
  srand (time (NULL));

  double interval=(duration-10)/count;
  int i = 0;
  while (i < count)
    {
      int fromId = rand () % apps.GetN ();
      int toId = rand () % apps.GetN ();
      if (fromId != toId)
        {
          ++i;
        }
      else
        {
          continue;
        }
      stringstream queryId;
      queryId << "query" << i;
      Simulator::Schedule (Seconds ((i-1)*interval+5), &InjectDsrQuery,
        GetRapidNetApp (apps, fromId),
	    GetRapidNetApp (apps, toId), queryId.str ());
      Simulator::Schedule (Seconds (i*interval+3), &ClearNodeStates, apps);
    }
}

void
ScheduleEpidemicMessages (ApplicationContainer& apps, int count,
  double duration)
{
  srand (time (NULL));

  double interval=(duration-10)/count;
  int i = 0;
  while (i < count)
    {
      int fromId = rand () % apps.GetN ();
      int toId = rand () % apps.GetN ();
      if (fromId != toId)
        {
          ++i;
        }
      else
        {
          continue;
        }
      stringstream messageId;
      messageId << "message" << i;
      Simulator::Schedule (Seconds ((i-1)*interval+5), &InjectEpidemicMessage,
        GetRapidNetApp (apps, fromId), GetRapidNetApp (apps, toId),
        messageId.str ());
      Simulator::Schedule (Seconds (i*interval), &ClearNodeStates, apps);
    }
}

void
SetMaxJitter (ApplicationContainer& apps, uint32_t maxJitter)
{
  Ptr<RapidNetApplicationBase> app;
  for (ApplicationContainer::Iterator it = apps.Begin (); it != apps.End ();
    ++it)
    {
      app = DynamicCast<RapidNetApplicationBase, Application> (*it);
      app->SetMaxJitter (maxJitter);
    }
}

/**
 * Chord specific functions
 */

/**
 * Insert the landmark tuple
 */
void
SetLandmark (Ptr<RapidNetApplicationBase> app,
  Ptr<RapidNetApplicationBase> landmarkApp)
{
  Ipv4Address localAddr = app->GetAddress ();
  Ptr<TupleAttribute> landmarkAttr;
  if (app == landmarkApp)
    {
      landmarkAttr = attr ("landmark_attr2", StrValue,
        "NIL");
    }
  else
    {
      landmarkAttr = attr ("landmark_attr2", Ipv4Value,
        landmarkApp->GetAddress ());
    }

  Ptr<Tuple> landmarkTuple = tuple (chord::Chord::LANDMARK,
    attr ("landmark_attr1", Ipv4Value, localAddr),
    landmarkAttr);
  app->Insert (landmarkTuple);
}


/**
 * Set the predecessor
 */
void
SetPred (Ptr<RapidNetApplicationBase> app)
{
  Ipv4Address localAddr = app->GetAddress ();
  Ptr<Tuple> predTuple = tuple (chord::Chord::PRED,
    attr ("pred_attr1", Ipv4Value, localAddr),
    attr ("pred_attr2", StrValue, "NIL"),
    attr ("pred_attr3", StrValue, "NIL"));
  app->Insert (predTuple);
}

/**
 * Set the next finger to be fixed to index 0
 */
void
SetNextFingerFix (Ptr<RapidNetApplicationBase> app)
{
  Ipv4Address localAddr = app->GetAddress ();
  Ptr<Tuple> nffTuple = tuple (chord::Chord::NEXTFINGERFIX,
    attr ("nextFingerFix_attr1", Ipv4Value, localAddr),
    attr ("nextFingerFix_attr2", Int32Value, 0));
  app->Insert (nffTuple);
}

void
ScheduleChordAppStart (Ptr<Application> app,
  Ptr<RapidNetApplicationBase> landmarkApp, Time time)
{
  app->Start (time);
  Ptr<RapidNetApplicationBase> rn_app =
    DynamicCast<RapidNetApplicationBase, Application> (app);
  Simulator::Schedule (time, &SetLandmark, rn_app, landmarkApp);
  Simulator::Schedule (time, &SetPred, rn_app);
  Simulator::Schedule (time, &SetNextFingerFix, rn_app);
}

void
InitChordApps (ApplicationContainer& apps)
{
  int num_nodes = apps.GetN ();
  int join_duration = 100;
//  int total_duration = num_nodes * join_duration + 500;
  SetMaxJitter (apps, 0);
  Ptr<RapidNetApplicationBase> landmarkApp =
    DynamicCast<RapidNetApplicationBase, Application> (apps.Get (0));

  ScheduleChordAppStart (landmarkApp, landmarkApp, Seconds (0));
  for (int i = 1; i < num_nodes; ++i)
    {
      ScheduleChordAppStart (apps.Get (i), landmarkApp,
        Seconds (i * join_duration));
    }
}

void
InstallChordDecoratorTriggers (ApplicationContainer& apps)
{
  for (uint32_t i =0; i < apps.GetN(); ++i)
    {
      Ptr<RapidNetApplicationBase> app = Ptr<RapidNetApplicationBase> (
          dynamic_cast<RapidNetApplicationBase*> (PeekPointer (apps.Get(i))));
      app->OnInsert += AppLinkStateTrigger::New (app, "bestSucc",
              "bestSucc", true, "bestSucc_attr3");
      app->OnDelete += AppLinkStateTrigger::New (app, "bestSucc",
                    "bestSucc", false, "bestSucc_attr3");
      app->OnInsert += AppLinkStateTrigger::New (app, "finger",
              "afinger", true, "finger_attr4");
      app->OnDelete += AppLinkStateTrigger::New (app, "finger",
                    "afinger", false, "finger_attr4");
      app->OnInsert += AppTupleStateTrigger::New (app, "node",
         true, strlist ("node_attr1", "node_attr2"));
      app->OnInsert += AppTupleStateTrigger::New (app, "pred",
              true, strlist ("pred_attr3"));
      app->OnDelete += AppTupleStateTrigger::New (app, "pred",
              false, strlist ("pred_attr3"));
    }
}

void
InstallDecoratorTriggers (string appName, ApplicationContainer& apps)
{
  if (appName == "Dsr")
    {
      InstallDsrDecoratorTriggers (apps);
    }
  else if (appName == "Epidemic")
    {
      InstallEpidemicDecoratorTriggers (apps);
    }
  else if (appName == "SimLsPeriodic" || appName == "SimLsTriggered"
    || appName == "SimHslsPeriodic" || appName == "SimHslsTriggered")
    {
      InstallLSUDecoratorTriggers (apps);
    }
  else if (appName == "Chord")
    {
      InstallChordDecoratorTriggers (apps);
    }
}

void
ScheduleEvents (string appName, ApplicationContainer& apps, int queryNum,
  double duration)
{
  if (appName == "Dsr")
    {
      ScheduleDsrQueries (apps, queryNum, duration);
    }
  if (appName == "Epidemic")
    {
      ScheduleEpidemicMessages (apps, queryNum, duration);
    }
}

void
StartApps (string appName, ApplicationContainer& apps)
{
  if (appName == "Chord")
    {
      InitChordApps (apps);
    }
  else
    {
      apps.Start (Seconds (0));
    }
}

} // namespace rapidnet
} // namespace ns3
