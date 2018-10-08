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

#ifndef RAPIDNET_CONTEXT_H
#define RAPIDNET_CONTEXT_H

#include <string>
#include <vector>
#include "ol-context.h"
#include "eca-context.h"
#include "table-store.h"
#include "parser-util.h"

#include "ns3/log.h"
#include "ns3/ref-count-base.h"

#define PERIOD "period"
#define DELAY "delay"
#define NUM_TIMES "times"
#define JITTER "jitter"

#define DEFAULT_RN_APP_BASE "RapidNetApplicationBase"
//#define DISCOVERY_LINK "link"

using namespace std;
using namespace ns3;

namespace ns3 {

/**
 * \brief RapidNet compiler.
 */
namespace rapidnet_compiler {

/* ******************* Utility Methods ************ */

/**
 * \brief Capitalizes the first character of a name string.
 */
string Capitalize (string str);

/**
 * \brief Capitalizes all the characters of a name string.
 */
string AllCaps (string str);

/**
 * \brief Un-capitalizes all the characters of a name string.
 */
string AllSmall (string str);

/**
 * \brief Returns the directory from the given path.
 */
string GetDir (string name);

/**
 * \brief Returns the filename without extension the from the given path.
 */
string GetFile (string name);

/**
 * \brief Prepared the camel case class name from the given ndlog file path.
 */
string PrepareClassname (string name);

/**
 * \brief To format the attribute name given the tuple/table
 *        name and the attribute position.
 */
string FormatAttrName (string tupleName, int attrPos);

/**
 * \brief To format a vector of strings into string with
 *        individual elements separated by a comma.
 */
string FormatVectorAsString (vector<string> list, bool withNewLines, int indent);

/**
 * \brief Returns the name of the assignor object for the given
 *        assignment function name. Like FAppend for f_append.
 */
string LookupFunction (string functionName);

/**
 * \brief Returns the name of the IsXEvent method (like IsInsertEvent,
 *        IsDeleteEvent, etc) by looking at the rule event.
 */
string LookupEventMethod (EcaRule* rule);

/**
 * \brief Returns the name of the action method (like Insert,
 *         Delete, etc) by looking at the rule action.
 */
string LookupActionMethod (EcaRule* rule);

/**
 * \brief Returns the name of the type enum in RapidNet
 */
string LookupRapidNetTypeCode (ValuePtr value);

/**
 * \brief Returns the value class name in RapidNet
 */
string LookupRapidNetValueClass (ValuePtr value);

/**
 * \brief Maps the type name used in NDlog to type name string
 * used by \ref GetTypeName()
 */
string LookupRapidNetTypeCode (string type);

/*****************  End ******************************/

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Represents the definition of a materialized table with its
 *        name and key attributes
 */
class RapidNetTable
{
public:

  static long TIME_INFINITY;

  RapidNetTable (string name = "noname")
  {
    m_name = name;
    m_timeout = TIME_INFINITY;
  }

  string GetName ()
  {
    return m_name;
  }

  /** Reads the keys from a TableInfo object */
  void ReadKeys (OlContext::TableInfo* tblInfo);

  vector<string> GetKeys ()
  {
    return m_keys;
  }

  vector<string> GetKeyTypes ()
  {
    return m_keyTypes;
  }

  void SetTimeout (long timeout);

  long GetTimeout ()
  {
    return m_timeout;
  }

  bool IsTimeoutInfinity ()
  {
    return m_timeout == TIME_INFINITY;
  }

protected:

  void AddKey (string key)
  {
    m_keys.push_back (key);
  }

  string m_name;

  vector<string> m_keys;

  vector<string> m_keyTypes;

  long m_timeout;
};

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Represents the tuples (similar to ParseFunctor) that
 * is manipulated in the rule method bodies.
 */
class RapidNetTuple
{
public:
  /** Reads the tuple from the functor. */
  RapidNetTuple (ParseFunctor* pfunctor, bool ignoreErrors);

  /** The identifier used for the tuple. */
  RapidNetTuple (ParseFunctor* pfunctor, string id, bool ignoreErrors);

  RapidNetTuple (string name = "noname", string id = "result");

  string GetName ()
  {
    return m_name;
  }

  string GetId ()
  {
    return m_id;
  }

  void AddAttribute (string var, string attr)
  {
    m_var2AttrNameMap [var] = attr;
  }

  string GetAttribute (string var);

  bool HasAttribute (string var)
  {
    return m_var2AttrNameMap.find (var) != m_var2AttrNameMap.end ();
  }

  bool IsPeriodicTuple ()
  {
    return m_isPeriodic;
  }

  string GetPeriod ();

  bool HasTimes ();

  string GetTimes ()
  {
    return m_var2AttrNameMap[NUM_TIMES];
  }

  map<string, string> GetAttrMap ()
  {
    return m_var2AttrNameMap;
  }

  int AttributeCount ()
  {
    return m_var2AttrNameMap.size ();
  }

  /**
   * \brief Computes the join conditions to join the current
   *        tuple with the given tuple. It returns the new
   *        tuple with its attribute names. It also computes
   *        the code to do the join and prints as a side effect.
   */
  RapidNetTuple Join (FILE* ccFile, RapidNetTuple tuple, bool qualify);

protected:

  void Parse (ParseFunctor* pfunctor, bool ignoreErrors);

  /** C++ identifier for the tuple **/
  string m_id;

  /** Tuple name. */
  string m_name;

  /** True if the functor is a periodic. **/
  bool m_isPeriodic;

  /** This maps the variable used in EcaContext to the
      attribute name in NS3. */
  map<string, string> m_var2AttrNameMap;
};


/**
 * \ingroup rapidnet
 * \defgroup rapidnet_compiler RapidNet Compiler
 */

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Rewrites from the EcaContext to C++ code for RapidNet.
 *
 * This class translates the intermediate ECA representation into
 * C++ code that runs on the RapidNet library.
 *
 * @see EcaContext
 */
class RapidNetContext : public RefCountBase
{
public:

  /** Initializes file streams for header and class files */
  RapidNetContext (string name, string appBase = DEFAULT_RN_APP_BASE);

  /** Closes the file streams */
  ~RapidNetContext ();

  /** Initializer for the NS3P2_Context object. This method should be
      invoked before any other generator methods. */
  void Rewrite (Ptr<EcaContext> ectxt, Ptr<TableStore> tableStore);

  void Generate ()
  {
    GenerateHeader ();
    GenerateClass ();
    CloseAll ();
  }

  bool IsSendlog ()
  {
    return m_isSendlog;
  }

protected:
  /** Generates the header file for the NS3P2 application class. */
  void GenerateHeader ();

  /** Reads aggregates if any and sets up variables accordingly. */
  void ReadAggregateIfAny (EcaRule *rule);

  /** Generates the definition or class file for the NS3P2 application
      class. */
  void GenerateClass ();

  /** Runs through all the rules and finds out all tuple names by
      looking at only the event and action functor names. This is
      used for declaring them as constant strings in the header file. */
  void PrepareAllTupleNames ();

  /** Runs through the tableStore and identifies the names and keys of
      the materialized tables. */
  void PrepareTables (Ptr<TableStore> tableStore);

  /** Generates the class file's details - header includes, namespace usages,
      type-id, constructor, destructor, start/stop app, etc. */
  void GenerateClassDetails ();

  /** Generates the InitDatabase method */
  void GenerateInitDatabase ();

  /** Generates aggregator initialization code in InitDatabase. */
  void GenerateInitAggregators ();

  /** Generates the DemuxRecv method */
  void GenerateDemuxRecv ();

  /** Generates the definitions for all non-watchStub rule methods. */
  void GenerateAllRuleMethods ();

  /** Generates the method definition for the given rule */
  void GenerateRuleMethod (EcaRule* rule);

  /** Generate the verify block in the rule. */
  void GenerateVerifyBlock (EcaRule* rule);

  /** Generates the code for all joins. */
  void GenerateAllJoins (EcaRule* rule);

  /** Generates the code for all assignments. */
  void GenerateAllAssignments (EcaRule* rule);

  /** Generates the code for a single assignment. */
  void GenerateAssignment (ParseAssign* assign);

  /** Generates the code for a periodic method/rule. */
  void GeneratePeriodicMethod (EcaRule* rule);

  /** Formats the argument list of a functor as string. */
  string FormatArgsAsString (ParseFunctor* func, bool aggWrap,
    bool withNewLines = false, int indent = 0);

  /** Formats the argument list of a function as string. */
  string FormatArgsAsString (ParseFunction* func, bool withNewLines = false, int indent = 0);

  /** A recursive method to generate the code for arbitrarily complex
      math expression.*/
  string FormatExpression (ParseExpr* expr, int indent);

  /** In assignments, the lhs could either be a new attribute in the tuple or
      an exisiting one. This method looksup the result and returns the
      corresponding attribute name if it is found or a quoted string if not. */
  string LookupOrQuote (string lhs);

  /** Generates code for all selects. */
  void GenerateAllSelects (EcaRule* rule);

  /** Generates code for the given select. */
  void GenerateSelect (ParseSelect* select);

  /** Generates code for action (includes project). */
  void GenerateAction (EcaRule* rule);

  /** Generate the Sign and Encrypt for SeNDlog. */
  void GenerateSignEncryptBlock (EcaRule* rule, string actionMethod);

  /** Looks up the given variable in the results. Throws an error
      if not found. */
  string Lookup (string lhs);

  /** Close all streams
   */
  void CloseAll ();

  /** The class name for the generated application. */
  string m_classname;

  /** The file name without extension. */
  string m_filename;

  /** The folder name with trailing slash. */
  string m_foldername;

  /** The base class for the generated application. */
  string m_appBase;

  /** The file name without extension of the base application.*/
  string m_baseFilename;

  /** Stores a link to the EcaContext. */
  Ptr<EcaContext> m_ectxt;

  /** Map of all materialized tables against their respective names. */
  map<string, RapidNetTable> m_tables;

  /** Map (essentially a list - the value is same as key) for all tuples being
      used. */
  map<string, string> m_allTupleNames;

  /** FILE objects for header file and class file */
  FILE *hFile, *ccFile, *helperFile;

  /** Stores the intermediate result tuple between joins, assignments, selects
      and projects. */
  RapidNetTuple m_result;

  /** String streams to record the schedule and cancel commands for the periodic
      events. This is computed when we run through the rules and used when the
      class file is generated. */
  stringstream m_periodicSchedule, m_periodicCancel;

  /** String streams to record the member variable initialization commands.
   */
  stringstream m_memberVarInit;

  /** Maps member variables (periodic events, aggreator instances, etc) to their
      declaration string. */
  map<string, string> m_memberVarDeclMap;

  /** Maps the rule name to the aggregator, if any. */
  map<string, string> m_ruleName2AggrVarMap;

  bool m_isSendlog;
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif // RAPIDNET_CONTEXT_H
