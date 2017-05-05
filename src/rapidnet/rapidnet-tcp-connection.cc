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

#include "rapidnet-tcp-connection.h"
#include "ns3/log.h"
#include <stdio.h>

namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("RapidNetTCPConnection");
NS_OBJECT_ENSURE_REGISTERED (RapidNetTCPHeader);

RapidNetTCPConnection::RapidNetTCPConnection (Ipv4Address ipAddress, uint16_t port, Ptr<Socket> socket)
{
  m_ipAddress = ipAddress;
  m_port = port;
  m_socket = socket;
  m_txState = TX_IDLE;
  m_rxState = RX_IDLE;
  m_totalTxBytes = 0;
  m_currentTxBytes = 0;
  m_lastActivityTime = Simulator::Now ();
  m_socket->SetRecvCallback (MakeCallback (&RapidNetTCPConnection::ReadTCPBuffer, this));
}

RapidNetTCPConnection::RapidNetTCPConnection ()
{ 
  m_ipAddress = 0;
  m_port = 0;
  m_socket = 0;
  m_txState = TX_IDLE;
  m_rxState = RX_IDLE;
  m_totalTxBytes = 0;
  m_currentTxBytes = 0;
  m_lastActivityTime = Simulator::Now ();
  m_socket->SetRecvCallback (MakeCallback (&RapidNetTCPConnection::ReadTCPBuffer, this));
}
RapidNetTCPConnection::RapidNetTCPConnection (const RapidNetTCPConnection &connection)
{
  Ptr<RapidNetTCPConnection> rapidNetTCPConnection = const_cast<RapidNetTCPConnection *> (&connection);
  m_ipAddress = rapidNetTCPConnection->GetIpAddress ();
  m_port = rapidNetTCPConnection->GetPort ();
  m_socket = rapidNetTCPConnection->GetSocket ();
  m_txState = TX_IDLE;
  m_rxState = RX_IDLE;
  m_totalTxBytes = 0;
  m_currentTxBytes = 0;
  m_lastActivityTime = Simulator::Now ();
  m_connState = rapidNetTCPConnection->GetConnState();
  m_socket->SetRecvCallback (MakeCallback (&RapidNetTCPConnection::ReadTCPBuffer, this));
}


RapidNetTCPConnection::~RapidNetTCPConnection ()
{
  if (m_socket != 0)
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_socket = 0;
    }
  m_port = 0;
  m_txState = TX_IDLE;
  m_rxState = RX_IDLE;
  m_totalTxBytes = 0;
  m_currentTxBytes = 0;
  m_txPacketList.clear ();
}


void
RapidNetTCPConnection::DoDispose ()
{
  if (m_socket != 0)
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_socket->SetConnectCallback (MakeNullCallback<void, Ptr<Socket> > () , MakeNullCallback<void, Ptr<Socket> > ());
      m_socket = 0;
    }
  m_port = 0;
  m_txState = TX_IDLE;
  m_rxState = RX_IDLE;
  m_totalTxBytes = 0;
  m_currentTxBytes = 0;
  m_txPacketList.clear ();
}

RapidNetTCPConnection::ConnectionState
RapidNetTCPConnection::GetConnState ()
{
  return m_connState;
}

void
RapidNetTCPConnection::SetConnState (RapidNetTCPConnection::ConnectionState connState)
{
  m_connState = connState;
}

Ipv4Address
RapidNetTCPConnection::GetIpAddress ()
{
  return m_ipAddress;
}

uint16_t
RapidNetTCPConnection::GetPort ()
{
  return m_port;
}

Ptr<Socket>
RapidNetTCPConnection::GetSocket ()
{
  return m_socket;
}

void 
RapidNetTCPConnection::SendTCPData (Ptr<Packet> packet)
{
  // Add packet to pending tx list
  m_txPacketList.push_back (packet);
  NS_LOG_INFO ("Preparing to send TCP Data. m_txState: " << m_txState << " m_connState: " << m_connState);
  // Set state to transmitting
  if (m_txState == TX_IDLE)
    {
      // Start transmitting
      m_txState = TRANSMITTING;
      m_socket->SetSendCallback (MakeCallback (&RapidNetTCPConnection::WriteTCPBuffer, this));
      if (m_connState == CONNECTED)
        {
          // Initiate transmission
          WriteTCPBuffer (m_socket, m_socket->GetTxAvailable ());
        }
    }
  return;
}

void
RapidNetTCPConnection::WriteTCPBuffer (Ptr<Socket> socket, uint32_t txSpace)
{

  m_lastActivityTime = Simulator::Now ();
  if (m_totalTxBytes == 0)
    {
      // Start new packet Tx
      m_currentTxBytes = 0;
      if (m_txPacketList.size () == 0)
        {
          m_txState = TX_IDLE;
          return;
        }
      m_currentTxPacket = *(m_txPacketList.begin ());
      m_totalTxBytes = m_currentTxPacket->GetSize ();
      RapidNetTCPHeader rapidNetTCPHeader;
      rapidNetTCPHeader.SetLength (m_totalTxBytes);
      m_currentTxPacket->AddHeader (rapidNetTCPHeader);
      m_totalTxBytes = m_currentTxPacket->GetSize ();
      NS_LOG_INFO ("Start new packet Tx, total bytes to send (with header): " << m_totalTxBytes);
    }

  // Transmit m_currentTxPacket
  uint32_t availTxBytes = socket->GetTxAvailable ();
  // 2 Things: Either full packet fits or we have to fragment
  if ((m_totalTxBytes - m_currentTxBytes) <= availTxBytes)
    {
      // Send entire packet at once and remove current packet from queue
      socket->Send (m_currentTxPacket, 0);
      m_txPacketList.erase (m_txPacketList.begin ());
      m_totalTxBytes = 0;
      m_currentTxBytes = 0;
      return;
    }
  else
    {
      // Fragment and send packet
      socket->Send (m_currentTxPacket->CreateFragment (0, availTxBytes));
      m_currentTxPacket->RemoveAtStart(availTxBytes);
      m_currentTxBytes = m_currentTxBytes + availTxBytes;
    }
}

void
RapidNetTCPConnection::ReadTCPBuffer (Ptr<Socket> socket)
{
  m_lastActivityTime = Simulator::Now ();
  uint32_t availRxBytes = socket->GetRxAvailable ();
  NS_LOG_INFO ("Reading from socket, bytes = " << availRxBytes);
  Ptr<Packet> packet = socket->Recv (availRxBytes, 0);
  while (availRxBytes > 0)
    {
      Ptr<Packet> messagePacket = AssembleMessage (packet, availRxBytes);
      if (messagePacket != NULL)
        {
          m_recvFn (messagePacket, this);
        }
    }
}

Ptr<Packet>
RapidNetTCPConnection::AssembleMessage (Ptr<Packet>& packet, uint32_t &availRxBytes)
{
  if (m_rxState == RX_IDLE)
    {
      // Receive new packet
      RapidNetTCPHeader rapidNetTCPHeader = RapidNetTCPHeader ();
      packet->RemoveHeader (rapidNetTCPHeader);
      availRxBytes = availRxBytes - rapidNetTCPHeader.GetSerializedSize ();
      m_totalRxBytes = rapidNetTCPHeader.GetLength ();
      m_currentRxBytes = 0;
      m_currentRxPacket = Create<Packet> ();
      m_rxState = RECEIVING;
      NS_LOG_INFO ("New message, m_totalRxBytes: " << m_totalRxBytes << " m_currentRxBytes: " << m_currentRxBytes << " Packet size: " << packet->GetSize ());
    }
  NS_LOG_INFO ("Assemble message, m_totalRxBytes: " << m_totalRxBytes << " m_currentRxBytes: " << m_currentRxBytes << " Packet size: " << packet->GetSize ());
  if ((m_totalRxBytes - m_currentRxBytes) <=  packet->GetSize ())
    {
      // Deliver message
      m_currentRxPacket->AddAtEnd (packet->CreateFragment (0, m_totalRxBytes - m_currentRxBytes));
      // remove bytes
      packet->RemoveAtStart (m_totalRxBytes - m_currentRxBytes);
      availRxBytes = availRxBytes - (m_totalRxBytes - m_currentRxBytes);
      m_rxState = RX_IDLE;
      NS_LOG_INFO ("Deliver packet of size = " << m_currentRxPacket->GetSize ());
      return m_currentRxPacket;
    }
  else
    {
      NS_LOG_INFO ("Assembling packet fragment of size = " << packet->GetSize ());
      m_currentRxBytes = m_currentRxBytes + packet->GetSize ();
      // concat received packet
      m_currentRxPacket->AddAtEnd (packet);
      availRxBytes = 0;
      return NULL;
    }
}

void
RapidNetTCPConnection::SetRecvCallback (Callback<void, Ptr<Packet>, Ptr<RapidNetTCPConnection> > recvFn)
{
  m_recvFn = recvFn;
}

Time
RapidNetTCPConnection::GetLastActivityTime ()
{
  return m_lastActivityTime;
}


// Needed for std::map
bool
operator < (const RapidNetTCPConnection &connectionL, const RapidNetTCPConnection &connectionR)
{
  Ptr<RapidNetTCPConnection> connL = const_cast<RapidNetTCPConnection *> (&connectionL);
  Ptr<RapidNetTCPConnection> connR = const_cast<RapidNetTCPConnection *> (&connectionR);
  if (connL->GetIpAddress () < connL->GetIpAddress ())
    {
      return true;
    }
  else if (connL->GetIpAddress () == connL->GetIpAddress ())
    {
      if (connL->GetPort () < connR->GetPort ())
        {
          return true;
        }
      else
        {
          return false;
        }
    }
  else
    {
      return false;
    }
}

bool
operator == (const RapidNetTCPConnection &connectionL, const RapidNetTCPConnection &connectionR)
{
  Ptr<RapidNetTCPConnection> connL = const_cast<RapidNetTCPConnection *> (&connectionL);
  Ptr<RapidNetTCPConnection> connR = const_cast<RapidNetTCPConnection *> (&connectionR);
  if ((connL->GetIpAddress () == connL->GetIpAddress ()) && (connL->GetPort () == connR->GetPort ()))
    {
      return true;
    }
  else
    {
      return false;
    }
}


RapidNetTCPHeader::RapidNetTCPHeader ()
{
}

RapidNetTCPHeader::~RapidNetTCPHeader ()
{
}

TypeId
RapidNetTCPHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RapidNetTCPHeader")
    .SetParent<Header> ()
    .AddConstructor<RapidNetTCPHeader> ()
  ;
  return tid;
}

void 
RapidNetTCPHeader::SetLength (uint32_t length)
{
  m_length = length;
}

uint32_t 
RapidNetTCPHeader::GetLength (void) const
{
  return m_length;
}

TypeId
RapidNetTCPHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
RapidNetTCPHeader::GetSerializedSize (void) const
{
  uint32_t size = sizeof (uint32_t);
  return size;
}

void
RapidNetTCPHeader::Print (std::ostream &os) const
{
  os << "Length: " << m_length;
}

void
RapidNetTCPHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteHtonU32 (m_length);
}

uint32_t
RapidNetTCPHeader::Deserialize (Buffer::Iterator start)
{
  uint32_t size;
  Buffer::Iterator i = start;
  m_length = i.ReadNtohU32 ();
  size = sizeof (uint32_t);
  return size;
}

} // namespace ns3
