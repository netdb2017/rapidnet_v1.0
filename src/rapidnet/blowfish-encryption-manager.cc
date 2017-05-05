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

#include "blowfish-encryption-manager.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

#define GetByteArrayValue(attr) \
  DynamicCast<ByteArrayValue, Value>(attr->GetValue ())

#define FormatMsg(msg,args) \
  "BlowfishEncryptionManager(" << m_principal->GetAddress () << "): " << msg << args

NS_LOG_COMPONENT_DEFINE ("BlowfishEncryptionManager");

BlowfishEncryptionManager::BlowfishEncryptionManager ()
{
}

BlowfishEncryptionManager::~BlowfishEncryptionManager ()
{
}

void
BlowfishEncryptionManager::SetPrincipal (
  Ptr<RapidNetApplicationBase> principal)
{
  SendlogEncryptionManager::SetPrincipal (principal);

  stringstream ss;
  ss << m_principal->GetAddress ();
  m_privateKey = EvpKey::New (ss.str (), EVP_PRIVATE_KEY);
}

Ptr<SendlogEncryptionManager> BlowfishEncryptionManager::New (Ptr<
    RapidNetApplicationBase> principal)
{
  Ptr<BlowfishEncryptionManager> retval = Create<BlowfishEncryptionManager> ();
  retval->SetPrincipal (principal);
  return retval;
}

Ptr<Tuple>
BlowfishEncryptionManager::Encrypt (Ptr<Tuple> tuple, Ptr<TupleAttribute> attr)
{
  Ptr<Tuple> rnAttrs = RemoveAllRapidNetAttributes (tuple);
  string targetId = attr->GetValue ()->ToString ();

  Ptr<Tuple> encryptedTuple = OpenSslSeal (tuple, targetId);

  if (!encryptedTuple->IsEmpty ())
  {
    // Add the attributes to both the tuples, just in case
    tuple->AddAllAttributes (rnAttrs);
    encryptedTuple->AddAllAttributes (rnAttrs);
  }

  return encryptedTuple;
}

Ptr<Tuple>
BlowfishEncryptionManager::OpenSslSeal (Ptr<Tuple> tuple, string keyId)
{
  Ptr<ByteArrayValue> tupleAsBytes = SerializeTupleToByteArray (tuple);
  uint8_t* message = tupleAsBytes->GetByteArrayPtr ();
  uint32_t msgLen = tupleAsBytes->GetByteArrayLen ();

  EVP_PKEY* publicKey = m_publicKeys.GetKey (keyId, EVP_PUBLIC_KEY)->GetKey ();

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init (&ctx);

  uint8_t* secretKey = new uint8_t[BLOWFISH_MAX_SECRET_KEY_LEN];
  int32_t secretKeyLen = 0;
  uint8_t* initVector = new uint8_t[BLOWFISH_MAX_INIT_VECTOR_LEN];
  int32_t initVectorLen = BLOWFISH_MAX_INIT_VECTOR_LEN;
  uint8_t* cipher = new uint8_t[BLOWFISH_MAX_CIPHER_LEN];

  int32_t outlen = 0, tmplen = 0;

  if (!EVP_SealInit (&ctx, EVP_bf_cbc (), &secretKey, &secretKeyLen,
    initVector, &publicKey, 1))
    {
      NS_LOG_ERROR (FormatMsg ("Error while EVP_SealInit", " for tuple " <<
        tuple));
      return Tuple::NewEmpty ();
    }

  if (!EVP_SealUpdate (&ctx, cipher, &outlen, message, msgLen))
    {
      NS_LOG_ERROR (FormatMsg ("Error while EVP_SealUpdate", " for tuple " <<
        tuple));
      return Tuple::NewEmpty ();
    }

  if (!EVP_SealFinal (&ctx, cipher + outlen, &tmplen))
    {
      NS_LOG_ERROR (FormatMsg ("Error while EVP_SealFinal", " for tuple " <<
        tuple));
      return Tuple::NewEmpty ();
    }

  EVP_CIPHER_CTX_cleanup (&ctx);

  if (outlen + tmplen > BLOWFISH_MAX_CIPHER_LEN)
    {
      NS_LOG_ERROR (FormatMsg ("Buffer overflow error", " for tuple " <<
        tuple));
      return Tuple::NewEmpty ();
    }

  Ptr<Value> secretKeyValue = ByteArrayValue::New (secretKey, secretKeyLen);
  Ptr<Value> initVectorValue = ByteArrayValue::New (initVector, initVectorLen);
  Ptr<Value> cipherValue = ByteArrayValue::New (cipher, outlen + tmplen);

  Ptr<Tuple> retval = Tuple::New (SENDLOG_NAME_OPAQUE);
  retval->AddAttribute (TupleAttribute::New (BLOWFISH_ATTR_SECRET_KEY,
    secretKeyValue));
  retval->AddAttribute (TupleAttribute::New (BLOWFISH_ATTR_INIT_VECTOR,
    initVectorValue));
  retval->AddAttribute (TupleAttribute::New (BLOWFISH_ATTR_CIPHER,
    cipherValue));

  return retval;
}

Ptr<Tuple>
BlowfishEncryptionManager::Decrypt (Ptr<Tuple> tuple)
{
  if (!tuple->HasAttribute (BLOWFISH_ATTR_SECRET_KEY) ||
      !tuple->HasAttribute (BLOWFISH_ATTR_INIT_VECTOR) ||
      !tuple->HasAttribute (BLOWFISH_ATTR_CIPHER))
    {
      NS_LOG_ERROR (FormatMsg ("Missing one or more Blowfish attributes",
        " for tuple " << tuple));
      return Tuple::NewEmpty ();
    }

  Ptr<ByteArrayValue> secretKey = GetByteArrayValue (tuple->GetAttribute (
    BLOWFISH_ATTR_SECRET_KEY));

  Ptr<ByteArrayValue> initVector = GetByteArrayValue (tuple->GetAttribute (
    BLOWFISH_ATTR_INIT_VECTOR));

  Ptr<ByteArrayValue> cipher = GetByteArrayValue (tuple->GetAttribute (
    BLOWFISH_ATTR_CIPHER));

  return OpenSslOpen (cipher, secretKey, initVector);
}

Ptr<Tuple>
BlowfishEncryptionManager::OpenSslOpen (Ptr<ByteArrayValue> cipher,
  Ptr<ByteArrayValue> secretKey, Ptr<ByteArrayValue> initVector)
{
  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init (&ctx);

  uint8_t* message = new uint8_t[BLOWFISH_MAX_DECIPHER_LEN];
  int32_t outlen = 0, tmplen = 0;

  EVP_OpenInit (&ctx, EVP_bf_cbc (), secretKey->GetByteArrayPtr (),
    secretKey->GetByteArrayLen (), initVector->GetByteArrayPtr (),
    m_privateKey->GetKey ());

  if (!EVP_OpenUpdate(&ctx, message, &outlen,
    cipher->GetByteArrayPtr (), cipher->GetByteArrayLen ()))
    {
      NS_LOG_ERROR (FormatMsg ("Error while EVP_OpenUpdate", ""));
      return Tuple::NewEmpty ();
    }

  if (!EVP_OpenFinal (&ctx, message + outlen, &tmplen))
    {
      NS_LOG_ERROR (FormatMsg ("Error while EVP_OpenUpdate", ""));
      return Tuple::NewEmpty ();
    }

  EVP_CIPHER_CTX_cleanup (&ctx);

  if (outlen + tmplen > BLOWFISH_MAX_DECIPHER_LEN)
    {
      NS_LOG_ERROR (FormatMsg ("Buffer overflow error", ""));
      return Tuple::NewEmpty ();
    }

  Ptr<Value> outputBytes = ByteArrayValue::New (message,
    outlen + tmplen);

  return DeserializeTupleFromByteArray (DynamicCast<ByteArrayValue, Value> (
      outputBytes));
}

