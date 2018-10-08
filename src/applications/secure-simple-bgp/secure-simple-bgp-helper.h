#ifndef SECURESIMPLEBGP_HELPER_H
#define SECURESIMPLEBGP_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "secure-simple-bgp.h"

namespace ns3 {
namespace rapidnet {
namespace securesimplebgp {

class SecureSimpleBgp;

class SecureSimpleBgpHelper: public RapidNetApplicationHelper
{
public:
  SecureSimpleBgpHelper ()
  {
    m_factory.SetTypeId (SecureSimpleBgp::GetTypeId ());
  }
  virtual ~SecureSimpleBgpHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SecureSimpleBgp> ();
  }
};

} // namespace securesimplebgp
} // namespace rapidnet
} // namespace ns3

#endif // SECURESIMPLEBGP_HELPER_H

