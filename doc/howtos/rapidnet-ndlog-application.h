/*!
\page rapidnet-ndlog-application How to create a new RapidNet NDlog application?
\anchor rapidnet-ndlog-application

<b>Question:</b> How to create a new RapidNet NDlog application?

<b>Answer:</b> Let us try to clone the Pathvector application to see the process
of creating a new application.

- Start out with creating new folder in src/applications/myproto.
- Now we need to write a RapidNet application in NDlog. For the purposes of this
  example let's use the Pathvector application. So create a file
  src/applications/myproto/myproto.olg with this NDlog program:
\code
materialize(link,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(4:list)).
materialize(bestPath,infinity,infinity,keys(2)).

r1 path(@X,Y,C,P) :- link(@X,Y,C),
          P1:=f_append(X),
          P2:=f_append(Y),
          P:=f_concat(P1,P2).

r2 path(@X,Y,C,P) :- link(@X,Z,C1),
         bestPath(@Z,Y,C2,P2),
         C:=C1+C2,
         f_member(P2,X)==0,
         P1:=f_append(X),
         P:=f_concat(P1,P2).

r3 bestPath(@X,Y,a_MIN<C>, P) :- path(@X,Y,C,P).
\endcode

- Let's use the RapidNet compiler to generate the C++ code. Run
  <b>./rapidnet/compiler/compile src/applications/myproto</b>. This generates the
  required files.
- Now let's build it. To do that you need to tell ns-3 that we have a new
  application folder. To do this, add an entry 'applications/myproto' in
  src/wscript under <i>all_modules</i>.
- Great. So now let's run <b>./waf</b> to build it.
- Now we are ready to run this application by writing a script. So let's create
  a new file examples/myproto-test.cc and add this (copied and modified from
  examples/pathvector-test.cc):
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
  insertlink (1, 2, 7);
  insertlink (2, 3, 4)
  insertlink (3, 4, 2);
  insertlink (4, 5, 6);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Myproto", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<MyprotoHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (10.0));

  schedule (0.0, UpdateLinks1);
  schedule (10.0, Print);

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
- See the generated output make sure the PATH and BESTPATH tables printed in the end
  have the right entries.
*/
