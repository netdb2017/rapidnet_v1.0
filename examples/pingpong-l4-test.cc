#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/pingpong-l4-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/l4-platform-helper.h"
#include "ns3/uinteger.h"


#define tlink(src, next) \
  tuple (PingpongL4::TLINK, \
	 attr ("tLink_attr1", StrValue, src), \
	 attr ("tLink_attr2", StrValue, next))

#define insertlink(from, to) \
  app(from)->Insert (tlink (app(from)->GetLocalLocSpec(), app(to)->GetLocalLocSpec())); \
  app(to)->Insert (tlink (app(to)->GetLocalLocSpec(), app(from)->GetLocalLocSpec()));


#define deletelink(from, to) \
  app(from)->Delete (tlink (app(from)->GetLocalLocSpec(), app(to)->GetLocalLocSpec())); \
  app(to)->Delete (tlink (app(to)->GetLocalLocSpec(), app(from)->GetLocalLocSpec()));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pingpongl4;

ApplicationContainer apps;
string destLocSpec = "", localPort = "";
string localIPAddress = "";

void
UpdateLinks1 ()
{
  app(1)->Insert (tlink (app(1)->GetLocalLocSpec(), destLocSpec));
}

int
main (int argc, char *argv[])
{

  std::time_t result = std::time(NULL);
  std::cout << std::asctime(std::localtime(&result)) << result << " -> Starting time\n";

  // Use realtime simulator
  GlobalValue::Bind ("SimulatorImplementationType",
		     StringValue ("ns3::RealtimeSimulatorImpl"));

  LogComponentEnable("PingpongL4", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  uint16_t port;

  CommandLine cmd;

  cmd.AddValue ("dest", "Destination LocSpec IP:Port", destLocSpec);
  cmd.AddValue ("localPort", "Local RapidNet port", localPort);
  cmd.AddValue ("localIPAddress", "Local IPAddress", localIPAddress);
  cmd.Parse (argc, argv);

  if (localPort != "")
    {
      std::istringstream sin (localPort);
      sin >> port;
    }
  else
    {
      port = 11111;
    }

  NodeContainer nodeContainer;
  nodeContainer.Create (1);
  L4PlatformHelper platform;
  platform.Install (nodeContainer);
  PingpongL4Helper helper = PingpongL4Helper();
  helper.SetL4Platform (true);
  if(localIPAddress != "")
    helper.SetLocalAddress(Ipv4Address((const char*)localIPAddress.c_str()));
  
  helper.SetAttribute ("RapidNetPort", UintegerValue (port));
  // Install ping pong
  apps = helper.Install (nodeContainer);

  ofstream* decorator_out = InstallDecorator(apps, ".", 2000, 2000);

  if (destLocSpec == "")
    destLocSpec = app (1)->GetLocalLocSpec ();

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (50.0));

  //schedule(0.00001, AddTLinkCounter);
  schedule (20, UpdateLinks1);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;

}
