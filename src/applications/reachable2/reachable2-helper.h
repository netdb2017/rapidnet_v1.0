#ifndef REACHABLE2_HELPER_H
#define REACHABLE2_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "reachable2.h"

namespace ns3 {
namespace rapidnet {
namespace reachable2 {

class Reachable2;

class Reachable2Helper: public RapidNetApplicationHelper
{
public:
  Reachable2Helper ()
  {
    m_factory.SetTypeId (Reachable2::GetTypeId ());
  }
  virtual ~Reachable2Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Reachable2> ();
  }
};

} // namespace reachable2
} // namespace rapidnet
} // namespace ns3

#endif // REACHABLE2_HELPER_H

