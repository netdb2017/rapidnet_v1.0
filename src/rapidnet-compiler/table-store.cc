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

#include "table-store.h"

using namespace ns3;
using namespace ns3::rapidnet_compiler;

TableStore::TableStore (Ptr<OlContext> ctxt)
{
  _ctxt = ctxt;
  _tableInfos = ctxt->GetTableInfos ();
}

OlContext::TableInfo*
TableStore::GetTableInfo (string tableName)
{
  OlContext::TableInfoMap::iterator theIterator;
  theIterator = _tableInfos->find (tableName);
  if (theIterator == _tableInfos->end ())
    {
      return NULL;
    }
  return theIterator->second;
}

OlContext::TableInfoMap*
TableStore::GetTableInfos ()
{
  return _tableInfos;
}

