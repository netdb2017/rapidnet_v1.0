#ifndef CHORD_HELPER_H
#define CHORD_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "chord.h"

namespace ns3 {
namespace rapidnet {
namespace chord {

class Chord;

class ChordHelper: public RapidNetApplicationHelper
{
public:
  ChordHelper ()
  {
    m_factory.SetTypeId (Chord::GetTypeId ());
  }
  virtual ~ChordHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Chord> ();
  }
};

} // namespace chord
} // namespace rapidnet
} // namespace ns3

#endif // CHORD_HELPER_H

