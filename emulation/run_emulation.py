import sys
from emulate import *

if len (sys.argv) != 8:
  print 'Usage: program <app> <dir> <path> <apptable-file> <testbed> <device> <duration>'

app = sys.argv[1]
dir = sys.argv[2]
path = sys.argv[3]
topofilename = sys.argv[4]
testbed = sys.argv[5]
device = sys.argv[6]
duration = int(sys.argv[7])

args = {}

args['testbed']=testbed
args['topofilename'] = topofilename
args['device'] = device
args['duration'] = duration
args['app'] = app
args['print-period'] = 5
args['log-discovery'] = 0
args['log-app'] = 0
args['dir']=dir
args['path']=path
if app == 'Dsr':
  route_quality = False
  bandwidth_color = False
  args['print-reln'] = 'tBestPath'
elif app == 'Epidemic':   
  route_quality = False
  bandwidth_color = False
  args['print-reln'] = 'tLink,tMessage'
elif app in ['EmuLsPeriodic', 'EmuLsTriggered', 'EmuHslsPeriodic', 'EmuHslsTriggered', 'Olsr2']:
  route_quality = True
  bandwidth_color = True
  args['print-reln'] = 'tLink,tLSU'
else:
  route_quality = False
  bandwidth_color = True
  args['print-reln'] = 'link'

emulate (args)
