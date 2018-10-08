#ifndef EMULSTRIGGERED_HELPER_H
#define EMULSTRIGGERED_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "emu-ls-triggered.h"

namespace ns3 {
namespace rapidnet {
namespace emulstriggered {

class EmuLsTriggered;

class EmuLsTriggeredHelper: public RapidNetApplicationHelper
{
public:
  EmuLsTriggeredHelper ()
  {
    m_factory.SetTypeId (EmuLsTriggered::GetTypeId ());
  }
  virtual ~EmuLsTriggeredHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<EmuLsTriggered> ();
  }
};

} // namespace emulstriggered
} // namespace rapidnet
} // namespace ns3

#endif // EMULSTRIGGERED_HELPER_H

