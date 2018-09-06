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

#include <iostream>
#include <fstream>
#include <string>
#include <sys/wait.h>

#include "ol-context.h"
#include "eca-context.h"
#include "localize-context.h"
#include "table-store.h"
#include "rapidnet-context.h"

#include "ns3/ptr.h"
#include "ns3/command-line.h"
#include "ns3/log.h"

#include <unistd.h>

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("RapidNetCompiler");

/**
 * \brief Preprocesses the overlog file using the C-preprocessor
 *        and returns the pre-processed program as a string.
 */
string preprocessReadOverlogProgram (string overlogFilename)
{
  cout << "Pre-processing..." << endl;
  string processedFilename = overlogFilename + ".cpp";
  char* args[6];
  int count = 0;

  args[count++] = (char *) "cpp";
  args[count++] = (char *) "-P";
  args[count++] = (char *) "-C";
  args[count++] = (char*) overlogFilename.c_str ();
  args[count++] = (char*) processedFilename.c_str ();
  args[count++] = NULL;

  // Invoke the preprocessor
  pid_t pid = fork ();
  if (pid == -1)
    {
      NS_LOG_ERROR ("Cannot fork a preprocessor.");
      exit (1);
    }
  else if (pid == 0)
    {
      if (execvp ("cpp", args) < 0)
        {
          NS_LOG_ERROR ("CPP ERROR");
        }
      exit (1);
    }
  else
    {
      wait (NULL);
    }

  // Read processed script.
  ifstream file;
  file.open (processedFilename.c_str ());

  if (!file.is_open ())
    {
      NS_LOG_ERROR ("Cannot open processed overlog file '" << processedFilename
          << "'!");
      return string ();
    }
  else
    {
      cout << "Generated pre-processed file " << processedFilename << endl;
      ostringstream scriptStream;
      string line;

      while (std::getline (file, line))
        {
          scriptStream << line << "\n";
        }
      file.close ();
      return scriptStream.str ();
    }
}

void printToFile (string filename, string contents)
{
  ofstream file;
  file.open (filename.c_str ());
  file << contents;
  file.close ();
  cout << "Generated file " << filename << endl;
}

/**
 * \brief Parses the overlog file and sets up overlog context and
 *        table information.
 */
void parseOverlog (string overlogFile, Ptr<OlContext> ctxt,
  Ptr<TableStore> tableStore, bool provenanceEnabled)
{
  string program = preprocessReadOverlogProgram (overlogFile);
  cout << "Parsing NDlog..." << endl;

  istringstream istr (program);
  ctxt->ParseStream (&istr, provenanceEnabled);

  if(provenanceEnabled && !ctxt->IsSendlog())
  {
	  string query_file = "rapidnet/compiler/provenance-query.olg";
	  string query_program = preprocessReadOverlogProgram (query_file);
	  istringstream querystr (query_program);
	  Ptr<OlContext> query (new OlContext ());
	  query->ParseStream (&querystr, false);

	  ctxt->CombineProvenanceQueryRules(query);
	  printToFile (overlogFile + ".prov", ctxt->ToString ());
  }
  // else case will never execute because this is handled
  // in the OlContext::SetContext method that is called
  // when the token stream is parsed.

  if (ctxt->GotErrors ())
    {
      NS_LOG_ERROR ("Parse Errors Found");
      ctxt->DumpErrors ();
      NS_LOG_ERROR ("Compilation aborted");
      exit (-1);
    }
}

/**
 * \brief Parses the discovery applications overlog file and adds
 *        the table information of the link table.
 */
void AddLinkTableInfo (string overlogFile, string baseOverlogFile, Ptr<
    TableStore> tableStore)
{
  if (baseOverlogFile == DEFAULT_RN_APP_BASE)
    {
      return;
    }

  Ptr<OlContext> ctxt (new OlContext ());
  Ptr<TableStore> baseTableStore (new TableStore (ctxt));
  parseOverlog (baseOverlogFile, ctxt, baseTableStore, false);
  OlContext::TableInfoMap baseTables = *baseTableStore->GetTableInfos();

  // Add all tables in the base application to this application for
  // the ECA context to know which tables are materialized.
  for (OlContext::TableInfoMap::iterator it = baseTables.begin ();
  it != baseTables.end (); ++it)
    {
      cout << "Adding " << it->first << endl;
      tableStore->AddTableInfo (it->second);
    }
  // tableStore->addTableInfo (baseTableStore->getTableInfo (DISCOVERY_LINK));
}

/**
 * \brief Compiles the application with given base application
 */
void compile (string overlogFile, string baseOverlogFile,
  bool provenanceEnabled)
{
  // Parse
  Ptr<OlContext> ctxt (new OlContext ());
  Ptr<TableStore> tableStore (new TableStore (ctxt));
  parseOverlog (overlogFile, ctxt, tableStore, provenanceEnabled);

  // Add link table info if discovery. Only when the Localize and
  // ECA context know that the link table is materialized as a part
  // of discovery, they generate the correct set of rules
  AddLinkTableInfo (overlogFile, baseOverlogFile, tableStore);

  // Localize
  cout << "Localizing..." << endl;
  Ptr<LocalizeContext> lctxt (new LocalizeContext ());
  lctxt->Rewrite (ctxt, tableStore);
  //printToFile (overlogFile + ".loc", lctxt->ToString ());

  // Translate to ECA
  cout << "Translating to ECA..." << endl;
  Ptr<EcaContext> ectxt (new EcaContext ());
  ectxt->Rewrite (lctxt, tableStore);
  printToFile (overlogFile + ".eca", ectxt->ToString ());

  // Translate to NS3
  cout << "Translating to ns-3..." << endl;
  Ptr<RapidNetContext> npctxt (new RapidNetContext (overlogFile,
    baseOverlogFile));
  npctxt->Rewrite (ectxt, tableStore);
  npctxt->Generate ();
}

int main (int argc, char** argv)
{
  LogComponentEnable ("OlContext", LOG_LEVEL_ERROR);
  LogComponentEnable ("ParserUtil", LOG_LEVEL_ERROR);
  LogComponentEnable ("LocalizeContext", LOG_LEVEL_ERROR);
  LogComponentEnable ("EcaContext", LOG_LEVEL_ERROR);
  LogComponentEnable ("RapidNetContext", LOG_LEVEL_ERROR);
  LogComponentEnable ("RapidNetCompiler", LOG_LEVEL_INFO);

  string overlogFile;
  string baseoverlogFile = DEFAULT_RN_APP_BASE;
  bool provenance = 0;

  CommandLine cmd;
  cmd.AddValue ("ndlog", "Application NDlog file", overlogFile);
  cmd.AddValue ("base-ndlog", "Application's base NDlog file (optional)",
    baseoverlogFile);
  cmd.AddValue ("provenance", "Enable provenance for this application (optional)",
    provenance);
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Application NDlog                             : "
      << overlogFile);
  NS_LOG_INFO ("Application base NDlog                        : "
      << baseoverlogFile);
  NS_LOG_INFO ("Provenance capability                         : "
    << (provenance ? "Enabled": "Disabled"));

  compile (overlogFile, baseoverlogFile, provenance);

  return 0;
}
