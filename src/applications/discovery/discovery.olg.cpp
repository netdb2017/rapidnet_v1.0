






materialize(link, 11, infinity, keys(1, 2)).
//tLink(@Src, Next, Cost)


// Broadcast beacons periodically
r1 beacon(@Broadcast, Src):-
    periodic(@Src, E, 5, 0, 1, 4).

// Localize the beacon
r2 beaconLoc(@Local, Nbr):-
    beacon(@X, Nbr).

// Insert link
r3 link(@Src, Nbr, Cost):-
    beaconLoc(@Src, Nbr),
    Cost := 1.
