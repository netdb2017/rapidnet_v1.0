

// HSLS periodic
materialize(tLink, 11, infinity, keys(1, 2)).
//tLink(@Src, Next, Cost, SeqNum)
materialize(tLSU, 65, infinity, keys(1, 2, 3)).
// tLSU(@Local, Src, Next, Cost, SeqNum, TTL)
///////////////////////////////// Link dynamics reflected by neighbor discovery
// If a link is inserted, issue a linkAdd event locally.
r01 eLinkAdd(@Src, Next, Cost):-
    link(@Src, Next, Cost),
    Src != Next.
// Check if the link is new by join and count.
r02 eLinkCount(@Src, Next, Cost, a_COUNT<*>):-
    eLinkAdd(@Src, Next, Cost),
    tLink(@Src, Next, Cost_2, SeqNum).
r03 tLink(@Src, Next, Cost, SeqNum):- // Insert if new link.
    eLinkCount(@Src, Next, Cost, Count),
    Count == 0,
    SeqNum := 0.
r04 tLink(@Src, Next, Cost, SeqNum):- // Refresh
    eLinkAdd(@Src, Next, Cost),
    tLink(@Src, Next, Cost_2, SeqNum).
//////////////////////////////////////////// Periodic update of LSUs (locally)
// Periodically send the links as LSUs locally.
r11A eLSU(@Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 120, 0, 60, 1),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 2 .
//Periodically increment the sequence numbers of all links.
r11B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 120, 0, 59),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
// similar afterwards... 
r12A eLSU(@Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 240, 0, 120, 1),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 4 .
r12B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 240, 0, 119),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
r13A eLSU(@Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 480, 0, 240, 1),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 8 .
r13B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 480, 0, 239),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
r14A eLSU(@Src, Src, Next, Cost, SeqNum, TTL):-
     periodic(@Src, E, 960, 0, 480, 1),
     tLink(@Src, Next, Cost, SeqNum),
     TTL := 16 .
r14B tLink(@Src, Next, Cost, SeqNumU):-
     periodic(@Src, E, 960, 0, 479),
     tLink(@Src, Next, Cost, SeqNum),
     SeqNumU := SeqNum + 1.
r15 eLSU(@Src, Src, Next, Cost, SeqNum, TTL):- // Triggered eLSU update
     tLink(@Src, Next, Cost, SeqNum),
     SeqNum == 0,
     Now := f_now (),
     Period := 60,
     TTL := f_hslsTtl (Now, Period). // Mapping TTL based on current time interval
/////////////////////////////////////////////////////// LSU insertion or update
// Replace broadcast address with current address
r21 eLSULoc(@Local, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSU(@X, LastHop, Dst, Cost, SeqNum, TTL).
// Check if LSU is new by join and count.
r22 eLSUCount(@Src, LastHop, Dst, Cost, SeqNum, TTL, a_COUNT<*>):-
    eLSULoc(@Src, LastHop, Dst, Cost, SeqNum, TTL),
    tLSU(@Src, LastHop, Dst, Cost_2, SeqNum_2, TTL_2).
// Insert the LSU if new.
r23 tLSU(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSUCount(@Src, LastHop, Dst, Cost, SeqNum, TTL, Count),
    Count == 0,
    Cost < 999999 .
// If not new LSU but greater sequence number then update sequence number.
r24 tLSU(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    eLSULoc(@Src, LastHop, Dst, Cost, SeqNum, TTL),
    tLSU(@Src, LastHop, Dst, Cost_2, SeqNum_2, TTL_2),
    SeqNum > SeqNum_2,
    Cost < 999999 .
///////////////////////////////////////////////////////// Broadcast LSU updates
// If LSU is updated then issue an LSUChange event.
r31 eLSUChange(@Src, LastHop, Dst, Cost, SeqNum, TTL):-
    tLSU(@Src, LastHop, Dst, Cost, SeqNum, TTL).
// If LSU changes then broadcast to all neighbors.
r32 eLSU(@Broadcast, LastHop, Dst, Cost, SeqNum, TTLU):-
    eLSUChange(@Src, LastHop, Dst, Cost, SeqNum, TTL),
    TTL > 0,
    TTLU := TTL - 1.
