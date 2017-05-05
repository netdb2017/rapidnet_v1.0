

materialize(tLink, infinity, infinity, keys(1,2)).

// Every 1 second, node "Src" will send out a ePing event to its neighbors, i.e. node "Next"
r1 ePing(@Next, Src):-
    periodic(@Src, E, 1),
    tLink(@Src, Next).

// When a node receive a ePing event from its neighbor, it sent back a ePong message
r2 ePong(@Next, Src):-
    ePing(@Src, Next).

// When a node receive a ePong event from its neighbor, this Ping-Pong procedure is indicated as finished
r3 ePingPongFinish(@Src):-
    ePong(@Src, Next).
