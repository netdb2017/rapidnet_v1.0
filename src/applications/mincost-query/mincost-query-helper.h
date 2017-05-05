#ifndef MINCOSTQUERY_HELPER_H
#define MINCOSTQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "mincost-query.h"

namespace ns3 {
namespace rapidnet {
namespace mincostquery {

class MincostQuery;

class MincostQueryHelper: public RapidNetApplicationHelper
{
public:
  MincostQueryHelper ()
  {
    m_factory.SetTypeId (MincostQuery::GetTypeId ());
  }
  virtual ~MincostQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<MincostQuery> ();
  }
};

} // namespace mincostquery
} // namespace rapidnet
} // namespace ns3

#endif // MINCOSTQUERY_HELPER_H

