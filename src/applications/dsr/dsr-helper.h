#ifndef DSR_HELPER_H
#define DSR_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dsr.h"

namespace ns3 {
namespace rapidnet {
namespace dsr {

class Dsr;

class DsrHelper: public RapidNetApplicationHelper
{
public:
  DsrHelper ()
  {
    m_factory.SetTypeId (Dsr::GetTypeId ());
  }
  virtual ~DsrHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Dsr> ();
  }
};

} // namespace dsr
} // namespace rapidnet
} // namespace ns3

#endif // DSR_HELPER_H

