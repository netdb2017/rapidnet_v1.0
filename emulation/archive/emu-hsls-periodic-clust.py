import sys
from emulate import *

if len (sys.argv) != 3:
  print 'Usage: program <dir> <apptable-file>'

args = {}

args['testbed']='clust'
args['topofilename'] = sys.argv[2]
args['device'] = 'eth1'
args['duration'] = 400
args['app'] = 'EmuHslsPeriodic'
args['print-period'] = 5
args['log-discovery'] = 0
args['log-app'] = 0
args['dir']=sys.argv[1]
#args['print-reln']='tLink,tLSU,apptable'
args['path']="/home/DBGROUP/xiaozhou/repos/ws_mshivk_ns3.5_wintech_camera_ready/"

emulate (args)


