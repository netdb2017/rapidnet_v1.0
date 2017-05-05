#ifndef DISCOVERYEPIDEMIC_HELPER_H
#define DISCOVERYEPIDEMIC_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "discovery-epidemic.h"

namespace ns3 {
namespace rapidnet {
namespace discoveryepidemic {

class DiscoveryEpidemic;

class DiscoveryEpidemicHelper: public RapidNetApplicationHelper
{
public:
  DiscoveryEpidemicHelper ()
  {
    m_factory.SetTypeId (DiscoveryEpidemic::GetTypeId ());
  }
  virtual ~DiscoveryEpidemicHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<DiscoveryEpidemic> ();
  }
};

} // namespace discoveryepidemic
} // namespace rapidnet
} // namespace ns3

#endif // DISCOVERYEPIDEMIC_HELPER_H

