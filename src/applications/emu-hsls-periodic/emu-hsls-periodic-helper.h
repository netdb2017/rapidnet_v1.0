#ifndef EMUHSLSPERIODIC_HELPER_H
#define EMUHSLSPERIODIC_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "emu-hsls-periodic.h"

namespace ns3 {
namespace rapidnet {
namespace emuhslsperiodic {

class EmuHslsPeriodic;

class EmuHslsPeriodicHelper: public RapidNetApplicationHelper
{
public:
  EmuHslsPeriodicHelper ()
  {
    m_factory.SetTypeId (EmuHslsPeriodic::GetTypeId ());
  }
  virtual ~EmuHslsPeriodicHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<EmuHslsPeriodic> ();
  }
};

} // namespace emuhslsperiodic
} // namespace rapidnet
} // namespace ns3

#endif // EMUHSLSPERIODIC_HELPER_H

