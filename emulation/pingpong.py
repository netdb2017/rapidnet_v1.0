#!/usr/bin/python
import sys, os
from getAddress import *

if len (sys.argv) != 4:
  print 'usage: python getAggress.py <interface (example: eth0)> <dst addr> <duration>'
  exit()

interface = sys.argv[1]
destip = sys.argv[2]
duration = int(sys.argv[3])

address=getAddress(interface)
command = 'sudo ./waf --run "emu-ping-pong-test1 --duration=%d --deviceName=%s --macAddress=%s --IPnet=%s --IPmask=%s --IPbase=%s --myaddr=%s --dstaddr=%s"'%(duration, interface, address['mac'], address['ipnet'], address['ipmask'], address['ipbase'], address['ipaddr'], destip)
print ('Running command %s' % command)
os.system (command)
