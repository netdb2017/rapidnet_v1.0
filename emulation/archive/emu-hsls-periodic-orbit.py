import sys
from emulate import *

args = {}

args['testbed']='orbit'
args['topofilename'] = sys.argv[2]
args['device'] = 'ath0'
args['duration'] = 1000
args['app'] = 'EmuHslsPeriodic'
args['print-period'] = 5
args['log-discovery'] = 0
args['log-app'] = 0
args['dir']=sys.argv[1]
args['path']="/home/repos/ws_mshivk_ns3.5_wintech_camera_ready/"

emulate (args)
