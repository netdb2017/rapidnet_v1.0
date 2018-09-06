/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Pennsylvania
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
 *
 */

#include "evp-key.h"
#include <cstdio>

using namespace ns3;
using namespace ns3::rapidnet;

string
ns3::rapidnet::EvpKeyTypeString (EvpKeyType type)
{
  return type == EVP_PUBLIC_KEY ? "public" : "private";
}

EvpKey::EvpKey ()
{
  m_key = NULL;
  m_keyId = "uninitialized";
}

EvpKey::~EvpKey ()
{
  EVP_PKEY_free (m_key);
}

string
EvpKey::ToString ()
{
  return "<EVP key for '" + m_keyId + "' (" + EvpKeyTypeString (m_type) + ")>";
}

Ptr<EvpKey>
EvpKey::New (string keyId, EvpKeyType type)
{
  Ptr<EvpKey> retval = Create<EvpKey> ();
  string keyFile = s_keysDir + GetKeyFileName (keyId, type);

  FILE* fp = fopen (keyFile.c_str (), "r");
  if (fp == NULL)
    {
      clog << "Key file '" << keyFile << "' not found!" << endl;
      return retval;
    }

  retval->m_key = type == EVP_PUBLIC_KEY ? PEM_read_PUBKEY (fp, NULL, NULL,
    NULL) : PEM_read_PrivateKey (fp, NULL, NULL, NULL);

  retval->m_keyId = keyId;
  retval->m_type = type;
  fclose (fp);

  if (retval->m_key == NULL)
    {
      clog << "Error loading key from key file '" << keyFile << "'" << endl;
    }

  return retval;
}

void
EvpKey::SetKeysDir (string keysDir)
{
  s_keysDir = keysDir;
}

string
EvpKey::GetKeyFileName (string keyId, EvpKeyType type)
{
  return keyId + "_" + EvpKeyTypeString (type) + ".pem";
}

string EvpKey::s_keysDir = DEFAULT_KEYS_DIR;

ostream&
ns3::rapidnet::operator << (ostream& stream, Ptr<EvpKey> evpKey)
{
  stream << evpKey->ToString ();
  return stream;
}

Ptr<EvpKey>
EvpKeyCache::GetKey (string keyId, EvpKeyType type)
{
  Ptr<EvpKey> retval;
  string name = EvpKey::GetKeyFileName (keyId, type);

  if (m_cache.find (name) == m_cache.end ())
    {
      clog << "Loading: " << name << endl;
      retval = m_cache[name] = EvpKey::New (keyId, type);
    }
  else
    {
      clog << "Already loaded: " << name << endl;
      retval = m_cache[name];
    }

  return retval;
}
