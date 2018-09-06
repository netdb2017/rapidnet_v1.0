#ifndef PATHVECTOR2_HELPER_H
#define PATHVECTOR2_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pathvector2.h"

namespace ns3 {
namespace rapidnet {
namespace pathvector2 {

class Pathvector2;

class Pathvector2Helper: public RapidNetApplicationHelper
{
public:
  Pathvector2Helper ()
  {
    m_factory.SetTypeId (Pathvector2::GetTypeId ());
  }
  virtual ~Pathvector2Helper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Pathvector2> ();
  }
};

} // namespace pathvector2
} // namespace rapidnet
} // namespace ns3

#endif // PATHVECTOR2_HELPER_H

