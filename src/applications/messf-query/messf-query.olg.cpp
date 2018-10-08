/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
materialize(records,infinity,infinity,keys(1,2:cid,3:cid)).
materialize(tuple, infinity, infinity, keys(1,2:str)).
materialize(results,infinity,infinity,keys(1,2:cid,3:cid)).
q1 1.0 provQuery(@X, QID, UID, ME):- periodic(@ME,E,4,2), tuple(@ME, Name, X, Y), UID:=f_sha1(Name+X+Y),
                                 Time := f_now(), QID:=f_sha1(""+UID+Time).
q2 1.0 records(@ME, QID, RID, Prov) :- pReturn(@ME, QID, RID, Prov).
q3 1.0 results(@ME, QID, RID, Scores) :- records(@ME, QID, RID, Prov), Scores := f_pCAL(Prov).
