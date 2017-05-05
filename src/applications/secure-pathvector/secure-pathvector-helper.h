#ifndef SECUREPATHVECTOR_HELPER_H
#define SECUREPATHVECTOR_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "secure-pathvector.h"

namespace ns3 {
namespace rapidnet {
namespace securepathvector {

class SecurePathvector;

class SecurePathvectorHelper: public RapidNetApplicationHelper
{
public:
  SecurePathvectorHelper ()
  {
    m_factory.SetTypeId (SecurePathvector::GetTypeId ());
  }
  virtual ~SecurePathvectorHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SecurePathvector> ();
  }
};

} // namespace securepathvector
} // namespace rapidnet
} // namespace ns3

#endif // SECUREPATHVECTOR_HELPER_H

