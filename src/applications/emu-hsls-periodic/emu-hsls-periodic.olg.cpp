

// HSLS periodic
materialize(tLink, 11, infinity, keys(1, 2)).
//tLink(@Src, Next, Cost, SeqNum)
materialize(tLSU, 65, infinity, keys(1, 2, 3)).
// tLSU(@Local, Src, Next, Cost, SeqNum, TTL)
//apptable for application level filtering, it is hard state!
materialize(apptable, infinity, infinity, keys(1, 2)).
// apptable(Src, Next, Cost)
//////////////////////////////////////////////////////////////// Link injection
// Issue a linkAdd event locally for apptable entries with Src attribute as self.
// This is periodic because apptable is hard state and tLink is soft state
r11 eLinkAdd(@Src, Next, Cost):-
    periodic(@Src, E, 5, 0, 0.5),
    apptable(@Src, Next, Cost),
    Src != Next.
//////////////////////////////////////////////////////////////// Link insertion
// Check if the link is new by join and count.
r33 eLinkCount(@Src, Next, Cost, a_COUNT<*>):-
    eLinkAdd(@Src, Next, Cost),
    tLink(@Src, Next, Cost_2, SeqNum).
// Insert if new link.
r34 tLink(@Src, Next, Cost, SeqNum):-
    eLinkCount(@Src, Next, Cost, Count),
    Count == 0,
    SeqNum := 0.
// Refresh
r35 tLink(@Src, Next, Cost, SeqNum):-
    eLinkAdd(@Src, Next, Cost),
    tLink(@Src, Next, Cost_2, SeqNum).
////////////////////////////////////////////////////////////////// Periodic update of LSUs (locally)
// Periodically send the links as LSUs locally.
r61A eLSU(@Src, Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 120, 0, 60, 30),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 2 .
//Periodically increment the sequence numbers of all links.
r61B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 120, 0, 59),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
// Periodically send the links as LSUs locally.
r62A eLSU(@Src, Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 240, 0, 120, 30),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 4 .
// Periodically increment the sequence numbers of all links.
r62B tLink(@Src, Next, Cost, SeqNumU):-
      periodic(@Src, E, 240, 0, 119),
      tLink(@Src, Next, Cost, SeqNum),
      SeqNumU := SeqNum + 1.
// Periodically send the links as LSUs locally.
r63A eLSU(@Src, Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 480, 0, 240, 30),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 8 .
// Periodically increment the sequence numbers of all links.
r63B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 480, 0, 239),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
// Periodically send the links as LSUs locally.
r64A eLSU(@Src, Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 960, 0, 480, 30),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 16 .
// Periodically increment the sequence numbers of all links.
r64B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 960, 0, 479),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
////////////////////////////////////////////////////////////////// LSU insertion or update
// Replace broadcast address with current address
r70 eLSULoc(@Local, From, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSU(@X, From, LastHop, Dst, Cost, SeqNum, TTL).
// Filter as per the apptable
rFilter eLSUFiltered(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSULoc(@Src, From, LastHop, Dst, Cost, SeqNum, TTL),
    apptable(@Src, From, Cost),
    Cost < 999999 .
// Check if LSU is new by join and count.
r71 eLSUCount(@Src, LastHop, Dst, Cost, SeqNum, TTL, a_COUNT<*>):-
    eLSUFiltered(@Src, LastHop, Dst, Cost, SeqNum, TTL),
    tLSU(@Src, LastHop, Dst, Cost_2, SeqNum_2, TTL_2).
// Insert the LSU if new.
r72 tLSU(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSUCount(@Src, LastHop, Dst, Cost, SeqNum, TTL, Count),
    Count == 0.
// If not new LSU but greater sequence number then update sequence number.
r73 tLSU(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSUFiltered(@Src, LastHop, Dst, Cost, SeqNum, TTL),
    tLSU(@Src, LastHop, Dst, Cost_2, SeqNum_2, TTL_2),
    SeqNum > SeqNum_2.
////////////////////////////////////////////////////////////////// Broadcast updates
// If LSU is updated then issue an LSUChange event.
r81 eLSUChange(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    tLSU(@Src, LastHop, Dst, Cost, SeqNum, TTL).
// If LSU changes then broadcast to all neighbors.
r82 eLSU(@Broadcast, Src, LastHop, Dst, Cost, SeqNum, TTLU):-
    eLSUChange(@Src, LastHop, Dst, Cost, SeqNum, TTL),
    TTL > 0,
    TTLU := TTL - 1.
