

/* Epidemic protocol for Ad-hoc network */


// after this value, message will be timeout, i.e. deleted table tMessage
///////////////////////////////////////////////////////////////////////////////

materialize(tLink, infinity, infinity, keys(1, 2)).
//tLink(@Src, Next, Cost)

materialize(tSummaryVec, infinity, 1, keys(1)). // epidemic summary vector
// tSummaryVec(@Src, SummaryVec)

materialize(tMessage, infinity, infinity, keys(1, 2, 3, 4:str)).
//messages for sending out and receive
//tMessage(@Local, Src, Dst, ID, T_begin), will be timed out by MESSAGE_TIMEOUT

//////////////Neighbor discovery and observation, not part of epidemic protocol

r01 tLink(@Src, Next, Cost) :-
    eLinkDiscoveryAdd(@Src, Next, Cost). // link actual ADD or REFRESH

r02 delete tLink(@Src, Next, Cost) :-
    eLinkDiscoveryDel(@Src, Next, Cost). // link actual DELETE

// only when there is actual ADD (not refresh) in tLink, then trigger a eLinkAdd
r03 eLinkAdd(@Src, Next, Cost) :-
    tLink(@Src, Next, Cost).

r04 eMessageBegin(@Src, Src, Dst, ID, T_begin):- // Observation: message begins
    eMessageInject(@Src, Dst, ID, T_begin).

//Observation: receive a new message, maybe triggered multiple times when this
//message in tMessage is timeout, and there are some intermidiate nodes holding
//this message
r05 eMessageEnd(@Local, Src, Dst, ID, T_begin):-
    tMessage(@Local, Src, Dst, ID, T_begin),
    Local == Dst.

////////////////////////////////// Epidemic summary vector and message exchange

r11 tSummaryVec(@Src, SummaryVec):- // initialize summary vector table
    periodic(@Src, E, 0, 1),
    SummaryVec := f_svcreate().

// currently bit vector contains "ID" as bit information
r12 eBitVectorRequest(@Next, Local, SummaryVec):-
    eLinkAdd(@Local, Next, Cost),
    tSummaryVec(@Local, SummaryVec).

r13 eBitVectorReply(@From, Local, SummaryVec_3):-
    eBitVectorRequest(@Local, From, SummaryVec_1),
    tSummaryVec(@Local, SummaryVec_2),
    SummaryVec_3 := f_svandnot(SummaryVec_1, SummaryVec_2).

// after receiving a reply for requesting message, send it this message
r14 eMessage(@From, Src, Dst, ID, T_begin):-
    eBitVectorReply(@Local, From, SummaryVec),
    tMessage(@Local, Src, Dst, ID, T_begin),
    Result := f_svin(SummaryVec, ID),
    Result == 1. // ID is in Summary Vector

//////////////////////////////// messages insert and update and normal flooding

r21 eMessageInject(@Src, Dst, ID, T_begin) :- // append ID and timestamp
    eMessageInjectOriginal(@Src, Dst),
    ID := f_rand(), // random number 32bit interger, as string type
    T_begin := f_now().

// once inject a message, update tMessage table
r22 tMessage(@Src, Src, Dst, ID, T_begin):-
    eMessageInject(@Src, Dst, ID, T_begin).

// once inject a message, update summary vector
r23 tSummaryVec(@Src, SummaryVecUpdate):-
    eMessageInject(@Src, Dst, ID, T_begin),
    tSummaryVec(@Src, SummaryVec),
    SummaryVecUpdate := f_svappend(SummaryVec, ID).

// if Local is not its destinationa, trigger a new message, 
// i.e. generate a event indicating new message (prepare to forward it)
// only message insert (not refresh) is considered to be message change
r24 eMessageNew(@Local, Src, Dst, ID, T_begin):-
    tMessage(@Local, Src, Dst, ID, T_begin),
    Local != Dst.

r25 eMessage(@Broadcast, Src, Dst, ID, T_begin):- // flooding messages 
    eMessageNew(@Local, Src, Dst, ID, T_begin). // broadcast

r26 eMessageLoc(@Local, Src, Dst, ID, T_begin):-
    eMessage(@X, Src, Dst, ID, T_begin).

// wheneven receive a message, put it in tMessage table
r27 tMessage(@Local, Src, Dst, ID, T_begin):-
    eMessageLoc(@Local, Src, Dst, ID, T_begin).

// receive a message sent from neighbor, put this in the summary 
r28 tSummaryVec(@Local, SummaryVecUpdate):-
    eMessageLoc(@Local, Src, Dst, ID, T_begin),
    tSummaryVec(@Local, SummaryVec),
    SummaryVecUpdate := f_svappend(SummaryVec, ID).

r29 eMessageDel(@Local, Src, Dst, ID, T_begin):- // message time out
    periodic(@Local, E, 1),
    tMessage(@Local, Src, Dst, ID, T_begin),
    Now := f_now(),
    T_exist := f_diffTime(Now, T_begin),
    T_exist > 120 .

// actually delete messages
r2A delete tMessage(@Local, Src, Dst, ID, T_begin):-
    eMessageDel(@Local, Src, Dst, ID, T_begin).

// update summary vector correspondingly when message expires
r2B tSummaryVec(@Local, SummaryVecUpdate):-
    eMessageDel(@Local, Src, Dst, ID, T_begin),
    tSummaryVec(@Local, SummaryVec),
    SummaryVecUpdate := f_svremove(SummaryVec, ID).

/////////////////////////////////////////////////////////////////
