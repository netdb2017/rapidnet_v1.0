#!/usr/bin/python
from simulate import simulate
from time import strftime, gmtime
import sys
from math import sqrt, ceil

join_period = 100

def run_simulation (nodes, duration):
  args = {}

  args ['app'] = 'Chord'
  args ['nodes'] = nodes
  args ['duration'] = duration

  args ['phy'] = 'csma'
  args ['mm'] = 'ns3::ConstantPositionMobilityModel' # Unused
  args ['mob-args'] = '' # Unused
  args ['queryNum'] = 0 # Unused
  args ['xpos'] = '' # Unused
  args ['ypos'] = '' # Unused

  args ['stream'] = 'cout'
  args ['print-period'] = join_period
  args ['log-discovery'] = 0
  args ['log-app'] = 0

  args['print-reln'] = 'bestSucc,pred,landmark,node,succ'

  simulate (args, False)

if len (sys.argv) < 3:
  print 'Usage: run_chors <num_nodes> <duration>'
  sys.exit (0)

nodes = int (sys.argv[1])
duration = int (sys.argv[2])
run_simulation (nodes, duration)

