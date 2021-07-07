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

#ifndef __LOCAL_CONTEXT_H__
#define __LOCAL_CONTEXT_H__

#include <vector>
#include <map>
#include <set>
#include "all-values.h"
#include "ol-context.h"
#include "parser-util.h"
#include "table-store.h"

#include "ns3/log.h"
#include "ns3/ref-count-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet_compiler {

typedef vector<uint32_t> Key;

class FieldNamesTracker
{
public:
  vector<string> fieldNames;
  FieldNamesTracker ();
  FieldNamesTracker (ParseTerm* pf);

  void Initialize (ParseTerm* pf);
  vector<int> MatchingJoinKeys (vector<string> names);
  void MergeWith (vector<string> names);
  void MergeWith (vector<string> names, int numJoinKeys);
  void JoinKeys (FieldNamesTracker* probeNames, Key& lookupKey, Key& indexKey,
    Key& remainingBaseKey);

  int FieldPosition (string var);
  string ToString ();
};

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Translates the parsed NDlog protocol in OlContext to
 *        a localized representation.
 *
 * @see OlContext
 */
class LocalizeContext: public RefCountBase
{

public:

  LocalizeContext ()
  {
    m_isSendlog = false;
  }

  void Rewrite (Ptr<OlContext> ctxt, Ptr<TableStore> tableStore);
  string ToString ();
  vector<OlContext::Rule*> GetRules ()
  {
    return _localizedRules;
  }

  bool IsSendlog ()
  {
    return m_isSendlog;
  }

private:
  void RewriteRule (OlContext::Rule* rule, Ptr<TableStore> tableStore);
  OlContext::Rule* AddSendRule (OlContext::Rule* nextRule,
    list<ParseTerm*> newTerms, string newFunctorName, ParseFunctor* functor,
    string loc, long minLifetime, vector<string> fieldNames,
    Ptr<TableStore> tableStore);
  void AddRule (OlContext::Rule* rule);
  vector<OlContext::Rule*> _localizedRules;

  bool m_isSendlog;
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif
