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
 */

#ifndef RAPIDNET_TCP_CONNECTION_H
#define RAPIDNET_TCP_CONNECTION_H

#include "ns3/socket.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/timer.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"
#include "ns3/simple-ref-count.h"
#include <vector>

namespace ns3 {

class RapidNetTCPHeader : public Header
{
public:
  RapidNetTCPHeader ();
  virtual ~RapidNetTCPHeader ();

  void SetLength (uint32_t length);

  uint32_t GetLength (void) const;

private:
  uint32_t m_length;

public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  /**
   *  \brief Packs length of RapidNetTCPMessage
   *  \verbatim
      Packed Structure:

      RapidNetTCPHeader:
      0 1 2 3 4 5 6 7 8
      +-+-+-+-+-+-+-+-+
      |               |
      |               |
      |    length     |
      |               |
      +-+-+-+-+-+-+-+-+
      \endverbatim
   *
   */
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
};

class RapidNetTCPConnection : public SimpleRefCount<ns3::RapidNetTCPConnection>
{
public:

  enum ConnectionState
  {
    NOT_CONNECTED = 0,
    CONNECTED = 1,
  };

  /**
   *  \brief Constructor
   *  \param ipAddress Ipv4Address of remote node
   *  \param port
   *  \param socket Ptr to Socket
   */
  RapidNetTCPConnection (Ipv4Address ipAddress, uint16_t port, Ptr<Socket> socket);
  RapidNetTCPConnection ();
  /**
   *  \brief Copy Constructor
   *  \param rapidNetTCPConnection
   */
  RapidNetTCPConnection (const RapidNetTCPConnection &rapidNetTCPConnection);
  virtual ~RapidNetTCPConnection ();
  virtual void DoDispose ();

  /**
   *  \returns connection state
   */
  ConnectionState GetConnState ();
  /**
   *  \sets connection state
   */
  void SetConnState (ConnectionState connState);
  
  /**
   *  \returns Ipv4Address of remote host
   */
  Ipv4Address GetIpAddress ();
  /**
   *  \returns port of remote host
   */
  uint16_t GetPort ();
  /**
   *  \returns TCP socket
   */
  Ptr<Socket> GetSocket ();
  /**
   *  \returns Timestamp of last activity on socket
   */
  Time GetLastActivityTime ();

  // TCP methods
  /**
   *  \brief Sends data on open connection
   *  \param packet Ptr to Packet
   */
  void SendTCPData (Ptr<Packet> packet);
  /**
   *  \brief Writes data on socket based on available space info
   *  \param Ptr to Socket
   *  \param txSpace
   */
  void WriteTCPBuffer (Ptr<Socket> socket, uint32_t txSpace);
  /**
   *  \brief Read data from socket
   *  \param socket Ptr to Socket
   *  \param txSpace
   */
  void ReadTCPBuffer (Ptr<Socket> socket);
  /**
   *  \brief Registers Receive Callback function
   *  \param recvFn Callback
   *
   *  This upcall is made whenever complete message is received
   */
  void SetRecvCallback (Callback<void, Ptr<Packet>, Ptr<RapidNetTCPConnection> > recvFn);

private:
  /**
   *  \cond
   */
  enum TxState
  {
    TX_IDLE = 0,
    TRANSMITTING = 1,
  };

  enum RxState
  {
    RX_IDLE = 0,
    RECEIVING = 1,
  };


  Ipv4Address m_ipAddress;
  uint16_t m_port;
  Ptr<Socket> m_socket;

  Time m_lastActivityTime;

  // TCP assembly/trasnmission buffer handling

  TxState m_txState;
  std::vector<Ptr<Packet> > m_txPacketList;
  Ptr<Packet> m_currentTxPacket;
  // Current packet tx counters
  uint32_t m_totalTxBytes;
  uint32_t m_currentTxBytes;

  RxState m_rxState;
  // rx counters
  uint32_t m_totalRxBytes;
  uint32_t m_currentRxBytes;
  Ptr<Packet> m_currentRxPacket;
  Callback<void, Ptr<Packet>, Ptr<RapidNetTCPConnection> > m_recvFn;

  ConnectionState m_connState;
  /**
   *  \endcond
   */
  // Assembly of rx packet
  /**
   *  \brief Assembles Message
   *  \param packet Ptr to Packet
   *  \param availRxBytes
   *  \returns Ptr to complete Message Packet
   */
  Ptr<Packet> AssembleMessage (Ptr<Packet>& packet, uint32_t& availRxBytes);

  // Operators
  friend bool operator < (const RapidNetTCPConnection &connectionL, const RapidNetTCPConnection &connectionR);
  friend bool operator == (const RapidNetTCPConnection &connectionL, const RapidNetTCPConnection &connectionR);
};


std::ostream & operator<< (std::ostream& os, Ptr<RapidNetTCPConnection> const &connection);
bool operator < (const RapidNetTCPConnection &connectionL, const RapidNetTCPConnection &connectionR);
bool operator == (const RapidNetTCPConnection &connectionL, const RapidNetTCPConnection &connectionR);
} // namespace ns3
#endif // RAPIDNET_TCP_CONNECTION_H
