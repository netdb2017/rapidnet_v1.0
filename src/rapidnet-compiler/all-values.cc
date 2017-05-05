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

#include "all-values.h"

using namespace ns3;
using namespace ns3::rapidnet_compiler;

ParsedValue::ParsedValue (TypeCode typeCode, string typeName)
  : m_typeCode (typeCode), m_typeName (typeName)
{
}

ValuePtr ValNull::New ()
{
  static ValuePtr _theSingleton = ValuePtr (new ValNull ());
  return _theSingleton;
}

ValId::ValId (string hexString)
  : ParsedValue (ParsedValue::ID, "ID")
{
  static const string zeros (WORDS * 8, '0');

  // Read it in in chunks of 4-byte words (8 hexadecimal digits) from
  // right to left. If it's too long, discard any extraneous prefix to
  // leave WORDS-worth of suffix characters
  unsigned position = hexString.length ();
  if (position > (WORDS * 8))
    {
      // This string is too long. Chop it down
      hexString = hexString.substr (position - (WORDS * 8), WORDS * 8);
      position = WORDS * 8;
    }

  // Now we have at most WORDS*8 characters but perhaps fewer.
  if (position < WORDS * 8)
    {
      // I don't have enough characters. Prepend 0's
      hexString = zeros.substr (0, WORDS * 8 - position) + hexString;
      position = WORDS * 8;
    }

  // Now we have exactly WORDS*8 characters.
  int wordIndex = WORDS - 1;
  for (; wordIndex >= 0; position -= 8, wordIndex--)
    {
      // Pick the last 8-char suffix
      string suffix = hexString.substr (position - 8, 8);
      const char* startPointer = suffix.c_str ();
      char* endPointer;
      words[wordIndex] = (uint32_t) strtoull (startPointer, &endPointer, 16);
      if (*endPointer != '\0')
        {
          // Decoding wasn't happy.  As much of a suffix as possible and pad
          // the rest
          while (*endPointer != '\0')
            {
              words[wordIndex] = (uint32_t) strtoull (endPointer + 1,
                &endPointer, 16);
            }

          // Take what we have and stop here. Fill in
          // the remainder with 0s
          wordIndex--;
          while (wordIndex >= 0)
            {
              words[wordIndex] = 0;
              wordIndex--;
            }
        }
    }
}

string ValId::ToString () const
{
  char buf[41];
  string result;
  for (unsigned i = 0; i < WORDS; i++)
    {
      sprintf (buf, "%08x", words[i]);
      result += buf;
    }
  return "0x" + result + "I";
}

string ValDouble::ToString() const
{
  ostringstream sb;
  sb << d;
  return sb.str();
}

