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
materialize(liveEvent, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(likeEvent, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(knowEvent, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(live, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(like, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(know, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(relation, infinity, infinity, keys(1, 2:int32, 3:int32)).
materialize(shaResult, infinity, infinity, keys(1, 2:cid, 3:str)).
ra live(@Local, A, C) :- liveEvent(@Local, A, C).
rb like(@Local, A, C) :- likeEvent(@Local, A, C).
rc know(@Local, A, B, S) :- knowEvent(@Local, A, B, S).
r1 knowEvent(@Local, A, B, S) :- liveEvent(@Local, A, C), live(@Local, B, C), A!=B, S:=1.
r2 knowEvent(@Local, A, B, S) :- likeEvent(@Local, A, C), like(@Local, B, C), A!=B, S:=1.
r4 knowEvent(@Local, A, B, S) :- relation(@Local, A, B), A!=B, S:=0.
r5 knowEvent(@Local, A, B, S) :- liveEvent(@Local, A, C1), live(@Local, B, C2), C1!=C2, A!=B, S:=0.
r6 shaResult(@Local, VID, Content) :- liveEvent(@Local, A, C), VID := f_sha1("liveEvent"+Local+A+C), Name := "liveEvent", Content := Name+A+C.
r7 shaResult(@Local, VID, Content) :- likeEvent(@Local, A, C), VID := f_sha1("likeEvent"+Local+A+C), Name := "likeEvent", Content := Name+A+C.
r8 shaResult(@Local, VID, Content) :- knowEvent(@Local, A, B, S), VID := f_sha1("knowEvent"+Local+A+B+S), Name := "knowEvent", Content := Name+A+B+S.
r9 shaResult(@Local, VID, Content) :- relation(@Local, A, B), VID := f_sha1("relation"+Local+A+C), Name := "relation", Content := Name+A+B.
r10 shaResult(@Local, VID, Content) :- live(@Local, A, C), VID := f_sha1("live"+Local+A+C), Name := "live", Content := Name+A+C.
r11 shaResult(@Local, VID, Content) :- like(@Local, A, C), VID := f_sha1("like"+Local+A+C), Name := "like", Content := Name+A+C.
r12 shaResult(@Local, VID, Content) :- know(@Local, A, B, S), VID := f_sha1("know"+Local+A+B+S), Name := "know", Content := Name+A+B+S.
