#ifndef MESSF_HELPER_H
#define MESSF_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "messf.h"

namespace ns3 {
namespace rapidnet {
namespace messf {

class Messf;

class MessfHelper: public RapidNetApplicationHelper
{
public:
  MessfHelper ()
  {
    m_factory.SetTypeId (Messf::GetTypeId ());
  }
  virtual ~MessfHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Messf> ();
  }
};

} // namespace messf
} // namespace rapidnet
} // namespace ns3

#endif // MESSF_HELPER_H

