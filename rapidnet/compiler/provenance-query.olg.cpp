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
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(pQList,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
/* Querying */
/* EDB vertex */
edb1 pReturn(@Ret,QID,VID,Prov) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc,Score), RID==VID, Prov:=f_pEDB(VID,X,Score).
/* IDB vertex */
idb1 pQList(@X,QID,a_LIST<RID>) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc,Score), RID!=VID.
idb2 pResultTmp(@X,QID,Ret,VID,Buf) :-
       provQuery(@X,QID,VID,Ret), Buf:=f_empty().
idb3 pIterate(@X,QID,N) :- provQuery(@X,QID,VID,Ret), N:=1.
idb4 pIterate(@X,QID,N) :- pIterate(@X,QID,N1),
       pQList(@X,QID,List), N1<f_size(List), N:=N1+1.
idb5 eRuleQuery(@X,NQID,RID) :- pIterate(@X,QID,N),
       pQList(@X,QID,List), N<=f_size(List),
       RID:=f_item(List,N), NQID:=f_sha1(""+QID+RID).
idb6 ruleQuery(@RLoc,QID,RID,X) :- eRuleQuery(@X,QID,RID),
       prov(@X,VID,RID,RLoc,Score).
idb7 pResultTmp(@X,QID,Ret,VID,Buf) :- rReturn(@X,NQID,RID,Prov),
       pResultTmp(@X,QID,Ret,VID,Buf1), NQID==f_sha1(""+QID+RID),
       Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).
idb8 ePReturn(@X,QID) :- pResultTmp(@X,QID,Ret,VID,Buf),
       pQList(@X,QID,List),
       f_size(Buf)==f_size(List), f_size(Buf)!=0.
idb9 pReturn(@Ret,QID,VID,Prov) :- ePReturn(@X,QID),
       pResultTmp(@X,QID,Ret,VID,Buf), Prov:=f_pIDB(Buf,X).
/* Rule Vertex */
rv1 rQList(@X,QID,List) :- ruleQuery(@X,QID,RID,Ret),
      ruleExec(@X,RID,RWeight,R,List).
rv2 rResultTmp(@X,QID,Ret,RID,Buf) :-
      ruleQuery(@X,QID,RID,Ret), Buf:=f_empty().
rv3 rIterate(@X,QID,N) :- ruleQuery(@X,QID,RID,Ret), N:=1.
rv4 rIterate(@X,QID,N) :- rIterate(@X,QID,N1),
      rQList(@X,QID,List), N1<f_size(List), N:=N1+1.
rv5 eProvQuery(@X,NQID,VID) :- rIterate(@X,QID,N),
      rQList(@X,QID,List),
      VID:=f_item(List,N), NQID:=f_sha1(""+QID+VID).
rv6 provQuery(@X,QID,VID,X) :- eProvQuery(@X,QID,VID).
rv7 rResultTmp(@X,QID,Ret,RID,Buf) :- pReturn(@X,NQID,VID,Prov),
      rResultTmp(@X,QID,Ret,RID,Buf1), NQID==f_sha1(""+QID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).
rv8 eRReturn(@X,QID) :- rResultTmp(@X,QID,Ret,RID,Buf),
      rQList(@X,QID,List), f_size(Buf)==f_size(List).
rv9 rReturn(@Ret,QID,RID,Prov) :- eRReturn(@X,QID),
      rResultTmp(@X,QID,Ret,RID,Buf),
      ruleExec(@X,RID,RWeight,R,List), Prov:=f_pRULE(Buf,X,R,RWeight).
