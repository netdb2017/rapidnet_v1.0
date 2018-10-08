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

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "ns3/test.h"
#include "ns3/packet.h"
#include "ns3/evp-key.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {


/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests whether a EvpKey loads properly. The assertions
 *        in the static constructor method should hold, that is
 *        enough for this test to pass.
 *
 */
class EvpKeyTest : public Test
{
public:

  EvpKeyTest () : Test ("Rapidnet-EvpKeyTest") {}

  virtual ~EvpKeyTest () {}

  virtual bool RunTests (void);

protected:

  bool Test1 ();

  bool Test2 ();

  bool Test3 ();
};

bool
EvpKeyTest::RunTests ()
{
  EvpKey::SetKeysDir ("../rapidnet/keys/");

  return Test1 () && Test2 () && Test3 ();
}

bool
EvpKeyTest::Test1 ()
{
  Ptr<EvpKey> privateKey = EvpKey::New ("10.1.1.1", EVP_PRIVATE_KEY);
  Ptr<EvpKey> publicKey = EvpKey::New ("10.1.1.1", EVP_PUBLIC_KEY);
  return true;
}

bool
EvpKeyTest::Test2 ()
{
  EvpKeyCache keys;
  keys.GetKey ("10.1.1.1", EVP_PRIVATE_KEY);
  keys.GetKey ("10.1.1.1", EVP_PUBLIC_KEY);
  keys.GetKey ("10.1.1.1", EVP_PRIVATE_KEY);
  return true;
}

bool
EvpKeyTest::Test3 ()
{
  Ptr<EvpKey> privateKey = EvpKey::New ("junk", EVP_PRIVATE_KEY);
  return privateKey->GetKey () == NULL;
}


static EvpKeyTest g_evpKeyTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
