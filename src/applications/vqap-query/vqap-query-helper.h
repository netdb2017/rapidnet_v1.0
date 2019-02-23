#ifndef VQAPQUERY_HELPER_H
#define VQAPQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "vqap-query.h"

namespace ns3 {
namespace rapidnet {
namespace vqapquery {

class VqapQuery;

class VqapQueryHelper: public RapidNetApplicationHelper
{
public:
  VqapQueryHelper ()
  {
    m_factory.SetTypeId (VqapQuery::GetTypeId ());
  }
  virtual ~VqapQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<VqapQuery> ();
  }
};

} // namespace vqapquery
} // namespace rapidnet
} // namespace ns3

#endif // VQAPQUERY_HELPER_H

