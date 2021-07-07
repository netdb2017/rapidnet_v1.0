#ifndef EPIDEMIC_HELPER_H
#define EPIDEMIC_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "epidemic.h"

namespace ns3 {
namespace rapidnet {
namespace epidemic {

class Epidemic;

class EpidemicHelper: public RapidNetApplicationHelper
{
public:
  EpidemicHelper ()
  {
    m_factory.SetTypeId (Epidemic::GetTypeId ());
  }
  virtual ~EpidemicHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Epidemic> ();
  }
};

} // namespace epidemic
} // namespace rapidnet
} // namespace ns3

#endif // EPIDEMIC_HELPER_H

