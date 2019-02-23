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
///// EDBs /////
materialize(word, infinity, infinity, keys(1,2)).
// word(@Local, W) -- W is a word. 
// Example: word(@Local, "horse").
materialize(hasImg, infinity, infinity, keys(1,2,3,4)).
// hasImg(@Local, X, Rln, Y) -- X and Y has relation R.
// Example: hasImg(@Local, "horse", "color", "grey"). 
// Example: hasImg(@Local, "horse", "in", "background"). 
// Example: hasImg(@Local, "tree", "behind", "hence"). 
materialize(hasQ, infinity, infinity, keys(1,2,3,4)).
// hasQ(@Local, X, Rln, Y) -- Predict X.
// Example: hasQ(@Local, "building", "is", "What"). 
// Example: hasQ(@Local, "background", "building", "What"). 
materialize(sim, infinity, infinity, keys(1,2,3)).
materialize(simRep1, infinity, infinity, keys(1,2,3)).
materialize(simRep2, infinity, infinity, keys(1,2,3)).
// sim(@Local, X, Y) -- X and Y has similarity.
// Example: sim(@Local, "horse", "barn")  with score 0.325
// Example: sim(@Local, "horse", "horse") with score 1
///// IDBs /////
materialize(candidate, infinity, infinity, keys(1,2)).
// candidate(@Local, X) -- X is a candidate to the question.
// Example: candidate(@Local, "barn").
materialize(hasImgAns, infinity, infinity, keys(1,2,3,4,5)).
// hasImgAns(@Local, Z, X, Rln, Y) -- candidate Z is related to the knowledge (X, Rln, Y).
// Example: hasImgAns(@Local, "horse", "horse", "in", "background"). 
materialize(ans, infinity, infinity, keys(1,2)).
// ans(@Local, X) -- X is an answer to the question. 
///// Rules /////
// In implementation, replace these two rules by adding additional insertion in the script
ra simRep1(@Local, Z, X) :- sim(@Local, Z, X).
rb simRep2(@Local, Z, X) :- sim(@Local, Z, X).
r1 hasImgAns(@Local, Z, X, Rln, Y) :- word(@Local, Z), hasImg(@Local, X, Rln, Y),
                    sim(@Local, Z, X), simRep1(@Local, Z, Y).
r2 candidate(@Local, Z) :- word(@Local, Z).
r3 candidate(@Local, Z) :- word(@Local, Z), hasQ(@Local, Y, Rln, X), hasImgAns(@Local, Z, X1, Rln1, Y1),
                sim(@Local, Rln, Rln1), simRep1(@Local, Y, Y1), simRep2(@Local, X, X1).
r4 ans(@Local, Z) :- candidate(@Local, Z), hasQ(@Local, X, Rln, Qtn), Qtn == "WHAT",
                hasImg(@Local, Z, Rln, X).
r5 ans(@Local, Z) :- candidate(@Local, Z), hasQ(@Local, X, Rln, Qtn), Qtn == "WHAT",
                hasImg(@Local, Z1, Rln, X), sim(@Local, Z, Z1).
r6 ans(@Local, Z) :- candidate(@Local, Z), hasQ(@Local, X, Rln, Qtn), Qtn == "WHAT",
                hasImg(@Local, Z1, Rln1, X1), sim(@Local, Z, Z1), simRep1(@Local, Rln, Rln1), simRep2(@Local, X, X1).
