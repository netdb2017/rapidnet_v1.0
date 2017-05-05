#ifndef SIMHSLSTRIGGERED_HELPER_H
#define SIMHSLSTRIGGERED_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sim-hsls-triggered.h"

namespace ns3 {
namespace rapidnet {
namespace simhslstriggered {

class SimHslsTriggered;

class SimHslsTriggeredHelper: public RapidNetApplicationHelper
{
public:
  SimHslsTriggeredHelper ()
  {
    m_factory.SetTypeId (SimHslsTriggered::GetTypeId ());
  }
  virtual ~SimHslsTriggeredHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SimHslsTriggered> ();
  }
};

} // namespace simhslstriggered
} // namespace rapidnet
} // namespace ns3

#endif // SIMHSLSTRIGGERED_HELPER_H

