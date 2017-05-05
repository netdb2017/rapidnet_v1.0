#ifndef PATHVECTOR_HELPER_H
#define PATHVECTOR_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pathvector.h"

namespace ns3 {
namespace rapidnet {
namespace pathvector {

class Pathvector;

class PathvectorHelper: public RapidNetApplicationHelper
{
public:
  PathvectorHelper ()
  {
    m_factory.SetTypeId (Pathvector::GetTypeId ());
  }
  virtual ~PathvectorHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Pathvector> ();
  }
};

} // namespace pathvector
} // namespace rapidnet
} // namespace ns3

#endif // PATHVECTOR_HELPER_H

