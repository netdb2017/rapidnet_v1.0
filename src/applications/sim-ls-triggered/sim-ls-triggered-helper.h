#ifndef SIMLSTRIGGERED_HELPER_H
#define SIMLSTRIGGERED_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sim-ls-triggered.h"

namespace ns3 {
namespace rapidnet {
namespace simlstriggered {

class SimLsTriggered;

class SimLsTriggeredHelper: public RapidNetApplicationHelper
{
public:
  SimLsTriggeredHelper ()
  {
    m_factory.SetTypeId (SimLsTriggered::GetTypeId ());
  }
  virtual ~SimLsTriggeredHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SimLsTriggered> ();
  }
};

} // namespace simlstriggered
} // namespace rapidnet
} // namespace ns3

#endif // SIMLSTRIGGERED_HELPER_H

