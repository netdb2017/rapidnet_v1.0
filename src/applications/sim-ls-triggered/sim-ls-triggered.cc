/* A RapidNet application. Generated by RapidNet compiler. */

#include "sim-ls-triggered.h"
#include <cstdlib>
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/type-ids.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-functions.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::simlstriggered;

const string SimLsTriggered::ELSU = "eLSU";
const string SimLsTriggered::ELSUCHANGE = "eLSUChange";
const string SimLsTriggered::ELSUCOUNT = "eLSUCount";
const string SimLsTriggered::ELSULOC = "eLSULoc";
const string SimLsTriggered::ELINKADD = "eLinkAdd";
const string SimLsTriggered::ELINKCOUNT = "eLinkCount";
const string SimLsTriggered::LINK = "link";
const string SimLsTriggered::PERIODIC = "periodic";
const string SimLsTriggered::R11_ECAPERIODIC = "r11_ecaperiodic";
const string SimLsTriggered::R12_ECAPERIODIC = "r12_ecaperiodic";
const string SimLsTriggered::TLSU = "tLSU";
const string SimLsTriggered::TLINK = "tLink";

NS_LOG_COMPONENT_DEFINE ("SimLsTriggered");
NS_OBJECT_ENSURE_REGISTERED (SimLsTriggered);

TypeId
SimLsTriggered::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::simlstriggered::SimLsTriggered")
    .SetParent<Discovery> ()
    .AddConstructor<SimLsTriggered> ()
    ;
  return tid;
}

SimLsTriggered::SimLsTriggered()
{
  NS_LOG_FUNCTION_NOARGS ();
}

SimLsTriggered::~SimLsTriggered()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
SimLsTriggered::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Discovery::DoDispose ();
}

void
SimLsTriggered::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Discovery::StartApplication ();
  m_event_r11_ecaperiodic=
    Simulator::Schedule (Seconds (60 + (drand48 () * 1)), &SimLsTriggered::R11_ecaperiodic, this);
  m_event_r12_ecaperiodic=
    Simulator::Schedule (Seconds (59), &SimLsTriggered::R12_ecaperiodic, this);
  RAPIDNET_LOG_INFO("SimLsTriggered Application Started");
}

void
SimLsTriggered::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  Discovery::StopApplication ();
  Simulator::Cancel(m_event_r11_ecaperiodic);
  Simulator::Cancel(m_event_r12_ecaperiodic);
  RAPIDNET_LOG_INFO("SimLsTriggered Application Stopped");
}

void
SimLsTriggered::InitDatabase ()
{
  //Discovery::InitDatabase ();

  AddRelationWithKeys (LINK, attrdeflist (
    attrdef ("link_attr1", IPV4),
    attrdef ("link_attr2", IPV4)),
    Seconds (11));

  AddRelationWithKeys (TLSU, attrdeflist (
    attrdef ("tLSU_attr1", IPV4),
    attrdef ("tLSU_attr2", IPV4),
    attrdef ("tLSU_attr3", IPV4)),
    Seconds (65));

  AddRelationWithKeys (TLINK, attrdeflist (
    attrdef ("tLink_attr1", IPV4),
    attrdef ("tLink_attr2", IPV4)),
    Seconds (11));

}

void
SimLsTriggered::DemuxRecv (Ptr<Tuple> tuple)
{
  Discovery::DemuxRecv (tuple);

  if (IsInsertEvent (tuple, LINK))
    {
      R01Eca0Ins (tuple);
    }
  if (IsRefreshEvent (tuple, LINK))
    {
      R01Eca0Ref (tuple);
    }
  if (IsRecvEvent (tuple, ELINKADD))
    {
      R02_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELINKCOUNT))
    {
      R03_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELINKADD))
    {
      R04_eca (tuple);
    }
  if (IsRecvEvent (tuple, R11_ECAPERIODIC))
    {
      R11_eca (tuple);
    }
  if (IsRecvEvent (tuple, R12_ECAPERIODIC))
    {
      R12_eca (tuple);
    }
  if (IsInsertEvent (tuple, TLINK))
    {
      R13Eca1Ins (tuple);
    }
  if (IsRefreshEvent (tuple, TLINK))
    {
      R13Eca1Ref (tuple);
    }
  if (IsRecvEvent (tuple, ELSU))
    {
      R21_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSULOC))
    {
      R22_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSUCOUNT))
    {
      R23_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSULOC))
    {
      R24_eca (tuple);
    }
  if (IsInsertEvent (tuple, TLSU))
    {
      R31Eca0Ins (tuple);
    }
  if (IsRefreshEvent (tuple, TLSU))
    {
      R31Eca0Ref (tuple);
    }
  if (IsRecvEvent (tuple, ELSUCHANGE))
    {
      R32_eca (tuple);
    }
}

void
SimLsTriggered::R01Eca0Ins (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("R01Eca0Ins triggered");

  Ptr<Tuple> result = link;

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("link_attr1"),
      VarExpr::New ("link_attr2"))));

  result = result->Project (
    ELINKADD,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3"));

  SendLocal (result);
}

void
SimLsTriggered::R01Eca0Ref (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("R01Eca0Ref triggered");

  Ptr<Tuple> result = link;

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("link_attr1"),
      VarExpr::New ("link_attr2"))));

  result = result->Project (
    ELINKADD,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3"));

  SendLocal (result);
}

void
SimLsTriggered::R02_eca (Ptr<Tuple> eLinkAdd)
{
  RAPIDNET_LOG_INFO ("R02_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    eLinkAdd,
    strlist ("tLink_attr2", "tLink_attr1"),
    strlist ("eLinkAdd_attr2", "eLinkAdd_attr1"));

  result = AggWrapCount::New ()->Compute (result, eLinkAdd);

  result = result->Project (
    ELINKCOUNT,
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3",
      "count"),
    strlist ("eLinkCount_attr1",
      "eLinkCount_attr2",
      "eLinkCount_attr3",
      "eLinkCount_attr4"));

  SendLocal (result);
}

void
SimLsTriggered::R03_eca (Ptr<Tuple> eLinkCount)
{
  RAPIDNET_LOG_INFO ("R03_eca triggered");

  Ptr<Tuple> result = eLinkCount;

  result->Assign (Assignor::New ("SeqNum",
    ValueExpr::New (Int32Value::New (0))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("eLinkCount_attr4"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    TLINK,
    strlist ("eLinkCount_attr1",
      "eLinkCount_attr2",
      "eLinkCount_attr3",
      "SeqNum"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimLsTriggered::R04_eca (Ptr<Tuple> eLinkAdd)
{
  RAPIDNET_LOG_INFO ("R04_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    eLinkAdd,
    strlist ("tLink_attr2", "tLink_attr1"),
    strlist ("eLinkAdd_attr2", "eLinkAdd_attr1"));

  result = result->Project (
    TLINK,
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3",
      "tLink_attr4"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimLsTriggered::R11_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R11_ecaperiodic triggered");

  SendLocal (tuple (R11_ECAPERIODIC, attrlist (
    attr ("r11_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r11_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r11_ecaperiodic = Simulator::Schedule (Seconds(60),
    &SimLsTriggered::R11_ecaperiodic, this);
}

void
SimLsTriggered::R11_eca (Ptr<Tuple> r11_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R11_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r11_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r11_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("r11_ecaperiodic_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (999999))));

  result = result->Project (
    ELSU,
    strlist ("r11_ecaperiodic_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimLsTriggered::R12_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R12_ecaperiodic triggered");

  SendLocal (tuple (R12_ECAPERIODIC, attrlist (
    attr ("r12_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r12_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r12_ecaperiodic = Simulator::Schedule (Seconds(60),
    &SimLsTriggered::R12_ecaperiodic, this);
}

void
SimLsTriggered::R12_eca (Ptr<Tuple> r12_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R12_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r12_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r12_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("SeqNumU",
    Operation::New (RN_PLUS,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    TLINK,
    strlist ("r12_ecaperiodic_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "SeqNumU"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimLsTriggered::R13Eca1Ins (Ptr<Tuple> tLink)
{
  RAPIDNET_LOG_INFO ("R13Eca1Ins triggered");

  Ptr<Tuple> result = tLink;

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("tLink_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (999999))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    ELSU,
    strlist ("tLink_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimLsTriggered::R13Eca1Ref (Ptr<Tuple> tLink)
{
  RAPIDNET_LOG_INFO ("R13Eca1Ref triggered");

  Ptr<Tuple> result = tLink;

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("tLink_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (999999))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    ELSU,
    strlist ("tLink_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimLsTriggered::R21_eca (Ptr<Tuple> eLSU)
{
  RAPIDNET_LOG_INFO ("R21_eca triggered");

  Ptr<Tuple> result = eLSU;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    ELSULOC,
    strlist ("Local",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6",
      "Local"),
    strlist ("eLSULoc_attr1",
      "eLSULoc_attr2",
      "eLSULoc_attr3",
      "eLSULoc_attr4",
      "eLSULoc_attr5",
      "eLSULoc_attr6",
      RN_DEST));

  Send (result);
}

void
SimLsTriggered::R22_eca (Ptr<Tuple> eLSULoc)
{
  RAPIDNET_LOG_INFO ("R22_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLSU)->Join (
    eLSULoc,
    strlist ("tLSU_attr3", "tLSU_attr2", "tLSU_attr1"),
    strlist ("eLSULoc_attr3", "eLSULoc_attr2", "eLSULoc_attr1"));

  result = AggWrapCount::New ()->Compute (result, eLSULoc);

  result = result->Project (
    ELSUCOUNT,
    strlist ("eLSULoc_attr1",
      "eLSULoc_attr2",
      "eLSULoc_attr3",
      "eLSULoc_attr4",
      "eLSULoc_attr5",
      "eLSULoc_attr6",
      "count"),
    strlist ("eLSUCount_attr1",
      "eLSUCount_attr2",
      "eLSUCount_attr3",
      "eLSUCount_attr4",
      "eLSUCount_attr5",
      "eLSUCount_attr6",
      "eLSUCount_attr7"));

  SendLocal (result);
}

void
SimLsTriggered::R23_eca (Ptr<Tuple> eLSUCount)
{
  RAPIDNET_LOG_INFO ("R23_eca triggered");

  Ptr<Tuple> result = eLSUCount;

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("eLSUCount_attr7"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Select (Selector::New (
    Operation::New (RN_LT,
      VarExpr::New ("eLSUCount_attr4"),
      ValueExpr::New (Int32Value::New (999999)))));

  result = result->Project (
    TLSU,
    strlist ("eLSUCount_attr1",
      "eLSUCount_attr2",
      "eLSUCount_attr3",
      "eLSUCount_attr4",
      "eLSUCount_attr5",
      "eLSUCount_attr6"),
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"));

  Insert (result);
}

void
SimLsTriggered::R24_eca (Ptr<Tuple> eLSULoc)
{
  RAPIDNET_LOG_INFO ("R24_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLSU)->Join (
    eLSULoc,
    strlist ("tLSU_attr3", "tLSU_attr2", "tLSU_attr1"),
    strlist ("eLSULoc_attr3", "eLSULoc_attr2", "eLSULoc_attr1"));

  result = result->Select (Selector::New (
    Operation::New (RN_GT,
      VarExpr::New ("eLSULoc_attr5"),
      VarExpr::New ("tLSU_attr5"))));

  result = result->Select (Selector::New (
    Operation::New (RN_LT,
      VarExpr::New ("eLSULoc_attr4"),
      ValueExpr::New (Int32Value::New (999999)))));

  result = result->Project (
    TLSU,
    strlist ("eLSULoc_attr1",
      "eLSULoc_attr2",
      "eLSULoc_attr3",
      "eLSULoc_attr4",
      "eLSULoc_attr5",
      "eLSULoc_attr6"),
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"));

  Insert (result);
}

void
SimLsTriggered::R31Eca0Ins (Ptr<Tuple> tLSU)
{
  RAPIDNET_LOG_INFO ("R31Eca0Ins triggered");

  Ptr<Tuple> result = tLSU;

  result = result->Project (
    ELSUCHANGE,
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"),
    strlist ("eLSUChange_attr1",
      "eLSUChange_attr2",
      "eLSUChange_attr3",
      "eLSUChange_attr4",
      "eLSUChange_attr5",
      "eLSUChange_attr6"));

  SendLocal (result);
}

void
SimLsTriggered::R31Eca0Ref (Ptr<Tuple> tLSU)
{
  RAPIDNET_LOG_INFO ("R31Eca0Ref triggered");

  Ptr<Tuple> result = tLSU;

  result = result->Project (
    ELSUCHANGE,
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"),
    strlist ("eLSUChange_attr1",
      "eLSUChange_attr2",
      "eLSUChange_attr3",
      "eLSUChange_attr4",
      "eLSUChange_attr5",
      "eLSUChange_attr6"));

  SendLocal (result);
}

void
SimLsTriggered::R32_eca (Ptr<Tuple> eLSUChange)
{
  RAPIDNET_LOG_INFO ("R32_eca triggered");

  Ptr<Tuple> result = eLSUChange;

  result->Assign (Assignor::New ("TTLU",
    Operation::New (RN_MINUS,
      VarExpr::New ("eLSUChange_attr6"),
      ValueExpr::New (Int32Value::New (1)))));

  result->Assign (Assignor::New ("Broadcast",
    BROADCAST_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_GT,
      VarExpr::New ("eLSUChange_attr6"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    ELSU,
    strlist ("Broadcast",
      "eLSUChange_attr2",
      "eLSUChange_attr3",
      "eLSUChange_attr4",
      "eLSUChange_attr5",
      "TTLU",
      "Broadcast"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6",
      RN_DEST));

  Send (result);
}

