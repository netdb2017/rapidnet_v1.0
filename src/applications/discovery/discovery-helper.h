#ifndef DISCOVERY_HELPER_H
#define DISCOVERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "discovery.h"

namespace ns3 {
namespace rapidnet {
namespace discovery {

class Discovery;

class DiscoveryHelper: public RapidNetApplicationHelper
{
public:
  DiscoveryHelper ()
  {
    m_factory.SetTypeId (Discovery::GetTypeId ());
  }
  virtual ~DiscoveryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Discovery> ();
  }
};

} // namespace discovery
} // namespace rapidnet
} // namespace ns3

#endif // DISCOVERY_HELPER_H

