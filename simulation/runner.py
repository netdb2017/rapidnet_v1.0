#!/usr/bin/python

import os, sys
from time import strftime, gmtime
import threading
from random import randint, seed

apps = ['SimLsPeriodic', 'SimHslsPeriodic', 'SimLsTriggered', 'SimHslsTriggered']
threads = {}

args = {}
args['path'] = 'pennNetDB/dens/branches/ws_mshivk_code_release'

if len (sys.argv) not in [9, 10]:
  print 'Usage: ./runner <user> <cluster-id> <nodes> <duration> <speed_low> <speed_up> <xbound> <ybound> [<mobility_model>]'
  print 'Path of the branch on cluster: ', args['path']
  print 'Make sure branch is updated on the cluster!!!'
  sys.exit (0)
 
args['user'] = sys.argv[1] #os.environ['USER']
if args['user'] == 'xiaozhou':
  ssh_cmd_template = 'ssh %(host)s "cd %(path)s; %(command)s"'
  args['host'] = 'dbclust%d'
else:
  ssh_cmd_template = 'ssh -tt %(user)s@%(host)s "cd %(path)s; %(command)s"'
  args['host'] = 'dbclust%d.cis.upenn.edu'


def main ():
  cluster_id = int (sys.argv[2])
  nodes = int (sys.argv[3])
  duration = int (sys.argv[4])
  speed_low = sys.argv[5]
  speed_up = sys.argv[6]
  timestamp = strftime ('%Y%m%d%H%M%S', gmtime ())

  # Set arena size and initial positions for all runs
  xbound = sys.argv[7]
  ybound = sys.argv[8]

  #xpos = str (randint (0, int(xbound)))
  #ypos = str (randint (0, int(xbound)))
  #seed ()
  #for i in range (0, nodes-1):
  #  xpos += ',' + str (randint (0,int(xbound)))
  #  ypos += ',' + str (randint (0, int(xbound)))
 
  if len (sys.argv) == 10 :
    mob_model = sys.argv[9]
  else:
    mob_model = 'rwp' 

  for i in range (len (apps)):
    threads[i] = threading.Thread(target=run_simulation, args=(cluster_id + i, apps[i], nodes, \
      duration, speed_low, speed_up, xbound, ybound, mob_model, 0, timestamp))
    threads[i].start()

  move_and_plot (cluster_id, nodes, duration, speed_low, speed_up, timestamp)

def run_simulation (cluster_id, app, nodes, duration, speed_low, speed_up, xbound, ybound, mob_model, queryNum, timestamp):
  args['command'] = './simulation/run_simulation.py %s %d %d %s %s %s %s %s %d %s'
  ssh_cmd = ssh_cmd_template % args
  ssh_cmd = ssh_cmd % (cluster_id, app, nodes, duration, speed_low, speed_up, xbound, ybound, mob_model, queryNum, timestamp)
  print ssh_cmd
  retval = os.system (ssh_cmd)
  print 'Simulation exited with code %d' % retval

def move_and_plot (cluster_id, nodes, duration, speed_low, speed_up, timestamp):
  args['command'] = './rapidnet/plot/move_and_plot.py %d %d %s %s %s'
  ssh_cmd = ssh_cmd_template % args
  ssh_cmd = ssh_cmd % (cluster_id, nodes, duration, speed_low, speed_up, timestamp)
  for thread in threads.values ():
    thread.join ()
  print ssh_cmd
  retval = os.system (ssh_cmd)
  print 'Plot exited with code %d' % retval

main ()
