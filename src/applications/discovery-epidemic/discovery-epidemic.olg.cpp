






materialize(link, 5, infinity, keys(1, 2)).
//tLink(@Src, Next, Cost)


// Broadcast beacons periodically
r1 beacon(@Broadcast, Src):-
    periodic(@Src, E, 2, 0, 1, 2).

// Localize the beacon
r2 beaconLoc(@Local, Nbr):-
    beacon(@X, Nbr).

// Insert link
r3 link(@Src, Nbr, Cost):-
    beaconLoc(@Src, Nbr),
    Cost := 1.

rAddLink eLinkDiscoveryAdd(@Src, Nbr, Cost):-
          link(@Src, Nbr, Cost).

rDelLink eLinkDiscoveryDel(@Src, Nbr, Cost):-
          delete link(@Src, Nbr, Cost).
