#!/usr/bin/python
import sys, os
sys.path.append ('./rapidnet/bandwidth')
from emu_plot_bandwidth import *

APPTABLE_PERIOD = 5

if len (sys.argv) < 7:
  print 'Usage: ./emulation/plot <app> <dir> <apptable-file> <node-count> <orbit|cluster> <duration> [blacklist]'
  sys.exit (0)

app = sys.argv[1]
dir = sys.argv[2]

if sys.argv[5] == 'cluster':
  from nodesmap_clust import nodes
else:
  from nodesmap_orbit import nodes

nodes_subset = {}

for i in range (1, int (sys.argv[4]) + 1):
  nodes_subset[i] = nodes[i];

blacklist = []
blacklist_string = ''
if len(sys.argv) >= 8:
  blacklist_string = sys.argv[7]
  listofnodes = sys.argv[7].split(',')
  for node in listofnodes:
    blacklist += [int (node)]

apptable_file = sys.argv[3] #'./dense/plotter/apptable_topo_04nodes_clust'
plot_stats (dir, app, nodes_subset, apptable_file, blacklist)

os.system ('./rapidnet/route-quality/emu_route_quality.py %s %d %s %s %s %s' % (dir, APPTABLE_PERIOD, sys.argv[6], sys.argv[4], sys.argv[5], blacklist_string))
os.system ('./rapidnet/bandwidth/emu_plot_losses.py %s %s %s %s %s %s'%(dir, sys.argv[4], sys.argv[6], apptable_file, app, sys.argv[5]))