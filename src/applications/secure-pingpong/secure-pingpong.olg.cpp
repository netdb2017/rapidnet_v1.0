

materialize(tLink, infinity, infinity, keys(1,2)).

At Src:

r1 encrypts ePing(Next, Src)@Next :- periodic(Src, E, 1),tLink(Src, Next).

r2 encrypts ePong(Next, Src)@Next :- encrypts ePing(Src, Next).

r3 ePingPongFinish(Src):- encrypts ePong(Src, Next).
