/*!
\page rapidnet-sendlog-application How to create a new RapidNet SeNDlog application?
\anchor rapidnet-sendlog-application

<b>Question:</b> How to create a new RapidNet SeNDlog application?

<b>Answer:</b> SeNDlog stands for SEcure NDlog. It includes additional constructs
for encryption and authentication for writing secure network protocols.

The procedure for creating a SeNDlog application is basically the same as that
for an NDlog application. The RapidNet compiler can detect the difference
between NDlog and SeNDlog programs by looking at the syntax in the .olg
file and generate appropriate code. SeNDlog applications have a context
declaration and they do not use the location specifier "@" in the predicates.

Let us try to clone the secure-pathvector application to see the process
of creating a new SeNDlog application.

- Start out with creating new folder in src/applications/myproto.
- Create a file src/applications/myproto/myproto.olg with this SeNDlog program:
\code
materialize(link,infinity,infinity,keys(1,2)).
materialize(linkD,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(4:list)).
materialize(bestPath,infinity,infinity,keys(2)).

At S:

r1 path(S,D,C,P) :- link(S,D,C),
                  P1:=f_append(S),
                  P2:=f_append(D),
                  P:=f_concat(P1,P2). 

r2 encrypts S says linkD(S,D,C)@D :- link(S,D,C).

r3 encrypts S says path(D,Z,C,P)@D :- encrypts D says linkD(D,S,C1),
                                   bestPath(S,Z,C2,P2),
                                   C:=C1+C2,
                                   f_member(P2,D)==0,	        
                                   P1:=f_append(D),
                                   P:=f_concat(P1,P2).

r4 bestPath(S,D,a_MIN<C>,P) :- encrypts W says path(S,D,C,P).
\endcode

- Now use the RapidNet compiler to generate the C++ code. Run
  <b>./rapidnet/compiler/compile src/applications/myproto</b>. This generates the
  required files.
- Now let's build it. To do that you need to tell ns-3 that we have a new
  application folder. To do this, add an entry 'applications/myproto' in
  src/wscript under <i>all_modules</i>.
- Great. So now let's run <b>./waf</b> to build it.
- Now we are ready to run this application by writing a script. So let's create
  a new file examples/myproto-test.cc and add this (copied and modified from
  examples/secure-pathvector-test.cc):
\code
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/myproto-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define link(src, next, cost) \
  tuple (Myproto::LINK, \
    attr ("link_attr1", Ipv4Value, src), \
    attr ("link_attr2", Ipv4Value, next), \
         attr ("link_attr3", Int32Value, cost))

#define insertlink(from, to, cost) \
  app(from)->Insert (link (addr (from), addr (to), cost)); \
  app(to)->Insert (link (addr (to), addr (from), cost));

#define deletelink(from, to, cost) \
  app(from)->Delete (link (addr (from), addr (to), cost)); \
  app(to)->Delete (link (addr (to), addr (from), cost));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::myproto;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, Myproto::PATH);
  PrintRelation (apps, Myproto::BESTPATH);
}


// Create a chain topology
void
UpdateLinks1 ()
{
  insertlink (1, 2, 2);
  insertlink (1, 3, 3);
  insertlink (1, 4, 5);
  insertlink (2, 3, 6);
  insertlink (3, 4, 2);
  insertlink (4, 5, 10);
}

void
UpdateLinks2 ()
{
  deletelink (1, 2, 2);
}

void
UpdateLinks3 ()
{
  insertlink (1, 2, 3);
  insertlink (2, 5, 1);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Myproto", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<MyprotoHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (40.0));

  Simulator::Schedule (Seconds(0.0001), &UpdateLinks1);
  Simulator::Schedule (Seconds(10.0000), &Print);
  Simulator::Schedule (Seconds(10.0001), &UpdateLinks2);
  Simulator::Schedule (Seconds(20.0000), &Print);
  Simulator::Schedule (Seconds(20.0001), &UpdateLinks3);
  Simulator::Schedule (Seconds(40.0000), &Print);

  SetMaxJitter (apps, 30);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
\endcode

- We should now edit examples/wscript and add this entry to it:
\code
    obj = bld.create_ns3_program('myproto-test')
    obj.source = 'myproto-test.cc'
\endcode

- Next step is to build everything using <b>./waf</b>
- And you are now ready run your new protocol using <b>./waf --run myproto-test</b>
- See the generated output make sure the PATH and BESTPATH tables printed have the
  right entries.

- A brief note on SeNDlog: The syntax of SeNDlog is different from NDlog in the
following ways:
  -# The "At Context:" declaration. This tells the rule engine the position of the
location specifier. All terms in the rule body must be local to this context.
  -# The outgoing tuples in the rule head is tagged with @Destination at the end of
the predicate.
  -# "says" keyword used to specify the pricipal that initiated the information, or the
<i>sayer</i>. This is used for authentication at the receiver end. The sayer signs the
tuple of information which the receiving principals can verify.
  -# "encrypts" keyword that is used to encrypt a tuple before sending it over the
network.

For more details on SeNDlog, please refer to the paper: http://www.netdb.cis.upenn.edu/papers/decsec_icde09.pdf

*/
