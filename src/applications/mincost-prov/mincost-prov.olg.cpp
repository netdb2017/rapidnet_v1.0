

materialize(link,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(1,2,3:int32)).
materialize(bestPath,infinity,infinity,keys(1,2)).


r1 path(@X,Y,C) :- link(@X,Y,C).

r2 path(@Z,Y,C) :- link(@X,Z,C1), bestPath(@X,Y,C2), C:=C1+C2, Z!=Y.

r3 bestPath(@X,Y,a_MIN<C>) :- path(@X,Y,C).
