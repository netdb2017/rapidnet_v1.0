#ifndef PINGPONGL4_HELPER_H
#define PINGPONGL4_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pingpong-l4.h"

namespace ns3 {
namespace rapidnet {
namespace pingpongl4 {

class PingpongL4;

class PingpongL4Helper: public RapidNetApplicationHelper
{
public:
  PingpongL4Helper ()
  {
    m_factory.SetTypeId (PingpongL4::GetTypeId ());
  }
  virtual ~PingpongL4Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PingpongL4> ();
  }
};

} // namespace pingpongl4
} // namespace rapidnet
} // namespace ns3

#endif // PINGPONGL4_HELPER_H

