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
//#include "ns3/udp-transport-socket-impl.h"
#include "ns3/udp-socket-factory.h"
//#include "ns3/udp-transport-socket-factory-impl.h"
#include "ns3/uinteger.h"

#include <fstream> // add-on
#include <sstream> //add-on
#include  <time.h>
#include  <sys/time.h>

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
    .AddConstructor<RapidNetApplicationBase> ()
    .AddAttribute ("RapidNetPort",
                   "Port for RapidNet application",
                   UintegerValue (11111),
                   MakeUintegerAccessor (&RapidNetApplicationBase::s_Port),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("MaxUDPTxSize",
                   "Max size of UDP packet, beyond which switch is made to TCP protocol",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&RapidNetApplicationBase::m_udpMaxBytes),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("ConnectionInactivityTimeout",
                   "Timeout value for tearing down inactive TCP connections.",
                   TimeValue (MilliSeconds (DEFAULT_CONNECTION_INACTIVITY_TIMEOUT)),
                   MakeTimeAccessor (&RapidNetApplicationBase::m_tcpInactivityTimeout),
                   MakeTimeChecker ())
    ;
  return tid;
}

RapidNetApplicationBase::RapidNetApplicationBase ()
  : m_auditTCPConnectionsTimer (Timer::CANCEL_ON_DESTROY)
{
  NS_LOG_FUNCTION_NOARGS ();
  BytesOfDataReceived = 0;
  BytesOfDataSent = 0;
  totalPacketsReceived = 0;
  totalPacketsSent = 0;
  SetAddress (Ipv4Address ("0.0.0.0"));
  SetPort (11111);
  //m_maxJitter = MAX_JITTER; // Use default
  m_maxJitter = 0;
  m_database = Database::New (Ptr<RapidNetApplicationBase> (this));
  m_eventSoftStateDelete = Simulator::ScheduleNow (
    &RapidNetApplicationBase::SoftStateDelete, this);
  m_decoratorFrontend = Ptr<RapidNetDecoratorFrontend> (NULL);
  m_pendingTuples.clear();
  m_processingPendingTuples = false;
  m_l4Platform = false;
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

Ipv4Address
RapidNetApplicationBase::GetIpv4Address (string locspec)
{
  std::vector<std::string> tokens;
  Tokenize (locspec, tokens, ":");
  if (tokens.size() == 0)
    return "0.0.0.0";
  std::vector<std::string>::iterator iter = tokens.begin();
  return Ipv4Address(iter->c_str());
}

uint16_t
RapidNetApplicationBase::GetPort (string locspec)
{
  if(m_l4Platform)
    {
      std::vector<std::string> tokens;
      Tokenize (locspec, tokens, ":");
      if (tokens.size() == 0)
        return 0;
      std::vector<std::string>::iterator iter = tokens.begin();
      iter++;
      return atoi(iter->c_str());
    }
  else
    {
      return s_Port;
    }
}

string
RapidNetApplicationBase::GetLocSpec (Ipv4Address ipAddr, uint16_t port)
{
  std::stringstream sstream;
  sstream << ipAddr << ":" << port;
  return sstream.str ();
}

void
RapidNetApplicationBase::SetPort (uint16_t port)
{
  s_Port = port;
  RAPIDNET_LOG_INFO ("Port set to " << port);
}

string
RapidNetApplicationBase::GetLocalLocSpec (void)
{
  return GetLocSpec (GetAddress(), GetPort());
}

uint16_t
RapidNetApplicationBase::GetPort ()
{
  return s_Port;
}



void
RapidNetApplicationBase::AddToAddressList (Ipv4Address addr)
{
  m_addressList.push_back(addr);
}

void
RapidNetApplicationBase::AddInterface (Ipv4Address addr, uint32_t deviceIndex, const string & name)
{
  if (deviceIndex == 0)
  {
    SetAddress(addr); //For CCC
    m_addressToDeviceMap[m_address] = deviceIndex;
    m_deviceNameMap.insert (make_pair (deviceIndex, name));
    m_numDevices ++;
  }
  else
  {
            m_addressList.push_back (addr);
            m_deviceNameList.push_back (name);
            //Build map right now
            m_addressToDeviceMap[addr] = deviceIndex;
            m_deviceNameMap.insert (make_pair (deviceIndex, name));
            m_numDevices ++;
  }
}


vector<Ipv4Address>
RapidNetApplicationBase::GetAddressList (void) const
{
  return m_addressList;
}

void
RapidNetApplicationBase::BuildAddressToDeviceMap()
{
  uint32_t deviceIndex = 0;

  // device 0 is reserved for CCC, as done in InstallWifi and InstallIpv4
  m_addressToDeviceMap[m_address] = deviceIndex;
  deviceIndex++;

  // left devices are for data
  for (uint32_t i = 0; i < m_addressList.size(); i++)
  {
         m_addressToDeviceMap[m_addressList[i] ] = deviceIndex;
        deviceIndex++;
  }

  // 1 CCC and others are data devices (The Loopback device is not counted)
  m_numDevices = 1 + m_addressList.size ();
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

  int status;
  status = m_database->Insert (tuple);
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
RapidNetApplicationBase::ClearRelation (std::string relName)
{
  Ptr<RelationBase> relation = GetRelation (relName);
  if (relation != NULL)
  {
    relation->ClearAllTuples();
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
  if(m_l4Platform)
    {
        NS_LOG_FUNCTION_NOARGS ();
        if (m_tcpSocket != 0)
          {
            m_tcpSocket->SetAcceptCallback (
                                            MakeNullCallback<bool, Ptr<Socket>, const Address & > (),
                                            MakeNullCallback<void, Ptr<Socket>, const Address &> ());
          }
        // Cancel timers
        m_auditTCPConnectionsTimer.Cancel ();
        Application::DoDispose ();
    }
  else
    {
      NS_LOG_FUNCTION_NOARGS ();
      Application::DoDispose ();
    }
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
      HandleClose(m_Socket);
      m_Socket->Close();
    }
  Simulator::Cancel (m_eventSoftStateDelete);
  clog << "Application Stopped at " << Now () << endl;
  Simulator::Stop ();
  //exit(0);
}

void
RapidNetApplicationBase::InitSocket (void)
{
  if(m_l4Platform)
    {

      NS_LOG_FUNCTION_NOARGS ();
  
      if (m_Socket == 0)
        {
          TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
          m_Socket = Socket::CreateSocket (GetNode (), tid);
          InetSocketAddress local = InetSocketAddress (m_address,
                                                       s_Port);
          m_Socket->Bind (local);
          NS_LOG_INFO ("RapidNet UDP socket listening on address: " << m_address << " port: " << s_Port);
        }
      
      m_Socket->SetRecvCallback (MakeCallback (&RapidNetApplicationBase::Receive,
                                               this));
      // Open a TCP socket, listening on s_Port+1
      if (m_tcpSocket == 0)
        {
          TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
          m_tcpSocket = Socket::CreateSocket (GetNode (), tid);
          uint16_t s_tcpPort;
          if(s_Port == 0)
            s_tcpPort = 0;
          else
            s_tcpPort = s_Port + 1;
          InetSocketAddress local = InetSocketAddress (m_address, s_tcpPort);
          m_tcpSocket->Bind (local);
          m_tcpSocket->SetAcceptCallback (
                                          MakeCallback (&RapidNetApplicationBase::HandleConnectionRequest, this),
                                          MakeCallback (&RapidNetApplicationBase::HandleAccept, this));
          NS_LOG_INFO ("RapidNet TCP socket listening on address: " << m_address << " port: " << s_tcpPort);
        }
      else
        {
          m_tcpSocket->SetAcceptCallback (
                                          MakeCallback (&RapidNetApplicationBase::HandleConnectionRequest, this),
                                          MakeCallback (&RapidNetApplicationBase::HandleAccept, this));
        }
      // Start listening
      m_tcpSocket->Listen ();
      // Audit connections timer init
      m_auditTCPConnectionsTimer.SetFunction (&RapidNetApplicationBase::DoPeriodicAuditConnections, this);
      m_auditTCPConnectionsTimer.Schedule (m_tcpInactivityTimeout);
    }
  else
    {
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
}

void
RapidNetApplicationBase::Send (Ptr<Tuple> tuple)
{
  // Do nothing if empty tuple
  if (tuple->IsEmpty ())
    {
      return;
    }
  
  if(m_l4Platform)
    {
      DoSend(tuple);
    }
  else
    {

      uint32_t jitter = m_maxJitter == 0 ? 0 : rand () % m_maxJitter;

      Simulator::Schedule (MilliSeconds (jitter), &RapidNetApplicationBase::DoSend,
                           this, tuple);
    }
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
RapidNetApplicationBase::PrintStats ()
{
  cout<<"Total Bytes Received = "<<BytesOfDataReceived<<endl;
  cout<<"Total Bytes Sent = "<<BytesOfDataSent<<endl;
  cout<<"Total Packets Received = "<<totalPacketsReceived<<endl;
  cout<<"Total Packets Sent = "<<totalPacketsSent<<endl;

  cout<<"*********************************************************"<<endl;
}

void
RapidNetApplicationBase::DoSend (Ptr<Tuple> tuple)
{

  if(m_l4Platform)
    {

      Ptr<TupleAttribute> attribute = tuple->GetAttribute(RN_DEST);
      Ptr<Value> value = attribute->GetValue();
      
      string destLocSpec = value->ToString();

      Ipv4Address destIpv4 = GetIpv4Address (destLocSpec);

      uint16_t destPort = GetPort (destLocSpec);
      
      tuple->RemoveAttribute (RN_DEST);
      
      if (destIpv4 == HOME_IP && destPort == GetPort ())
        {
          SendLocal (tuple);
          return;
        }
      
      if (!tuple->HasAttribute (RN_ACTION))
        {
          OnSend.Invoke (tuple);
        }


      InetSocketAddress addr = InetSocketAddress (destIpv4, destPort);
      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader (RapidNetHeader (tuple));
      
      if (!tuple->HasAttribute (RN_ACTION))
        {
          RAPIDNET_LOG_INFO ("Sending " << tuple << " to " << addr.GetIpv4 ());
        }


      SendOverTCP (addr.GetIpv4 (), addr.GetPort ()+1, packet);

    }
  else
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
  
}

void
RapidNetApplicationBase::SendLocal (Ptr<Tuple> tuple)
{
  if(m_l4Platform)
    {
      string localLocSpec = GetLocSpec (HOME_IP, GetPort());
      tuple->AddAttribute (TupleAttribute::New (RN_DEST, StrValue::New (localLocSpec)));
      //No need to add jitter for local sending
      //DoSend (tuple);
      if (!tuple->HasAttribute (RN_ACTION))
        {
          OnSend.Invoke (tuple);
        }
      //TODO : atomic async execution
      //Simulator::ScheduleNow (&RapidNetApplicationBase::ProcessTuple, this, tuple, InetSocketAddress(HOME_IP, GetPort()));
      //ProcessTuple (tuple, InetSocketAddress (HOME_IP, GetPort ()));
      m_pendingTuples.push_back (tuple);
      if (!m_processingPendingTuples)
        {
          ProcessPendingTuples ();
        }
    }
  else
    {
      tuple->AddAttribute (TupleAttribute::New (RN_DEST, Ipv4Value::New (HOME_IP)));
      DoSend(tuple);
    }
  
}

void
RapidNetApplicationBase::ProcessPendingTuples ()
{
  m_processingPendingTuples = true;
  while (m_pendingTuples.size ())
  {
    Ptr<Tuple> tuple = m_pendingTuples.front ();
    m_pendingTuples.pop_front ();
    ProcessTuple (tuple, InetSocketAddress (HOME_IP, GetPort ()));
  }
  m_processingPendingTuples = false;
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

  if(m_l4Platform)
    {
      NS_LOG_FUNCTION (this << socket);
      NS_LOG_INFO ("RapidNetApplicationBase::Receive on UDP");
      Ptr<Packet> packet;
      Address from;
      while (packet = socket->RecvFrom (from))
        {
          if (InetSocketAddress::IsMatchingType (from))
            {
              RapidNetHeader header;
              packet->RemoveHeader (header);
              Ptr<Tuple> tuple = header.GetTuple ();
              ProcessTuple (tuple, from);
              totalPacketsReceived++;
              BytesOfDataReceived += packet->GetSize(); 
              
            }
        }
    }
  else
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
}

void
RapidNetApplicationBase::ProcessTCPMessage (Ptr<Packet> packet, Ptr<RapidNetTCPConnection> tcpConnection)
{
  totalPacketsReceived++;
  BytesOfDataReceived += packet->GetSize(); 
  NS_LOG_INFO ("RapidNetApplicationBase::ProcessTCPMessage: Packet size = " << packet->GetSize ());
  Address from = InetSocketAddress (tcpConnection->GetIpAddress (), tcpConnection->GetPort ()-1);
  RapidNetHeader header;
  packet->RemoveHeader (header);
  Ptr<Tuple> tuple = header.GetTuple ();
  ProcessTuple (tuple, from);
}

void
RapidNetApplicationBase::ProcessTuple (Ptr<Tuple> tuple, Address from)
{
  Ipv4Address fromIpv4 =
      InetSocketAddress::ConvertFrom (from).GetIpv4 ();
  uint16_t fromPort = InetSocketAddress::ConvertFrom(from).GetPort ();

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
  string srcLocSpec = GetLocSpec (fromIpv4, fromPort);
  tuple->OverwriteAttribute (TupleAttribute::New (RN_SRC,
    StrValue::New (srcLocSpec)));
  DemuxRecv (tuple);
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

#define IS_INTERNAL_L4(tuple) \
   ((GetIpv4Address(str_value(tuple->GetAttribute (RN_SRC))) == m_address || \
   GetIpv4Address(str_value (tuple->GetAttribute (RN_SRC))) == HOME_IP) && \
    (GetPort(str_value (tuple->GetAttribute (RN_SRC))) == s_Port))

#define IS_INTERNAL_SIMULATION(tuple)\
   (ipv4_value(tuple->GetAttribute (RN_SRC)) == m_address || \
   ipv4_value (tuple->GetAttribute (RN_SRC)) == HOME_IP)

bool
RapidNetApplicationBase::IsInsertEvent (Ptr<Tuple> tuple)
{
  bool isInsert = tuple->HasAttribute (RN_ACTION) && tuple->GetAttribute (
    RN_ACTION)->GetValue ()->ToString () == RN_INSERT;

  if(m_l4Platform)
    return IS_INTERNAL_L4(tuple) && isInsert;
  else
    return IS_INTERNAL_SIMULATION(tuple) && isInsert;
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

  if(m_l4Platform)
    return IS_INTERNAL_L4(tuple) && isDelete;
  else
    return IS_INTERNAL_SIMULATION(tuple) && isDelete;
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

  if(m_l4Platform)
    return isNameMatch && IS_INTERNAL_L4(tuple) && isRefresh;
  else
    return isNameMatch && IS_INTERNAL_SIMULATION(tuple) && isRefresh;
}

bool
RapidNetApplicationBase::IsRecvEvent (Ptr<Tuple> tuple)
{
  if(m_l4Platform)
    return tuple->HasAttribute (RN_SRC)
      && GetIpv4Address(str_value (tuple->GetAttribute (RN_SRC))) != m_address;
  else
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
	//Lin: fix a bug in a3 here
  //if(!m_l4Platform)
  GetApplication ()->SendLocal (tuple);
}

void
RefreshTrigger::Invoke (Ptr<Tuple> tuple)
{
  tuple->OverwriteAttribute (TupleAttribute::New (RN_ACTION, StrValue::New (
    RN_REFRESH)));
  GetApplication ()->SendLocal (tuple);
}

void 
RapidNetApplicationBase::Tokenize(const std::string& str,
    std::vector<std::string>& tokens,
    const std::string& delimiters)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

bool 
RapidNetApplicationBase::HandleConnectionRequest (Ptr<Socket> socket, const Address& address)
{
  // Accept all connections
  return true;
}

void
RapidNetApplicationBase::HandleAccept (Ptr<Socket> socket, const Address& address)
{
  InetSocketAddress from = InetSocketAddress::ConvertFrom (address);
  AddConnection (socket, from.GetIpv4(), from.GetPort(), RapidNetTCPConnection::CONNECTED);
}

void
RapidNetApplicationBase::HandleClose (Ptr<Socket> socket)
{
  // Remove from connection list
  RemoveConnection (socket);
}


void
RapidNetApplicationBase::HandleConnectSuccess (Ptr<Socket> socket)
{
  Ptr<RapidNetTCPConnection> connection = FindConnection (socket);
  if (connection != NULL)
  {
    RAPIDNET_LOG_INFO ("RapidNet TCP connection established!");
    connection->SetConnState (RapidNetTCPConnection::CONNECTED);
    //Start transmission
    //connection->WriteTCPBuffer (connection->GetSocket (), connection->GetSocket ()->GetTxAvailable ());
  }
}

void
RapidNetApplicationBase::HandleConnectFailure (Ptr<Socket> socket)
{
  RAPIDNET_LOG_INFO("Rapidnet TCP connection failed to be established!");
  HandleClose (socket);
}

Ptr<RapidNetTCPConnection>
RapidNetApplicationBase::AddConnection (Ptr<Socket> socket, Ipv4Address ipAddress, uint16_t port, RapidNetTCPConnection::ConnectionState connState)
{
  Ptr<RapidNetTCPConnection> rapidNetTCPConnection = Create<RapidNetTCPConnection> (ipAddress, port, socket);
  rapidNetTCPConnection->SetRecvCallback (MakeCallback (&RapidNetApplicationBase::ProcessTCPMessage, this));
  rapidNetTCPConnection->SetConnState (connState);
  socket->SetCloseCallbacks (MakeCallback (&RapidNetApplicationBase::HandleClose, this),
                             MakeCallback (&RapidNetApplicationBase::HandleClose, this));
  socket->SetConnectCallback (MakeCallback (&RapidNetApplicationBase::HandleConnectSuccess, this),
                              MakeCallback (&RapidNetApplicationBase::HandleConnectFailure, this));
  // Add new connection to map
  m_tcpConnectionTable.insert (std::make_pair (socket, rapidNetTCPConnection));
  return rapidNetTCPConnection;
}

Ptr<RapidNetTCPConnection> 
RapidNetApplicationBase::FindConnection (Ptr<Socket> socket)
{
  TCPConnectionMap::iterator iterator = m_tcpConnectionTable.find (socket);
  if (iterator == m_tcpConnectionTable.end ())
    {
      return NULL;
    }
  return (*iterator).second;
}

Ptr<RapidNetTCPConnection>
RapidNetApplicationBase::FindConnection (Ipv4Address ipAddress, uint16_t port) 
{
  for (TCPConnectionMap::iterator iterator = m_tcpConnectionTable.begin (); iterator != m_tcpConnectionTable.end (); iterator++)
    {
      Ptr<RapidNetTCPConnection> connection = (*iterator).second;
      if (connection->GetIpAddress () == ipAddress && connection->GetPort () == port)
        {
          // Connection found
          return connection;
        }
    }
  return NULL;
}

void
RapidNetApplicationBase::RemoveConnection (Ptr<Socket> socket)
{
  TCPConnectionMap::iterator iterator = m_tcpConnectionTable.find (socket);
  if (iterator == m_tcpConnectionTable.end ())
    {
      return;
    }

  m_tcpConnectionTable.erase (iterator);
  
  return;
}

void
RapidNetApplicationBase::DoPeriodicAuditConnections ()
{
  // Remove inactive connections
  for (TCPConnectionMap::iterator iterator = m_tcpConnectionTable.begin (); iterator != m_tcpConnectionTable.end (); )
    {
      Ptr<RapidNetTCPConnection> tcpConnection = (*iterator).second;
      if ((tcpConnection->GetLastActivityTime ().GetMilliSeconds () + m_tcpInactivityTimeout.GetMilliSeconds ()) < Simulator::Now ().GetMilliSeconds ())
        {
          // Remove from table
          m_tcpConnectionTable.erase (iterator++);
        }
      else
        {
          ++iterator;
        }
    }
  // Restart timer
  m_auditTCPConnectionsTimer.Schedule (m_tcpInactivityTimeout);
}

void
RapidNetApplicationBase::SendOverTCP (Ipv4Address ipAddress, uint16_t port, Ptr<Packet> packet)
{
  if (packet->GetSize ())
  {

    totalPacketsSent++; 
    BytesOfDataSent += packet->GetSize();
    // Check for existing connections
    Ptr<RapidNetTCPConnection> connection = FindConnection (ipAddress, port);
    if (connection == NULL)
    {
      // Open new connection
      TypeId tid = TypeId::LookupByName ("ns3::TcpSocketFactory");
      Ptr<Socket> socket = Socket::CreateSocket (GetNode (), tid);
      connection = AddConnection (socket, ipAddress, port, RapidNetTCPConnection::NOT_CONNECTED);
      socket->Bind ();
      socket->Connect (InetSocketAddress (ipAddress, port));
    }

    connection->SendTCPData (packet);
        
  }
}

void
RapidNetApplicationBase::SetL4Platform (bool l4Platform)
{
  m_l4Platform = l4Platform;
}

BOOST_CLASS_EXPORT_IMPLEMENT(ns3::rapidnet::AppTrigger)
BOOST_CLASS_EXPORT_IMPLEMENT(ns3::rapidnet::InsertTrigger)
BOOST_CLASS_EXPORT_IMPLEMENT(ns3::rapidnet::DeleteTrigger)
BOOST_CLASS_EXPORT_IMPLEMENT(ns3::rapidnet::RefreshTrigger)
