#ifndef VQAP_HELPER_H
#define VQAP_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "vqap.h"

namespace ns3 {
namespace rapidnet {
namespace vqap {

class Vqap;

class VqapHelper: public RapidNetApplicationHelper
{
public:
  VqapHelper ()
  {
    m_factory.SetTypeId (Vqap::GetTypeId ());
  }
  virtual ~VqapHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Vqap> ();
  }
};

} // namespace vqap
} // namespace rapidnet
} // namespace ns3

#endif // VQAP_HELPER_H

