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

#ifndef __OL_LEXER_H__
#define __OL_LEXER_H__

#include <sstream>
#include <iostream>

#include "all-values.h"

#ifndef yyFlexLexer
#define yyFlexLexer OLBaseFlexLexer
#include <FlexLexer.h>
#endif

#include "parser-util.h"
#include "ol-parser.hh"

namespace ns3 {
namespace rapidnet_compiler {

class OlContext;


/**
 * \ingroup rapidnet_compiler
 *
 * \brief A C++ wrapper for the RapidNet lexical analyzer.
 */
class OlLexer : public OLBaseFlexLexer {

private:
  int comment_depth;
  ostringstream *cstring;

  yy_buffer_state *bufstate;
  std::istringstream strb;

public:

  // Default: yyin == std::cin.
  OlLexer(std::istream *str);
  // Give it a string...
  OlLexer(const char *prog);
  virtual ~OlLexer();

  int yylex (YYSTYPE *lvalp, OlContext *env);

  int line_num() const { return yylineno; };

};

} // namespace ns3
} // namespace rapidnet_compiler

#endif /* __OL_LEXER_H_ */
