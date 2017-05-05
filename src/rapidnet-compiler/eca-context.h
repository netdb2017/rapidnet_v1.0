/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
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

#ifndef __ECA_CONTEXT_H__
#define __ECA_CONTEXT_H__

#include <vector>
#include <map>
#include <set>
#include "all-values.h"
#include "ol-context.h"
#include "parser-util.h"
#include "table-store.h"
#include "localize-context.h"

#include "ns3/log.h"
#include "ns3/ref-count-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet_compiler {

/**
 * \ingroup rapidnet_compiler
 */
class ParseEvent
{
public:
  virtual ~ParseEvent ()
  {
  }

  enum Event
  {
      INSERT,
      DELETE,
      RECV,
      REFRESH
  };

  ParseEvent (ParseFunctor *pf, Event e);

  virtual string ToString ();

  ParseFunctor* pFunctor;

  Event event;

  bool del;     //provenance: delete delta rule flag.
};

/**
 * \ingroup rapidnet_compiler
 */
class ParseAction
{
public:
  virtual ~ParseAction ()
  {
  }

  enum Action
  {
      LOCAL_SEND,
      REMOTE_SEND, // Generate a new event, local or remote
      ADD, // Add to the database
      DELETE, // Remove from the database
      DROP
  // Do nothing. This is a no-op rule
  };

  ParseAction (ParseFunctor *pf, Action a) :
    pFunctor (pf), action (a)
  {
  }

  virtual string ToString ();

  ParseFunctor* pFunctor;

  Action action;
};

/**
 * \ingroup rapidnet_compiler
 *
 * \brief An individual ECA rule structure.
 */
class EcaRule
{
public:
  /** Create a new empty rule */
  EcaRule (string r);

  /** Generate a string representation of the rule */
  string ToString ();

  string ToRuleString ();

  /** What's the name of my event tuple? */
  string GetEventName ();

  string ruleID;

  /** The event of this ECA rule */
  ParseEvent* pEvent;

  /** The action of this rule */
  ParseAction* pAction;

  /** The join conditions of this rule */
  vector<ParseFunctor*> probeTerms;

  /** The selection/assginment conditions of this rule */
  vector<ParseTerm*> selectAssignTerms;

  /** Is this an aggwrap rule? */
  bool aggWrap;
};

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Converts a localized representation into ECA or Event Condition
 *        Action representation.
 *
 * @see LocalizeContext
 */
class EcaContext : public RefCountBase
{
public:

  EcaContext ()
  {
    m_isSendlog = false;
  }

  /** Rewrite all Localized OverLog rules in the localize context into
   ECA rules in the current ECA context, using the given table store
   for state access */
  void Rewrite (Ptr<LocalizeContext> lctxt, Ptr<TableStore> tableStore);

  vector<EcaRule*> GetRules ()
  {
    return m_ecaRules;
  }

  string ToString ();

  vector<EcaRule*> GetEcaRules ()
  {
    return m_ecaRules;
  }

  bool IsSendlog ()
  {
    return m_isSendlog;
  }

private:

  bool m_isSendlog;

  vector<EcaRule*> m_ecaRules;

  void AddRule (EcaRule* eca_rule);

  void RewriteViewRule (OlContext::Rule* rule, Ptr<TableStore> tableStore);

  /** Rewrites a localized, single-event OverLog rule into ECA */
  void RewriteEventRule (OlContext::Rule* rule, Ptr<TableStore> tableStore);

  /** Provenance: Rewrites a localized, single-event OverLog rule with del and ins delta rules into ECA
   * for provenance rewritten rules*/
  void RewriteProvEventRule (OlContext::Rule* rule, Ptr<TableStore> tableStore);

  void RewriteAggregateView (OlContext::Rule* rule, Ptr<TableStore> tableStore);

  /** Generate an action for a rule given its old version (from the
   localized OverLog), its new version so far (i.e., the right hand
   side ECA rule), the location specifier of the body and the table
   store */
  void GenerateActionHead (OlContext::Rule* rule, string bodyLoc,
    Ptr<TableStore> tableStore, EcaRule* eca_rule);

  /** Create a new ECA rule that does nothing. This is used for watched
   events that have no actual ECA rules listening for them.  Without
   an event listener, no watches can be implemented further down in
   the planner */
  void WatchStubRule (string watchedTupleName);
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif
