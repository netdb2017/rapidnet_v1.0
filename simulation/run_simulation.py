#!/usr/bin/python
from simulate import simulate
from time import strftime, gmtime
from random import randint, seed
import sys
from math import sqrt, ceil

DSR_INTERVAL = 10
EPIDEMIC_INTERVAL = 120

def set_jitter (args, app):
  if app == 'Dsr':
    args['maxJitter'] = 200
  elif app == 'Epidemic':
    args['maxJitter'] = 200
  elif app in ['SimLsPeriodic','SimLsTriggered',  'SimHslsPeriodic', 'SimHslsTriggered']:
    args['maxJitter'] = 500

def run_simulation (app, nodes, duration, speed_low, speed_up, xbound, ybound, queryNum, mob_model, timestamp):
  args = {}

  mobilityModel = {'rwp': 'ns3::RandomWaypointMobilityModel', 'rw2d': 'ns3::RandomWalk2dMobilityModel'}

  args ['app'] = app
  args ['nodes'] = nodes
  args ['duration'] = duration
  args ['queryNum'] = queryNum
  args ['timestamp'] = timestamp

  args ['phy'] = 'wifi'
  args ['xbound'] = xbound
  args ['ybound'] = ybound
  #args ['xpos'] = xpos
  #args ['ypos'] = ypos

  set_jitter (args, app)

  args ['mm'] = mobilityModel[mob_model]
  distance = (xbound + ybound)/20 # one tenth of the mean of xbound and ybound
  args ['mob-args'] = '--speed_low=%s --speed_up=%s --distance=%d'%(speed_low, speed_up, distance)
  if app == 'Dsr':
    args ['tag'] = 'speed%s-%s_queryNum%s'%(speed_low, speed_up,queryNum)
  elif app == 'Epidemic':
    args ['tag'] = 'speed%s-%s_messageNum%s'%(speed_low, speed_up,queryNum)
  else:
    args ['tag'] = 'speed%s-%s'%(speed_low, speed_up)

  args ['stream'] = 'clog'
  args ['print-period'] = 5
  args ['log-discovery'] = 0
  args ['log-app'] = 0

  if app == 'Dsr':
    route_quality = False
    bandwidth_color = False
    args['print-reln'] = 'tBestPath'
  elif app == 'Epidemic':
    route_quality = False
    bandwidth_color = False
    args['print-reln'] = 'tLink,tMessage'
  elif app in ['SimLsPeriodic', 'SimLsTriggered', 'SimHslsPeriodic', 'SimHslsTriggered', 'Olsr2']:
    route_quality = True
    bandwidth_color = True
    args['print-reln'] = 'tLink,tLSU'
  else:
    route_quality = False
    bandwidth_color = True
    args['print-reln'] = 'link'
    
  simulate (args, route_quality, bandwidth_color)


if len (sys.argv) not in [4, 6, 8, 9, 10, 11]:
  print 'Usage: run_simulation <app> <nodes> <duration> [<speed_low> <speed_up>] [<xbound> <ybound>] [<mobility_model>] [<query/messageNum>] [<timestamp>]'
  sys.exit (0)

app = sys.argv[1]
nodes = int (sys.argv[2])
duration = int (sys.argv[3])

if len (sys.argv) >= 6:
  speed_low = sys.argv[4]
  speed_up = sys.argv[5]
else:
  speed_low = '1'
  speed_up = '2'

if len (sys.argv) >= 8:
  xbound = int (sys.argv[6])
  ybound = int (sys.argv[7])
else:
  xbound = 1000
  ybound = 1000
   
if len (sys.argv) >= 9 :
  mob_model = sys.argv[8]
else:
  mob_model = 'rwp' 
  
if len (sys.argv) >= 10 :
  queryNum = int (sys.argv[9])
else:
  if app == 'Dsr':
    queryNum = duration / DSR_INTERVAL
  elif app == 'Epidemic':
    queryNum = duration / EPIDEMIC_INTERVAL
  else: 
    queryNum = 0

#if len (sys.argv) >= 12:
#  xpos = sys.argv[10]
#  ypos = sys.argv[11]
#else:
#  xpos = str (randint (0, xbound))
#  ypos = str (randint (0, xbound))
#  # Generate random topology
#  seed ()
#  for i in range(0, nodes-1):
#    xpos += ',' + str (randint (0, xbound))
#    ypos += ',' + str (randint (0, ybound))

if len (sys.argv) == 11:
  timestamp = sys.argv[10]
else:
  timestamp = strftime ("%Y%m%d%H%M%S", gmtime ())


run_simulation (app, nodes, duration, speed_low, speed_up, xbound, ybound, queryNum, mob_model, timestamp)

