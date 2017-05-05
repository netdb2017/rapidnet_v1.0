

materialize(link,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(4:list)).
materialize(bestPath,infinity,infinity,keys(1,2)).

/* Base Rules */

r1 path(@X,Y,C,P) :- link(@X,Y,C),
       P1:=f_append(X),
    P2:=f_append(Y),
    P:=f_concat(P1,P2).

r2 path(@Z,Y,C,P) :- link(@X,Z,C1),
       bestPath(@X,Y,C2,P2),
    C:=C1+C2,
    f_member(P2,Z)==0,
    P1:=f_append(Z),
    P:=f_concat(P1,P2).

r3 bestPath(@X,Y,a_MIN<C>,P) :- path(@X,Y,C,P).
