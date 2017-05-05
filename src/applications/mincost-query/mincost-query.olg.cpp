

materialize(records,infinity,infinity,keys(1,2:cid,3:cid)).
materialize(tuple, infinity, infinity, keys(1,2:str,3,4)).

q1 provQuery(@X, QID, UID, ME):- periodic(@ME,E,4,2), tuple(@ME, Name, X, Y, C), UID:=f_sha1(Name+X+Y+C),
                                 Time := f_now(), QID:=f_sha1(""+UID+Time).
q2 records(@ME, QID, RID, Prov) :- pReturn(@ME, QID, RID, Prov).
