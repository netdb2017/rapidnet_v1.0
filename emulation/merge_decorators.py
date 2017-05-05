#!/usr/bin/python

import os, sys

if len (sys.argv) != 4:
  print 'Usage: ./merge_decorators.py <emu_dir> <topo_dir> <orbit|cluster>'
  sys.exit (0)

emu_dir = sys.argv[1]
topo_dir = sys.argv[2]

if sys.argv[3] == 'cluster':
  from nodesmap_clust import *
else:
  from nodesmap_orbit import *


topo_decorator=os.path.join (topo_dir,'decorator.log')
decorator_log = os.path.join (emu_dir, 'decorator.log')

ofile = open (decorator_log, 'w')

events = {}
ipmap = {}

def add_event (time, event):
  if time not in events:
    events[time] = [event]
  else:
    events[time] += [event]

def process_decorator(decorator_file,nodeid):
  file = open (decorator_file, 'r')
  arena = file.readline ()
  ip = file.readline ()
  for line in file:
    time = long (line.partition ('ns')[0])
    event = line.split(' 0 ')[0]+' %d '%nodeid+line.split(' 0 ')[1]
    add_event (time, event)
  
  
print 'Reading emu_decorators_log...'
emu_decorators = os.listdir (emu_dir)
for file in emu_decorators:
  if file.startswith ('decorator-'):
    nodeid = nodesinv[file.split('-')[1].split('.log')[0]]-1
  #  print file, nodeid
    process_decorator(os.path.join (emu_dir, file),nodeid)
  if file.startswith ('output_'):
    os.system ('cp %s %s'%(os.path.join (emu_dir, file),os.path.join (emu_dir, 'output.log')))

print 'Reading topo_decorators_log...'
file = open (topo_decorator, 'r')
arena = file.readline ()
for line in file:
    time = long (line.partition ('ns')[0])
    if line.split()[1] == 'ip':
      node = int(line.split()[2])
      ip = nodes[node+1]
      ipmap[line.split()[3]] = ip
      event = line.split()[0] + ' ' + line.split()[1] + ' ' + line.split()[2] + ' ' + ip + '\n'
    elif line.split()[1] == 'link':
      words = line.split()
      event = words[0] + ' ' + words[1] + ' ' + words[2] + ' ' + ipmap[words[3]]+ ' ' + words[4] + '\n'
    else:
      event = line
    add_event (time, event)


times = events.keys ()
times.sort ()
ofile.write (arena) # Echo arena line
for time in times:
  for event in events[time]:
    ofile.write (event)
    
    
    
    
    