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

#ifndef RAPIDNET_APPLICATION_BASE_H_
#define RAPIDNET_APPLICATION_BASE_H_

#include <string>
#include <iostream>
#include <list>
#include <ctime>
#include "ns3/log.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/nstime.h"
#include "ns3/timer.h"
#include "ns3/simulator.h"
#include "rapidnet-header.h"
#include "tuple.h"
#include "database.h"
#include "relation-base.h"
#include "trigger.h"
#include "rapidnet-utils.h"
#include "rapidnet-decorator-frontend.h"
#include "temp-relation.h"
#include "heap-relation.h"
#include "sendlog-authentication-manager.h"
#include "sendlog-encryption-manager.h"
#include "rapidnet-tcp-connection.h"
#include "ns3/event-impl.h"

#define RAPIDNET_LOG(level,msg) \
  NS_LOG(level, '['<< Simulator::Now() << "] " << m_address << ": " << msg)

#define RAPIDNET_LOG_INFO(msg) RAPIDNET_LOG(ns3::LOG_INFO, msg)

#define RAPIDNET_LOG_WARN(msg) RAPIDNET_LOG(ns3::LOG_WARN, msg)

#define RAPIDNET_ASSERT_MSG(cond, msg) \
  NS_ASSERT_MSG(cond, m_address << ": " << msg)

#define ipv4_value(attr) rn_ipv4 (attr->GetValue ())
#define str_value(attr) rn_str (attr->GetValue ())
/* Static defines */
#define DEFAULT_CONNECTION_INACTIVITY_TIMEOUT 10000

using namespace std;

namespace ns3 {

class Socket;
class Packet;

/**
 * \brief RapidNet library
 */
namespace  rapidnet {

class SendlogAuthenticationManager;
class SendlogEncryptionManager;

const string RN_SRC = "rn-src";
const string RN_DEST = "rn-dest";
const string RN_ACTION = "rn-action";
const string RN_INSERT = "rn-insert";
const string RN_DELETE = "rn-delete";
const string RN_REFRESH = "rn-refresh";
const Ipv4Address HOME_IP = Ipv4Address::GetLoopback ();
const Time SOFTSTATE_DELETE_PERIOD = Seconds (1.0);

class Tuple;
class Database;
class RapidNetApplicationBase;
class AppTrigger;

/**
 * \ingroup rapidnet
 * \defgroup rapidnet_library RapidNet Library
 */

/**
 * \ingroup rapidnet_library
 *
 * \brief A base class for all RapidNet applications.
 *
 * The basic book-keeping common to all RapidNet applications is implemented
 * here. This includes a database, database triggers, methods to send and
 * receive tuples
 *
 */
class RapidNetApplicationBase : public Application
{
public:

  static TypeId GetTypeId (void);
  
  RapidNetApplicationBase ();

  virtual ~RapidNetApplicationBase ();

  /*Set L4 Platform*/
  void SetL4Platform(bool l4Platform);
  
  /*
   * \brief Returns the associated database object.
   */
  Ptr<Database> GetDatabase();

  /**
   * \brief Sets the IP address for this application instance.
   */
  virtual void SetAddress (Ipv4Address addr);

  /**
   * \brief Returns the IP address of this application instance.
   */
  virtual Ipv4Address GetAddress (void);

  /*
   * \brief Gets Ipv4Address from LocSpec.
   */
  Ipv4Address GetIpv4Address (string locspec);

  /*
   * \brief Gets Port number from LocSpec.
   */
  uint16_t GetPort (string locspec);

  /*
   * \brief Get location specifier from IpAddress and port number.
   */
  string GetLocSpec (Ipv4Address ipAddr, uint16_t port);

  /**
   * \brief Sets the port number on which the application communicates.dd IP address to the m_addressList which is for data on
   *  its devices/interfaces
   */
  virtual void AddToAddressList (Ipv4Address addr);
  virtual void AddInterface (Ipv4Address addr, uint32_t deviceIndex, const std::string & name);
  /**
   * \brief Returns the IP m_addressList of this application instance.
   */
  virtual vector<Ipv4Address> GetAddressList (void) const;

  /**
   * \brief Map Ipv4Address (m_address and m_addressList) to Device Index
   * number of devices for this node/application
   * 1 Ipv4Address <==> 1 Interface <==> 1 Device
   */
  virtual void BuildAddressToDeviceMap();


  /**
   * \brief Sets the port number on which the application communicates.
   */
  virtual void SetPort (uint16_t port);
  /**
   * \brief Gets the local LocSpec (Ipv4Address:Port) for this application instance.
   */
  virtual string GetLocalLocSpec (void);

  /**
   * \brief Returns the port number on which the application communicates.
   */
  virtual uint16_t GetPort ();

  /**
  * \brief Inserts a new tuple to the application database.
  */
  virtual void Insert (Ptr<Tuple> tuple);

  /**
   * \brief Injects the tuple into this application.
   *
   * This can be invoked explicitly to simulate a network event being received.
   */
  virtual void Inject (Ptr<Tuple> tuple)
  {
    SendLocal (tuple);
  }

  void SetDecoratorFrontend (Ptr<RapidNetDecoratorFrontend> decorator);

  Ptr<RapidNetDecoratorFrontend> GetDecoratorFrontend ();

  /**
   * \brief Inserts all tuples of the given relation.
   */
  virtual void Insert (Ptr<RelationBase> reln);

  /**
   * \brief Clear a RapidNet relation table
   */

  virtual void ClearRelation (std::string relName);

  /**
   * \brief Deletes the given tuple from the application database.
   */
  virtual void Delete (Ptr<Tuple> tuple);

  /**
   * \brief Deletes the given relation from the application database.
   */
  virtual void Delete (Ptr<RelationBase> reln);

  /**
   * \brief Returns the relation in the application database with the
   *        given name. Throws an error if it does not exist.
   */
  virtual Ptr<RelationBase> GetRelation (string name);

  /**
   * \brief Sets the maximum value of the jitter in milliseconds.
   */
  void SetMaxJitter (uint32_t maxJitter);

  /**
   * \brief Serialize the designated relation
   */
  // virtual void SerializeRel(vector<string>& relNames, int nodeID, string storePath);
  
  
  // TCP processing
  void ProcessTCPMessage (Ptr<Packet> packet, Ptr<RapidNetTCPConnection> tcpConnection);

  // TCP Callbacks
  bool HandleConnectionRequest (Ptr<Socket> socket, const Address& address);
  void HandleAccept (Ptr<Socket> socket, const Address& address);
  void HandleClose (Ptr<Socket> socket);

  // Traffic sockets
  void HandleConnectSuccess (Ptr<Socket> socket);
  void HandleConnectFailure (Ptr<Socket> socket);

  // Periodic processes
  void DoPeriodicAuditConnections ();

  // Pending Tuples
  void ProcessPendingTuples ();



  /*
   *  \brief Default maximum jitter in milliseconds.
   */
  static const uint32_t MAX_JITTER = 500;

  /**
   * \brief Application specific send triggers can be added to this.
   */
  TriggerList OnSend;

  /**
   * \brief Application specific recv triggers can be added to this.
   */
  TriggerList OnRecv;

  /**
   * \brief Application specific insert triggers can be added to this.
   */
  TriggerList OnInsert;

  /**
   * \brief Application specific delete triggers can be added to this.
   */
  TriggerList OnDelete;

  void SendOverTCP (Ipv4Address ipAddress, uint16_t port, Ptr<Packet> packet);
  void PrintStats();

  /*
   *  \brief Total number of packets sent. 
   */
  uint32_t totalPacketsSent;
  
  /*
   *  \brief Total Number of packets received. 
   */
  uint32_t totalPacketsReceived;
  
  /*
   *  \brief Total bytes of Packets being sent/received.
   */
  uint32_t BytesOfDataSent;
  
  /*
   *  \brief Total bytes of Packets being sent/received.
   */
  uint32_t BytesOfDataReceived;

protected:

  /**
   * \brief Clean up after stopping.
   */
  virtual void DoDispose (void);

  /**
   * \brief Initializes and starts the application.
   */
  virtual void StartApplication (void);

  /**
   * \brief Stops the application.
   */
  virtual void StopApplication (void);

  /**
  * \brief Application should use this to define the database schema.
  */
  virtual void InitDatabase (void) {}

  /**
   * \brief Initialize SeNDlog specific members.
   */
  virtual void SendlogInit ();

  /**
  * \brief Sends the given tuple over the network. The destination is
  *        is specified in a special attribute of the tuple.
  */
  virtual void Send (Ptr<Tuple> tuple);


  /**
  * \brief Signs the given tuple.
  */
  virtual void Sign (Ptr<Tuple> tuple);

  /**
  * \brief Verify the given tuple.
  */
  virtual bool Verify (Ptr<Tuple> tuple, string attrName);

  /**
  * \brief Encrypts the given tuple.
  */
  virtual Ptr<Tuple> Encrypt (Ptr<Tuple> tuple, string attrName);

  /**
  * \brief Decrypts the given tuple.
  */
  virtual Ptr<Tuple> Decrypt (Ptr<Tuple> tuple);

  /**
  * \brief Sends a tuple locally, i.e. to itself.
  */
  virtual void SendLocal (Ptr<Tuple> tuple);

  /**
   * \brief Broadcast tuple.
   */
  virtual void SendBroadcast (Ptr<Tuple> tuple);

  /**
  * \brief Send all tuples in the given relation.
  */
  virtual void Send (Ptr<RelationBase> reln);

  /**
  * \brief Sign all tuples in the given relation.
  */
  virtual void Sign (Ptr<RelationBase> reln);

  /**
  * \brief Encrypt all tuples in the given relation.
  */
  virtual Ptr<RelationBase> Encrypt (Ptr<RelationBase> reln, string attrName);

  /**
  * \brief Sends locally all tuples in the given relation.
  */
  virtual void SendLocal (Ptr<RelationBase> reln);

  /**
  * \brief Receives the network event retrieves the @see Tuple and
  * invokes the @see ProcessTuple() method.
  */
  void Receive (Ptr<Socket> socket);

  /**
  * \brief Processes the network event retrieves the @see Tuple and
  * invokes the @see DemuxRecv() method.
  */
  void ProcessTuple (Ptr<Tuple> tuple, Address from);

  /**
  * \brief Factory method to create a new Tuple.
  */
  virtual Ptr<Tuple> CreateNewTuple (string name);

  /**
   * \brief Factory method to create a new relation and set triggers.
   */
  virtual Ptr<Relation> CreateNewRelation (string name);

  /**
   * \brief Inserts a @see Relation with the given name.
   *
   * @param relnName Name of the relation
   * @param attributes List of key attributes
   * @param ttl Soft-state timeout duration
   */
  virtual void AddRelationWithKeys (string relnName,
    list<attrdeftype> attributes, Time ttl = Seconds(TIME_INFINITY));

  /**
   * \brief Inserts a new @see HeapRelation with the given name.
   *
   * @param relnName Name of the relation
   * @param attributes List of key attributes
   * @param heapAttr The heap attribute
   * @param type The heap type (min or max)
   * @param size The maximum size of the heap.
   * @param ttl Soft-state timeout duration
   */
  virtual void AddHeapRelationWithKeys (string relnName,
    list<attrdeftype> attributes, string heapAttr, HeapRelation::Type type,
    uint32_t size, Time ttl = Seconds(TIME_INFINITY));

  /**
  * \brief The demux method that all applications must implement by specifying
  *        what to do when the given tuple is received.
  */
  virtual void DemuxRecv (Ptr<Tuple> tuple);

  /**
   * \brief Triggered periodically, it checks for any tuples in the database
   *        that have timed out and deletes them.
   */
  virtual void SoftStateDelete ();

  bool IsInsertEvent (Ptr<Tuple> tuple);

  bool IsInsertEvent (Ptr<Tuple> tuple, string name);

  bool IsDeleteEvent (Ptr<Tuple> tuple);

  bool IsDeleteEvent (Ptr<Tuple> tuple, string name);

  bool IsRefreshEvent (Ptr<Tuple> tuple, string name);

  bool IsRecvEvent (Ptr<Tuple> tuple);

  bool IsRecvEvent (Ptr<Tuple> tuple, string name);

  /*
   * String tokenizer
   */
  void Tokenize(const std::string& str,
      std::vector<std::string>& tokens,
      const std::string& delimiters);

  /**
   * \brief other Ipv4 addresses of the node, except the CCC channel address,
   * for date on data devices/interfaces
   */
  vector<Ipv4Address> m_addressList;
  vector<std::string> m_deviceNameList;
  /**
   * \brief number of devices for this node/application
   * 1 Ipv4Address <==> 1 Interface <==> 1 Device
   */
  map<Ipv4Address, uint32_t> m_addressToDeviceMap;
  map<uint32_t, string> m_deviceNameMap;

  /**
   * \brief 1 CCC and others are data devices (The Loopback device is not counted)
   * == 1 + m_addressList.size ()
   */
  uint32_t m_numDevices;

  /**
   * \brief Address of the node on which this application instance is running
   */
  Ipv4Address m_address;

  /**
   * \brief Port at which the  applications communicate
   */
  uint16_t s_Port;

  /**
   * \brief Single socket per  application for all communications
   */
  Ptr<Socket> m_Socket;

  /**
   * \brief Single TCP socket per  application for all communications
   */
  Ptr<Socket> m_tcpSocket;

  /**
   * \brief List of tuples
   */
  Ptr<Database> m_database;

  /**
   * \brief Event object for periodic soft-state deletes.
   */
  EventId m_eventSoftStateDelete;

  /**
   * \brief Decorator for visualization
   */
  Ptr<RapidNetDecoratorFrontend> m_decoratorFrontend;

  uint32_t m_maxJitter;
  uint32_t m_udpMaxBytes;
  Time m_tcpInactivityTimeout;
  Timer m_auditTCPConnectionsTimer;
  typedef std::map<Ptr<Socket>, Ptr<RapidNetTCPConnection> > TCPConnectionMap;
  TCPConnectionMap m_tcpConnectionTable;

  friend class InsertTrigger;
  friend class DeleteTrigger;
  friend class RefreshTrigger;


  Ptr<SendlogAuthenticationManager> m_authenticationMgr;

  /**
   * \brief SeNDlog encryption manager.
   */
  Ptr<SendlogEncryptionManager> m_encryptionMgr;

private:

  /**
   * Actually sends the packet.
   */
  void DoSend (Ptr<Tuple> tuple);

  /**
  * \brief Initializes the socket.
  */
  void InitSocket();

  /**
   * \brief TCP Connections layer
   */

  Ptr<RapidNetTCPConnection> AddConnection (Ptr<Socket> socket, Ipv4Address ipAddress, uint16_t port, RapidNetTCPConnection::ConnectionState state);
  Ptr<RapidNetTCPConnection> FindConnection (Ptr<Socket> socket);
  void RemoveConnection (Ptr<Socket> socket);
  Ptr<RapidNetTCPConnection> FindConnection (Ipv4Address ipAddress, uint16_t port);

  // Pending List
  std::list<Ptr<Tuple> > m_pendingTuples;
  bool m_processingPendingTuples;
  bool m_l4Platform;

};

  /**
   * \brief An application specific trigger.
   */
class AppTrigger : public Trigger
{
public:

  AppTrigger(){};

  virtual void SetApplication (Ptr<RapidNetApplicationBase> app)
  {
    m_app = app;
  }

  virtual Ptr<RapidNetApplicationBase> GetApplication () { return m_app; }

  virtual ~AppTrigger () {}

protected:

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Process App trigger" << endl;
    ar & boost::serialization::base_object<Trigger>(*this);
    ar & m_app;
  }

  Ptr<RapidNetApplicationBase> m_app;
};

/**
 * \brief A trigger that is invoked when a tuple is inserted.
 */
class InsertTrigger : public AppTrigger
{
public:

  InsertTrigger(){};

  virtual ~InsertTrigger () {}

  virtual void Invoke (Ptr<Tuple> tuple);

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Process Insert trigger" << endl;
    ar & boost::serialization::base_object<AppTrigger>(*this);
  }
};

/**
 * \brief A trigger that is invoked when a tuple is deleted.
 */
class DeleteTrigger: public AppTrigger
{
public:

  DeleteTrigger(){};

  virtual ~DeleteTrigger () {}

  virtual void Invoke (Ptr<Tuple> tuple);

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Process Delete trigger" << endl;
    ar & boost::serialization::base_object<AppTrigger>(*this);
  }
};

/**
 * \brief A trigger that is invoked when a tuple is refreshed.
 */
class RefreshTrigger: public AppTrigger
{
public:

  RefreshTrigger(){};

  virtual ~RefreshTrigger () {}

  virtual void Invoke (Ptr<Tuple> tuple);

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Process Refresh trigger" << endl;
    ar & boost::serialization::base_object<AppTrigger>(*this);
  }
};

} // namespace rapidnet
} // namespace ns3

BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::AppTrigger)
BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::InsertTrigger)
BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::DeleteTrigger)
BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::RefreshTrigger)

#endif // RAPIDNET_APPLICATION_BASE_H

