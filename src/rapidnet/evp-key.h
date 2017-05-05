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

#ifndef EVP_KEY_H
#define EVP_KEY_H

#include <string>
#include <map>
#include "ns3/ptr.h"
#include "ns3/ref-count-base.h"
#include "openssl/pem.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet {

const string DEFAULT_KEYS_DIR = "rapidnet/keys/";

enum EvpKeyType
{
  EVP_PRIVATE_KEY,
  EVP_PUBLIC_KEY,
};

string EvpKeyTypeString (EvpKeyType type);

/**
 * \brief Encapsulates EVP_KEY* type from OpenSSL for
 *        automatic memory management and load utilities.
 */
class EvpKey: public RefCountBase
{
public:

  EvpKey ();

  virtual ~EvpKey ();

  string ToString ();

  /**
   * \brief Static constructor method that creates and returns a new
   *        EvpKey object by loading the key value for the given id.
   *
   * Note: You can choose to define more such static constructors
   * for alternate ways of generating/loading the key.
   *
   * @param keyId The identifier used to load the key
   */
  static Ptr<EvpKey> New (string keyId, EvpKeyType type);

  /**
   * \brief Set the keys directory for loading keys using
   *        static constructor method.
   *
   */
  static void SetKeysDir (string keysDir);

  static string GetKeyFileName (string keyId, EvpKeyType type);

  EVP_PKEY* GetKey () const
  {
    return m_key;
  }

protected:

  static string s_keysDir;

  string m_keyId;

  EvpKeyType m_type;

  EVP_PKEY* m_key;
};

/**
 * \brief A read-only cache for loading EvpKey objects.
 */
class EvpKeyCache
{
public:

  /**
   * \brief Gets a key from the cache. If not in cache, loads
   *        it, adds it to the cache and returns.
   */
  Ptr<EvpKey> GetKey (string keyId, EvpKeyType type);

protected:
  map<string, Ptr<EvpKey> > m_cache;
};

ostream& operator << (ostream& stream, Ptr<EvpKey> evpKey);

} // namespace rapidnet
} // namespace ns3

#endif // EVP_KEY_H
