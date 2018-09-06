#ifndef SECUREPINGPONG_HELPER_H
#define SECUREPINGPONG_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "secure-pingpong.h"

namespace ns3 {
namespace rapidnet {
namespace securepingpong {

class SecurePingpong;

class SecurePingpongHelper: public RapidNetApplicationHelper
{
public:
  SecurePingpongHelper ()
  {
    m_factory.SetTypeId (SecurePingpong::GetTypeId ());
  }
  virtual ~SecurePingpongHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SecurePingpong> ();
  }
};

} // namespace securepingpong
} // namespace rapidnet
} // namespace ns3

#endif // SECUREPINGPONG_HELPER_H

