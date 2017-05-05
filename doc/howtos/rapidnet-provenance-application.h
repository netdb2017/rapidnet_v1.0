/*!
\page rapidnet-provenance-application How to create a new RapidNet NDlog application with provenance capability?
\anchor rapidnet-provenance-application

<b>Question:</b> How to create a new RapidNet NDlog application with provenance capability?

<b>Answer:</b> RapidNet allows you to write NDlog applications with provenance support with
little extra effort. Essentially, the exact same NDlog application needs to be compiled with
the --provenance (or -p) flag when compiling using the rapidnet compiler: rapidnet/compiler/compile.
This translates the input NDlog rules into provenance enabled rules (in NDlog itself) and then
compiles them into C++. Note that currently provenance cannot be enabled for SeNDlog applications.

Let us try to clone the mincost-prov application to see the process
of creating a new provenance enabled application.

- Start out with creating new folder in src/applications/myproto.
- Create a file src/applications/myproto/myproto.olg with this NDlog program:
\code
materialize(link,infinity,infinity,keys(1,2)).
materialize(path,infinity,infinity,keys(1,2,3:int32)).
materialize(bestPath,infinity,infinity,keys(1,2)).

r1 path(@X,Y,C) :- link(@X,Y,C).
r2 path(@Z,Y,C) :- link(@X,Z,C1), bestPath(@X,Y,C2), C:=C1+C2, Z!=Y.
r3 bestPath(@X,Y,a_MIN<C>) :- path(@X,Y,C).
\endcode

- Let's use the RapidNet compiler to generate the C++ code. Run
  <b>./rapidnet/compiler/compile src/applications/myproto --provenance</b>. This
  generates the required files. Take a look at the file src/applications/myproto/myproto.olg.prov
  for the provenance enabled rules.
- Now let's build it. To do that you need to tell ns-3 that we have a new
  application folder. To do this, add an entry 'applications/myproto' in
  src/wscript under <i>all_modules</i>.
- Great. So now let's run <b>./waf</b> to build it.
- Now we are ready to run this application by writing a script. So let's create
  a new file examples/myproto-test.cc and add this (copied and modified from
  examples/mincost-prov-test.cc):
\code
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/myproto-module.h"
#include <time.h>

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

void
UpdateLinks1 ()
{
  insertlink (1, 2, 3);
  insertlink (2, 3, 2);
  insertlink (1, 3, 5);
}

void
Print ()
{
  PrintRelation (apps, Myproto::BESTPATH);
  PrintRelation (apps, Myproto::PROV);
  PrintRelation (apps, Myproto::RULEEXEC);
}


int
main (int argc, char *argv[])
{
  LogComponentEnable("Myproto", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (3, Create<MyprotoHelper> ());
  SetMaxJitter (apps, 0.001);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (5.0));

  schedule (2.0, UpdateLinks1);
  schedule (5.0, Print);

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
- See the generated output make sure the BESTPATH, PROV and RULEEXEC tables
  printed in the end have the right entries.
- Brief note on Provenance: When provenance is enabled, the input NDlog rules are
  translated through a provenance-rewrite process to generate a new set of rules
  in NDlog that are stored with a .olg.prov extension in the application folder.
  A few more rules, called query rules, are added to this by copying them from the
  source file ./rapidnet/compiler/provenance-query.olg. This is done automatically
  by the compiler. These rules are used to support the querying of the provenance
  data. When provenance enabled applications are run, the provenance information
  of the tuples is stored in the "prov" table and the rule execution records are
  stored in the "ruleExec" table.

*/

