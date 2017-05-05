#!/usr/bin/python
import sys, os
from time import strftime, gmtime
from getAddress import *

#Global variables

MODEL = {
  'ns3::ConstantPositionMobilityModel': 'static',
  'ns3::RandomWaypointMobilityModel': 'randomwaypoint',
  'ns3::RandomWalk2dMobilityModel': 'randomwalk2d',
  'ns3::HierarchicalMobilityModel': 'hierarchical'
  }

def process_pcap (pcap_file):
  print ('Processing ' + pcap_file)

  # Trace file for dumping individual node IP packet traces
  trace_file = pcap_file + '.trace'

  # Only dumping sender and destination address is always broadcast
  print ('Creating ' + trace_file)
  os.system ('sudo tcpdump -tt -nn -r %s broadcast | grep "IP" | grep ".4000: UDP, length" | cut -d \' \' -f 1,3,8 > %s' % (pcap_file, trace_file))

def emulate(args):

  address=getAddress(args['device'])

  if 'tag' not in args.keys ():
    args['tag'] = ''

  if 'print-reln' not in args.keys ():
    args['print-reln'] = 'tLink'

  #Emulation variables
  
  dir = args['dir']
  logfile = '%s/output_%s.log' % (dir,address['ipaddr'])
  pcap_file= '%s/pcaps/%s_%s-0-0.pcap' % (dir,args['app'],address['ipaddr'])

  #Command string
#  command = 'sudo ./waf --run rapidnet-app-emulator --command-template="valgrind %%s --dir=%s --app=%s --duration=%d --log-discovery=%d --log-app=%d --device=%s --mac=%s --ipnet=%s --ipmask=%s --ipbase=%s --ipaddr=%s --topo=%s --print-period=%s --print-reln=%s  --start-time=%f" 2> %s'
  command = 'sudo ./waf --run "rapidnet-app-emulator --dir=%s --app=%s --duration=%d --log-discovery=%d --log-app=%d --device=%s --mac=%s --ipnet=%s --ipmask=%s --ipbase=%s --ipaddr=%s --topo=%s --print-period=%s --print-reln=%s --start-time=%s" 2> %s'

  #Create directory
  if args['testbed']=='orbit':
    print "Creating dir %s" % dir
    os.system ('mkdir %s' % dir)
  print "Log file: %s" % logfile

  # Calculate start time
  if args['testbed'] == 'orbit':
    from nodesmap_orbit import nodes, nodesinv
  else:
    from nodesmap_clust import nodes, nodesinv
    
  start_time = 0

  #Run command
  topopath=args['path']+args['topofilename']
#  command = command % (dir, args['app'], args['duration'], args['log-discovery'], args['log-app'], args['device'], address['mac'], address['ipnet'], address['ipmask'], address['ipbase'], address['ipaddr'], topopath, args['print-period'], args['print-reln'], logfile, logfile)
  command = command % (dir, args['app'], args['duration'], args['log-discovery'], args['log-app'], args['device'], address['mac'], address['ipnet'], address['ipmask'], address['ipbase'], address['ipaddr'], topopath, args['print-period'], args['print-reln'], start_time, logfile)
  print ('Running command %s' % command)
  os.system (command)
  process_pcap (pcap_file)

