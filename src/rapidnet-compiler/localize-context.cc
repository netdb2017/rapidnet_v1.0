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

#include "localize-context.h"
#include "errno.h"
#include <set>

using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("LocalizeContext");

FieldNamesTracker::FieldNamesTracker ()
{
}

FieldNamesTracker::FieldNamesTracker (ParseTerm* pf)
{
  Initialize (pf);
}

void FieldNamesTracker::Initialize (ParseTerm* term)
{
  ParseFunctor* pf = dynamic_cast<ParseFunctor*> (term);

  if (pf != NULL)
    {
      for (int k = 0; k < pf->Args (); k++)
        {
          ParseVar* parse_var = dynamic_cast<ParseVar*> (pf->Arg (k));
          if (parse_var != NULL)
            {
              fieldNames.push_back (parse_var->ToString ());
            }
        }
    }
}

int FieldNamesTracker::FieldPosition (string var)
{
  for (size_t k = 0; k < fieldNames.size (); k++) 
    {
      if (fieldNameEq (fieldNames.at (k), var))
        {
          return k;
        }
    }
  return -1;
}

void FieldNamesTracker::MergeWith (vector<string> names)
{
  for (size_t k = 0; k < names.size (); k++)
    {
      string nextStr = names.at (k);
      if (FieldPosition (nextStr) == -1)
        {
          fieldNames.push_back (nextStr);
        }
    }
}

void FieldNamesTracker::MergeWith (vector<string> names, int numJoinKeys)
{
  int count = 0;
  for (size_t k = 0; k < names.size (); k++)
    {
      string nextStr = names.at (k);
      if (count == numJoinKeys || FieldPosition (nextStr) == -1)
        {
          fieldNames.push_back (nextStr);
          count++;
        }
    }
}

string FieldNamesTracker::ToString ()
{
  ostringstream toRet;

  toRet << "FieldNamesTracker<";

  for (size_t k = 0; k < fieldNames.size (); k++)
    {
      toRet << fieldNames.at (k) << " ";
    }
  toRet << ">";
  return toRet.str ();
}

void FieldNamesTracker::JoinKeys (FieldNamesTracker* probeNames,
  Key& lookupKey, Key& indexKey, Key& remainingBaseKey)
{
  unsigned myFieldNo = 1; // start at one to skip the table name
  for (vector<string>::iterator i = fieldNames.begin (); i
      != fieldNames.end (); i++, myFieldNo++)
    {
      string myNextArgument = *i;
      int probePosition = probeNames->FieldPosition (myNextArgument);

      // Does my argument match any probe arguments?
      if (probePosition == -1)
        {
          // My argument doesn't match. It's a "remaining" base key
          remainingBaseKey.push_back (myFieldNo);
        }
      else
        {
          // My argument myNextArgument at field number myFieldNo matches
          // the probe's argument at field number probePosition. The lookup
          // key will project probePosition on the probe tuple onto
          // myFieldNo.
          lookupKey.push_back (probePosition + 1); // add 1 for the table name
          indexKey.push_back (myFieldNo);
        }
    }
}

string LocalizeContext::ToString ()
{
  ostringstream b;
  for (unsigned k = 0; k < _localizedRules.size (); k++)
    {
      b << _localizedRules.at (k)->ToString () << "\n";
    }
  return b.str ();
}

void LocalizeContext::AddRule (OlContext::Rule* rule)
{
  for (unsigned k = 0; k < _localizedRules.size (); k++)
    {
      if (_localizedRules.at (k)->ToString () == rule->ToString ())
        {
          return;
        }
    }

  NS_LOG_INFO ("Add localized rule: " << rule->ToString ());
  _localizedRules.push_back (rule);
}

OlContext::Rule*
LocalizeContext::AddSendRule (OlContext::Rule* nextRule, list<
    ParseTerm*> newTerms, string newFunctorName, ParseFunctor* functor,
  string loc, long minLifetime,
  vector<string> fieldNames, Ptr<TableStore> tableStore)
{
  ParseExprList* pe = new ParseExprList ();
  for (size_t k = 0; k < fieldNames.size (); k++)
    {

      string varName = fieldNames.at (k);
      ParseVar* pv = new ParseVar (ValStr::New (varName));
      if (fieldNameEq (loc, varName))
        {
          // The new variable should be a locspec
          pv->SetLocSpec ();
        }
      pe->push_back (pv);
    }

  ParseFunctor* newHead = new ParseFunctor (new ParseFunctorName (
    new ParseVar (ValStr::New (newFunctorName))), pe);
  OlContext::Rule* newRule = new OlContext::Rule (
    nextRule->ruleID + "Local1", newHead, false);

  newRule->terms = newTerms;
  NS_LOG_INFO ("Localized send rule " << newRule->ToString ());

  // Materialize what we send if the source has been materialized
  OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
    functor->fName->name);

  long zeroLifetime = 0;

  if (tableInfo != NULL && minLifetime != zeroLifetime)
    {
      OlContext::TableInfo* newTableInfo = new OlContext::TableInfo ();
      newTableInfo->tableName = newFunctorName;
      newTableInfo->size = tableInfo->size; // XXX depends on size of first table
      newTableInfo->timeout = minLifetime;
      newTableInfo->primaryKeys = tableInfo->primaryKeys; // XXX depends on first table
      newTableInfo->primaryKeyTypes = tableInfo->primaryKeyTypes; // XXX depends on first table
      tableStore->AddTableInfo (newTableInfo);
      NS_LOG_INFO ("Old table " << tableInfo->ToString ());
      NS_LOG_INFO (" Create table for " << newTableInfo->ToString ());
    }
  return newRule;
}

void LocalizeContext::Rewrite (Ptr<OlContext> ctxt, Ptr<TableStore> tableStore)
{
  m_isSendlog = ctxt->IsSendlog ();

  for (unsigned k = 0; k < ctxt->GetRules ()->size (); k++)
    {
      OlContext::Rule* nextRule = ctxt->GetRules ()->at (k);
      RewriteRule (nextRule, tableStore);
    }
}

void LocalizeContext::RewriteRule (OlContext::Rule* nextRule,
  Ptr<TableStore> tableStore)
{
  NS_LOG_DEBUG ("Perform localization Rewrite on " << nextRule->ToString ());

  vector<OlContext::Rule*> toRet;
  vector<ParseFunctor*> probeTerms;
  vector<ParseTerm*> otherTerms;
  bool hasEvent = false; // Does this rule have an event?
  set<string, less<string> > probeLocales; // The locspecs of
  // materialized
  // tables in the
  // rule body?

  // separate out the probeTerms and other terms
  for (list<ParseTerm*>::iterator t = nextRule->terms.begin (); t
      != nextRule->terms.end (); t++)
    {
      ParseTerm* nextTerm = (*t);
      ParseFunctor *functor = dynamic_cast<ParseFunctor*> (nextTerm);
      if (functor != NULL)
        {
          // Is this materialized?
          OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
            functor->fName->name);
          if (tableInfo != NULL)
            {
              // This is materialized
              probeTerms.push_back (functor);

              // It's locspec is
              string locspec = functor->GetLocSpec ();
              probeLocales.insert (locspec);
            }
          else
            {
              // put events first
              //              PLANNER_WORDY_NOPC("Put to front " << functor->fn->name);
              probeTerms.insert (probeTerms.begin (), functor);
              hasEvent = true;
            }
        }
      else
        {
          otherTerms.push_back (nextTerm);
        }
    }

  // go through all the probe terms,
  // do a left to right join ordering transformation
  bool local = true;
  ostringstream headName;
  headName << nextRule->ruleID;

  long minLifetime = -1L;
  long zeroLifetime = 0L;

  uint boundary = 0;

  list<ParseTerm*> beforeBoundaryTerms;
  FieldNamesTracker* namesTracker = NULL;
  for (size_t k = 0; k < probeTerms.size () - 1; k++)
    {
      if (k == 0)
        {
          namesTracker = new FieldNamesTracker (probeTerms.at (k));
        }
      else
        {
          FieldNamesTracker* otherNamesTracker = new FieldNamesTracker (
            probeTerms.at (k));
          namesTracker->MergeWith (otherNamesTracker->fieldNames);
        }
      headName << probeTerms.at (k)->fName->name;
      OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
        probeTerms.at (k)->fName->name);
      NS_LOG_DEBUG ("Get table " << probeTerms.at (k)->fName->name);
      if (tableInfo != NULL)
        {
          if (minLifetime > tableInfo->timeout)
            {
              minLifetime = tableInfo->timeout;
            }
        }
      else
        {
          minLifetime = zeroLifetime;
        }
      beforeBoundaryTerms.push_back (probeTerms.at (k));
      if (!fieldNameEq (probeTerms.at (k)->GetLocSpec (),
        probeTerms.at (k + 1)->GetLocSpec ()))
        {
          // getlocspec is guaranteed to start with @ sign
          headName << probeTerms.at (k + 1)->GetLocSpec ().substr (1);
          local = false;
          boundary = k;
          break;
        }
    }

  if (local == true)
    {
      NS_LOG_INFO (nextRule->ToString () << " is already localized");
      AddRule (nextRule);
      delete namesTracker;
      return;
    }
  else if (hasEvent && // it's not a view
      probeLocales.size () > 1 && // the materialized tables are
      // on multiple locations
      nextRule->head->Aggregate () != -1)
    { // and it's an aggregate
      // This rule is an non-local event aggregate. We don't handle these currently
      NS_LOG_INFO ("Rule '" << nextRule->ToString ()
          << "' is a non-local streaming aggregate, which "
          << "is not currently supported.\n");
      cout << "Boom! Something went wrong!\n";
      exit (-1);
    }

  NS_LOG_DEBUG (headName.str () << " minimumLifetime "
      << minLifetime << " boundaryIndex "
      << boundary << " " << namesTracker->ToString ());

  // add a new rule that takes all terms up to boundary, and send them to dst
  OlContext::Rule* newRule = AddSendRule (nextRule, beforeBoundaryTerms,
    headName.str (), probeTerms.at (0),
    probeTerms.at (boundary + 1)->GetLocSpec (), minLifetime,
    namesTracker->fieldNames, tableStore);
  AddRule (newRule);

  // recursively call localization on new rule that has
  list<ParseTerm*> newTerms;
  newTerms.push_back (newRule->head);
  for (unsigned k = boundary + 1; k < probeTerms.size (); k++)
    {
      newTerms.push_back (probeTerms.at (k));
    }
  for (unsigned k = 0; k < otherTerms.size (); k++)
    {
      newTerms.push_back (otherTerms.at (k));
    }

  OlContext::Rule* newRuleTwo = new OlContext::Rule (nextRule->ruleID
      + "Local2", nextRule->head, false);

  newRuleTwo->terms = newTerms;
  RewriteRule (newRuleTwo, tableStore);
}

