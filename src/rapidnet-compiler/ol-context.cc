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

#include "ol-context.h"
#include "ol-lexer.h"
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>

using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("OlContext");

/** No expiration is represented as positive infinity. */
long OlContext::NO_EXPIRATION = -1L;

/** Default table expiration time is no expiration */
long OlContext::DEFAULT_EXPIRATION = OlContext::NO_EXPIRATION;

/** No size is represented as 0 size. */
uint32_t OlContext::NO_SIZE = 0;

/** Default table max size is no max size */
uint32_t OlContext::DEFAULT_SIZE (OlContext::NO_SIZE);

int OlContext::ruleCount = 0;

const string OlContext::PROVENANCE_RULE_PREFIX = "prov_";

/**********************************************************************
 *
 * Rule methods
 *
 *********************************************************************/

//
// Print out the rule for debugging purposes
//
string
OlContext::Rule::ToString ()
{
  ostringstream r;
  r << ruleID << " ";
  r << head->ToString () << " :- ";
  list<ParseTerm*>::iterator t = terms.begin ();

  r << (*t)->ToString ();
  t++;
  for (; t != terms.end (); t++)
    {
      r << ", " << (*t)->ToString ();
    }
  r << ".";
  return r.str ();
}

string
OlContext::TableInfo::ToString ()
{
  ostringstream t;
  t << "materialize(" << tableName << ", " << timeout << ", " << size
      << ", keys(";

  for (size_t k = 0; k + 1 < primaryKeys.size (); k++)
    {
      t << primaryKeys.at (k) << ", ";
    }
  if (primaryKeys.size () > 0)
    {
      t << primaryKeys.at (primaryKeys.size () - 1);
    }
  t << ")";

  return t.str ();
}

string
OlContext::Context::ToString ()
{
  return mId->ToString ();
}

void
OlContext::SetContext (ParseExpr *ctx)
{
  mContext.mId = ctx;
  dynamic_cast<ParseVar*> (mContext.mId) ->SetLocSpec ();

  //seNDlog current doesn't have provenance support
  //compilation will halt if the provenance flag is turned on
  if (isProvenanceEnabled)
    {
      NS_LOG_ERROR ("Error: Currently provenance is not available"
          " for SeNDlog applications.");
      exit (-1);
    }

}

/**********************************************************************
 *
 * Context methods
 *
 *********************************************************************/

//
// Adding a new rule to the environment.  Subtle: we need to resolve
// all variable references at this point and convert them to
// positional arguments.
//
OlContext::Rule*
OlContext::CreateRule (ParseTerm *lhs, ParseTermList *rhs,
  bool deleteFlag, ParseExpr *n, ParseExpr *wt, bool deleteEventFlag)
{
  //  TRACE_FUNCTION;

  //SeNDlog: Rewrite flag is set when context is set.
  bool sendlog_rewrite = IsSendlog ();

  ParseFunctor *h = dynamic_cast<ParseFunctor*> (lhs);
  string ruleName = (n) ? n->value->ToString () : "";
  double ruleWeight = (wt) ? atof ((wt->value->ToString ()).c_str()) : 1.0;
  int fict_varnum = 1; // Counter for inventing anonymous variables.


  // Get hold of the functor (rule head) for this new rule.  This
  // holds a list of all the rule bodies that match the head.  Note
  // that this match is only performed on (name,arity), since we
  // convert all bound values to extra "eq" terms in the body down
  // below.

  // Create a new rule and register it.
  Rule *r = new Rule (ruleName, ruleWeight, h, deleteFlag, deleteEventFlag);

  // Next, we canonicalize all the args in the rule head.  We build up
  // a list of argument names - the first 'arity' of these will be the
  // free variables in the rule head.  Literals and duplicate free
  // variables here are eliminated, by a process of appending extra
  // "eq" terms to the body, and inventing new free variables.

  //SeNDlog: Outgoing flag is set when a header tuple is going out, in this case
  //the destination need to be taken into the functor and set as location
  //specifier.
  bool outgoing = false;
  if (sendlog_rewrite)
    {
      outgoing = true;
    }

  //SeNDlog: The sayer of an authenticated tuple needs to be considered when doing join.
  //So take sayer into the tuple if it is not already there.
  //To check whether the sayer is already there, assure not first.
  bool has_sayer = true;
  if (h->m_auth)
    {
      has_sayer = false;
    }

  //SeNDlog: Syntax check. There should be no @ in the rules.
  if (sendlog_rewrite && (!h->m_loc.empty ()))
    {
      if (sendlog_rewrite)
        {
          ReportError ("Location specifier not allowed in SeNDlog rules.");
        }
    }

  for (int i = 0; i < h->Args (); i++)
    {
      ParseAgg *agg = NULL;
      ParseVar *var = NULL;
      ParseVal *val = NULL;

      if ( (agg = dynamic_cast<ParseAgg*> (h->Arg (i))) != NULL)
        {
          agg->Position (i);
        }
      else if ( (var = dynamic_cast<ParseVar*> (h->Arg (i))) != NULL)
        {
          // The argument is a free variable - the usual case.

          //SeNDlog: Do SeNDlog re-write when the flag is set.
          if (sendlog_rewrite)
            {
              //Check if this variable is the sayer when this is an authenticated tuple.
              if (h->m_auth)
                {
                  if (var->ToString () == h->m_sayer->ToString ())
                    {
                      has_sayer = true;
                    }
                }

              //No out going destination, this is a local functor.
              //The location specifier for this functor should be the context.
              if (!h->m_dest)
                {
                  outgoing = false;
                  //The variable which equals the context should be set location specifier.
                  if (var->ToString ()
                      == dynamic_cast<ParseVar*> (mContext.mId)->ToString ())
                    {
                      //If no location specifier yet, set this variable.
                      if (h->m_loc.empty ())
                        {
                          var->SetLocSpec ();
                        }
                    }
                }
              //Has out going destination, this is an outgoing functor.
              //The location specifier for this functor should be the destination.
              else
                {
                  //The variable which equals the destination should be set location specifier.
                  if (var->ToString () == h->m_dest->ToString ())
                    {
                      var->SetLocSpec ();
                      outgoing = false; //Turn off the flag since this functor is located.
                    }
                }
            }

          int loc = h->Find (var->ToString ());
          if (loc < i)
            {
              ostringstream oss;
              oss << "$" << fict_varnum++;
              // We've found a duplicate variable in the head. Add a new
              // "eq" term to the front of the term list.
              ParseVar *tmp = new ParseVar (oss.str ());
              tmp->Position (i);
              h->replace (i, tmp);
              r->terms.push_back (new ParseAssign (tmp, h->Arg (loc)));
            }
          else
            {
              var->Position (i);
            }
        }
      else if ( (val = dynamic_cast<ParseVal*> (h->Arg (i))) != NULL)
        {
          ostringstream oss;
          oss << "$" << fict_varnum++;
          ParseVar *tmp = new ParseVar (oss.str ());
          tmp->Position (i);
          h->replace (i, tmp);
          r->terms.push_back (new ParseAssign (tmp, val));
        }
      else
        {
          ReportError ("Parse rule unknown functor body type.");
        }

    }

  //SeNDlog: Previously the location specifier is only set on in the variable.
  //Now the functor will locate its locations specifier variable.
  if (sendlog_rewrite)
    {
      //For out going functors whose destination dosen't appear in the
      //the variables, take in the destination explicitly and set is as
      //location specifier.
      if (outgoing)
        {
          ParseVar *d = new ParseVar (h->m_dest);
          d->SetLocSpec ();
          h->m_args->push_back (d);
        }
      //Other cases, the location specifier should be set already.
      //Just let the functor to relocate it.
      else
        {
          (void) h->GetLocSpec ();
        }

      //Add the sayer into the tuple if it isn't already there.
      //And always do this last to make sure it will be at the same position
      //as in body functor rewrite.
      if (!has_sayer)
        {
          h->arg (h->m_sayer);
        }

    }

  // Now we've taken care of the head (and possibly created a few
  // terms), we run through the rest of the terms converting all the
  // variables we encounter to indices.
  int tpos = 1;
  for (ParseTermList::iterator iter = rhs->begin (); iter != rhs->end (); (*iter)->Position (
    tpos++), iter++)
    {
      ParseFunctor *f = NULL;
      ParseAssign *a = NULL;
      ParseSelect *s = NULL;

      if ( (f = dynamic_cast<ParseFunctor*> (*iter)) != NULL)
        {
          //SeNDlog: not_located flag is on for rule body tuples before the re-write
          //assign it the location specifier through parse the variables in the tuple.
          //In this case, the context needs to be added into the tuple explicitly as
          //the location specifier.
          bool not_located = false;
          if (sendlog_rewrite)
            {
              not_located = true;
            }

          //SeNDlog: The sayer of an authenticated tuple needs to be considered when doing join.
          //So take sayer into the tuple if it is not already there.
          //To check whether the sayer is already there, assure not first.
          bool has_sayer = true;
          if (f->m_auth)
            {
              has_sayer = false;
            }

          //SeNDlog: Syntax check. There should be no @ in the rules.
          if (sendlog_rewrite && (!f->m_loc.empty ()))
            {
              if (sendlog_rewrite)
                ReportError (
                  "[SeNDlog] There should be no location specifier in the rules.");
            }

          for (int i = 0; i < f->Args (); i++)
            {
              ParseVar *var = NULL;
              ParseVal *val = NULL;

              if ( (var = dynamic_cast<ParseVar*> (f->Arg (i))) != NULL)
                {
                  //SeNDlog: Do SeNDlog re-write when the flag is set.
                  if (sendlog_rewrite)
                    {
                      //check if this variable is the sayer when this is an authenticated tuple.
                      if (f->m_sayer != NULL)
                        {
                          if (var->ToString () == f->m_sayer->ToString ())
                            {
                              has_sayer = true;
                            }
                        }

                      //All functors in the rule body should be local.
                      //The variable which equals the context should be set location specifier.
                      if (var->ToString ()
                          == dynamic_cast<ParseVar*> (mContext.mId)->ToString ())
                        {
                          if (f->m_loc.empty ())
                            {
                              var->SetLocSpec ();
                            }
                          not_located = false; //located, turn of this flag.
                        }
                    }

                  var->Position (i);
                }
              else if ( (val = dynamic_cast<ParseVal*> (f->Arg (i))) != NULL)
                {
                  val->Position (i);
                }
              else
                {
                  ReportError ("Parse functor term unknown argument type.");
                }
            }

          //SeNDlog: Previously the location specifier is only set on in the variable.
          //Now the functor will locate its locations specifier variable.
          if (sendlog_rewrite)
            {
              //For body functors whose context dosen't appear in the
              //the variables, take in the context explicitly and set is as
              //location specifier.
              if (not_located)
                {
                  f->arg (mContext.mId);
                }

              (void) f->GetLocSpec (); //relocate

              //Add the sayer into the tuple if it isn't already there.
              //And always do this last to make sure it will be at the same position
              //as in body functor rewrite.
              if (!has_sayer)
                {
                  f->arg (f->m_sayer);
                }

            }

        }
      else if ( (a = dynamic_cast<ParseAssign*> (*iter)) != NULL)
        {

        }
      else if ( (s = dynamic_cast<ParseSelect*> (*iter)) != NULL)
        {

        }
      else
        {
          ReportError ("Internal parse error: unknown term type!");
        }
      r->terms.push_back (*iter);
    }
  return r;
}

void
OlContext::AddRule (ParseRule* rule)
{

  Rule* r = (OlContext::Rule*) rule->GetRule ();
  /*if provenance support is enabled, rewrite before add*/
  if (isProvenanceEnabled)
    {
      ProvenanceRewrite (r);
    }
  else
    {
      mRules->push_back (r);
    }
}

OlContext::Rule*
OlContext::CreateAggRule (ParseTerm *lhs, ParseAggTerm *rhs,
  bool deleteFlag, ParseExpr *n, ParseExpr *wt)
{
  ParseFunctor *h = dynamic_cast<ParseFunctor*> (lhs);
  string ruleName = (n) ? n->value->ToString () : "";
  double ruleWeight = (wt) ? atof ((wt->value->ToString ()).c_str()) : 1.0;
  Rule *r = new Rule (ruleName, ruleWeight, h, deleteFlag);
  r->terms.push_back (rhs);
  return r;
}

//
// Print out the whole parse result, if we can
//
string
OlContext::ToString ()
{
  ostringstream r;

  // Errors first
  for (ErrorList::iterator e = errors.begin (); e != errors.end (); e++)
    {
      r << "error(" << (*e)->line_num << ",'" << (*e)->msg << "').\n";
    }
  for (TableInfoMap::iterator i = mTables->begin (); i != mTables->end (); i++)
    {
      r << i->second->ToString () << "\n";
    }

  for (RuleList::iterator rule = mRules->begin (); rule != mRules->end (); rule++)
    {
      r << (*rule)->ToString () << "\n";
    }
  return r.str ();
}

OlContext::OlContext () :
  lexer (NULL)
{
  mRules = new RuleList ();
  mTables = new TableInfoMap ();
  mContext.mId = NULL;
}

OlContext::~OlContext ()
{
  delete mRules;
  delete mTables;
}

void
OlContext::ParseString (const char *prog)
{
  assert(lexer==NULL);
  lexer = new OlLexer (prog);
  ol_parser_parse (this);
  delete lexer;
  lexer = NULL;
}

void
OlContext::ParseStream (istream *str, bool provenanceEnabled)
{
  isProvenanceEnabled = provenanceEnabled;

  assert (lexer == NULL);
  lexer = new OlLexer (str);

  std::cout << "Lexer Completed" << std::endl;
  ol_parser_parse (this);

  std::cout << "Parser Completed" << std::endl;
  
  if (isProvenanceEnabled)
    {
      AddEdbProvenanceRule ();
    }

  delete lexer;
  lexer = NULL;
}

void
OlContext::ReportError (string msg)
{
  NS_LOG_ERROR ("PARSER ERROR (line " << lexer->line_num () << "): " << msg);
  errors.push_back (new OlContext::Error (lexer->line_num (), msg));
}

void
OlContext::Watch (ParseExpr *w, string modifiers)
{
  NS_LOG_INFO ("Added watched functor " << w->ToString ()
      << " with modifiers [" << modifiers << "]");
  watchTables.insert (make_pair (w->value->ToString (), modifiers));
}

void
OlContext::Stage (ParseExpr *stageName, ParseExpr *inTupleName,
  ParseExpr *outTupleName)
{
  NS_LOG_INFO ("Add Stage Info " << stageName->ToString ()
      << " taking inTupleName= " << inTupleName->value->ToString ()
      << " outTupleName= " << outTupleName->value->ToString ());
  mStages[inTupleName->ToString ()] = new ExtStageSpec (stageName->ToString (),
    inTupleName->value->ToString (), outTupleName->value->ToString ());
}

void
OlContext::TraceTuple (ParseExpr *w)
{
  NS_LOG_INFO ("Add trace variable " << w->ToString ());
  tuplesToTrace.insert (w->value->ToString ());
}

void
OlContext::TraceTable (ParseExpr *w)
{
  NS_LOG_INFO ("Add traced table " << w->ToString ());
  tablesToTrace.insert (w->value->ToString ());
}

void
OlContext::table (ParseExpr *name, ParseExpr *ttl, ParseExpr *size,
  ParseExprList *keys)
{
  TableInfo *tableInfo = new TableInfo ();
  tableInfo->tableName = name->ToString ();

  int myTtl =
      (dynamic_cast<ValInt32*> (PeekPointer (ttl->value)))->GetInt32Value ();
  if (myTtl == -1)
    {
      tableInfo->timeout = OlContext::NO_EXPIRATION;
    }
  else if (myTtl == 0)
    {
      ReportError ("bad timeout for materialized table");
    }
  else
    {
      tableInfo->timeout = myTtl;
    }

  int mySize =
      (dynamic_cast<ValInt32*> (PeekPointer (size->value)))->GetInt32Value ();
  // Hack because infinity token has a -1 value
  if (mySize == -1)
    {
      tableInfo->size = OlContext::NO_SIZE;
    }
  else
    {
      tableInfo->size = mySize;
    }

  if (keys)
    {
      for (ParseExprList::iterator i = keys->begin (); i != keys->end (); i++)
        {
          ParseKey* key = dynamic_cast<ParseKey*> (*i);
          ValInt32* val = dynamic_cast<ValInt32*> (PeekPointer (
            key->getAttribute ()->value));
          tableInfo->primaryKeys.push_back (val->GetInt32Value ());
          tableInfo->primaryKeyTypes.push_back (key->GetType ());
        }
    }
  else
    {
      // If keys was NULL, then I'm going to leave primaryKeys
      // alone, which means it is going to be empty
      // which means it's going to be KEYID
    }

  mTables->insert (make_pair (tableInfo->tableName, tableInfo));

  NS_LOG_DEBUG ("Materialize " << tableInfo->tableName << "/" << ", timeout "
      << tableInfo->timeout << ", size " << tableInfo->size);
}

//
// Adding a fact [not used]
//
void
OlContext::Fact (ParseTerm *term)
{
}

void
OlContext::Query (ParseTerm *term)
{
  singleQuery = dynamic_cast<ParseFunctor*> (term);
  NS_LOG_INFO ("Add query " << singleQuery->ToString ());
}

bool
OlContext::GotErrors ()
{
  return !errors.empty ();
}

void
OlContext::DumpErrors ()
{
  for (ErrorList::iterator e = errors.begin (); e != errors.end (); e++)
    {
      NS_LOG_INFO ("error at line " << (*e)->line_num << ": '" << (*e)->msg
          << "'.");
    }
}

void
OlContext::CombineProvenanceQueryRules (Ptr<OlContext> othertxt)
{
  RuleList * otherRules = othertxt->GetRules ();
  for (RuleList::iterator rule = otherRules->begin (); rule
      != otherRules->end (); rule++)
    {
      mRules ->push_back (*rule);
    }

  TableInfoMap* otherTables = othertxt->GetTableInfos ();

  for (TableInfoMap::iterator entry = otherTables-> begin (); entry
      != otherTables->end (); entry++)
    {
      TableInfo* ti = entry->second;
      mTables->insert (make_pair (ti->tableName, ti));
    }

}

void
OlContext::ProvenanceRewrite (Rule * rule)
{
  this->ruleCount--;

  // Generate 1st rule
  ParseFunctor* eHTemp = GenerateLocalEHTempEventRule (rule);

  // Rewrite rules 2-5 only work on different rule headers
  if (rewrittenIdbs.insert (rule->head->fName->name).second)
    {
      // Generate 2nd rule
      GenerateLocalRuleExecEntryRule (rule, eHTemp);

      // Generate 3rd rule
      ParseFunctor* eH = GenerateSendEventMsgEHRule (rule, eHTemp);

      // Generate 4th rule
      GenerateCreateResultTupleHRule (rule, eH);

      // Generate 5th rule
      GenerateCreateRemoveProvEntryRule (rule, eH);
    }
}

ParseFunctor*
OlContext::GenerateLocalEHTempEventRule (Rule* rule)
{
  //  1st rule: eHTemp(@RLoc,H1,..., Ho,RID,RWeight,R,List) :-
  //            t1(@X,P11,...,Po11), ..., tn(@X,P1n,...Pnon),
  //            c1, ..., cp, RLoc = X, RWeight = weight, R = rid,
  //            PID1 = f_sha1(t1 + X, + P11 + ... + P1o1),
  //            ...,
  //            PIDn = f_sha1(tn + X, + Pn1 + ... + Pnon),
  //            List = f_append(PID1, ..., PIDn),
  //            RID = f_sha1(R+RLoc+List).

  //new rule id
  ParseExpr * rID = new ParseVal (ValStr::New (PROVENANCE_RULE_PREFIX + (rule->ruleID)
      + "_1"));
	  
  //new rule weight: weight is not required for prov rules. 
  //ParseExpr * rWeight = new ParseVal (ValDouble::New (rule->ruleWeight));

  //new rule header
  ParseExpr * hN = new ParseVal (ValStr::New ("e" + rule->head->fName->name
      + "Temp"));
  ParseFunctorName * hName = new ParseFunctorName (hN);

  ParseExprList * hArgs1 = new ParseExprList ();
  //original rule head primitives
  ParseExprList * orig_head_args = rule->head->m_args;
  //put all original rule head primitives into new rule 1 head
  for (ParseExprList::iterator e = orig_head_args->begin (); e
      != orig_head_args->end (); e++)
    {
      ParseAgg* a = NULL;
      ParseVar* v = NULL;
      if ( (a = dynamic_cast<ParseAgg*> (*e)) != NULL)
        {
          //aggregate primitive
          ParseExpr * aggVar = new ParseVar (a->value);
          hArgs1->push_back (aggVar);
          rule->isAggregate = true;
        }
      else if ( (v = dynamic_cast<ParseVar*> (*e)) != NULL && v->LocSpec ())
        {
          //add a non_locSpec version
          ParseExpr * non_loc = new ParseVar (v->ToString ());
          hArgs1->push_back (non_loc);
        }
      else
        {
          hArgs1->push_back (*e);
        }
    }

  ParseExprList * dummyArgs = new ParseExprList ();
  ParseFunctor * dummyHead = NULL;

  //if aggregate rule, create a rule header without aggregating
  if (rule->isAggregate)
    {
      for (ParseExprList::iterator e = hArgs1->begin (); e != hArgs1->end (); e++)
        {
          //make a copy of the header without aggregate
          ParseVar* v = NULL;
          ParseVal* l = NULL;

          if (dynamic_cast<ParseVar*> (*e) != NULL)
            {
              v = new ParseVar (dynamic_cast<ParseVar*> (*e)->value);
              dummyArgs->push_back (v);
            }
          else if (dynamic_cast<ParseVal*> (*e) != NULL)
            {
              l = new ParseVal (dynamic_cast<ParseVal*> (*e)->value);
              dummyArgs->push_back (l);
            }
        }
      for (ParseExprList::iterator e = dummyArgs->begin (); e
          != dummyArgs->end (); e++)
        {
          if ( (*e)->ToString () == rule->head->GetLocSpecValue ())
            {
              dynamic_cast<ParseVar*> (*e) ->SetLocSpec ();
              break;
            }
        }
      dummyHead = new ParseFunctor (rule->head->fName, dummyArgs);
    }

  ParseExpr *RLOC_loc = new ParseVar ("RLOC");
  dynamic_cast<ParseVar*> (RLOC_loc)->SetLocSpec ();
  hArgs1->push_front (RLOC_loc);
  ParseExpr *RID = new ParseVar ("RID");
  hArgs1->push_back (RID);
  ParseExpr *RWeight = new ParseVar ("RWeight");
  hArgs1->push_back (RWeight);
  ParseExpr *R = new ParseVar ("R");
  hArgs1->push_back (R);
  ParseExpr *List = new ParseVar ("List");
  hArgs1->push_back (List);
  ParseFunctor* h1 = new ParseFunctor (hName, hArgs1);

  //new rule body
  ParseTermList * rBody1 = new ParseTermList ();
  ParseExpr * X = NULL; //for original rule body location
  int n = 0; //PID counter

  for (list<ParseTerm*>::iterator t = rule->terms.begin (); t
      != rule->terms.end (); t++)
    {
      //put all original terms into new rule body
      if (!rule->isAggregate)
        {
          rBody1->push_back ( (*t));
        }
      else
        {
          //if agg rule, triger this rewritten rule when insert the agg result
          if (n == 0)
            {
              rBody1->push_back (dummyHead);
            }
        }

      if (dynamic_cast<ParseFunctor*> (*t) != NULL)
        {
          potentialEdbs.insert (make_pair (
            dynamic_cast<ParseFunctor*> (*t)->fName->name,
            dynamic_cast<ParseFunctor*> (*t)));

          if (dynamic_cast<ParseFunctor*> (*t)->fName->name == "periodic")
            {
              //If the original rule is periodic, set every rewritten rule "periodic".
              //In this case, they don't have delete delta rules in eca context
              rule ->isPeriodic = true;
            }

          n++;
          if (X == NULL)
            {
              //original rule body location
              X = new ParseVar (
                dynamic_cast<ParseFunctor*> (*t)->GetLocSpecValue ());
            }

          //generate PID
          ostringstream tmp;
          tmp << "PID" << n;
          ParseExpr * PID_n = new ParseVar (tmp.str ());

          ParseExprList * PIDagList = new ParseExprList ();
          ValuePtr fNameVal = ValStr::New (
            (dynamic_cast<ParseFunctor*> (*t))->fName->name);
          ParseExpr * fNameExpr = new ParseVal (fNameVal);

          ParseExprList::iterator a =
              (dynamic_cast<ParseFunctor*> (*t)->m_args)->begin ();
          ParseExpr* PIDag = new ParseMath (ParseMath::PLUS, fNameExpr, (*a));
          a++;
          for (; a != (dynamic_cast<ParseFunctor*> (*t)->m_args)->end (); a++)
            {
              PIDag = new ParseMath (ParseMath::PLUS, PIDag, (*a));
            }
          PIDagList ->push_front (PIDag);

          ParseExpr * PID_func = new ParseFunction (new ParseVar ("f_sha1"),
            PIDagList);
          ParseTerm * PID_AS = new ParseAssign (PID_n, PID_func);

          rBody1->push_back (PID_AS);

          //construct list
          if (n == 1)
            {
              ParseExprList * appAgList = new ParseExprList ();
              appAgList->push_back (PID_n);
              ParseExpr * fappd = new ParseVar ("f_append");
              ParseExpr * List_func = new ParseFunction (fappd, appAgList);
              ParseTerm * List_AS = new ParseAssign (List, List_func);
              rBody1->push_back (List_AS);
            }
          if (n >= 2)
            {
              ostringstream t;
              t << "List" << n;
              ParseExpr *List2 = new ParseVar (t.str ()); //tmp list, for concat
              ParseExprList * appAgList = new ParseExprList ();
              appAgList->push_back (PID_n);
              ParseExpr * fappd = new ParseVar ("f_append");
              ParseExpr * List_func1 = new ParseFunction (fappd, appAgList);
              ParseTerm * List_AS1 = new ParseAssign (List2, List_func1);
              rBody1->push_back (List_AS1);

              ParseExprList * ccAgList = new ParseExprList ();
              ccAgList->push_back (List);
              ccAgList->push_back (List2);
              ParseExpr * fconcat = new ParseVar ("f_concat");
              ParseExpr * List_func2 = new ParseFunction (fconcat, ccAgList);
              ParseTerm * List_AS2 = new ParseAssign (List, List_func2);
              rBody1->push_back (List_AS2);
            }
        }
    }

  ParseTerm * AS1 = new ParseAssign (RLOC_loc, X);
  rBody1->push_back (AS1);

  ValuePtr rWeightVal = ValDouble::New (rule->ruleWeight);
  ParseExpr * orig_rule_weight = new ParseVal (rWeightVal);
  ParseTerm * AS2 = new ParseAssign (RWeight, orig_rule_weight);
  rBody1->push_back (AS2);
  
  ValuePtr rIDVal = ValStr::New (rule->ruleID);
  ParseExpr * orig_rule_ID = new ParseVal (rIDVal);
  ParseTerm * AS3 = new ParseAssign (R, orig_rule_ID);
  rBody1->push_back (AS3);  

  ParseExprList * RIDagList = new ParseExprList ();
  ParseExpr* RIDag = new ParseMath (ParseMath::PLUS, R, RLOC_loc);
  RIDag = new ParseMath (ParseMath::PLUS, RIDag, List);
  RIDagList->push_front (RIDag);
  ParseExpr * RID_func = new ParseFunction (new ParseVar ("f_sha1"), RIDagList);
  ParseTerm * RID_AS = new ParseAssign (RID, RID_func);
  rBody1->push_back (RID_AS);

  Rule * r1 = CreateRule (h1, rBody1, false, rID, NULL, false); // set NULL to rWeight if need weight for prov rules
  r1->isProvenanceRule = true;
  r1->isPeriodic = rule->isPeriodic;
  mRules->push_back (r1);

  return h1;
}

void OlContext::GenerateLocalRuleExecEntryRule (Rule* rule,
  ParseFunctor* eHTemp)
{
  // 2nd rule: ruleExec(@RLoc,RID,RWeight,R,List) :- eHTemp(@RLoc,H1,...,Ho,RID,RWeight,R,List).

  //new rule id
  ParseExpr * rID = new ParseVal (ValStr::New (PROVENANCE_RULE_PREFIX + (rule->ruleID)
      + "_2"));
	  
  //new rule weight
  //ParseExpr * rWeight = new ParseVal (ValDouble::New (rule->ruleWeight));

  //new rule header
  ParseExpr *hN = new ParseVal (ValStr::New ("ruleExec")); //header name
  ParseFunctorName * hName = new ParseFunctorName (hN);

  //	ParseExpr *RLOC_loc= new ParseVar ("RLOC");
  //	dynamic_cast<ParseVar*> (RLOC_loc)->SetLocSpec();
  //	hArgs1->push_front(RLOC_loc);
  //	ParseExpr *RID = new ParseVar ("RID");
  //	hArgs1->push_back(RID);  
  //	ParseExpr *RWeight = new ParseVar ("RWeight"):
  //	hArgs1->push_back (RWeight)
  //	ParseExpr *R = new ParseVar ("R");
  //	hArgs1->push_back(R);
  //	ParseExpr *List = new ParseVar ("List");
  //	hArgs1->push_back(List);
  //	ParseFunctor* h1= new ParseFunctor (hName, hArgs1);

  ParseExprList * hArgs2 = new ParseExprList ();
  hArgs2 ->push_front (eHTemp->Arg (0));
  int n = eHTemp ->Args ();
  hArgs2->push_back (eHTemp->Arg (n - 4));
  hArgs2->push_back (eHTemp->Arg (n - 3));
  hArgs2->push_back (eHTemp->Arg (n - 2));
  hArgs2->push_back (eHTemp->Arg (n - 1));
  ParseFunctor* h2 = new ParseFunctor (hName, hArgs2);

  //new rule body:
  ParseTermList * rBody2 = new ParseTermList ();
  rBody2 ->push_front (eHTemp);

  Rule * r2 = CreateRule (h2, rBody2, false, rID, NULL, false);
  r2->isProvenanceRule = true;
  r2->isPeriodic = rule->isPeriodic;
  mRules->push_back (r2);

  //ruleExec table
  ParseExprList *rekeys = new ParseExprList ();
  ParseExpr * rek1 = new ParseKey (new ParseVal (ValInt32::New (1)));
  rekeys->push_back (rek1);
  ParseExpr * rek2 = new ParseKey (new ParseVal (ValInt32::New (2)),
    new ParseVal (ValStr::New ("cid")));
  rekeys->push_back (rek2);

  table (new ParseVal (ValStr::New ("ruleExec")), new ParseVal (ValInt32::New (
    -1)), new ParseVal (ValInt32::New (0)), rekeys);
}

ParseFunctor*
OlContext::GenerateSendEventMsgEHRule (Rule * rule, ParseFunctor* eHTemp)
{
  // 3rd rule: eH(@H1,...Ho,RID,RWeight,RLoc) :- eHTemp(@RLoc,H1,..., Ho,RID,RWeight,R,List).

  //new rule id
  ParseExpr * rID = new ParseVal (ValStr::New (PROVENANCE_RULE_PREFIX + (rule->ruleID)
      + "_3"));
	  
  //new rule weight
  //ParseExpr * rWeight = new ParseVal (ValDouble::New (rule->ruleWeight));

  //new rule header
  ParseExpr * hN = new ParseVal (ValStr::New ("e" + rule->head->fName->name)); //header name
  ParseFunctorName* hName = new ParseFunctorName (hN);
  ParseExprList * hArgs3 = new ParseExprList ();
  for (ParseExprList::iterator e = eHTemp->m_args->begin (); e
      != eHTemp->m_args->end (); e++)
    {
      ParseVar* v = dynamic_cast<ParseVar*> (*e);
      if (v != NULL
          && dynamic_cast<ParseFunctor*> (rule->head)->GetLocSpecValue ()
              == v->ToString ())
        {
          //add a loc_spec version
          ParseExpr * loc = new ParseVar (v->ToString ());
          dynamic_cast<ParseVar*> (loc)->SetLocSpec ();
          hArgs3->push_back (loc);
        }
      else
        {
          hArgs3->push_back (*e);
        }
    }
  hArgs3->pop_front ();
  hArgs3->pop_back ();
  hArgs3->pop_back ();
  ParseExpr * RLOC = new ParseVar ("RLOC");
  hArgs3->push_back (RLOC);

  ParseFunctor* h3 = new ParseFunctor (hName, hArgs3);

  //new rule body:
  ParseTermList * rBody3 = new ParseTermList ();
  rBody3 ->push_front (eHTemp);

  Rule* r3 = CreateRule (h3, rBody3, false, rID, NULL, false);
  r3->isProvenanceRule = true;
  r3->isPeriodic = rule->isPeriodic;
  mRules->push_back (r3);

  return h3;
}

void
OlContext::GenerateCreateResultTupleHRule (Rule* rule, ParseFunctor* eH)
{
  // 4th rule: h(@H1,...,Ho) :- eH(@H,...Ho,RID,RWeight,RLoc).
  //new rule body:
  if (rule->isAggregate)
    {
      this->ruleCount++; //increase the counter back
      mRules->push_back (rule); //keep the original rule
    }
  else
    {
      //new rule id
      ParseExpr * rID = new ParseVal (ValStr::New (PROVENANCE_RULE_PREFIX + (rule->ruleID)
          + "_4"));
		  
	  //new rule weight
  	  //ParseExpr * rWeight = new ParseVal (ValDouble::New (rule->ruleWeight));
	  
      //new rule header is the original rule header
      ParseTermList * rBody4 = new ParseTermList ();
      rBody4 ->push_front (eH);
      Rule * r4 = CreateRule (rule->head, rBody4, false, rID, NULL, false);
      r4->isProvenanceRule = true;
      r4->isPeriodic = rule->isPeriodic;
      mRules->push_back (r4);
    }
}

void
OlContext::GenerateCreateRemoveProvEntryRule (Rule* rule, ParseFunctor* eH)
{
  // 5th rule: prov(@H1,VID,RID,RLoc) :- eH(@H1,...,Ho,RID,RWeight,RLoc),
  //                                     VID = f_sha1(h + H1 + ... + Ho).
   
  // modified 5th rule: prov(@H1,VID,RID,RLoc,Score) :- eH(@H1,...,Ho,RID,RWeight,RLoc),
  //                                     VID = f_sha1(h + H1 + ... + Ho). 
  // Set Score to -1 since the derived tuples don't have score. Only base tuples have score

  //new rule id
  ParseExpr* rID = new ParseVal (ValStr::New (PROVENANCE_RULE_PREFIX + (rule->ruleID)
      + "_5"));
	  
  //new rule weight
  //ParseExpr * rWeight = new ParseVal (ValDouble::New (rule->ruleWeight));

  //new rule header
  ParseExpr* hN = new ParseVal (ValStr::New ("prov")); //header name
  ParseFunctorName* hName = new ParseFunctorName (hN);

  ParseExpr * H1 = new ParseVar (rule->head->GetLocSpecValue ());
  dynamic_cast<ParseVar*> (H1)->SetLocSpec ();
  ParseExpr * VID = new ParseVar ("VID");
  ParseExpr * Score = new ParseVar ("Score");

  ParseExprList * hArgs5 = new ParseExprList ();
  hArgs5->push_front (H1);
  hArgs5->push_back (VID);
  int n = eH->Args ();
  hArgs5->push_back (eH->Arg (n - 3));  // RID in eH
  hArgs5->push_back (eH->Arg (n - 1));  // RLoc in eH
  hArgs5->push_back (Score);
  ParseFunctor* h5 = new ParseFunctor (hName, hArgs5);

  //new rule body:
  ParseTermList * rBody5 = new ParseTermList ();
  rBody5 ->push_front (eH);

  ValuePtr hNameVal = ValStr::New (rule->head->fName->name);
  ParseExpr * hNameExpr = new ParseVal (hNameVal);
  ParseExprList::iterator b = rule->head->m_args->begin ();
  ParseExpr* VIDag = new ParseMath (ParseMath::PLUS, hNameExpr, (*b));
  b++;
  for (; b != rule->head->m_args->end (); b++)
    {
      ParseAgg * a = NULL;
      a = dynamic_cast<ParseAgg*> (*b);
      if (a == NULL)
        {
          VIDag = new ParseMath (ParseMath::PLUS, VIDag, (*b));
        }
      else
        {
          ParseExpr * aggVar = new ParseVar (a->value);
          VIDag = new ParseMath (ParseMath::PLUS, VIDag, aggVar);
        }
    }
  ParseExprList * VIDagList = new ParseExprList ();
  VIDagList->push_front (VIDag);
  ParseExpr * func3 = new ParseFunction (new ParseVar ("f_sha1"), VIDagList);
  ParseTerm * AS1 = new ParseAssign (VID, func3);
  rBody5->push_back (AS1);

  ValuePtr scoreVal = ValDouble::New (-1.0);
  ParseExpr * orig_score = new ParseVal (scoreVal);
  ParseTerm * AS2 = new ParseAssign (Score, orig_score);
  rBody5->push_back (AS2);

  Rule * r5 = CreateRule (h5, rBody5, false, rID, NULL, false);
  r5->isProvenanceRule = true;
  r5->isPeriodic = rule->isPeriodic;
  mRules->push_back (r5);

  //provenance table
  ParseExprList *pvkeys = new ParseExprList ();
  ParseExpr * pvk1 = new ParseKey (new ParseVal (ValInt32::New (1)));
  pvkeys->push_back (pvk1);
  ParseExpr * pvk2 = new ParseKey (new ParseVal (ValInt32::New (2)),
    new ParseVal (ValStr::New ("cid")));
  pvkeys->push_back (pvk2);
  ParseExpr * pvk3 = new ParseKey (new ParseVal (ValInt32::New (3)),
    new ParseVal (ValStr::New ("cid")));
  pvkeys->push_back (pvk3);

  table (new ParseVal (ValStr::New ("prov")),
    new ParseVal (ValInt32::New (-1)), new ParseVal (ValInt32::New (0)), pvkeys);
}

void
OlContext::AddEdbProvenanceRule ()
{
  uint32_t n = 0;
  for (map<string, ParseFunctor*>::iterator entry = potentialEdbs.begin (); entry
      != potentialEdbs.end (); entry++)
    {
      ParseFunctor* f = entry->second;
      if ( (mTables->count (f->fName->name) > 0) && rewrittenIdbs.insert (
        f->fName->name).second) //this table is edb
        {
          //new rule id
          ostringstream tmp;
          tmp << "prov_edb_" << (++n);
          ParseExpr* rID = new ParseVal (ValStr::New (tmp.str ()));
		  
		  //new rule weight
  		  //ParseExpr * rWeight = new ParseVal (ValDouble::New (1.0)); 

          //new rule header
          ParseExpr * hN = new ParseVal (ValStr::New ("prov")); //header name
          ParseFunctorName * hName = new ParseFunctorName (hN);

          ParseExpr * X = new ParseVar (f->GetLocSpecValue ());
          dynamic_cast<ParseVar*> (X)->SetLocSpec ();
          ParseExpr * VID = new ParseVar ("VID");
          ParseExpr * RID = new ParseVar ("RID");
          ParseExpr * RLoc = new ParseVar ("RLoc");
          ParseExpr * Score = new ParseVar ("Score");

          ParseExprList * hArgs = new ParseExprList ();
          hArgs->push_front (X);
          hArgs->push_back (VID);
          hArgs->push_back (RID);
          hArgs->push_back (RLoc);
          hArgs->push_back (Score);
          ParseFunctor* h = new ParseFunctor (hName, hArgs);

          //new rule body:
          ParseTermList * rBody = new ParseTermList ();
		  ParseExpr * RT = new ParseVar (f->GetLocSpecValue ());
		  ParseTerm * RLoc_AS = new ParseAssign (RLoc, RT);
		  rBody->push_front (RLoc_AS);
		  rBody->push_back (f);
		  
          ValuePtr hNameVal = ValStr::New (f->fName->name);
          ParseExpr * hNameExpr = new ParseVal (hNameVal);
          ParseExprList::iterator b = f->m_args->begin ();
          ParseExpr* VIDag = new ParseMath (ParseMath::PLUS, hNameExpr, (*b));
          b++;
          
          for (; b != f->m_args->end (); b++)
            {
              ParseAgg * a = NULL;
              a = dynamic_cast<ParseAgg*> (*b);
              if (a == NULL)
                {
                  VIDag = new ParseMath (ParseMath::PLUS, VIDag, (*b));
                }
              else
                {
                  ParseExpr * aggVar = new ParseVar (a->value);
                  VIDag = new ParseMath (ParseMath::PLUS, VIDag, aggVar);
                }
            }
          ParseExprList * VIDagList = new ParseExprList ();
          VIDagList->push_front (VIDag);
          ParseExpr * func = new ParseFunction (new ParseVar ("f_sha1"),
            VIDagList);
          ParseTerm * VID_AS = new ParseAssign (VID, func);
          rBody->push_back (VID_AS);
          ParseTerm * RID_AS = new ParseAssign (RID, VID);
          rBody->push_back (RID_AS);
		  
          int n = f->Args (); //return the last col as score
          ParseTerm * Score_AS = new ParseAssign (Score, f->Arg (n-1));
          
          /*
          ValuePtr scoreVal = ValDouble::New (-1.0); //return -1.0 as score
          ParseExpr * orig_score = new ParseVal (scoreVal);          
          ParseTerm * Score_AS = new ParseAssign (Score, orig_score);          
		  */
          rBody->push_back (Score_AS);

          Rule * rule = CreateRule (h, rBody, false, rID, NULL, false);  //set rWeight to NULL
          mRules->push_back (rule);
        }
    }

}

OlContext::ExtStageSpec::ExtStageSpec (string name, string input, string output) :
  stageName (name), inputTupleName (input), outputTupleName (output)
{
}

OlContext::ExtStageSpec::ExtStageSpec (const ExtStageSpec& s)
{
  stageName = s.stageName;
  inputTupleName = s.inputTupleName;
  outputTupleName = s.outputTupleName;
}

const OlContext::ExternalStageSpecMap&
OlContext::GetExtStagesInfo ()
{
  return mStages;
}
