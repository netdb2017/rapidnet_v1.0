#ifndef SIMHSLSPERIODIC_HELPER_H
#define SIMHSLSPERIODIC_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sim-hsls-periodic.h"

namespace ns3 {
namespace rapidnet {
namespace simhslsperiodic {

class SimHslsPeriodic;

class SimHslsPeriodicHelper: public RapidNetApplicationHelper
{
public:
  SimHslsPeriodicHelper ()
  {
    m_factory.SetTypeId (SimHslsPeriodic::GetTypeId ());
  }
  virtual ~SimHslsPeriodicHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SimHslsPeriodic> ();
  }
};

} // namespace simhslsperiodic
} // namespace rapidnet
} // namespace ns3

#endif // SIMHSLSPERIODIC_HELPER_H

