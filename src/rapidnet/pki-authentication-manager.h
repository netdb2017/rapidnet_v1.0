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

#ifndef PKI_AUTHENTICATION_MANAGER_H
#define PKI_AUTHENTICATION_MANAGER_H

#include <string>
#include "ns3/ptr.h"
#include "tuple.h"
#include "ns3/byte-array-value.h"
#include "sendlog-authentication-manager.h"
#include "rapidnet-application-base.h"
#include "evp-key.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet {

const string PKI_ATTR_SIGNATURE = "signature";

/**
 * \ingroup rapidnet
 *
 * \brief A PKI based authentication manager implementation.
 */
class PkiAuthenticationManager: public SendlogAuthenticationManager
{
public:

  PkiAuthenticationManager ();

  ~PkiAuthenticationManager ();

  /**
   * \brief Signs the given tuple on behalf of the principal.
   */
  void Sign (Ptr<Tuple> tuple);

  /**
   * \brief Verify that the received tuple is signed by a principal.
   *
   * @param sigAttr The identity information of the principal who should
   *                sign the tuple is provided.
   */
  bool Verify (Ptr<Tuple> tuple, Ptr<TupleAttribute> sigAttr);

  /**
   * \brief Static constructor. Creates and returns an instance of the
   *        PkiAuthenticationManager for the given principal.
   */
  static Ptr<SendlogAuthenticationManager> New (
    Ptr<RapidNetApplicationBase> principal);

protected:

  virtual void SetPrincipal (Ptr<RapidNetApplicationBase> principal);

  /**
   * \brief Invokes OpenSSL primitives for generating the signature and
   *        returns the @see ByteArrayValue with the signature bytes.
   */
  Ptr<Value> OpenSslSign (Ptr<Tuple> tuple);

  /**
   * \brief Invokes OpenSSL primitives for verifying the signature.
   */
  bool OpenSslVerify (Ptr<ByteArrayValue> signature, Ptr<ByteArrayValue> bytes,
    string keyId);

  /**
   * \brief My private key
   */
  Ptr<EvpKey> m_privateKey;

  /**
   * \brief Cache of public keys of other principals.
   */
  EvpKeyCache m_publicKeys;
};

} // namespace rapidnet
} // namespace ns3

#endif // PKI_AUTHENTICATION_MANAGER_H
