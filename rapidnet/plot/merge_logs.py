#!/usr/bin/python

import os, sys
import math, gmpy

if len (sys.argv) not in [2, 3, 4]:
  print 'Usage: merge_logs <logs_dir> [<route_quality(True/False)>] [<bandwidth_color(True/False)>]'
  sys.exit (0)

dir = sys.argv[1]
if len (sys.argv) >= 3:
  route_quality = sys.argv[2]
else :
  route_quality = 'False'

if len (sys.argv) >= 4:
  bandwidth_color = sys.argv[3]
else :
  bandwidth_color = 'True'

if not os.path.isdir (dir):
  print '%s is not a directory.' % dir
  sys.exit (0)

decorator_log = os.path.join (dir, 'decorator.log')
bw_sent_points = os.path.join (dir, 'bandwidth-sent.points')
validity_points = os.path.join (dir, 'validity.points')
stretch_points = os.path.join (dir, 'stretch.points')
losses_points = os.path.join (dir, 'losses.points')
data_points = os.path.join (dir, 'dataRate.points')
output_log = os.path.join (dir, 'output.log')
events_log = os.path.join (dir, 'events.log')

ofile = open (events_log, 'w')
ipaddr = {}
ipaddrinv = {}
events = {}
linkSrc = {}
bestPath = {}
eBitVectorReply = {}
count = 0 
tMessageCount = {}
currentMessage = ''
previousTime = 0
linkevents = {}
LSUevents = {}
maxLSU = 0

Bandwidth = True
Losses = True
Linkevent = True
Validity = False
Stretch = False
Bandwidth_Color = True

if route_quality == 'True':
  Validity = True
  Stretch = True   

elif route_quality == 'False':
  Validity = False
  Stretch = False

if bandwidth_color == 'True':
  Bandwidth_Color = True
  
elif bandwidth_color == 'False':
  Bandwidth_Color = False


def add_event (time, event):
  if time not in events:
    events[time] = [event]
  else:
    events[time] += [event]


####################################### For LS and HSLS
def check_lsu(time, line):
  global maxLSU
  if line.split()[1] == 'tuple':
      if line.split()[3] == '+eLSU':
        node = line.split()[2]
        src = line.split()[4].split(':')[1]
        if src != '127.0.0.1':
          t = long(time)/2000000000L
          time = t * 2000000000L
          smallid = min (int(node),ipaddrinv[src])
          bigid = max (int(node),ipaddrinv[src])
          if (time, smallid, bigid) not in LSUevents:
            LSUevents[(time, smallid, bigid)] = 1
          else:
            LSUevents[(time, smallid, bigid)] += 1
          if LSUevents[(time, smallid, bigid)] > maxLSU:
            maxLSU = LSUevents[(time, smallid, bigid)]


####################################### For DSR
def add_path_dsr (line):
  global count
  if line.split()[1] == 'link':
    if line.split()[4] == '+linkSrc':
      linkSrc[count] = [line.split()[2],line.split()[3]]
      count += 1
    
def delete_path_dsr (time, line):
  if line.split()[3] == '+Clear':
    for items in linkSrc.keys():
      event = str(time) + 'ns link ' + linkSrc[items][0] + ' ' + linkSrc[items][1] + ' -linkSrc\n'
      add_event (time, event)
    linkSrc.clear()
    for items in bestPath.keys():
      event = str(time) + 'ns link ' + bestPath[items][0] + ' ' + bestPath[items][1] + ' -bestPath\n'
      add_event (time, event)
    bestPath.clear()



def update_BestPath (time, line):
  if line.split()[3] == '+tBestPath' and line.split()[1] == 'tuple':
    time=time+10
    for items in bestPath.keys():
      event = str(time) + 'ns link ' + bestPath[items][0] + ' ' + bestPath[items][1] + ' -bestPath\n'
      add_event (time, event)
    bestPath.clear()   
    time=time+100
    path = line.split()[4].split('(')[1].partition (')')[0]
    pathNode = path.split(',')
    length = len(pathNode)
    for i in range(0,length-1):
      bestPath[i] = [str(int(pathNode[i].split('.')[3])-1),pathNode[i+1].split(':')[1]] 
    for items in bestPath.keys():
      event = str(time) + 'ns link ' + bestPath[items][0] + ' ' + bestPath[items][1] + ' +bestPath\n'
      add_event (time, event)

def parse_for_dsr(time,line):
    add_path_dsr (line)
    delete_path_dsr (time, line)
    update_BestPath (time, line)

####################################### For Epidemic
def delete_eBitVectorReply (time, line):
  time=time+1000000000
  if line.split()[1] == 'link':
    if line.split()[4] == '+eBitVectorReply':
      event = str(time) + 'ns link ' + line.split()[2] + ' ' + line.split()[3] + ' -eBitVectorReply\n'
      add_event (time, event)

def add_eMessage_link (time, line):
  if line.split()[1] == 'tuple':
      if line.split()[3] == '+eMessage':
        src = line.split()[4].split(':')[1]
        event = str(time) + 'ns link ' + line.split()[2] + ' ' + src + ' +recv_eMessage\n'
        add_event (time, event)
        time=time+1000000000
        event = str(time) + 'ns link ' + line.split()[2] + ' ' + src + ' -recv_eMessage\n'
        add_event (time, event)

def count_tMessage (time, line):
   global currentMessage, previousTime
   if line.split()[1] == 'tuple':
      if line.split()[3] == '+tMessage':
        messageID = line.split(':')[1]
        if messageID not in tMessageCount:
          currentMessage = messageID
          tMessageCount[currentMessage] = 1
          event = str(previousTime) + 'ns point tmessage 0\n'
          add_event (previousTime, event)
          timejustnow = time-100
          event = str(timejustnow) + 'ns point tmessage 0\n'
          add_event (timejustnow, event)
        else:
          tMessageCount[messageID] += 1
        event = str(time) + 'ns point tmessage %d\n'%tMessageCount[currentMessage]
        add_event (time, event)
        previousTime = time + 1000
      
def parse_for_epidemic(time,line):
  delete_eBitVectorReply (time, line)
  add_eMessage_link (time, line)
  count_tMessage (time, line)


####################################### For LinkEvent
def count_linkevents (time, line):
  global linkevents
  if line.split()[1] == 'link':
    if line.split()[4] in ['+link', '-link']:
      if int(line.split()[2]) != int(line.split()[3].split('.')[3])-1 :
        t = long(time)/1000000000L
        time = (t+1) * 1000000000L
        if time not in linkevents:
          linkevents[time] = 1
        else:
          linkevents[time] += 1

def add_linkevents ():
  max_linkevent = 0
  for time in linkevents.keys():
    if linkevents[time] > max_linkevent and time > 5000000000L: # Exclude first 5 seconds
      max_linkevent = linkevents[time]  
    add_event (time, '%ldns point linkevent %d\n' % (time, linkevents[time]))
  if (route_quality == 'False'):
    ofile.write ('info graph linkevent Per-Second_Link_Events 0 %s 0 %d %d\n'%(duration, max_linkevent, 3)) 


##### For Chord

def parse_for_chord (time, line):
  MAX = (gmpy.mpz (1) << 160) - 1
  RADIUS = gmpy.mpz (700)
  CX, CY = [gmpy.mpz (1000), gmpy.mpz (1000)]
  node_id = line.split(' ')[2]
  if ('node_attr2' in line):
    id = gmpy.mpz (line.rpartition(':')[2], 16)
    angle = gmpy.mpf (math.pi) * id / MAX
    x, y = CX + RADIUS * math.cos (angle), CY + RADIUS * math.sin(angle)
    add_event (time, '%ldns position %s %f %f 0 0 0 0\n' % (time, node_id, x, y))
    add_event (time, '%ldns state %s +chordRing\n' % (time, node_id))
    

####################################### For All
print 'Reading output.log...'
file = open (output_log, 'r')
count = 0
speed_low = 0
speed_up = 0
duration = 0
node_count = 0
for line in file:
  count += 1
  if count > 30:
    continue
  words = line.split (' : ')
  if words[0].startswith('Duration'):
    value = words[1].split(' ')[0]
    duration = value
    ofile.write ('info label duration Duration %s %d\n'%(value,5))
  if words[0].startswith('Number of nodes'):
    value = words[1].strip('\n')
    node_count = value
    ofile.write ('info label node_count Nodes_Count %s %d\n'%(value,3))
  if words[0].startswith('RapidNet Application'):
    app = words[1].strip('\n') 
    if app == 'SimLsPeriodic' or app == 'EmuLsPeriodic':
      protocol = 'Link_State_Periodic'
    elif app == 'SimLsTriggered' or app == 'EmuLsTriggered':
      protocol = 'Link_State_Triggered'
    elif app == 'SimHslsPeriodic' or app == 'EmuHslsPeriodic':
      protocol = 'HSLS_Periodic'
    elif app == 'SimHslsTriggered' or app == 'EmuHslsTriggered':
      protocol = 'HSLS_Triggered'
    elif app == 'Epidemic':
      protocol = 'Epidemic'
    elif app == 'Dsr':
      protocol = 'DSR'
    else:
      protocol = app
    ofile.write ('info label protocol Protocol %s %d\n'%(protocol,1))
  if words[0].startswith('Mobility model speed attribute (low)'):
     speed_low = words[1].strip('\n')
  if words[0].startswith('Mobility model speed attribute (up)'):
     speed_up = words[1].strip('\n')
     speed=speed_low+'m/s_to_'+speed_up+'m/s'
     ofile.write ('info label speed Speed %s %d\n'%(speed,6))
  if words[0].startswith('Mobility model      '):
    value = words[1].split('::')[1].strip('\n').split('MobilityModel')[0]
    if value == 'RandomWaypoint':
      value = 'Random_Waypoint'
    elif value == 'RandomWalk2d':
      value = 'Random_Walk_2-Dimension'
    elif value == 'ConstantPosition':
      value = 'Constant_Position'
    ofile.write ('info label mobility_model Mobility_Model %s %d\n'%(value,2))
print 'Done!'

print 'Reading decorator_log...'
file = open (decorator_log, 'r')
arena = file.readline ()
words = arena.strip('\n').split(' ')
ofile.write ('info label arena Arena %sm_X_%sm %d\n'%(words[3],words[4],4))
for line in file:
  time = long (line.partition ('ns')[0])
  if line.split()[1] == 'ip':
    node = int (line.split()[2])
    assert node not in ipaddr
    ipaddr[node] = line.split()[3] 
    ipaddrinv[line.split()[3]]=node
  check_lsu(time, line)
  if line.split()[1] != 'tuple':
    add_event (time, line)
  count_linkevents (time, line)
  if route_quality == 'False':
    parse_for_dsr (time,line)
    parse_for_epidemic (time, line)
    parse_for_chord (time, line)
if Linkevent:
  add_linkevents()
print 'Done!'

if Bandwidth:
  print 'Reading bandwidth-sent.points...'
  max_bandwidth = 0.0
  file = open (bw_sent_points, 'r')
  for line in file:
    words = line.split (' ')
    time = long (words[0]) * 1000000000L
    value = float (words[1]) / 1000.0
    if value > max_bandwidth:
      max_bandwidth = value
    add_event (time, '%ldns point sent %.2f\n' % (time, value))
  ofile.write ('info graph sent Per-node_Bandwidth_(kB/s) 0 %s 0 %f %d\n'%(duration, max_bandwidth, 1))
  print 'Done!'

if Validity:
  print 'Reading validity.points...'
  file = open (validity_points, 'r')
  for line in file:
    words = line.split (' ')
    time = long (words[0]) * 1000000000L
    value = float (words[1])
    add_event (time, '%ldns point validity %.3f\n' % (time, value))
  ofile.write ('info graph validity Avg._Route_Validity 0 %s 0 1.1 %d\n'%(duration, 3))
  print 'Done!'

if Stretch:
  print 'Reading stretch.points...'
  file = open (stretch_points, 'r')
  max_stretch = 0.0
  for line in file:
    words = line.split (' ')
    time = long (words[0]) * 1000000000L
    value = float (words[1])
    if value > max_stretch:
      max_stretch = value
    add_event (time, '%ldns point stretch %.3f\n' % (time, value))
  ofile.write ('info graph stretch Avg._Route_Stretch 0 %s 1.0 %s %d\n'%(duration, max_stretch, 4))
  print 'Done!'

if Losses:
  print 'Reading losses.points...'
  file = open (losses_points, 'r')
  max_loss = 0.0
  for line in file:
    words = line.split (' ')
    time = long (words[0]) * 1000000000L
    value = float (words[1])
    if value > max_loss:
      max_loss = value
    add_event (time, '%ldns point loss %.3f\n' % (time, value))
  ofile.write ('info graph loss Loss_Rate_(%%) 0 %s 0 %s %d\n'%(duration, max_loss, 2))
  print 'Done!'

if Bandwidth_Color:
  print 'Reading dataRate.points...'
  file = open (data_points, 'r')
  dataline=file.readlines()
  max_rate = dataline[len(dataline)-1].split(' ')[1]
  level_value = float(max_rate) / 4.0
  rate_level=['low','med','high','veryhigh','veryhigh']
  for line in dataline:
    words = line.split (' ')
    if words[0] != 'MAX_RATE':
      time = long (words[0]) * 1000000000L
      node = int (words[1])
      value = float (words[2])
      level = int (value/level_value)
      add_event (time, '%ldns state %d +%s\n' % (time, node, rate_level[level]))
  print 'Done!'

if Bandwidth_Color:
  print 'Computing eLSU events...'
  level_value = maxLSU / 4
  rate_level=['low','med','high','veryhigh','veryhigh']
  for key in LSUevents.keys ():
    level = int (LSUevents[key]/level_value)
    linkstate = rate_level[level]
    time = key[0]
    event = str(time) + 'ns link ' + str(key[1]) + ' ' + ipaddr[key[2]] + ' +%s\n'%linkstate    
    add_event (time, event)
    time = key[0]+1900000000L
    event = str(time) + 'ns link ' + str(key[1]) + ' ' + ipaddr[key[2]] + ' -%s\n'%linkstate
    add_event (time, event)


if route_quality == 'False':
  ofile.write ('info graph tmessage Message_Propogation 0 %s 0 %s %d\n'%(duration, node_count, 4))

times = events.keys ()
times.sort ()

ofile.write (arena) # Echo arena line
for time in times:
  for event in events[time]:
    ofile.write (event)

print 'Events written to %s' % events_log
