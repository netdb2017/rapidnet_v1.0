

// Constants
// Tables
materialize(node, infinity, 1, keys(1)).
materialize(landmark, infinity, 1, keys(1)).
materialize(finger, infinity, 160, keys(2:int32)).
materialize(uniqueFinger, 180, 160, keys(2)).
materialize(bestSucc, infinity, 1, keys(1)).
materialize(succEvictPolicy, infinity, 2, keys(3:cid)).
materialize(pred, infinity, 1, keys(1)).
materialize(join, 30, 5, keys(2:int32)).
materialize(pendingPing, infinity, infinity, keys(3:str)).
materialize(fFix, 180, 160, keys(2:int32)).
materialize(nextFingerFix, 180, 1, keys(1)).
node_init node(@NI, ID) :- landmark(@NI, L), periodic(@NI, E, 0, 1),
                           ID := f_sha1(NI).
// Lookups
l1 lookupResults(@R,K,S,SI,E) :- node(@NI,N), lookup(@NI,K,R,E),
                                 bestSucc(@NI,S,SI), K in (N,S].
l2 bestLookupDist(@NI,K,R,E,a_MIN<D>) :- node(@NI,N),
      lookup(@NI,K,R,E),
      finger(@NI,I,B,BI),
      D := K - B - 1,
      B in (N,K).
l3 forwardLookup(@NI, a_MIN<BI>,K,R,E) :- node(@NI,N),
          bestLookupDist(@NI,K,R,E,D),
          finger(@NI,I,B,BI),
          D == K - B - 1,
          B in (N,K).
l4 lookup(@BI, K, R, E) :- forwardLookup(@NI, BI, K, R, E),
                      f_typeOf(BI) != "null".
// Neighbor Selection
n0 newSuccEvent(@NI) :- succEvictPolicy(@NI,D, S,SI).
n2 newSuccEvent(@NI) :- deleteSucc(@NI,D,S,SI).
n1 bestSuccDist(@NI, a_MIN<D>) :- newSuccEvent(@NI),
 succEvictPolicy(@NI, D, S, SI).
n3 bestSucc(@NI, S, SI) :- succEvictPolicy(@NI, D, S, SI),
 bestSuccDist(@NI,D).
n4 finger(@NI,0,S,SI) :- bestSucc(@NI,S,SI).
// Finger fixing
f1 fFix(@NI,E,I) :- periodic(@NI,E,50, 0, 10, 30),
 nextFingerFix(@NI,I).
f2 fFixEvent(@NI,E,I) :- fFix(@NI,E,I).
f3 lookup(@NI,K,NI,E) :- fFixEvent(@NI,E,I), node(@NI,N), K:=0x1I << I +
 N.
f4 eagerFinger(@NI,I,B,BI) :- fFix(@NI,E,I),
 lookupResults(@NI,K,B,BI,E).
f5 finger(@NI,I,B,BI) :- eagerFinger(@NI,I,B,BI).
f6 eagerFinger(@NI,I,B,BI) :- node(@NI,N), eagerFinger(@NI,I1,B,BI),
 I:=I1 + 1, K:=0x1I << I + N, K in (N,B), BI != NI.
f7 delete fFix(@NI,E,I1) :- eagerFinger(@NI,I,B,BI), fFix(@NI,E,I1), I >
 0, I1 == I - 1.
f8 nextFingerFix(@NI,0) :- eagerFinger(@NI,I,B,BI),
      ((I == 159) || (BI == NI)).
f9 nextFingerFix(@NI,I) :- node(@NI,N),
      eagerFinger(@NI,I1,B,BI),
      I:=I1 + 1,
      K:=0x1I << I + N,
      K in (B,N),
      NI != BI.
watch(nextFingerFix).
watch(finger).
f10 uniqueFinger(@NI,BI) :- finger(@NI,I,B,BI).
// Churn Handling
c1 joinEvent(@NI,E) :- periodic(@NI, E, 5, 3).
c2 join(@NI,E) :- joinEvent(@NI,E).
// If I am not the landmark
c3 joinReq(@LI,N,NI,E) :- joinEvent(@NI, E),
 node(@NI, N),
 landmark(@NI, LI),
 LI != "NIL".
// If I am the landmark, set myself as succ
c4 succEvictPolicy(@NI, D, N, NI) :- landmark(@NI, LI),
 joinEvent(@NI, E),
 node(@NI, N), D:= N - N - 1,
 LI == "NIL".
c5 lookup(@LI,N,NI,E) :- joinReq(@LI,N,NI,E).
c6 succEvictPolicy(@NI,D, S,SI) :- join(@NI, E), lookupResults(@NI,K,S,SI,E), node(@NI, N), D:= S - N - 1.
// Stabilization
sb0 stabilizeEvent(@NI) :- periodic(@NI, E, 50, 0, INITIAL_DELAY, 30).
watchmod(stabilizeEvent, "s").
sb1A succEvictPolicyEvent(@NI,P,PI) :- stabilizeEvent(@NI), node(@NI,N),
 bestSucc(@NI,S,SI), pred(@SI,P,PI), PI != "NIL", P in (N,S).
sb1B succEvictPolicy(@NI,D, P,PI) :- succEvictPolicyEvent(@NI,P,PI), node(@NI,N),
                                     D:= P - N - 1.
sb2 succEvictPolicy(@NI, D2, S1, SI1) :- stabilizeEvent(@NI), node(@NI, N),
 succEvictPolicy(@NI, D, S, SI),
 succEvictPolicy(@SI, D1, S1, SI1), SI1!=NI, D2:=S1 - N - 1.
sb3 pred(@SI, N, NI) :- stabilizeEvent(@NI), node(@NI, N),
 succEvictPolicy(@NI, D, S, SI),
 pred(@SI, P, PI),
 node(@SI, N1),
 ((PI == "NIL") || (N in (P, N1))) && (NI != SI).
// Ping Nodes
pp1 pendingPing(@NI, SI, E1, T) :- periodic(@NI, E, 10),
 succEvictPolicy(@NI, D, S, SI), E1 := f_rand(), SI != NI, T := f_now().
pp2 pendingPing(@NI, PI, E1, T) :- periodic(@NI, E, 10),
 pred(@NI, P, PI), E1 := f_rand(), PI != "NIL", PI != NI, T := f_now().
pp3 pendingPing(@NI, FI, E1, T) :- periodic(@NI, E, 10),
 uniqueFinger(@NI, FI), E1 := f_rand(), FI != NI, T := f_now().
pp4 pingResp(@RI, NI, E) :- pingReq(@NI, RI, E).
pp5 pingReq(@PI, NI, E) :- periodic(@NI, E1, 3),
 pendingPing(@NI, PI, E, T).
// Once a ping response arrives, remove all pending pings for the same
// destination
pp6 delete pendingPing(@NI, SI, E1, T) :- pingResp(@NI, SI, E),
 pendingPing(@NI, SI, E1, T).
// Failure Detection
cm1 nodeFailure(@NI,PI,E1,D) :- periodic(@NI, E, 1),
 pendingPing(@NI,PI,E1,T),
 T1 := f_now(),
 D := T1 - T,
 D > 7.
cm1a delete pendingPing(@NI,PI,E,T) :- nodeFailure(@NI,PI,E,D),
 pendingPing(@NI,PI,E,T).
cm2a deleteSucc(@NI,D, S,SI) :- succEvictPolicy(@NI, D, S,SI), nodeFailure(@NI,SI,E,DD).
cm2b delete succEvictPolicy(@NI,D, S,SI) :- deleteSucc(@NI,D, S,SI).
cm3 pred(@NI,"NIL","NIL") :- pred(@NI,P,PI), nodeFailure(@NI,PI,E,D).
cm4 delete finger(@NI,I,B,BI) :- finger(@NI,I,B,BI),
 nodeFailure(@NI,BI,E,D).
cm6 delete uniqueFinger(@NI,FI) :- uniqueFinger(@NI,FI),
 nodeFailure(@NI,FI,E,D).
