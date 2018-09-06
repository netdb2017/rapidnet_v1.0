#ifndef PATHVECTORPROV_HELPER_H
#define PATHVECTORPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pathvector-prov.h"

namespace ns3 {
namespace rapidnet {
namespace pathvectorprov {

class PathvectorProv;

class PathvectorProvHelper: public RapidNetApplicationHelper
{
public:
  PathvectorProvHelper ()
  {
    m_factory.SetTypeId (PathvectorProv::GetTypeId ());
  }
  virtual ~PathvectorProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PathvectorProv> ();
  }
};

} // namespace pathvectorprov
} // namespace rapidnet
} // namespace ns3

#endif // PATHVECTORPROV_HELPER_H

