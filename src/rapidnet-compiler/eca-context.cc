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

#include "eca-context.h"
#include "parser-util.h"
#include "errno.h"

using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("EcaContext");

string ParseEvent::ToString ()
{
  ostringstream b;
  if (event == RECV)
    {
      b << "EVENT_RECV<" << pFunctor->ToString () << ">";
    }
  if (event == INSERT)
    {
      b << "EVENT_INSERT<" << pFunctor->ToString () << ">";
    }
  if (event == DELETE)
    {
      b << "EVENT_DELETE<" << pFunctor->ToString () << ">";
    }
  if (event == REFRESH)
    {
      b << "EVENT_REFRESH<" << pFunctor->ToString () << ">";
    }
  return b.str ();
}

ParseEvent::ParseEvent (ParseFunctor *pf, Event e) :
  pFunctor (pf), event (e)
{
	del = false;   //provenance del set to be false
}

string ParseAction::ToString ()
{
  ostringstream b;
  if (action == LOCAL_SEND)
    {
      b << "ACTION_LOCAL_SEND<" << pFunctor->ToString () << ">";
    }
  if (action == REMOTE_SEND)
    {
      b << "ACTION_REMOTE_SEND<" << pFunctor->ToString () << ">";
    }
  if (action == ADD)
    {
      b << "ACTION_ADD<" << pFunctor->ToString () << ">";
    }
  if (action == DELETE)
    {
      b << "ACTION_DELETE<" << pFunctor->ToString () << ">";
    }
  if (action == DROP)
    {
      b << "ACTION_DROP<" << pFunctor->ToString () << ">";
    }
  return b.str ();
}

string EcaRule::ToString ()
{
  ostringstream b;
  b << "ECA Rule " << ruleID << " " << ToRuleString ();
  return b.str ();
}

string EcaRule::ToRuleString ()
{
  ostringstream b;
  if (pEvent != NULL)
    {
      b << pAction->ToString () << " :- " << pEvent->ToString ();

      if (probeTerms.size () + selectAssignTerms.size () == 0)
        {
          b << ".";
        }
      else
        {
          b << ", ";
        }
    }
  else
    {
      b << pAction->ToString () << " :- ";
    }

  unsigned k = 0;
  for (k = 0; k + 1 < probeTerms.size (); k++)
    {
      b << probeTerms.at (k)->ToString () << ", ";
    }
  if (probeTerms.size () > 0)
    {
      b << probeTerms.at (k)->ToString ();
      if (selectAssignTerms.size () > 0)
        {
          b << ", ";
        }
      else
        {
          b << ".";
        }
    }
  for (k = 0; k + 1 < selectAssignTerms.size (); k++)
    {
      b << selectAssignTerms.at (k)->ToString () << ", ";
    }
  if (selectAssignTerms.size () > 0)
    {
      b << selectAssignTerms.at (k)->ToString () << ".";
    }
  return b.str ();
}

EcaRule::EcaRule (string r) :
  ruleID (r)
{
  pEvent = NULL;
  pAction = NULL;
  aggWrap = false;
}

string EcaRule::GetEventName ()
{
  return pEvent->pFunctor->fName->name;
}

void EcaContext::Rewrite (Ptr<LocalizeContext> lctxt,
  Ptr<TableStore> tableStore)
{
  m_isSendlog = lctxt->IsSendlog ();

  for (unsigned k = 0; k < lctxt->GetRules ().size (); k++)
    {
      OlContext::Rule* nextRule = lctxt->GetRules ().at (k);
      int countEvents = 0;

      // First count the events in the next rule
      list<ParseTerm*>::iterator t = nextRule->terms.begin ();
      for (; t != nextRule->terms.end (); t++)
        {
          ParseTerm* nextTerm = (*t);
          // Is the next rule term a tuple?
          ParseFunctor* nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
          if (nextFunctor != NULL)
            {
              // Next term is a tuple
              string termName = nextFunctor->fName->name;
              // Is it materialized?
              OlContext::TableInfo* termTableInfo = tableStore->GetTableInfo (
                termName);

              if (termTableInfo == NULL)
                {
                  // Not materialized, so this is an event
                  countEvents++;
                }
            }
        }

      // Do I have more than 1 events in the rule?
      if (countEvents > 1)
        {
          NS_LOG_ERROR (nextRule->ToString ()
              << " should have at most one event.");
          exit (-1);
        }

      // Does this rule have 0 events?
      if (countEvents == 0)
        {
          // view rules with no events
          int aggField = nextRule->head->Aggregate ();
          if (aggField >= 0)
            { // there is an aggregate
              // This is an aggregate view rule. Handle it
              RewriteAggregateView (nextRule, tableStore);
            }
          else
            {
              // No aggregate. This is a normal view rule
        	  //Provenance: view rule rewrite for provenance rewritten rules is integrated
              RewriteViewRule (nextRule, tableStore);
            }
        }
      else
        {
          // This rule has exactly 1 event. Handle like a traditional ECA
          // rules
    	  if(!nextRule->isProvenanceRule)
    	    {
    		  RewriteEventRule (nextRule, tableStore);
    	    }
    	  else
    	    {
    		  //Provenance: event rule rewrite for provenance rewritten rules
    		  RewriteProvEventRule(nextRule, tableStore);
    	    }

        }
    }
}

void EcaContext::WatchStubRule (string watchedName)
{
  // Ignore watch rules
}

void EcaContext::RewriteEventRule (OlContext::Rule* rule,
  Ptr<TableStore> tableStore)
{
  // figure out which is the event.
  NS_LOG_INFO ("Perform ECA Rewrite on " << rule->ToString ());

  // The location specifier of the rule body. Since this is a localized
  // rule, this is also the location specifier of the event tuple
  string bodyloc ("");
  string ruleIDPrefix = rule->ruleID + "_eca";

  /** The new (currently empty) ECA rule */
  EcaRule* eca_rule = new EcaRule (ruleIDPrefix);

  /** For all old-rule terms */
  int counter = 1;
  for (list<ParseTerm*>::iterator t = rule->terms.begin (); t
      != rule->terms.end (); t++, counter++)
    {
      // The actual term
      ParseTerm* nextTerm = (*t);

      // Attempt to cast it as a tuple, selection, or assignment
      ParseFunctor *nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
      ParseSelect *nextSelect = dynamic_cast<ParseSelect*> (nextTerm);
      ParseAssign *nextAssign = dynamic_cast<ParseAssign*> (nextTerm);

      // Is it a tuple?
      if (nextFunctor != NULL)
        {
          // This is a tuple term
          bodyloc = nextFunctor->GetLocSpec ();
          string termName = nextFunctor->fName->name;
          OlContext::TableInfo* termTableInfo = tableStore->GetTableInfo (
            termName);
          if (termTableInfo != NULL)
            {
              // this is not the event, append it to the conditions of the ECA
              // rule
              eca_rule->probeTerms.push_back (nextFunctor);
            }
          else
            {
              // This is indeed an event. Handle the "periodic" event in a
              // separate way.
              if (termName == "periodic")
                {
                  // Break this up into two rules. eca_rule1 will be
                  // LOCAL_SEND_ACTION<ruleID+periodic(@NodeID, EventID)> :-
                  // EVENT_RECV<periodic(NodeID, EventID, Period, Repeats)>.
                  string periodicRuleID = ruleIDPrefix + "periodic";
                  EcaRule* eca_rule1 = new EcaRule (periodicRuleID);

                  // The event generated by the periodic
                  ParseExprList* periodicArgs = new ParseExprList ();
                  periodicArgs->push_back (nextFunctor->Arg (0)); // the locspec
                  periodicArgs->push_back (nextFunctor->Arg (1)); // the event ID
                  ValuePtr name = ValStr::New (periodicRuleID);
                  ValuePtr loc = ValStr::New (nextFunctor->GetLocSpec ());
                  ParseFunctor* sendFunctor = new ParseFunctor (
                    new ParseFunctorName (new ParseVal (name)), periodicArgs,
                    new ParseVal (loc));

                  // The actual rule has the sendFunctor as the action and the
                  // periodic tuple as the event
                  eca_rule1->pAction = new ParseAction (sendFunctor,
                    ParseAction::LOCAL_SEND);
                  eca_rule1->pEvent = new ParseEvent (nextFunctor,
                    ParseEvent::INSERT);
                  AddRule (eca_rule1);

                  // The rest of the original periodic rule, to be stored in
                  // eca_rule. Listening for the tuples generated by eca_rule1
                  ParseFunctor* recvFunctor = new ParseFunctor (
                    new ParseFunctorName (new ParseVal (name)), periodicArgs,
                    new ParseVal (loc));
                  eca_rule->pEvent = new ParseEvent (recvFunctor,
                    ParseEvent::RECV);
                  // The rest of the processing of further original terms will
                  // be appended to eca_rule
                }
              else
                {
                  // Just plonk down the event to the eca rule
                  eca_rule->pEvent = new ParseEvent (nextFunctor,
                    ParseEvent::RECV);
                }
            }
        }
      else
        {
          // OK this is not a tuple. It might be a selection or an
          // assignment. Regardless of what it is, plonk it into the "other
          // terms" of the ECA rule.
          if (nextSelect != NULL || nextAssign != NULL)
            {
              eca_rule->selectAssignTerms.push_back (nextTerm);
            }
          else
            {
              // This is a term type that we didn't think of. Complain but
              // move on
              NS_LOG_WARN ("The " << counter << "-th term of rule '"
                  << rule->ToString () << "' has an unknown type. Ignoring.");
            }
        }
    }

  // now generate the head action
  GenerateActionHead (rule, bodyloc, tableStore, eca_rule);

  // And carry over the aggregate-ness of this rule into the newly
  // minted ECA rule
  int aggField = rule->head->Aggregate ();
  if (aggField >= 0)
    { // there is an aggregate
      eca_rule->aggWrap = true;
    }

  AddRule (eca_rule);
}

void EcaContext::RewriteProvEventRule (OlContext::Rule* rule,
  Ptr<TableStore> tableStore)
{
	 // figure out which is the event.
	  NS_LOG_INFO ("Perform ECA Rewrite on " << rule->ToString ());

	  // The location specifier of the rule body. Since this is a localized
	  // rule, this is also the location specifier of the event tuple
	  string bodyloc ("");
	  string ruleIDPrefix = rule->ruleID + "_eca";

	  for(list<ParseTerm*>::iterator t = rule->terms.begin(); t!= rule->terms.end();t++)
	  {
	  	 ParseFunctor* tf = dynamic_cast<ParseFunctor *> (*t);
	  	 if (tf!=NULL && tf->fName->name =="periodic")
	  	 {
	  		//for the periodic rule itself (rewritten rule 1), use the original event rule rewrite
	  		RewriteEventRule (rule, tableStore);
	  		return;
	  	  }
	  }

	  /** The 1st new ECA rule: ADD */
	  EcaRule* eca_rule1 = new EcaRule (ruleIDPrefix+"Add");
	  /** For all old-rule terms */
	  int counter = 1;
	  for (list<ParseTerm*>::iterator t = rule->terms.begin (); t
	      != rule->terms.end (); t++, counter++)
	    {
	      // The actual term
	      ParseTerm* nextTerm = (*t);

	      // Attempt to cast it as a tuple, selection, or assignment
	      ParseFunctor *nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
	      ParseSelect *nextSelect = dynamic_cast<ParseSelect*> (nextTerm);
	      ParseAssign *nextAssign = dynamic_cast<ParseAssign*> (nextTerm);

	      // Is it a tuple?
	      if (nextFunctor != NULL)
	        {
	          // This is a tuple term
	          bodyloc = nextFunctor->GetLocSpec ();
	          string termName = nextFunctor->fName->name;
	          OlContext::TableInfo* termTableInfo = tableStore->GetTableInfo (
	            termName);
	          if (termTableInfo != NULL)
	            {
	              // this is not the event, append it to the conditions of the ECA
	              // rule
	              eca_rule1->probeTerms.push_back (nextFunctor);
	            }
	          else
	            {
                	// Just plonk down the event to the eca rule
                   eca_rule1->pEvent = new ParseEvent (nextFunctor,
                		   ParseEvent::RECV);
	            }
	        }
	      else
	        {
	          // OK this is not a tuple. It might be a selection or an
	          // assignment. Regardless of what it is, plonk it into the "other
	          // terms" of the ECA rule.
	          if (nextSelect != NULL || nextAssign != NULL)
	            {
	              eca_rule1->selectAssignTerms.push_back (nextTerm);
	            }
	          else
	            {
	              // This is a term type that we didn't think of. Complain but
	              // move on
	              NS_LOG_WARN ("The " << counter << "-th term of rule '"
	                  << rule->ToString () << "' has an unknown type. Ignoring.");
	            }
	        }
	    }

	  // now generate the head action
	  GenerateActionHead (rule, bodyloc, tableStore, eca_rule1);
	  AddRule (eca_rule1);


	  /** The 2nd new ECA rule: DELETE */
	  if(!rule->isPeriodic)  //periodic rewritten rules 2-5 do not have delete delta rules
	  {
		  EcaRule* eca_rule2 = new EcaRule (ruleIDPrefix+"Del");
		  	  /** For all old-rule terms */
		  	  counter = 1;
		  	  for (list<ParseTerm*>::iterator t = rule->terms.begin (); t
		  	       != rule->terms.end (); t++, counter++)
		  	   {
		  	      // The actual term
		  	       ParseTerm* nextTerm = (*t);

		  	      // Attempt to cast it as a tuple, selection, or assignment
		  	      ParseFunctor *nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
		  	      ParseSelect *nextSelect = dynamic_cast<ParseSelect*> (nextTerm);
		  	      ParseAssign *nextAssign = dynamic_cast<ParseAssign*> (nextTerm);

		  	       // Is it a tuple?
		  	       if (nextFunctor != NULL)
		  	       {
		  	         // This is a tuple term
		  	          bodyloc = nextFunctor->GetLocSpec ();
		  	          string termName = nextFunctor->fName->name;
		  	          OlContext::TableInfo* termTableInfo = tableStore->GetTableInfo (
		  	  	            termName);
		  	          if (termTableInfo != NULL)
		  	          {
		  	               // this is not the event, append it to the conditions of the ECA
		  	               // rule
		  	              eca_rule2->probeTerms.push_back (nextFunctor);
		  	           }
		  	          else
		  	          {
		  	                // Just plonk down the event to the eca rule
		  	        	  ParseFunctorName * fname = new ParseFunctorName
		  							 (new ParseVal (ValStr::New(nextFunctor->fName->name + "Delete")));
		  	        	  ParseFunctor * delFunctor = new ParseFunctor(fname, nextFunctor->m_args); //location?
		  	              eca_rule2->pEvent = new ParseEvent (delFunctor,
		  	              ParseEvent::RECV);
		  	              eca_rule2->pEvent->del = true;
		  	            }
		  	       }
		  	       else
		  	        {
		  	        // OK this is not a tuple. It might be a selection or an
		  	         // assignment. Regardless of what it is, plonk it into the "other
		  	          // terms" of the ECA rule.
		  	           if (nextSelect != NULL || nextAssign != NULL)
		  	            {
		  	               eca_rule2->selectAssignTerms.push_back (nextTerm);
		  	            }
		  	           else
		  	          {
		  	               // This is a term type that we didn't think of. Complain but
		  	               // move on
		  	             NS_LOG_WARN ("The " << counter << "-th term of rule '"
		  	  	                  << rule->ToString () << "' has an unknown type. Ignoring.");
		  	           }
		  	  	   }
		  	  }

		  	  	  // now generate the head action
		   	  GenerateActionHead (rule, bodyloc, tableStore, eca_rule2);
		    	  AddRule (eca_rule2);

	  }

}

void EcaContext::GenerateActionHead (OlContext::Rule* rule, string bodyLoc,
  Ptr<TableStore> tableStore, EcaRule* eca_rule)
{

  // Is the rule head an unmaterialized tuple name?
  string headName = rule->head->fName->name;
  OlContext::TableInfo* headTableInfo = tableStore->GetTableInfo (headName);
  if (headTableInfo == NULL)
    {
      // It's unmaterialized, i.e., an event.
      // is it local or remote?
      if (!rule->head->GetLocSpec ().empty () && fieldNameEq (
        rule->head->GetLocSpec (), bodyLoc))
        {
          eca_rule->pAction = new ParseAction (rule->head,
            ParseAction::LOCAL_SEND);
        }
      else
        {
          if(!eca_rule->pEvent->del)  //if not prov del delta rule or not prov eca rule at all
          {
        	  eca_rule->pAction = new ParseAction (rule->head,
        	             ParseAction::REMOTE_SEND);
          }
          else  //generate a delete functor to send
          {
        	  ParseFunctorName * fname = new ParseFunctorName
        	  		(new ParseVal (ValStr::New(rule->head->fName->name + "Delete")));
        	  ParseFunctor * delFunctor = new ParseFunctor(fname, rule->head->m_args); //location?
        	  eca_rule->pAction = new ParseAction (delFunctor,
        	           	             ParseAction::REMOTE_SEND);
          }
        }
    }
  else
    {
      // to be materialized
      if (!rule->head->GetLocSpec ().empty () && fieldNameEq (
        rule->head->GetLocSpec (), bodyLoc))
        {
          // local materialization
          if (rule->deleteFlag ||eca_rule->pEvent->del)  //delete delta rule for provenance
            {
              eca_rule->pAction = new ParseAction (rule->head,
                ParseAction::DELETE);
            }
          else
            {
              // cliu: new stuff
              if (eca_rule->pEvent->event == ParseEvent::DELETE)
                { // event_delete -> action_delete
                  eca_rule->pAction = new ParseAction (rule->head,
                    ParseAction::DELETE);
                }
              else
                { // event_insert -> action_add
                  eca_rule->pAction = new ParseAction (rule->head,
                    ParseAction::ADD);
                }
            }
        }
      else
        {
          // remote materialization. Generate a separate send rule and a
          // separate local materialization rule. The current rule eca_rule
          // does the sending. The new rule eca_rule1 will do the storing.
          ValuePtr name = ValStr::New (rule->ruleID + rule->head->fName->name
              + "send");
          ValuePtr loc = ValStr::New (rule->head->GetLocSpec ());
          ParseFunctor* sendFunctor = new ParseFunctor (new ParseFunctorName (
            new ParseVal (name)), rule->head->m_args, new ParseVal (loc),
            rule->head->m_sayer, rule->head->m_auth, rule->head->m_encrypt);

          eca_rule->pAction = new ParseAction (sendFunctor,
            ParseAction::REMOTE_SEND);

          string headName = rule->head->fName->name;
          OlContext::TableInfo* headTableInfo = tableStore->GetTableInfo (
            headName);
          if (headTableInfo != NULL)
            {
              string materializationRuleName = rule->ruleID + "ECAMat";
              EcaRule* eca_rule1 = new EcaRule (materializationRuleName);
              // The event is the reception of the head of the rule above
              eca_rule1->pEvent = new ParseEvent (sendFunctor,
                ParseEvent::RECV);
              if (rule->deleteFlag)
                {
                  eca_rule1->pAction = new ParseAction (rule->head,
                    ParseAction::DELETE);
                }
              else
                {
                  eca_rule1->pAction = new ParseAction (rule->head,
                    ParseAction::ADD);
                }
              AddRule (eca_rule1);
            }
        }
    }
}

void EcaContext::AddRule (EcaRule* eca_rule)
{
  for (unsigned k = 0; k < m_ecaRules.size (); k++)
    {
      if (m_ecaRules.at (k)->ToRuleString () == eca_rule->ToRuleString ())
        {
          return;
        }
    }
  NS_LOG_INFO ("Add rule: " << eca_rule->ToString ());
  m_ecaRules.push_back (eca_rule);
}

void EcaContext::RewriteViewRule (OlContext::Rule* rule,
  Ptr<TableStore> tableStore)
{
  NS_LOG_INFO ("Perform ECA view Rewrite on " << rule->ToString ());

  string headName = rule->head->fName->name;
  OlContext::TableInfo* headTableInfo = tableStore->GetTableInfo (headName);

  bool softStateRule = false;
  list<ParseTerm*>::iterator t = rule->terms.begin ();
  for (; t != rule->terms.end (); t++)
    {
      ParseTerm* nextTerm = (*t);
      ParseFunctor *nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
      if (nextFunctor == NULL)
        {
          continue;
        }
      OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
        nextFunctor->fName->name);
      if (tableInfo == NULL || tableInfo->timeout != OlContext::NO_EXPIRATION)
        {
          softStateRule = true; // if any rule body is soft-state, rule is soft-state
          break;
        }
    }
  if (headTableInfo == NULL || headTableInfo->timeout
      != OlContext::NO_EXPIRATION)
    {
      softStateRule = true; // if head is unmaterialized or softstate,
      // then the rule is soft-state
    }
  NS_LOG_INFO ("Processing soft state rule " << softStateRule << " "
      << rule->ToString ());

  t = rule->terms.begin ();
  int count = 0;

  // For every materialized rule-body term, create a set of delta rules
  // (one for insertions, one for refreshes, and one for deletes)
  // containing all other terms. Softstate rules only get the insert
  // rule per materialized term, whereas hardstate rules get all three
  // rules per term. If the action is a local materialized table, the
  // actions are performed directly (insert for insertions and
  // refreshes, delete for deletes).  If the action is a remote
  // materialized table, then a proxy rule is generated that sends the
  // appropriate message across, which causes the remote materialized
  // table to be affected as would have been the case for a locally
  // materialized table. If the action is a non-materialized table
  // (local or remote), then only a send action is created (local or
  // remote as the case may be).
  for (; t != rule->terms.end (); t++)
    {
      ParseTerm* nextTerm = (*t);
      ParseFunctor* nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
      if (nextFunctor == NULL)
        {
          count++;
          continue;
        }

      // create an event
      ostringstream oss;
      oss << rule->ruleID << "Eca" << count;
      EcaRule* eca_insert_rule = new EcaRule (oss.str () + "Ins");
      EcaRule* eca_delete_rule = new EcaRule (oss.str () + "Del");
      EcaRule* eca_refresh_rule = new EcaRule (oss.str () + "Ref");

      // delete functor generated from delete event
      // If deleteEventFlag is set, not need to append "delete" to the functor name since
      // this is the only rule that is going to be emitted
      ValuePtr name = ValStr::New (rule->head->fName->name
          + (rule->deleteEventFlag ? "" : "Delete"));
      ValuePtr loc = ValStr::New (rule->head->GetLocSpec ());

      ParseFunctor *deleteFunctor = new ParseFunctor (new ParseFunctorName (
        new ParseVal (name)), rule->head->m_args, new ParseVal (loc),
        rule->head->m_sayer, rule->head->m_auth, rule->head->m_encrypt);

      ValuePtr nameSend = ValStr::New (rule->ruleID + rule->head->fName->name
          + "send");
      ValuePtr locSend = ValStr::New (rule->head->GetLocSpec ());
      ParseFunctor* sendFunctor = new ParseFunctor (new ParseFunctorName (
        new ParseVal (nameSend)), rule->head->m_args, new ParseVal (locSend),
        rule->head->m_sayer, rule->head->m_auth, rule->head->m_encrypt);

      // create the events
      eca_insert_rule->pEvent = new ParseEvent (nextFunctor,
        ParseEvent::INSERT);
      eca_refresh_rule->pEvent = new ParseEvent (nextFunctor,
        ParseEvent::REFRESH);
      eca_delete_rule->pEvent = new ParseEvent (nextFunctor,
        ParseEvent::DELETE);

      bool softStatePredicate = false;
      OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
        nextFunctor->fName->name);
      if (tableInfo == NULL || tableInfo->timeout != OlContext::NO_EXPIRATION)
        {
          softStatePredicate = true;
        }

      if (!rule->head->GetLocSpec ().empty () && fieldNameEq (
        rule->head->GetLocSpec (), nextFunctor->GetLocSpec ()))
        {
          // if this is local, we can simply add local table or send as an event
          if (headTableInfo != NULL)
            {
              eca_insert_rule->pAction = new ParseAction (rule->head,
                ParseAction::ADD);
              eca_refresh_rule->pAction = new ParseAction (rule->head,
                ParseAction::ADD);
              eca_delete_rule->pAction = new ParseAction (rule->head,
                ParseAction::DELETE);
            }
          else
            {
              // send head events
              eca_insert_rule->pAction = new ParseAction (rule->head,
                ParseAction::LOCAL_SEND);
              eca_refresh_rule->pAction = new ParseAction (rule->head,
                ParseAction::LOCAL_SEND);
              eca_delete_rule->pAction = new ParseAction (deleteFunctor,
                ParseAction::LOCAL_SEND);
            }
        }
      else
        {
          // if the head is remote, we have to do a send,
          // followed by another recv/add rule strand
          eca_insert_rule->pAction = new ParseAction (sendFunctor,
            ParseAction::REMOTE_SEND);
          eca_delete_rule->pAction = new ParseAction (deleteFunctor,
            ParseAction::REMOTE_SEND);
          eca_refresh_rule->pAction = new ParseAction (sendFunctor,
            ParseAction::REMOTE_SEND);
          if (headTableInfo == NULL)
            {
              // The rule head is a remote event. Just send the appropriate info
              eca_insert_rule->pAction = new ParseAction (rule->head,
                ParseAction::REMOTE_SEND);
              eca_refresh_rule->pAction = new ParseAction (rule->head,
                ParseAction::REMOTE_SEND);
              eca_delete_rule->pAction = new ParseAction (deleteFunctor,
                ParseAction::REMOTE_SEND);
            }
          else
            {
        	  //The rule head is a remote table
              ostringstream oss;
              oss << rule->ruleID << "Eca" << count << "Remote";

              // insert
              EcaRule* eca_insert_rule1 = new EcaRule (oss.str () + "Ins");
              eca_insert_rule1->pEvent = new ParseEvent (sendFunctor,
                ParseEvent::RECV);
              eca_insert_rule1->pAction = new ParseAction (rule->head,
                ParseAction::ADD);

              // refresh
              EcaRule* eca_refresh_rule1 = new EcaRule (oss.str () + "Ref");
              eca_refresh_rule1->pEvent = new ParseEvent (sendFunctor,
                ParseEvent::RECV);
              eca_refresh_rule1->pAction = new ParseAction (rule->head,
                ParseAction::ADD);

              // delete
              EcaRule* eca_delete_rule1 = new EcaRule (oss.str () + "Del");
              eca_delete_rule1->pEvent = new ParseEvent (deleteFunctor,
                ParseEvent::RECV);
              eca_delete_rule1->pAction = new ParseAction (rule->head,
                ParseAction::DELETE);

              if (rule->deleteEventFlag)
                {
                  AddRule (eca_delete_rule1);
                }
              else
                {
                  AddRule (eca_insert_rule1);
                  if (softStatePredicate)
                    {
                      AddRule (eca_refresh_rule1);
                    }
                }
              if (!softStateRule || rule->isProvenanceRule)
                {
            	  //if this is a prov rewritten rule, even if it is soft state,
				  // add delete rule rule for it.
                  AddRule (eca_delete_rule1);    //prov del: for remote table
                }
            }
        }

      // create the other terms
      int count1 = 0;
      list<ParseTerm*>::iterator t = rule->terms.begin ();
      for (; t != rule->terms.end (); t++)
        {
          if (count1 != count)
            {
              ParseTerm* nextTerm1 = (*t);
              ParseFunctor *nextFunctor1 =
                  dynamic_cast<ParseFunctor*> (nextTerm1);
              ParseSelect *nextSelect =
                  dynamic_cast<ParseSelect*> (nextTerm1);
              ParseAssign *nextAssign =
                  dynamic_cast<ParseAssign*> (nextTerm1);

              if (nextFunctor1 != NULL)
                {
                  eca_insert_rule->probeTerms.push_back (nextFunctor1);
                  eca_delete_rule->probeTerms.push_back (nextFunctor1);
                  eca_refresh_rule->probeTerms.push_back (nextFunctor1);
                }
              if (nextSelect != NULL || nextAssign != NULL)
                {
                  eca_insert_rule->selectAssignTerms.push_back (nextTerm1);
                  eca_refresh_rule->selectAssignTerms.push_back (nextTerm1);
                  eca_delete_rule->selectAssignTerms.push_back (nextTerm1);
                }
            }
          count1++;
        }

      if (rule->deleteEventFlag)
        {
          AddRule (eca_delete_rule);
        }
      else
        {
          AddRule (eca_insert_rule);
          //cout << rule->ruleID << ":" << nextFunctor->fn->name << " SSP " <<
          //  softStatePredicate << " " << softStateRule << " " <<
          //  tableInfo->timeout << endl;
          if (softStatePredicate)
            {
              AddRule (eca_refresh_rule);
            }
        }
      if (!softStateRule || rule->isProvenanceRule)
        {
          //cascade deletes only for hard-state rules
    	  //or in provenance rewrite rules
          AddRule (eca_delete_rule);  //prov del: for remote event, local event and local table
        }
      count++;

      // Break out if deleteEvent Flag is set
      // We do not need one rule per term
      if (rule->deleteEventFlag)
        {
          break;
        }
    }
}

void EcaContext::RewriteAggregateView (OlContext::Rule* rule,
  Ptr<TableStore> tableStore)
{
  NS_LOG_INFO ("Perform ECA aggregate view Rewrite on " << rule->ToString ());
  if (rule->terms.size () != 1)
    {
      NS_LOG_WARN ("Currently only support simple table view aggregates");
      exit (-1);
    }

  list<ParseTerm*>::iterator t = rule->terms.begin ();
  ParseTerm* nextTerm = (*t);

  ParseFunctor *nextFunctor = dynamic_cast<ParseFunctor*> (nextTerm);
  if (nextFunctor == NULL)
    {
      NS_LOG_WARN ("Currently only support simple table view aggregates");
      exit (-1);
    }

  ostringstream oss;
  oss << rule->ruleID << "eca";
  EcaRule* eca_rule = new EcaRule (oss.str ());
  eca_rule->pEvent = new ParseEvent (nextFunctor, ParseEvent::INSERT);

  // generate the head action
  GenerateActionHead (rule, nextFunctor->GetLocSpec (), tableStore, eca_rule);
  AddRule (eca_rule);

  // cliu: new stuff: adding "event_delete -> action_delete"
  ostringstream oss2;
  oss2 << rule->ruleID << "eca2";
  EcaRule* eca_rule2 = new EcaRule (oss2.str ());
  eca_rule2->pEvent = new ParseEvent (nextFunctor, ParseEvent::DELETE);

  // generate the head action
  GenerateActionHead (rule, nextFunctor->GetLocSpec (), tableStore, eca_rule2);
  AddRule (eca_rule2);
}

string EcaContext::ToString ()
{
  ostringstream b;
  for (unsigned k = 0; k < m_ecaRules.size (); k++)
    {
      b << m_ecaRules.at (k)->ToString () << "\n";
    }
  return b.str ();
}
