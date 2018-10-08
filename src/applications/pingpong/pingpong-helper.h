#ifndef PINGPONG_HELPER_H
#define PINGPONG_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pingpong.h"

namespace ns3 {
namespace rapidnet {
namespace pingpong {

class Pingpong;

class PingpongHelper: public RapidNetApplicationHelper
{
public:
  PingpongHelper ()
  {
    m_factory.SetTypeId (Pingpong::GetTypeId ());
  }
  virtual ~PingpongHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Pingpong> ();
  }
};

} // namespace pingpong
} // namespace rapidnet
} // namespace ns3

#endif // PINGPONG_HELPER_H

