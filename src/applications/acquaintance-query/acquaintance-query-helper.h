#ifndef ACQUAINTANCEQUERY_HELPER_H
#define ACQUAINTANCEQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "acquaintance-query.h"

namespace ns3 {
namespace rapidnet {
namespace acquaintancequery {

class AcquaintanceQuery;

class AcquaintanceQueryHelper: public RapidNetApplicationHelper
{
public:
  AcquaintanceQueryHelper ()
  {
    m_factory.SetTypeId (AcquaintanceQuery::GetTypeId ());
  }
  virtual ~AcquaintanceQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<AcquaintanceQuery> ();
  }
};

} // namespace acquaintancequery
} // namespace rapidnet
} // namespace ns3

#endif // ACQUAINTANCEQUERY_HELPER_H

