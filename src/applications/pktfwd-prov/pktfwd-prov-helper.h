#ifndef PKTFWDPROV_HELPER_H
#define PKTFWDPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-prov.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdprov {

class PktfwdProv;

class PktfwdProvHelper: public RapidNetApplicationHelper
{
public:
  PktfwdProvHelper ()
  {
    m_factory.SetTypeId (PktfwdProv::GetTypeId ());
  }
  virtual ~PktfwdProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdProv> ();
  }
};

} // namespace pktfwdprov
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDPROV_HELPER_H

