#ifndef MINCOSTPROV_HELPER_H
#define MINCOSTPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "mincost-prov.h"

namespace ns3 {
namespace rapidnet {
namespace mincostprov {

class MincostProv;

class MincostProvHelper: public RapidNetApplicationHelper
{
public:
  MincostProvHelper ()
  {
    m_factory.SetTypeId (MincostProv::GetTypeId ());
  }
  virtual ~MincostProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<MincostProv> ();
  }
};

} // namespace mincostprov
} // namespace rapidnet
} // namespace ns3

#endif // MINCOSTPROV_HELPER_H

