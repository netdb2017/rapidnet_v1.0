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

#ifndef BLOWFISH_ENCRYPTION_MANAGER_H
#define BLOWFISH_ENCRYPTION_MANAGER_H

#include <string>
#include <openssl/evp.h>

#include "ns3/ptr.h"
#include "tuple.h"
#include "ns3/ipv4-value.h"
#include "evp-key.h"
#include "rapidnet-utils.h"
#include "sendlog-encryption-manager.h"
#include "rapidnet-application-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet {

const int32_t BLOWFISH_MAX_SECRET_KEY_LEN = 64;
const int32_t BLOWFISH_MAX_INIT_VECTOR_LEN = 8;
const int32_t BLOWFISH_MAX_CIPHER_LEN = 2048;
const int32_t BLOWFISH_MAX_DECIPHER_LEN = 4096;

const string BLOWFISH_ATTR_SECRET_KEY = "sKey";
const string BLOWFISH_ATTR_INIT_VECTOR = "iVec";
const string BLOWFISH_ATTR_CIPHER = "cipher";

/**
 * \ingroup rapidnet
 *
 * \brief A blowfish based encryption manager implementation.
 */
class BlowfishEncryptionManager: public SendlogEncryptionManager
{
public:

  BlowfishEncryptionManager ();

  virtual ~BlowfishEncryptionManager ();

  Ptr<Tuple> Encrypt (Ptr<Tuple> tuple, Ptr<TupleAttribute> attr);

  Ptr<Tuple> Decrypt (Ptr<Tuple> tuple);

  static Ptr<SendlogEncryptionManager> New (
    Ptr<RapidNetApplicationBase> principal = NULL);

protected:

  virtual void SetPrincipal (Ptr<RapidNetApplicationBase> principal);

  /*
   * \brief Uses OpenSSL primitives to encrypt and return the given tuple for
   *        the given id of the target principal.
   */
  Ptr<Tuple> OpenSslSeal (Ptr<Tuple> tuple, string keyId);

  /**
   * \brief Returns the decrypted tuple by using OpenSSL primitives when
   *        given the cipher, secret key and initialization vector as
   *        ByteArrayValues
   */
  Ptr<Tuple> OpenSslOpen (Ptr<ByteArrayValue> cipher, Ptr<ByteArrayValue> secretKey,
    Ptr<ByteArrayValue> initVector);

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

#endif // BLOWFISH_ENCRYPTION_MANAGER_H
