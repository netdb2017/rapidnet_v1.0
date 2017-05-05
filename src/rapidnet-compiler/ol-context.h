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

#ifndef __OL_PARSEENV_H__
#define __OL_PARSEENV_H__

#include <vector>
#include <map>
#include <set>
#include "all-values.h"
#include <list>
#include "parser-util.h"

#include "ns3/log.h"
#include "ns3/ref-count-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet_compiler {

/**
 * \ingroup rapidnet_compiler
 *
 * \brief A parsed form of the entire protocol in NDlog.
 *
 * This is the first stage of compilation. From here, it is localized
 * by the LocalizeContext and then translated to ECA representation by
 * EcaContext and finally C++ code is generated to run on RapidNet library
 * by RapidNetContext.
 *
 * OL stands for overlog or NDlog.
 *
 * @see LocalizeContext, EcaContext, RapidNetContext
 */
class OlContext : public RefCountBase
{

public:
  /* A few static constants borrowed from Table2. */

  /** The non-expiring expiration time */
  static long NO_EXPIRATION;

  /** The default table expiration time */
  static long DEFAULT_EXPIRATION;

  /** The non-size-limited size */
  static uint32_t NO_SIZE;

  /** The default table size */
  static uint32_t DEFAULT_SIZE;

  static int ruleCount;

  struct Error
  {
    int line_num; // Line number
    string msg; // What?
    Error (int l, string m) :
      line_num (l), msg (m)
    {
    }
  };

  /** An OverLog rule structure */
  struct Rule
  {
    Rule (string r, ParseFunctor *h, bool d, bool dEvent = false) :
      ruleID (r), head (h), deleteFlag (d), deleteEventFlag (dEvent),
      ruleNum (OlContext::ruleCount++)
    {
    	isPeriodic = false;
    	isAggregate = false;
    	isProvenanceRule =false;
    }
    ;

    void SetName (ParseExpr* name)
    {
      ruleID = name->value->ToString ();
    }

    string ToString ();

    string ruleID;

    /** The functor at the rule head */
    ParseFunctor* head;

    /** Is this rule a delete rule? */
    bool deleteFlag;

    /** Is this rule having a delete event? */
    bool deleteEventFlag;

    /** The list of terms in the rule body */
    list<ParseTerm*> terms;

    int ruleNum;

    /** (Provenance) Provenance rewritten rules from a original periodic rule.
     * In this case, delete rewritten rule is not needed.*/
    bool isPeriodic;

    /** (Provenance) Whether this is an aggregate rule
     * if yes, no.4 rewritten rule will be the original rule itself*/
	bool isAggregate;

    /** (Provenance) Indicate whether this is a provenance rewritten rule*/
    bool isProvenanceRule;
  };

  /**
   * \brief The meta-data of the table.
   */
  struct TableInfo
  {
    string ToString ();

    string tableName;

    /** Timeout duration in seconds. */
    long timeout;

    uint32_t size;

    vector<unsigned> primaryKeys;

    /* Corresponding types of the primary keys */
    vector<string> primaryKeyTypes;
  };


  /**
   * \brief The meta-data of SeNDlog context.
   */
  struct Context
  {
	  string ToString();

	  ParseExpr* mId;
  };

  /*******************************************************************/
  OlContext ();
  ~OlContext ();

  // Parsing programs
  void ParseString (const char *prog);

  void ParseStream (istream *str, bool provenance);

  // Create a new rule
  OlContext::Rule* CreateRule (ParseTerm *lhs, ParseTermList *rhs, bool deleteFlag,
    ParseExpr *n = NULL, bool deleteEventFlag = false);

  // Add a new rule to the system
  void AddRule (ParseRule* rule);

  OlContext::Rule* CreateAggRule (ParseTerm *lhs, ParseAggTerm *rhs, bool deleteFlag,
    ParseExpr *n = NULL);

  //SeNDlog: set the context for the rules
  void SetContext (ParseExpr *c = NULL);

  // Materialize a table
  void table (ParseExpr *n, ParseExpr *t, ParseExpr *s, ParseExprList *k =
      NULL);

  void Query (ParseTerm *term);

  void Fact (ParseTerm *term);

  /** Register a watch fact */
  void Watch (ParseExpr *t, string modifiers);

  /** Register a stage fact, made of the stage name, the input tuple
   name and the output tuple name */
  void Stage (ParseExpr* stageName, ParseExpr* inputTuple,
    ParseExpr* outputTuple);

  void TraceTuple (ParseExpr *t);

  /** Keep track of tables to be traced */
  void TraceTable (ParseExpr *t);

  /** Error management */
  void ReportError (string msg);

  /** Return true if I encountered errors during parsing */
  bool GotErrors ();

  /** Dump any errors into the ERROR stream */
  void DumpErrors ();

  OlLexer *lexer;

  string ToString ();

  typedef vector<Rule*> RuleList;

  typedef map<string, OlContext::TableInfo *> TableInfoMap;

  typedef vector<OlContext::Error *> ErrorList;

  /** The type of watched table mappings */
  typedef map<string, string> WatchTableType;

  /** The external stage structure */
  struct ExtStageSpec
  {
    string stageName;

    string inputTupleName;

    string outputTupleName;

    ExtStageSpec (string name, string input, string output);

    ExtStageSpec ()
    {
    }

    ExtStageSpec (const ExtStageSpec& s);
  };

  /** The type of stage catalogs */
  typedef map<string, ExtStageSpec*> ExternalStageSpecMap;


  /**
   * (Provenance) Combines the input rules after rewriting them for provenance
   * with query rules.
   */
  void CombineProvenanceQueryRules (Ptr<OlContext> othertxt);

private:

  /**
   * (Provenance) Rewrite the input rule for provenance, if provenance is
   * enabled. Each rule is tranlated into 5 rules.
   */
  void ProvenanceRewrite (Rule* rule);

  /**
   * (Provenance) Generates the first rule that generates the local
   * event eHtemp. (line 4)
   */
  ParseFunctor* GenerateLocalEHTempEventRule (Rule* rule);

  /**
   * (Provenance) Generates the second rule that creates the local
   * ruleExec entry corresponding to metadata for rule execution. (line 5)
   */
  void GenerateLocalRuleExecEntryRule (Rule* rule, ParseFunctor* eHTemp);

  /**
   * (Provenance) Generates the third rule that sends the event message eH
   * to the target node. (line 6)
   */
  ParseFunctor* GenerateSendEventMsgEHRule (Rule * rule, ParseFunctor* eHTemp);

  /**
   * (Provenance) Generates the fourth rule that creates the corresponding result
   * tuple h. (line 7)
   */
  void GenerateCreateResultTupleHRule (Rule* rule, ParseFunctor* eH);

  /**
   * (Provenance) Generates the fifth rule that creates the remote prov entry.
   * (line 8)
   */
  void GenerateCreateRemoveProvEntryRule (Rule* rule, ParseFunctor* eH);

  /**
   * (Provenance) Add provenance rule to initialize the provenance tree with
   * the leaf nodes that correspond to EDBs.
   */
  void AddEdbProvenanceRule ();

  TableInfoMap* mTables;

  RuleList* mRules;

  /** The watched table map */
  WatchTableType watchTables;

  ParseFunctor* singleQuery;

  set<string, less<string> > tuplesToTrace;

  /** A set of table names for those tables we wish to trace during
   execution */
  set<string> tablesToTrace;

  /** Declared external stages */
  ExternalStageSpecMap mStages;

  /**
   * (SeNDlog) the context for the rules
   */
  Context mContext;

  /**
   * (Provenance) A set of rule head recording provenance rewriten rules, to avoid
   * repetition.
   */
  set<string> rewrittenIdbs;

  /**
   * (Provenance) Potential edb functors.
   */
  map <string, ParseFunctor*> potentialEdbs;

  bool isProvenanceEnabled;

  static const string PROVENANCE_RULE_PREFIX;

public:

  ErrorList errors;

  RuleList* GetRules ()
  {
    return mRules;
  }

  TableInfoMap* GetTableInfos ()
  {
    return mTables;
  }

  WatchTableType GetWatchTables ()
  {
    return watchTables;
  }

  set<string> GetTuplesToTrace ()
  {
    return tuplesToTrace;
  }

  set<string> GetTablesToTrace ()
  {
    return tablesToTrace;
  }

  string GetContext ()
  {
	  return mContext.ToString();
  }

  bool IsSendlog ()
  {
    return mContext.mId != NULL;
  }

  const ExternalStageSpecMap& GetExtStagesInfo ();
};

} // namespace ns3
} // namespace rapidnet_compiler

extern int ol_parser_parse (ns3::rapidnet_compiler::OlContext *env);

#endif /* __OL_PARSEENV_H_ */
