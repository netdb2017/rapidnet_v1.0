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

#include <string>
#include <sstream>
#include <cstdlib>

#include "ns3/pki-authentication-manager.h"
#include "ns3/test.h"

#include "ns3/assert.h"
#include "ns3/ptr.h"
#include "ns3/inet-socket-address.h"
#include "ns3/simulator.h"
#include "ns3/rapidnet-types.h"
#include "ns3/tuple.h"
#include "ns3/tuple-attribute.h"
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
 * \brief Tests RapidNet PkiAuthenticationManager.
 *
 * @see PkiAuthenticationManager
 */
class PkiAuthenticationTest: public Test
{
public:

  PkiAuthenticationTest () :
    Test ("Rapidnet-PkiAuthenticationTest")
  {
  }

  virtual ~PkiAuthenticationTest ()
  {
  }

  virtual bool RunTests (void);

protected:

  Ptr<SendlogAuthenticationManager> GetAuthenticationMgr (string addr);

  Ptr<Tuple> CreateSomeTuple ();

  bool TestPositive ();

  bool TestNegative1 ();

  bool TestNegative2 ();
};

Ptr<SendlogAuthenticationManager>
PkiAuthenticationTest::GetAuthenticationMgr (string addr)
{
  Ptr<RapidNetApplicationBase> dummyApp = CreateObject<RapidNetApplicationBase> ();
  dummyApp->SetAddress (Ipv4Address (addr.c_str ()));
  return PkiAuthenticationManager::New (dummyApp);
}

Ptr<Tuple>
PkiAuthenticationTest::CreateSomeTuple ()
{
  Ptr<Tuple> tuple = Tuple::New ("Tuple");
  tuple->AddAttribute (TupleAttribute::New ("attr1", Int32Value::New (10)));
  tuple->AddAttribute (TupleAttribute::New ("attr2", StrValue::New ("content")));
  tuple->AddAttribute (TupleAttribute::New ("attr3", RealValue::New (6.01)));
  tuple->AddAttribute (TupleAttribute::New ("attr4", Ipv4Value::New (
    Ipv4Address ("192.168.1.1"))));
  tuple->AddAttribute (TupleAttribute::New ("rn-src", Ipv4Value::New (
    Ipv4Address ("6.0.0.1"))));
  return tuple;
}

bool
PkiAuthenticationTest::RunTests ()
{
  //LogComponentEnable("PkiAuthenticationManager", LOG_LEVEL_INFO);
  EvpKey::SetKeysDir ("../rapidnet/keys/");

  bool result = true;

  result = TestPositive () && TestNegative1 () && TestNegative2 ();

  return result;
}

bool
PkiAuthenticationTest::TestPositive ()
{
  string principalIp = "10.1.1.2";
  Ptr<SendlogAuthenticationManager> authMgr = GetAuthenticationMgr (principalIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();
  authMgr->Sign (tuple);

  Ptr<TupleAttribute> attr = TupleAttribute::New ("principal",
    Ipv4Value::New (principalIp.c_str ()));

  return authMgr->Verify (tuple, attr);
}

bool
PkiAuthenticationTest::TestNegative1 ()
{
  string principalIp = "10.1.1.1";
  Ptr<SendlogAuthenticationManager> authMgr = GetAuthenticationMgr (principalIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();
  authMgr->Sign (tuple);

  Ptr<TupleAttribute> attr = TupleAttribute::New ("principal",
    Ipv4Value::New ("10.1.1.2"));

  return !authMgr->Verify (tuple, attr);
}

bool
PkiAuthenticationTest::TestNegative2 ()
{
  string principalIp = "10.1.1.1";
  Ptr<SendlogAuthenticationManager> authMgr = GetAuthenticationMgr (principalIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();
  authMgr->Sign (tuple);

  tuple->RemoveAttribute (PKI_ATTR_SIGNATURE);

  Ptr<TupleAttribute> attr = TupleAttribute::New ("principal",
    Ipv4Value::New (principalIp.c_str ()));

  return !authMgr->Verify (tuple, attr);
}

static PkiAuthenticationTest g_pkiAuthentictionTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
