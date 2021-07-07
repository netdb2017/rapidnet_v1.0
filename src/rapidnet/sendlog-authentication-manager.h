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

#ifndef SENDLOG_AUTHENTICATION_MANAGER_H
#define SENDLOG_AUTHENTICATION_MANAGER_H

#include "ns3/ptr.h"
#include "ns3/tuple.h"
#include "rapidnet-application-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet {

class RapidNetApplicationBase;

/**
 * \ingroup rapidnet
 *
 * \brief RapidNet applications delegate the authentication
 *        features to the SendlogAuthenticationManager. RapidNet
 *        applications written in SeNDlog (and not NDlog) require
 *        authentication capabilities. This object represents
 *        an interface to the authentication operations "sign"
 *        and "verify". Concrete implementations of this interface
 *        can be associated to the RapidNet applications at the time
 *        of initialization.
 */
class SendlogAuthenticationManager: public RefCountBase
{
public:

  SendlogAuthenticationManager ();

  virtual ~SendlogAuthenticationManager ();

  /**
   * \brief Signs the tuple in place. Add authentication
   *        signature attribute(s) to the given tuple.
   */
  virtual void Sign (Ptr<Tuple> tuple) = 0;

  /**
   * \brief Verify the tuple. It is recommended to remove authentication
   *        specific attribute(s).
   *
   * @param attr Information about the principal who is the "sayer".
   *
   * @return true if verified that the principal indicated by attr
   *         is indeed the sayer.
   */
  virtual bool Verify (Ptr<Tuple> tuple, Ptr<TupleAttribute> attr) = 0;

protected:

  virtual void SetPrincipal (Ptr<RapidNetApplicationBase> principal);

  Ptr<RapidNetApplicationBase> m_principal;
};

} // namespace rapidnet
} // namespace ns3

#endif // SENDLOG_AUTHENTICATION_MANAGER_H
