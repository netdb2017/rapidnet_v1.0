/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef DISCOVERYEPIDEMIC_H
#define DISCOVERYEPIDEMIC_H

#include <string>
#include <iostream>
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-address.h"
#include "ns3/rapidnet-header.h"
#include "ns3/relation-base.h"
#include "ns3/database.h"
#include "ns3/rapidnet-application-base.h"
#include "ns3/aggregator.h"
#include "ns3/aggwrap.h"

using namespace std;
using namespace ns3;

namespace ns3 {

class Socket;
class Packet;

namespace rapidnet {
namespace discoveryepidemic {

class DiscoveryEpidemic : public RapidNetApplicationBase
{
public:
  static const string BEACON;
  static const string BEACONLOC;
  static const string ELINKDISCOVERYADD;
  static const string ELINKDISCOVERYDEL;
  static const string LINK;
  static const string PERIODIC;
  static const string R1_ECAPERIODIC;

  static TypeId GetTypeId (void);

  DiscoveryEpidemic ();

  virtual ~DiscoveryEpidemic ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void R1_ecaperiodic ();

  virtual void R1_eca (Ptr<Tuple> r1_ecaperiodic);

  virtual void R2_eca (Ptr<Tuple> beacon);

  virtual void R3_eca (Ptr<Tuple> beaconLoc);

  virtual void RAddLinkEca0Ins (Ptr<Tuple> link);

  virtual void RAddLinkEca0Ref (Ptr<Tuple> link);

  virtual void RDelLinkEca0Del (Ptr<Tuple> link);

  EventId m_event_r1_ecaperiodic;
};

} // namespace discoveryepidemic
} // namespace rapidnet
} // namespace ns3

#endif // DISCOVERYEPIDEMIC_H
