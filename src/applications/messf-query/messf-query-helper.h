#ifndef MESSFQUERY_HELPER_H
#define MESSFQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "messf-query.h"

namespace ns3 {
namespace rapidnet {
namespace messfquery {

class MessfQuery;

class MessfQueryHelper: public RapidNetApplicationHelper
{
public:
  MessfQueryHelper ()
  {
    m_factory.SetTypeId (MessfQuery::GetTypeId ());
  }
  virtual ~MessfQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<MessfQuery> ();
  }
};

} // namespace messfquery
} // namespace rapidnet
} // namespace ns3

#endif // MESSFQUERY_HELPER_H

