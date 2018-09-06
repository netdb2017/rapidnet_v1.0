#ifndef EMULSPERIODIC_HELPER_H
#define EMULSPERIODIC_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "emu-ls-periodic.h"

namespace ns3 {
namespace rapidnet {
namespace emulsperiodic {

class EmuLsPeriodic;

class EmuLsPeriodicHelper: public RapidNetApplicationHelper
{
public:
  EmuLsPeriodicHelper ()
  {
    m_factory.SetTypeId (EmuLsPeriodic::GetTypeId ());
  }
  virtual ~EmuLsPeriodicHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<EmuLsPeriodic> ();
  }
};

} // namespace emulsperiodic
} // namespace rapidnet
} // namespace ns3

#endif // EMULSPERIODIC_HELPER_H

