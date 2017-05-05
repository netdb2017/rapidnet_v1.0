

materialize(link,infinity,infinity,keys(1,2)).
materialize(linkD,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(4:list)).
materialize(bestPath,infinity,infinity,keys(2)).

/* Rules */
At S:

r1 path(S,D,C,P) :- link(S,D,C),
                  P1:=f_append(S),
                  P2:=f_append(D),
                  P:=f_concat(P1,P2).

r2 encrypts S says linkD(S,D,C)@D :- link(S,D,C).

r3 encrypts S says path(D,Z,C,P)@D :- encrypts D says linkD(D,S,C1),
                                   bestPath(S,Z,C2,P2),
                                   C:=C1+C2,
                                   f_member(P2,D)==0,
                                   P1:=f_append(D),
                                   P:=f_concat(P1,P2).

r4 bestPath(S,D,a_MIN<C>,P) :- encrypts W says path(S,D,C,P).


/* Equivalent NDlog rules */
/*
r1 path(@S,D,C,P) :- link(@S,D,C),
                  P1:=f_append(S), 
                  P2:=f_append(D),
                  P:=f_concat(P1,P2).

r2 linkD(S,@D,C) :- link(@S,D,C).

r3 path(@D,Z,C,P,S) :- linkD(D,@S,C1),
                                   bestPath(@S,Z,C2,P2),
                                   C:=C1+C2,
                                   f_member(P2,D)==0,	        
                                   P1:=f_append(D),
                                   P:=f_concat(P1,P2).

r4 bestPath(@S,D,a_MIN<C>,P) :- path(@S,D,C,P,W).
*/
