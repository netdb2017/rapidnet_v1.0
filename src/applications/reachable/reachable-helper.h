#ifndef REACHABLE_HELPER_H
#define REACHABLE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "reachable.h"

namespace ns3 {
namespace rapidnet {
namespace reachable {

class Reachable;

class ReachableHelper: public RapidNetApplicationHelper
{
public:
  ReachableHelper ()
  {
    m_factory.SetTypeId (Reachable::GetTypeId ());
  }
  virtual ~ReachableHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Reachable> ();
  }
};

} // namespace reachable
} // namespace rapidnet
} // namespace ns3

#endif // REACHABLE_HELPER_H

