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
#include "database.h"

using namespace ns3;
using namespace ns3::rapidnet;

TypeId
Database::GetTypeId(void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::Database")
    .SetParent<Object> ()
    ;
  return tid;
}

Database::Database ()
{
}

Database::~Database ()
{}

bool
Database::HasRelation (string name)
{
  return m_relations.find (name) != m_relations.end ();
}

void
Database::AddRelation (Ptr<RelationBase> reln)
{
  NS_ASSERT_MSG (!HasRelation (reln->GetName ()),
    "Relation with name '" << reln->GetName () << "' already exists.");
  m_relations[reln->GetName ()] = reln;
}

bool
Database::RemoveRelation (string name)
{
  return m_relations.erase (name) == 1;
}

Ptr<RelationBase>
Database::GetRelation (string name)
{
  NS_ASSERT_MSG (HasRelation (name),
    "Relation with name '" << name << "' does not exist.");
  return m_relations [name];
}

int
Database::Insert (Ptr<Tuple> tuple)
{
  return GetRelation (tuple->GetName ())->Insert (tuple);
}

void
Database::Delete (Ptr<Tuple> tuple)
{
  GetRelation (tuple->GetName ())->Delete (tuple);
}

/* Delete this method later */
void
Database::CleanTupleBeforeInsert (Ptr<Tuple> &tuple)
{
  tuple->RemoveAttribute (RN_SRC);
  tuple->RemoveAttribute (RN_DEST);
  tuple->RemoveAttribute (RN_ACTION);
}

Ptr<Database>
Database::New (Ptr<RapidNetApplicationBase> application)
{
  Ptr<Database> database = CreateObject<Database> ();
  database->m_application = application;
  return database;
}
