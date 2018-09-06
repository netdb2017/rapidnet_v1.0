/*!
\page rapidnet-generic-script How to use the generic script?
\anchor rapidnet-generic-script

<b>Question:</b> How to use the generic script?

<b>Answer:</b> You will frequently find your self wanting to run your
protocols under different experimental conditions. For example, by
varying the number of nodes, wired vs wireless network, different
mobility models in wireless networks and so on. It is hard to write
one C++/Python script for each one of them individually in the examples
folder. To make things easier, we have written a generic script that
takes these variables are parameters. There is one generic script for
simulation: examples/rapidnet-app-simulator.cc and one script for
emulation: examples/rapidnet-app-emulator.cc. In this page, we are
only going to talk about the simulation script for now.

Run the command <b>./waf --run "rapidnet-app-simulator --PrintHelp"</b>
to see this help:

\code
--PrintHelp: Print this help message.
--PrintGroups: Print the list of groups.
--PrintTypeIds: Print all TypeIds.
--PrintGroup=[group]: Print all TypeIds of group.
--PrintAttributes=[typeid]: Print all attributes of typeid.
--PrintGlobals: Print the list of globals.
User Arguments:
    --duration: Duration of simulation in seconds (double)
    --nodes: Number of nodes (integer)
    --phy: The physical layer (wifi/csma)
    --stream: The print stream (cout/clog)
    --print-period: Period for printing, 0 if only once
    --print-reln: Names of relations to be printed (comma separated list of strings)
    --baseIp: Base IP address
    --app: RapidNet Application
    --log-discovery: Set discovery application logging (0=Off/1=On)
    --log-app: Set application logging (0=Off/1=On)
    --pcap: Set pcap tracing (0=Off/1=On)
    --xpos: X-axis positions of the nodes (comma separated list of double values)
    --ypos: Y-axis positions of the nodes (comma separated list of double values)
    --mm: Mobility model
    --xbound: X-axis bound for nodes to move
    --ybound: Y-axis bound for nodes to move
    --speed_low: Mobility model speed attribute lower bound
    --speed_up: Mobility model speed attribute upper bound
    --distance: Mobility model distance attribute
    --queryNum: Number of DSR/Epidemic queries
    --maxJitter: Maximum value of jitter (in milliseconds)
    --dir: Directory where the simulation results are dumped
\endcode

As an example you can try this command:
<b>./waf --run "rapidnet-app-simulator --app=Discovery --nodes=10 --duration=100 --phy=wifi --xbound=300 --ybound=300 --mm=ns3::RandomWaypointMobilityModel --speed_low=0.5 --speed_up=1.0 --dir=discovery --stream=clog" 2> discovery/output.log</b>

This creates a folder with name 'discovery' and dumps all the outputs in it. This includes the log file
output.log, a trace file decorator.log (for the RapidNet visualizer) and pcap trace files. You might as
well launch the visualizer with this trace file using this command from the visualizer folder root:
<b>./runner &lt;path-to-the-output.log-above&gt;</b>. If you notice, the arena and the nodes will show
well. But not the graphs. This is because the script only runs the experiment. We need to post-process
the results to generate the graphs and update the visualizer traces.

This is best done outside of C++, we use Python scripts for this. We wrap another generic python script on top of the
C++ generic script to do everything for us in one command. This script is: simulations/run_simulate.py that internally
invokes simulation/simulate.py that makes the call to the C++ script using a waf command like above. Just type
<b>./simulation/run_simulation.py</b> to see this usage string:

Usage: run_simulation <app> <nodes> <duration> [<speed_low> <speed_up>] [<xbound> <ybound>] [<mobility_model>] [<query/messageNum>] [<timestamp>]

So to run discovery using this script we can use the command: <b>./simulation/run_simulation.py Discovery 10 100 0.5 1.0 300 300 rwp</b>.
This generates a folder with name Discovery_<info>_<timestamp>, where info is some information about the selected parameters and timestamp
is the time at which you ran the command. Now try launching the visualizer with the path to the "events.log" file in this generated folder
to see the graphs as well. The events.log contains all events from decorator.log and additionally, events for the graphs. You will also find graphs
generated in postscript format are generated in the folder. There are scripts for generating various graphs in the rapidnet folder, for example
bandwidth, loss rate, link events per second and so on. All graphs may not be meaningful for all protocols. So which graphs are generated for which
protocol is determined by the simulations/simulate.py script. For example, in the case of link state or HSLS protocols, it plots bandwidth, loss rate,
route validity and route stretch.

To run your own application using the generic script, you should declare the application helper in the configuration file examples/rapidnet-applications.h.

Please note that these Python scripts may change in the future releases. You are free to use them as-is, make changes or write new ones as you
prefer.

*/
