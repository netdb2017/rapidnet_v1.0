

materialize(neighbor,infinity,infinity,keys(1,2)).
materialize(route,infinity,infinity,keys(3:list)).
materialize(carryTraffic,infinity,infinity,keys(2,3)).
materialize(acceptRoute,infinity,infinity,keys(2,3)).

At Z:

z1 route(Z,X,P) :- neighbor(Z,X),
                   P1:=f_append(Z),
                   P2:=f_append(X),
                   P:=f_concat(P1,P2).


z2 route(Z,Y,P) :- X says advertise(Y,P),acceptRoute(Z,X,Y).

z3 Z says advertise(Y,P1)@X :- neighbor(Z,X),
                               route(Z,Y,P),
                               carryTraffic(Z,X,Y),
                               P0:=f_append(X),
                               P1:=f_concat(P0,P).

/* Equivalent NDlog rule*/
/*
z1 route(@Z,X,P) :- neighbor(@Z,X), 
                    P1:=f_append(Z), 
                    P2:=f_append(X),
                    P:=f_concat(P1,P2).
		      
  
z2 route(@Z,Y,P) :- X says advertise(Y,P,@Z,X),                                                       
                    acceptRoute(@Z,X,Y).
                                                       
z3 Z says advertise(Y,P1,@X,Z) :- neighbor(@Z,X), 
                                  route(@Z,Y,P),
                                  carryTraffic(@Z,X,Y),
                                  P0:=f_append(X),
                                  P1:=f_concat(P0,P).
*/
