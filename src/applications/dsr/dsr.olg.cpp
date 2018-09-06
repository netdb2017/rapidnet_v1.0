

/* Ad-hoc DSR routing protocol */



materialize(tPath, infinity, infinity, keys(1, 2, 4:list)).
//tPath(@Src, Dst, Cost, P_vec, Qid), tPath is all "multi-hop"(2 or more)
//routing Path

materialize(tBestPath, infinity, infinity, keys(1, 2)).
//tBestPath(@Src, Dst, Cost, P_vec, Qid), tBestPath is the shortest "multi-hop"
//(2 or more) routing Path

materialize(tPathDst, infinity, infinity, keys(1, 2, 3, 5:list)).
//tPathDst(@Local, Src, Dst, Cost, P_vec, Qid),

materialize(tBestPathDst, infinity, infinity, keys(1, 2, 3)).
//tBestPathDst(@Local, Src, Dst, Cost, P_vec, Qid),

////route discovery(1): sending path discovery route to destination by flooding

// broadcast ePathDst to neighbors when receiving a query
r11 ePathDst(@Broadcast, Src, Dst, Cost, P_vec, Qid, Pre) :-
     eQuery(@Src, Dst, Qid), //  Inject tuple/eQuery here
     Cost := 0,
     P_vec := f_append(Src),
     Pre := Src.

r12 ePathDstLoc(@Local, Src, Dst, Cost, P_vec, Qid, Pre):-
     ePathDst(@X, Src, Dst, Cost, P_vec, Qid, Pre).

r13 tPathDst(@Local, Src, Dst, CostUpdate, P_vecUpdate, Qid):-
     ePathDstLoc(@Local, Src, Dst, Cost, P_vec, Qid, Pre),
     f_member(P_vec, Local) == 0, // reject ePathDst which is broadcast back
     Cost_2 := 1,
     CostUpdate := Cost + Cost_2,
     P_vec_2 := f_append(Local),
     P_vecUpdate := f_concat(P_vec, P_vec_2).

// compute the best tPathDst
r14 tBestPathDst(@Local, Src, Dst, a_MIN<Cost>, P_vec, Qid) :-
     tPathDst(@Local, Src, Dst, Cost, P_vec, Qid).

// trigger a event when a best one is found
r15 eBestPathDst(@Local, Src, Dst, Cost, P_vec, Qid) :-
     tBestPathDst(@Local, Src, Dst, Cost, P_vec, Qid).

// broadcast only the best one
r16 ePathDst(@Broadcast, Src, Dst, Cost, P_vec, Qid, Pre) :-
     eBestPathDst(@Local, Src, Dst, Cost, P_vec, Qid),
     Local != Dst, // not reach the destination yet
     f_size(P_vec) < 16,
     Pre := Local.

//////route discovery(2): route found, send back route along the path to source

// reaching the destination, sending back ePathSrc
r21 ePathSrc(@Pre, P_track_vec, P_vec, Cost, Qid) :-
     eBestPathDst(@Local, Src, Dst, Cost, P_vec, Qid),
     Local == Dst,
     Temp_vec := P_vec,
     P_track_vec := f_removeLast(Temp_vec),
     Pre := f_last(P_track_vec).

///during the sending back process and intermediate nodes, keep on sending back
r22 ePathSrc(@Pre, P_new_track_vec, P_vec, Cost, Qid) :-
    ePathSrc(@Mid, P_track_vec, P_vec, Cost, Qid),
    Last := f_last(P_track_vec), // redundancy, just to make sure
    Mid == Last,
    f_size(P_track_vec) >= 2, // not reaching the Dst
    Temp_vec := P_track_vec,
    P_new_track_vec := f_removeLast(Temp_vec), // keep sending back
    Pre := f_last(P_new_track_vec).

// finally reaching the Src
r23 tPath(@Src, Dst, Cost, P_vec, Qid) :-
    ePathSrc(@Mid, P_track_vec, P_vec, Cost, Qid),
    Last := f_last(P_track_vec),
    Mid == Last,
    f_size(P_track_vec) == 1,
    Src := Mid,
    Dst := f_last(P_vec).

//////////////////////////////////Compute the best routing path from Src to Dst

r31 tBestPath(@Src, Dst, a_MIN<Cost>, P_vec, Qid):-
    tPath(@Src, Dst, Cost, P_vec, Qid).

////////////////////////////////////////////////// 
