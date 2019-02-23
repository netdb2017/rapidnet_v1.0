#ifndef VQAQUERY_HELPER_H
#define VQAQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "vqa-query.h"

namespace ns3 {
namespace rapidnet {
namespace vqaquery {

class VqaQuery;

class VqaQueryHelper: public RapidNetApplicationHelper
{
public:
  VqaQueryHelper ()
  {
    m_factory.SetTypeId (VqaQuery::GetTypeId ());
  }
  virtual ~VqaQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<VqaQuery> ();
  }
};

} // namespace vqaquery
} // namespace rapidnet
} // namespace ns3

#endif // VQAQUERY_HELPER_H

