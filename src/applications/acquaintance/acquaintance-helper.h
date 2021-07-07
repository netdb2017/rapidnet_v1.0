#ifndef ACQUAINTANCE_HELPER_H
#define ACQUAINTANCE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "acquaintance.h"

namespace ns3 {
namespace rapidnet {
namespace acquaintance {

class Acquaintance;

class AcquaintanceHelper: public RapidNetApplicationHelper
{
public:
  AcquaintanceHelper ()
  {
    m_factory.SetTypeId (Acquaintance::GetTypeId ());
  }
  virtual ~AcquaintanceHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Acquaintance> ();
  }
};

} // namespace acquaintance
} // namespace rapidnet
} // namespace ns3

#endif // ACQUAINTANCE_HELPER_H

