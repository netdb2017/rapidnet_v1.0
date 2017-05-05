

materialize(link,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(4:list)).
materialize(bestPath,infinity,infinity,keys(1,2)).
materialize(status,infinity,infinity,keys(1)).
materialize(table,infinity,infinity,keys(1,2:int32)).

/* Base Rules */

p1 path(@X,Y,C,P,N) :- link(@X,Y,C),
         P1:=f_append(X),
      P2:=f_append(Y),
      P:=f_concat(P1,P2),
      N:=Y.

p2 path(@Z,Y,C,P,N) :- link(@X,Z,C1),
     bestPath(@X,Y,C2,P2,N1),
    C:=C1+C2,
    f_member(P2,Z)==0,
    P1:=f_append(Z), P:=f_concat(P1,P2),
    N:=X.

p3 bestPath(@X,Y,a_MIN<C>,P,N) :- path(@X,Y,C,P,N).

i0 init(@X,Ind) :- periodic(@X,E,0.01,9000), status(@X), Ind:=f_rand().

i1 eSend(@X,D) :- init(@X,Ind), table(@X,Ind,D).

r0 packet(@X,D,PID,Data) :- eSend(@X,D), Time:=f_now(), PID:=f_sha1(""+Time),
     Data:="0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef".

r1 packet(@N,D,PID,Data) :- packet(@X,D,PID,Data), bestPath(@X,D,C,P,N), X!=D.
