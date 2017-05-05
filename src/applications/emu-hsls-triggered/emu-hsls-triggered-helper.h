#ifndef EMUHSLSTRIGGERED_HELPER_H
#define EMUHSLSTRIGGERED_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "emu-hsls-triggered.h"

namespace ns3 {
namespace rapidnet {
namespace emuhslstriggered {

class EmuHslsTriggered;

class EmuHslsTriggeredHelper: public RapidNetApplicationHelper
{
public:
  EmuHslsTriggeredHelper ()
  {
    m_factory.SetTypeId (EmuHslsTriggered::GetTypeId ());
  }
  virtual ~EmuHslsTriggeredHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<EmuHslsTriggered> ();
  }
};

} // namespace emuhslstriggered
} // namespace rapidnet
} // namespace ns3

#endif // EMUHSLSTRIGGERED_HELPER_H

