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

#include "ns3/log.h"
#include "ns3/test.h"
#include "ns3/ptr.h"
#include "ns3/assert.h"
#include "ns3/rapidnet-types.h"
#include "ns3/tuple.h"
#include "ns3/tuple-attribute.h"
#include "ns3/evp-key.h"
#include "ns3/blowfish-encryption-manager.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

namespace ns3 {
namespace rapidnet {
namespace tests {

/**
 * \ingroup rapidnet_tests
 *
 * \brief Tests RapidNet BlowfishEncryptionManager.
 *
 * @see BlowfishEncryptionManager
 */
class BlowfishEncryptionTest: public Test
{
public:

	BlowfishEncryptionTest () :
    Test ("Rapidnet-BlowfishEncryptionTest")
  {
  }

  virtual ~BlowfishEncryptionTest ()
  {
  }

  virtual bool RunTests (void);

protected:

  Ptr<SendlogEncryptionManager> GetEncryptionMgr (string addr);

  Ptr<Tuple> CreateSomeTuple ();

  bool TestPositive ();

  bool TestNegativeTargetMismatch ();

  bool TestNegativeAttrsMissing1 ();

  bool TestNegativeAttrsMissing2 ();

  bool TestNegativeAttrsMissing3 ();
};

Ptr<SendlogEncryptionManager>
BlowfishEncryptionTest::GetEncryptionMgr (string addr)
{
  Ptr<RapidNetApplicationBase> dummyApp = CreateObject<RapidNetApplicationBase> ();
  dummyApp->SetAddress (Ipv4Address (addr.c_str ()));
  return BlowfishEncryptionManager::New (dummyApp);
}

Ptr<Tuple>
BlowfishEncryptionTest::CreateSomeTuple ()
{
  Ptr<Tuple> tuple = Tuple::New ("Tuple");
  tuple->AddAttribute (TupleAttribute::New ("attr1", Int32Value::New (10)));
  tuple->AddAttribute (TupleAttribute::New ("attr2", StrValue::New ("content")));
  tuple->AddAttribute (TupleAttribute::New ("attr3", RealValue::New (6.01)));
  tuple->AddAttribute (TupleAttribute::New ("attr4", Ipv4Value::New (
    Ipv4Address ("192.168.1.1"))));
  return tuple;
}

bool
BlowfishEncryptionTest::RunTests ()
{
  //LogComponentEnable("BlowfishEncryptionManager", LOG_LEVEL_INFO);
  EvpKey::SetKeysDir ("../rapidnet/keys/");

  bool result = true;

  result = TestPositive () &&
    TestNegativeTargetMismatch () &&
    TestNegativeAttrsMissing1 () &&
    TestNegativeAttrsMissing2 () &&
    TestNegativeAttrsMissing3 ();

  return result;
}

bool
BlowfishEncryptionTest::TestPositive ()
{
  string targetIp = "10.1.1.2";
  Ptr<SendlogEncryptionManager> encryMgr = GetEncryptionMgr (targetIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();

  Ptr<TupleAttribute> targetAttr = TupleAttribute::New ("target",
    Ipv4Value::New (targetIp.c_str ()));
  Ptr<Tuple> encryptedTuple = encryMgr->Encrypt (tuple, targetAttr);

  Ptr<Tuple> decryptedTuple = encryMgr->Decrypt (encryptedTuple);
  return decryptedTuple->Equals(tuple);
}

bool
BlowfishEncryptionTest::TestNegativeTargetMismatch ()
{
  string targetIp = "10.1.1.1";
  Ptr<SendlogEncryptionManager> encryMgr = GetEncryptionMgr (targetIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();

  Ptr<TupleAttribute> otherTarget = TupleAttribute::New ("target",
    Ipv4Value::New ("10.1.1.2"));

  Ptr<Tuple> encryptedTuple = encryMgr->Encrypt (tuple, otherTarget);

  Ptr<Tuple> decryptedTuple = encryMgr->Decrypt(encryptedTuple);

  return decryptedTuple->IsEmpty ();
}

bool
BlowfishEncryptionTest::TestNegativeAttrsMissing1 ()
{
  string targetIp = "10.1.1.2";
  Ptr<SendlogEncryptionManager> encryMgr = GetEncryptionMgr (targetIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();

  Ptr<TupleAttribute> targetAttr = TupleAttribute::New ("target",
    Ipv4Value::New (targetIp.c_str ()));
  Ptr<Tuple> encryptedTuple = encryMgr->Encrypt (tuple, targetAttr);

  // Remove an attribute
  encryptedTuple->RemoveAttribute (BLOWFISH_ATTR_CIPHER);

  Ptr<Tuple> decryptedTuple = encryMgr->Decrypt (encryptedTuple);
  return decryptedTuple->IsEmpty ();
}

bool
BlowfishEncryptionTest::TestNegativeAttrsMissing2 ()
{
  string targetIp = "10.1.1.2";
  Ptr<SendlogEncryptionManager> encryMgr = GetEncryptionMgr (targetIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();

  Ptr<TupleAttribute> targetAttr = TupleAttribute::New ("target",
    Ipv4Value::New (targetIp.c_str ()));
  Ptr<Tuple> encryptedTuple = encryMgr->Encrypt (tuple, targetAttr);

  // Remove an attribute
  encryptedTuple->RemoveAttribute (BLOWFISH_ATTR_SECRET_KEY);

  Ptr<Tuple> decryptedTuple = encryMgr->Decrypt (encryptedTuple);
  return decryptedTuple->IsEmpty ();
}

bool
BlowfishEncryptionTest::TestNegativeAttrsMissing3 ()
{
  string targetIp = "10.1.1.2";
  Ptr<SendlogEncryptionManager> encryMgr = GetEncryptionMgr (targetIp);
  Ptr<Tuple> tuple = CreateSomeTuple ();

  Ptr<TupleAttribute> targetAttr = TupleAttribute::New ("target",
    Ipv4Value::New (targetIp.c_str ()));
  Ptr<Tuple> encryptedTuple = encryMgr->Encrypt (tuple, targetAttr);

  // Remove an attribute
  encryptedTuple->RemoveAttribute (BLOWFISH_ATTR_INIT_VECTOR);

  Ptr<Tuple> decryptedTuple = encryMgr->Decrypt (encryptedTuple);
  return decryptedTuple->IsEmpty ();
}

static BlowfishEncryptionTest g_blowfishEncryptionTest;

} // namespace tests
} // namespace rapidnet
} // namespace ns3
