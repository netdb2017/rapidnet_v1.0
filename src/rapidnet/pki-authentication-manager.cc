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

#include "pki-authentication-manager.h"
#include "ns3/log.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

NS_LOG_COMPONENT_DEFINE ("PkiAuthenticationManager");

PkiAuthenticationManager::PkiAuthenticationManager ()
{
}

PkiAuthenticationManager::~PkiAuthenticationManager ()
{
}

void
PkiAuthenticationManager::SetPrincipal (Ptr<RapidNetApplicationBase> principal)
{
  SendlogAuthenticationManager::SetPrincipal (principal);

  stringstream ss;
  ss << m_principal->GetAddress ();
  m_privateKey = EvpKey::New (ss.str (), EVP_PRIVATE_KEY);
}

Ptr<SendlogAuthenticationManager>
PkiAuthenticationManager::New (Ptr<RapidNetApplicationBase> principal)
{
  Ptr<PkiAuthenticationManager> retval = Create<PkiAuthenticationManager> ();
  retval->SetPrincipal (principal);
  return retval;
}

void
PkiAuthenticationManager::Sign (Ptr<Tuple> tuple)
{
  Ptr<Tuple> rnAttrs = RemoveAllRapidNetAttributes (tuple);

  Ptr<Value> signature = OpenSslSign (tuple);
  tuple->AddAttribute (TupleAttribute::New (PKI_ATTR_SIGNATURE, signature));

  tuple->AddAllAttributes (rnAttrs);
}

Ptr<Value>
PkiAuthenticationManager::OpenSslSign (Ptr<Tuple> tuple)
{
  Ptr<ByteArrayValue> bytes = SerializeTupleToByteArray (tuple);
  uint8_t* message = bytes->GetByteArrayPtr ();
  int32_t msgLen = bytes->GetByteArrayLen ();

  EVP_MD_CTX md_ctx;
  EVP_MD_CTX_init (&md_ctx);

  EVP_SignInit (&md_ctx, EVP_sha1 ());
  EVP_SignUpdate (&md_ctx, message, msgLen);

  EVP_PKEY* privateKey = m_privateKey->GetKey ();
  uint8_t* signature = new uint8_t[EVP_PKEY_size (privateKey)];
  uint32_t sigLen;

  int err = EVP_SignFinal (&md_ctx, signature, &sigLen, privateKey);

  // Create this object here so that if alternate ways of
  // handling the err is chosen then the ByteArrayValue can
  // take care of freeing up the memory.
  Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

  EVP_MD_CTX_cleanup (&md_ctx);

  if (err != 1)
    {
      NS_LOG_ERROR ("PkiAuthenticationManager(" << m_principal->GetAddress ()
        << "): OpenSslSign error while signing tuple " << tuple);
      return ByteArrayValue::New ();
    }

  return retval;
}

bool
PkiAuthenticationManager::Verify (Ptr<Tuple> tuple,
  Ptr<TupleAttribute> attr)
{
  if (!tuple->HasAttribute (PKI_ATTR_SIGNATURE))
    {
      NS_LOG_WARN ("PkiAuthenticationManager: Verification failed due to "
          "missing signature attribute in tuple: " << tuple);
      return false;
    }

  Ptr<Tuple> rnAttrs = RemoveAllRapidNetAttributes (tuple);

  string srcId = attr->GetValue ()->ToString ();

  Ptr<TupleAttribute> signatureAttr = tuple->GetAttribute (PKI_ATTR_SIGNATURE);
  Ptr<ByteArrayValue> signature = DynamicCast<ByteArrayValue, Value> (
      signatureAttr->GetValue ());
  tuple->RemoveAttribute (PKI_ATTR_SIGNATURE);

  Ptr<ByteArrayValue> serializedTuple = SerializeTupleToByteArray (tuple);

  bool sigValid = OpenSslVerify (signature, serializedTuple, srcId);
  tuple->AddAllAttributes (rnAttrs);

  return sigValid;
}

bool
PkiAuthenticationManager::OpenSslVerify (Ptr<ByteArrayValue> signature,
  Ptr<ByteArrayValue> bytes, string keyId)
{
  EVP_MD_CTX md_ctx;
  EVP_MD_CTX_init (&md_ctx);

  Ptr<EvpKey> publicKey = m_publicKeys.GetKey(keyId, EVP_PUBLIC_KEY);

  EVP_VerifyInit (&md_ctx, EVP_sha1 ());
  EVP_VerifyUpdate (&md_ctx, bytes->GetByteArrayPtr (),
    bytes->GetByteArrayLen ());

  int err = EVP_VerifyFinal (&md_ctx, signature->GetByteArrayPtr (),
    signature->GetByteArrayLen (), publicKey->GetKey ());

  EVP_MD_CTX_cleanup (&md_ctx);

  return err == 1;
}
