#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/acquaintance-module.h"
#include "ns3/acquaintance-query-module.h"
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define KnowObs \
"./data/acquaintance/knows_obs.txt"

#define KnowObsBack \
"./data/acquaintance/knows_obs.bak"

#define LikeObs \
"./data/acquaintance/likes_obs_back.txt"

#define LikeObsBack \
"./data/acquaintance/likes_obs_back.bak"

#define LiveObs \
"./data/acquaintance/lived_obs.txt"

#define LiveObsBack \
"./data/acquaintance/live_obs.bak"

#define KnowTar \
"./data/acquaintance/knows_target.txt"

#define KnowTru \
"./data/acquaintance/knows_truth.txt"


#define live(local, person, city) \
tuple (Acquaintance::LIVEEVENT, \
attr ("liveEvent_attr1", Ipv4Value, local), \
attr ("liveEvent_attr2", Int32Value, person), \
attr ("liveEvent_attr3", Int32Value, city))

#define like(local, person, hobby) \
tuple (Acquaintance::LIKEEVENT, \
attr ("likeEvent_attr1", Ipv4Value, local), \
attr ("likeEvent_attr2", Int32Value, person), \
attr ("likeEvent_attr3", Int32Value, hobby))

#define know(local, person1, person2, state) \
tuple (Acquaintance::KNOWEVENT, \
attr ("knowEvent_attr1", Ipv4Value, local), \
attr ("knowEvent_attr2", Int32Value, person1), \
attr ("knowEvent_attr3", Int32Value, person2), \
attr ("knowEvent_attr4", Int32Value, state))

#define relation(local, person1, person2) \
tuple (Acquaintance::RELATION, \
attr ("relation_attr1", Ipv4Value, local), \
attr ("relation_attr2", Int32Value, person1), \
attr ("relation_attr3", Int32Value, person2))


#define insertknow(local, person1, person2, state) \
app(local)->Insert(know(addr(local), person1, person2, state)); \
app(local)->Insert(know(addr(local), person2, person1, state));

#define insertlike(local, person, hobby) \
app(local)->Insert(like(addr(local), person, hobby));

#define insertlive(local, person, city) \
app(local)->Insert(live(addr(local), person, city));

#define insertrelation(local, person1, person2) \
app(local)->Insert(relation(addr(local), person1, person2));

//define the tuple you would like to query and how to insert it
#define tupleQuery(loc, name, attr1, attr2, attr3, attr4)         \
tuple (AcquaintanceQuery::TUPLE, \
attr ("tuple_attr1", Ipv4Value, loc), \
attr ("tuple_attr2", StrValue, name), \
attr ("tuple_attr3", Ipv4Value, attr1), \
attr ("tuple_attr4", Int32Value, attr2), \
attr ("tuple_attr5", Int32Value, attr3), \
attr ("tuple_attr6", Int32Value, attr4))

#define inserttuple(loc, name, attr1, attr2, attr3, attr4)     \
queryNode->Insert (tupleQuery(queryNode->GetAddress(), name, addr(attr1), attr2, attr3, attr4));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::acquaintance;
using namespace ns3::rapidnet::acquaintancequery;

map<string, int> people;
map<string, int> cities;
map<string, int> hobbies;

ApplicationContainer apps;
ApplicationContainer queryapps;

void 
initApps()
{
	NodeContainer mainAppNodes;
	mainAppNodes.Create (3);

	NodeContainer queryAppNodes;
	queryAppNodes.Create (1);

  	NodeContainer csmaNodes;
  	csmaNodes.Add(mainAppNodes);
  	csmaNodes.Add(queryAppNodes);

  	CsmaHelper csma;

  	NetDeviceContainer csmaDevices;
  	csmaDevices = csma.Install (csmaNodes);

  	InternetStackHelper stack;
  	stack.Install (csmaNodes);

  	Ipv4AddressHelper address;
  	Ipv4Address base = "10.1.1.0";

  	address.SetBase (base, "255.255.255.0");
  	address.Assign (csmaDevices);

  	apps.Add(Create<AcquaintanceHelper>()->Install(mainAppNodes));
  	queryapps.Add(Create<AcquaintanceQueryHelper>()->Install(queryAppNodes));

  	SetMaxJitter (apps, 0.001);
  	SetMaxJitter (queryapps, 0.001);
}

vector<string> readFile(string filename){
	vector<string> re;
	string s;
	ifstream f(filename.c_str(), ios_base::in);

	while(!f.eof()){
		getline(f, s);
		re.push_back(s);
	}
	f.close();
	return re;
}


void parse(vector<string> know_obs,
		   vector<string> live_obs,
		   vector<string> like_obs)
{

	// first parse know_obs
	cout<<"testing"<<endl;
	for(int i=0; i<know_obs.size()-1; i++){
		string know = know_obs[i];
		if(know.size()==0) continue;
		int l = 0;
		while(know.at(l)!=' ') l++;
		string person1 = know.substr(0, l);
		l++;
		string person2 = know.substr(l, know.length()-l);
		if(people.count(person1)==0)
			people.insert(pair<string, int>(person1, people.size()));
		if(people.count(person2)==0)
			people.insert(pair<string, int>(person2, people.size()));
		cout << people[person1] << ' ' << people[person2] << endl;
		insertknow(1, people[person1], people[person2], 1);
	}	
	cout << endl;

	cout << "live size: " << live_obs.size() << endl;

	// parse live_obs
	for(int i=0; i<live_obs.size()-1; i++){
		string live = live_obs[i];
		if(live.size()==0) continue;
		int l = 0;
		while(live.at(l)!=' ') l++;
		string person = live.substr(0, l);
		l++;
		string city = live.substr(l, live.length()-l);
		if(people.count(person)==0)
			people.insert(pair<string, int>(person, people.size()));
		if(cities.count(city)==0)
			cities.insert(pair<string, int>(city, cities.size()));
		cout << people[person] << ' ' << cities[city] << endl;
		insertlive(1, people[person], cities[city]);
	}
	cout << endl;

	cout << "like size: " << like_obs.size() << endl;
	for(int i=0; i<like_obs.size(); i++){
		cout << like_obs[i] << endl;
	}

	//parse like_obs
	for(int i=0; i<like_obs.size()-1; i++){
		cout << i << endl;
		string like = like_obs[i];
		if(like.size()==0) continue;
		int l = 0;
		while(like.at(l)!=' ') l++;
		string person = like.substr(0, l);
		l++;
		string hobby = like.substr(l, like.length()-l);
		if(people.count(person)==0)
			people.insert(pair<string, int>(person, people.size()));
		if(hobbies.count(hobby)==0)
			hobbies.insert(pair<string, int>(hobby, hobbies.size()));
		cout << people[person] << ' ' << hobbies[hobby] << endl;
		insertlike(1, people[person], hobbies[hobby]);
	}
}

void
TupleToQuery ()
{
  Ptr<RapidNetApplicationBase> queryNode = queryapps.Get(0)->GetObject<RapidNetApplicationBase>();
  inserttuple(1, "know", 1, 9, 8, 1); 
  
   
}

void Print(){
	PrintRelation(apps, Acquaintance::KNOW);
	//PrintRelation(apps, Acquaintance::LIVE);
	//PrintRelation(apps, Acquaintance::LIKE);

	//PrintRelation(apps, Acquaintance::PROV);

	PrintRelation (queryapps, AcquaintanceQuery::TUPLE);
  	PrintRelation (queryapps, AcquaintanceQuery::RECORDS); //modify: add col tuple's vid (hash)
}


void train(){
	vector<string> know_obs = readFile(KnowObsBack);
	vector<string> live_obs = readFile(LiveObs);
	vector<string> like_obs = readFile(LikeObs);
	parse(know_obs, live_obs, like_obs);
}



int main(int argc, char *argv[]){

	/*
	LogComponentEnable("Acquaintance", LOG_LEVEL_INFO);
  	LogComponentEnable("AcquaintanceQuery", LOG_LEVEL_INFO);
  	LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
    */

  	initApps();

	apps.Start (Seconds (0.0));
	apps.Stop (Seconds (10.0));
	queryapps.Start (Seconds (0.0));
  	queryapps.Stop (Seconds (10.0));

	schedule (1.0, TupleToQuery);	
	schedule (2.0, train);
	schedule (5.0, Print);

	Simulator::Run ();
	Simulator::Destroy ();

	return 0;

}
