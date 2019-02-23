#ifndef VQA_HELPER_H
#define VQA_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "vqa.h"

namespace ns3 {
namespace rapidnet {
namespace vqa {

class Vqa;

class VqaHelper: public RapidNetApplicationHelper
{
public:
  VqaHelper ()
  {
    m_factory.SetTypeId (Vqa::GetTypeId ());
  }
  virtual ~VqaHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Vqa> ();
  }
};

} // namespace vqa
} // namespace rapidnet
} // namespace ns3

#endif // VQA_HELPER_H

