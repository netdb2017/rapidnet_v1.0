/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Pennsylvania
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <map>
#include "ns3/log.h"

#include "ns3/sim-ls-periodic-helper.h"
#include "ns3/sim-ls-triggered-helper.h"
#include "ns3/sim-hsls-periodic-helper.h"
#include "ns3/sim-hsls-triggered-helper.h"

#include "ns3/emu-ls-periodic-helper.h"
#include "ns3/emu-ls-triggered-helper.h"
#include "ns3/emu-hsls-periodic-helper.h"
#include "ns3/emu-hsls-triggered-helper.h"

#include "ns3/dsr-helper.h"
#include "ns3/epidemic-helper.h"

#include "ns3/chord-helper.h"

#include "ns3/discovery-helper.h"
#include "ns3/reachable2-helper.h"
#include "ns3/pathvector2-helper.h"

// Define app names as constants for convenience
#define DEFAULT_RAPIDNET_APP_HELPER_NAME "None"

#define SIM_LS_PERIODIC_APP "SimLsPeriodic"
#define SIM_LS_TRIGGERED_APP "SimLsTriggered"
#define SIM_HSLS_PERIODIC_APP "SimHslsPeriodic"
#define SIM_HSLS_TRIGGERED_APP "SimHslsTriggered"

#define EMU_LS_PERIODIC_APP "EmuLsPeriodic"
#define EMU_LS_TRIGGERED_APP "EmuLsTriggered"
#define EMU_HSLS_PERIODIC_APP "EmuHslsPeriodic"
#define EMU_HSLS_TRIGGERED_APP "EmuHslsTriggered"

#define DISCOVERY_APP "Discovery"
#define DSR_APP "Dsr"
#define EPIDEMIC_APP "Epidemic"

#define CHORD_APP "Chord"

#define REACHABLE2_APP "Reachable2"
#define PATHVECTOR2_APP "Pathvector2"


namespace ns3 {
namespace rapidnet {

Ptr<RapidNetApplicationHelper>
GetRapidNetAppHelper (string appName)
{
  map<string, Ptr<RapidNetApplicationHelper> > helpers;

  helpers[SIM_LS_PERIODIC_APP] = Create<simlsperiodic::SimLsPeriodicHelper> ();
  helpers[SIM_LS_TRIGGERED_APP] = Create<simlstriggered::SimLsTriggeredHelper> ();
  helpers[SIM_HSLS_PERIODIC_APP] = Create<simhslsperiodic::SimHslsPeriodicHelper> ();
  helpers[SIM_HSLS_TRIGGERED_APP] = Create<simhslstriggered::SimHslsTriggeredHelper> ();

  helpers[EMU_LS_PERIODIC_APP] = Create<emulsperiodic::EmuLsPeriodicHelper> ();
  helpers[EMU_LS_TRIGGERED_APP] = Create<emulstriggered::EmuLsTriggeredHelper> ();
  helpers[EMU_HSLS_PERIODIC_APP] = Create<emuhslsperiodic::EmuHslsPeriodicHelper> ();
  helpers[EMU_HSLS_TRIGGERED_APP] = Create<emuhslstriggered::EmuHslsTriggeredHelper> ();

  helpers[DSR_APP] = Create<dsr::DsrHelper> ();
  helpers[EPIDEMIC_APP] = Create<epidemic::EpidemicHelper> ();

  helpers[CHORD_APP] = Create<chord::ChordHelper> ();

  helpers[DISCOVERY_APP] = Create<discovery::DiscoveryHelper> ();
  helpers[REACHABLE2_APP] = Create<pathvector2::Pathvector2Helper> ();
  helpers[PATHVECTOR2_APP] = Create<reachable2::Reachable2Helper> ();

  // Add your new application's helper here:
  // helpers[YOUR_APP] = Create<yourAppNS::yourAppHelper> ();

  if (helpers.find (appName) == helpers.end ())
    {
      cout << "ERROR: No helper found for name: " << appName << endl;
      return Ptr<RapidNetApplicationHelper> (NULL);
    }

    return helpers[appName];
}

} // namespace rapidnet
} // namespace ns3
