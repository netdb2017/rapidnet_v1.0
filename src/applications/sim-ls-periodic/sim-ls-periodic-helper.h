#ifndef SIMLSPERIODIC_HELPER_H
#define SIMLSPERIODIC_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sim-ls-periodic.h"

namespace ns3 {
namespace rapidnet {
namespace simlsperiodic {

class SimLsPeriodic;

class SimLsPeriodicHelper: public RapidNetApplicationHelper
{
public:
  SimLsPeriodicHelper ()
  {
    m_factory.SetTypeId (SimLsPeriodic::GetTypeId ());
  }
  virtual ~SimLsPeriodicHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SimLsPeriodic> ();
  }
};

} // namespace simlsperiodic
} // namespace rapidnet
} // namespace ns3

#endif // SIMLSPERIODIC_HELPER_H

