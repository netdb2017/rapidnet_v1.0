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

#ifndef __PL_TABLESTORE_H__
#define __PL_TABLESTORE_H__

#include <vector>
#include <map>
#include <set>
#include "ol-context.h"

#include "ns3/ref-count-base.h"

using namespace std;

namespace ns3 {
namespace rapidnet_compiler {

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Stores the definition of materialized tables for
 *        the protocol.
 */
class TableStore : public RefCountBase
{
public:
  TableStore (Ptr<OlContext> ctxt);
  ~TableStore ()
  {
  }

  /** Get table specifications */
  OlContext::TableInfoMap* GetTableInfos ();

  OlContext::TableInfo* GetTableInfo (string tableName);
  void AddTableInfo (OlContext::TableInfo* ti)
  {
    _tableInfos->insert (make_pair (ti->tableName, ti));
  }

protected:
  OlContext::TableInfoMap* _tableInfos;
  Ptr<OlContext> _ctxt;
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif
