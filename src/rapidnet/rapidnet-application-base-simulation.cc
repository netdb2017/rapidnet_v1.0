/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2009 University of Pennsylvania
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
 */
#include "rapidnet-application-base.h"
#include <ctime>
#include "ns3/ptr.h"
#include "ns3/inet-socket-address.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-value.h"
#include "pki-authentication-manager.h"
#include "blowfish-encryption-manager.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

NS_LOG_COMPONENT_DEFINE ("RapidNetApplicationBase");
NS_OBJECT_ENSURE_REGISTERED (RapidNetApplicationBase);

TypeId
RapidNetApplicationBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::RapidNetApplicationBase")
    .SetParent<Application> ()
    ;
  return tid;
}

RapidNetApplicationBase::RapidNetApplicationBase ()
{
  NS_LOG_FUNCTION_NOARGS ();
  SetAddress (Ipv4Address ("0.0.0.0"));
  SetPort (4000);
  m_maxJitter = MAX_JITTER; // Use default
  m_database = Database::New (Ptr<RapidNetApplicationBase> (this));
  m_eventSoftStateDelete = Simulator::ScheduleNow (
    &RapidNetApplicationBase::SoftStateDelete, this);
  m_decoratorFrontend = Ptr<RapidNetDecoratorFrontend> (NULL);
}

RapidNetApplicationBase::~RapidNetApplicationBase()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<Database>
RapidNetApplicationBase::GetDatabase ()
{
  return m_database;
}

void
RapidNetApplicationBase::SetAddress (Ipv4Address addr)
{
  m_address = addr;
}

Ipv4Address
RapidNetApplicationBase::GetAddress (void)
{
  return m_address;
}

void
RapidNetApplicationBase::SetPort (uint16_t port)
{
  s_Port = port;
  RAPIDNET_LOG_INFO ("Port set to " << port);
}

uint16_t
RapidNetApplicationBase::GetPort ()
{
  return s_Port;
}

void
RapidNetApplicationBase::SetDecoratorFrontend (Ptr<
  RapidNetDecoratorFrontend> decorator)
{
  m_decoratorFrontend = decorator;
}

Ptr<RapidNetDecoratorFrontend>
RapidNetApplicationBase::GetDecoratorFrontend ()
{
  return m_decoratorFrontend;
}

void
RapidNetApplicationBase::Insert (Ptr<Tuple> tuple)
{
  if (tuple->IsEmpty ())
    {
      RAPIDNET_LOG_INFO ("ApplicationBase::Insert: Skipping empty tuple");
      return;
    }

  int status = m_database->Insert (tuple);
  RAPIDNET_LOG_INFO (GetInsertStatusAsString (status) << " " << tuple);
}

void
RapidNetApplicationBase::Insert (Ptr<RelationBase> reln)
{
  list<Ptr<Tuple> > tuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      Insert (*it);
    }
}

void
RapidNetApplicationBase::Delete (Ptr<Tuple> tuple)
{
  if (tuple->IsEmpty ())
    {
      RAPIDNET_LOG_INFO ("ApplicationBase::Delete: Skipping empty tuple");
      return;
    }

  RAPIDNET_LOG_INFO ("Deleting " << tuple);
  m_database->Delete (tuple);
}

void
RapidNetApplicationBase::Delete (Ptr<RelationBase> reln)
{
  list<Ptr<Tuple> > tuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      Delete (*it);
    }
}

Ptr<RelationBase>
RapidNetApplicationBase::GetRelation (string name)
{
  return m_database->GetRelation (name);
}

void
RapidNetApplicationBase::SetMaxJitter (uint32_t maxJitter)
{
  m_maxJitter = maxJitter;
}

void
RapidNetApplicationBase::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Application::DoDispose ();
}

void
RapidNetApplicationBase::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  InitDatabase ();
  InitSocket ();

  clog << "Application Started at " << Now () << endl;
}

void
RapidNetApplicationBase::SendlogInit ()
{
  m_authenticationMgr = PkiAuthenticationManager::New (
    Ptr<RapidNetApplicationBase> (this));
  m_encryptionMgr = BlowfishEncryptionManager::New (
    Ptr<RapidNetApplicationBase> (this));

  LogComponentEnable("BlowfishEncryptionManager", LOG_LEVEL_INFO);
  LogComponentEnable("PkiAuthenticationManager", LOG_LEVEL_INFO);
}

void
RapidNetApplicationBase::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_Socket != 0)
    {
      m_Socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  Simulator::Cancel (m_eventSoftStateDelete);
  clog << "Application Stopped at " << Now () << endl;
}

void
RapidNetApplicationBase::InitSocket (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_Socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_Socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
        s_Port);
      m_Socket->Bind (local);
    }

  m_Socket->SetRecvCallback (MakeCallback (&RapidNetApplicationBase::Receive,
    this));
}

void
RapidNetApplicationBase::Send (Ptr<Tuple> tuple)
{
  // Do nothing if empty tuple
  if (tuple->IsEmpty ())
    {
      return;
    }

  uint32_t jitter = m_maxJitter == 0 ? 0 : rand () % m_maxJitter;

  Simulator::Schedule (MilliSeconds (jitter), &RapidNetApplicationBase::DoSend,
    this, tuple);
}

void
RapidNetApplicationBase::Sign (Ptr<Tuple> tuple)
{
  // Do nothing if empty tuple
  if (tuple->IsEmpty ())
    {
      return;
    }

  m_authenticationMgr-> Sign (tuple);
}

bool
RapidNetApplicationBase::Verify (Ptr<Tuple> tuple, string attrName)
{
  // Do nothing if empty tuple
  if (tuple->IsEmpty ())
    {
      return false;
    }

  Ptr<TupleAttribute> attr = tuple->GetAttribute (attrName);
  return m_authenticationMgr->Verify (tuple, attr);
}

Ptr<Tuple>
RapidNetApplicationBase::Encrypt (Ptr<Tuple> tuple, string attrName)
{
  // Do nothing if empty tuple
  if (tuple->IsEmpty ())
    {
      return tuple;
    }

  Ptr<TupleAttribute> attr = tuple->GetAttribute (attrName);
  return m_encryptionMgr->Encrypt (tuple, attr);
}

Ptr<Tuple>
RapidNetApplicationBase::Decrypt (Ptr<Tuple> tuple)
{
  return m_encryptionMgr->Decrypt (tuple);
}

void
RapidNetApplicationBase::DoSend (Ptr<Tuple> tuple)
{
  if (!tuple->HasAttribute (RN_ACTION))
    {
      OnSend.Invoke (tuple);
    }

  Ipv4Address destIpv4 = ipv4_value(tuple->GetAttribute (RN_DEST));
  tuple->RemoveAttribute (RN_DEST);

  if (destIpv4 == m_address)
    {
      destIpv4 = HOME_IP;
    }

  InetSocketAddress addr = InetSocketAddress (destIpv4, s_Port);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (RapidNetHeader (tuple));

  if (!tuple->HasAttribute (RN_ACTION))
    {
      RAPIDNET_LOG_INFO ("Sending " << tuple << " to " << addr.GetIpv4 ());
    }
  m_Socket->SendTo (packet, 0, addr);
}

void
RapidNetApplicationBase::SendLocal (Ptr<Tuple> tuple)
{
  tuple->AddAttribute (TupleAttribute::New (RN_DEST, Ipv4Value::New (HOME_IP)));
  //No need to add jitter for local sending
  DoSend (tuple);
}

void
RapidNetApplicationBase::SendBroadcast (Ptr<Tuple> tuple)
{
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (RapidNetHeader (tuple));

  RAPIDNET_LOG_INFO ("Sending Broadcast ");
  m_Socket->SendTo (packet, 0, InetSocketAddress (Ipv4Address (
    "255.255.255.255"), s_Port));
}

void
RapidNetApplicationBase::Send (Ptr<RelationBase> reln)
{
  list<Ptr<Tuple> > tuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      Send (*it);
    }
}

void
RapidNetApplicationBase::Sign (Ptr<RelationBase> reln)
{
  list<Ptr<Tuple> > tuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;
  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      Sign (*it);
    }
}

Ptr<RelationBase>
RapidNetApplicationBase::Encrypt (Ptr<RelationBase> reln,
  string attrName)
{
  Ptr<RelationBase> retval = TempRelation::New (reln->GetName ());
  list<Ptr<Tuple> > tuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;
  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      retval->Insert (Encrypt (*it, attrName));
    }
  return retval;
}

void
RapidNetApplicationBase::SendLocal (Ptr<RelationBase> reln)
{
  list<Ptr<Tuple> > tuples = reln->GetAllTuples ();
  list<Ptr<Tuple> >::iterator it;

  for (it = tuples.begin (); it != tuples.end (); ++it)
    {
      SendLocal (*it);
    }
}

void
RapidNetApplicationBase::Receive (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while (packet = socket->RecvFrom (from))
    {
      if (InetSocketAddress::IsMatchingType (from))
        {
          RapidNetHeader header;
          packet->RemoveHeader (header);
          Ptr<Tuple> tuple = header.GetTuple ();

          Ipv4Address fromIpv4 =
              InetSocketAddress::ConvertFrom (from).GetIpv4 ();

          //received an encrypted tuple, decrypt it
          if (tuple->GetName () == SENDLOG_NAME_OPAQUE)
            {
              RAPIDNET_LOG_INFO ("Received encrypted tuple " << tuple <<
                " from " << fromIpv4);

              tuple = Decrypt (tuple);
            }

          if (!tuple->HasAttribute (RN_ACTION))
            {
              RAPIDNET_LOG_INFO ("Received " << tuple << " from " << fromIpv4);
            }

          //Add -src attribute
          tuple->OverwriteAttribute (TupleAttribute::New (RN_SRC,
            Ipv4Value::New (fromIpv4)));
          DemuxRecv (tuple);
        }
    }
}

Ptr<Tuple>
RapidNetApplicationBase::CreateNewTuple (string name)
{
  return Tuple::New (name);
}

Ptr<Relation>
RapidNetApplicationBase::CreateNewRelation (string name)
{
  Ptr<RelationBase> reln = Relation::New (name);

  Ptr<InsertTrigger> insTrigger = Create<InsertTrigger> ();
  insTrigger->SetApplication (Ptr<RapidNetApplicationBase> (this));
  reln->OnInsert += insTrigger;

  Ptr<DeleteTrigger> delTrigger = Create<DeleteTrigger> ();
  delTrigger->SetApplication (Ptr<RapidNetApplicationBase> (this));
  reln->OnDelete += delTrigger;

  Ptr<RefreshTrigger> refTrigger = Create<RefreshTrigger> ();
  refTrigger->SetApplication (Ptr<RapidNetApplicationBase> (this));
  reln->OnRefresh += refTrigger;

  return Ptr<Relation> (dynamic_cast<Relation*> (PeekPointer (reln)));
}

void
RapidNetApplicationBase::AddRelationWithKeys (string relnName,
  list<attrdeftype> attributes, Time ttl)
{
  Ptr<Relation> reln = CreateNewRelation (relnName);
  list<attrdeftype>::iterator it;

  for (it = attributes.begin (); it != attributes.end (); it++)
    {
      reln->AddKeyAttribute (*it);
    }
  reln->SetTimeToLive (ttl);
  m_database->AddRelation (reln);
}

void
RapidNetApplicationBase::AddHeapRelationWithKeys (string relnName,
  list<attrdeftype> attributes, string heapAttr, HeapRelation::Type type,
  uint32_t size, Time ttl)
{
  Ptr<HeapRelation> reln = HeapRelation::New (relnName, heapAttr, type, size);

  Ptr<InsertTrigger> insTrigger = Create<InsertTrigger> ();
  insTrigger->SetApplication (Ptr<RapidNetApplicationBase> (this));
  reln->OnInsert += insTrigger;

  Ptr<DeleteTrigger> delTrigger = Create<DeleteTrigger> ();
  delTrigger->SetApplication (Ptr<RapidNetApplicationBase> (this));
  reln->OnDelete += delTrigger;

  Ptr<RefreshTrigger> refTrigger = Create<RefreshTrigger> ();
  refTrigger->SetApplication (Ptr<RapidNetApplicationBase> (this));
  reln->OnRefresh += refTrigger;

  list<attrdeftype>::iterator it;

  for (it = attributes.begin (); it != attributes.end (); it++)
    {
      reln->AddKeyAttribute (*it);
    }
  reln->SetTimeToLive (ttl);
  m_database->AddRelation (reln);
}

void
RapidNetApplicationBase::DemuxRecv (Ptr<Tuple> tuple)
{
  if (IsInsertEvent (tuple))
    {
      OnInsert.Invoke (tuple);
    }
  if (IsDeleteEvent (tuple))
    {
      OnDelete.Invoke (tuple);
    }
  if (IsRecvEvent (tuple))
    {
      OnRecv.Invoke (tuple);
    }
}

#define IS_EXTERNAL(tuple) (!tuple->HasAttribute (RN_SRC))

#define IS_INTERNAL(tuple) \
   (ipv4_value(tuple->GetAttribute (RN_SRC)) == m_address || \
   ipv4_value (tuple->GetAttribute (RN_SRC)) == HOME_IP)

bool
RapidNetApplicationBase::IsInsertEvent (Ptr<Tuple> tuple)
{
  bool isInsert = tuple->HasAttribute (RN_ACTION) && tuple->GetAttribute (
    RN_ACTION)->GetValue ()->ToString () == RN_INSERT;

  return IS_INTERNAL(tuple) && isInsert;
}

bool
RapidNetApplicationBase::IsInsertEvent (Ptr<Tuple> tuple, string name)
{
  bool isNameMatch = tuple->GetName () == name;

  return isNameMatch && IsInsertEvent (tuple);
}

bool
RapidNetApplicationBase::IsDeleteEvent (Ptr<Tuple> tuple)
{
  bool isDelete = tuple->HasAttribute (RN_ACTION) && tuple->GetAttribute (
    RN_ACTION)->GetValue ()->ToString () == RN_DELETE;

  return IS_INTERNAL(tuple) && isDelete;
}

bool
RapidNetApplicationBase::IsDeleteEvent (Ptr<Tuple> tuple, string name)
{
  bool isNameMatch = tuple->GetName () == name;

  return isNameMatch && IsDeleteEvent (tuple);
}

bool
RapidNetApplicationBase::IsRefreshEvent (Ptr<Tuple> tuple, string name)
{
  bool isNameMatch = tuple->GetName () == name;

  bool isRefresh = tuple->HasAttribute (RN_ACTION) && tuple->GetAttribute (
    RN_ACTION)->GetValue ()->ToString () == RN_REFRESH;

  return isNameMatch && IS_INTERNAL(tuple) && isRefresh;
}

bool
RapidNetApplicationBase::IsRecvEvent (Ptr<Tuple> tuple)
{
  return tuple->HasAttribute (RN_SRC)
      && ipv4_value (tuple->GetAttribute (RN_SRC)) != m_address;
}

bool
RapidNetApplicationBase::IsRecvEvent (Ptr<Tuple> tuple, string name)
{
  return tuple->GetName () == name && IsRecvEvent (tuple);
}

void
RapidNetApplicationBase::SoftStateDelete ()
{
  //RAPIDNET_LOG_INFO ("Checking for timed out tuples");
  map<string, Ptr<RelationBase> > relations = m_database->GetAllRelations ();
  Time now = Simulator::Now ();

  for (map<string, Ptr<RelationBase> >::iterator it = relations.begin ();
    it != relations.end (); it ++)
    {
      Ptr<RelationBase> relation = it->second;
      Time ttl = relation->GetTimeToLive ();
      list<Ptr<Tuple> > tuples = relation->GetAllTuples ();
      for (list<Ptr<Tuple> >::iterator jt = tuples.begin (); jt
          != tuples.end (); ++jt)
        {
          if (HasTimedout ((*jt)->GetTimestamp (), ttl, now))
            {
              RAPIDNET_LOG_INFO ("Timed out " << *jt << " timestamp: " << (*jt)->GetTimestamp ());
              Delete (*jt);
            }
        }
    }

  m_eventSoftStateDelete = Simulator::Schedule (SOFTSTATE_DELETE_PERIOD,
    &RapidNetApplicationBase::SoftStateDelete, this);
}

void
InsertTrigger::Invoke (Ptr<Tuple> tuple)
{
  tuple->OverwriteAttribute (TupleAttribute::New (RN_ACTION, StrValue::New (
    RN_INSERT)));
  GetApplication ()->SendLocal (tuple);
}

void
DeleteTrigger::Invoke (Ptr<Tuple> tuple)
{
  tuple->OverwriteAttribute (TupleAttribute::New (RN_ACTION, StrValue::New (
    RN_DELETE)));
  GetApplication ()->SendLocal (tuple);
}

void
RefreshTrigger::Invoke (Ptr<Tuple> tuple)
{
  tuple->OverwriteAttribute (TupleAttribute::New (RN_ACTION, StrValue::New (
    RN_REFRESH)));
  GetApplication ()->SendLocal (tuple);
}
