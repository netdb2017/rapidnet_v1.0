#!/usr/bin/python
import os, sys

if len (sys.argv) != 5:
  print 'Usage: plot_losses <dir> <nodes> <duration> <app>'
  raise SystemExit

# Indices
# IMPORTANT: Indices are shared for all event types
# If new event attributes are added to any event type
# make sure the indices are correct.
TYPE = 0 # event type
NODE = 1 # node id 0-based
ADD = 2 # link is added/removed (boolean)
BYTES = 2

#event types
LINK = 'l'
SENT = 's'
RECD = 'r'
COMPUTE = 'c'
BANDWIDTH = 'b'

MS = 1000 #millisecond
NS = 1000000000 #nanosecond
PRECISION = 1000000 # millisecond precision is enough
COMPUTE_PERIOD = 5
BANDWIDTH_PERIOD = 2

STATS_FILE = 'losses.points'
PLOT_FILE = 'losses.ps'
DATARATE_FILE = 'dataRate.points'
TITLE = 'Loss (%)'

sim_dir = sys.argv[1]
nodes = int (sys.argv[2])
duration = int (sys.argv[3])
app = sys.argv[4]
decorator_log = os.path.join (sim_dir, 'decorator.log')
stats_file_loss = os.path.join (sim_dir, STATS_FILE)
plot_file_loss = os.path.join (sim_dir, PLOT_FILE)
stats_file_dataRate = os.path.join (sim_dir, DATARATE_FILE)
events = {}
ipaddr = {}

# output file
ofile = open (stats_file_loss, 'w')
odataRate = open (stats_file_dataRate, 'w')
MAX_RATE = 0
if app == 'Epidemic':
  discovery_timeout = 5000
  discovery_period_half = 1000
else:
  discovery_timeout = 10000
  discovery_period_half = 2500

# All events should be tuples
# First element of which is the event type
def add_event (time, event):
  if time not in events:
    events[time] = []
  events[time] += [event]

def parse_link_events ():
  file = open (decorator_log, 'r')
  for line in file:
    words = line.split ()
    if words[1] == 'link':
      time = float (words[0].strip ('ns'))
      # timestamp in decorator.log is in nanoseconds
      time = int (round (time / PRECISION))
      node = int (words[2])
      nbr = int(words[3].split('.')[3])-1
      if node != nbr:
        if words[4] == '+link':
          add = True
          add_event (time-discovery_period_half, (LINK, node, add))
        elif words[4] =='-link':
          add = False
          add_event (time-discovery_timeout, (LINK, node, add))
    elif words[1] == 'ip':
      node = int (words[2])
      assert node not in ipaddr
      ipaddr[node] = words[3]      

def add_compute_events ():
  # Add after every COMPUTE_PERIOD seconds
  period = COMPUTE_PERIOD
  for t in range (0, duration + period, period):
    add_event (t * MS, (COMPUTE))
  period = BANDWIDTH_PERIOD
  for t in range (0, duration + period, period):
    add_event (t * MS, (BANDWIDTH))

def parse_trace_files ():
  for filename in os.listdir (os.path.join (sim_dir, 'pcaps')):
    if filename.endswith ('.pcap.trace'):
      #print filename
      node_id = int (filename.split ('-')[1])
      node_ip = ipaddr[node_id]
      #print file, node_ip
      file = open (os.path.join (sim_dir, 'pcaps', filename), 'r')
      for line in file:
        words = line.strip ().split (' ')
        time = float (words[0])
        # timestamp in trace file is in seconds
        time = int (round (time * MS))
        ip = words[1].rpartition ('.')[0]
        bytes = int (words[2])
        if ip == node_ip:
          add_event (time, (SENT, node_id, bytes))
        else:
          add_event (time, (RECD, node_id, bytes))

def process_events ():
  global MAX_RATE

  # Sort the times before processing
  times = events.keys ()
  times.sort ()

  # Setup graph with node to neighbor counts
  links = {}
  for i in range (0, nodes):
    links[i] = 0
  data = {}
  for i in range (0, nodes):
    data[i] = 0

  sent = 0
  recd = 0

  # Do not delete the print statements
  # On uncommenting them the processing becomes verbose
  for t in times:
    #print t, events[t]
    #print t, links
    for event in events[t]:
      if event[TYPE] == LINK:
        if event[ADD]:
          #print t, event[NODE], 'adding link'
          links[event[NODE]] += 1
          #print t, event[NODE], links[event[NODE]]
        else:
          assert links[event[NODE]] > 0
          #print t, event[NODE], 'removing link'
          links[event[NODE]] -= 1
          #print t, event[NODE], links[event[NODE]]
      elif event[TYPE] == SENT:
        #print t, event[NODE], 'sent', event[BYTES], 'X', links[event[NODE]], 'bytes'
        sent += event[BYTES] * links[event[NODE]]
        data[event[NODE]] += event[BYTES]
        
      elif event[TYPE] == RECD:
        #print t, event[NODE], 'recd', event[BYTES], 'bytes'
        recd += event[BYTES]
        data[event[NODE]] += event[BYTES]
        
      elif event[TYPE] == COMPUTE:
        if sent > 0 and sent >= recd:
          sent = sent / 1000.0
          recd = recd / 1000.0
          loss = sent - recd
          #print '%d %.2f loss %.3f %.3f %.3f' % (t / MS, 100.0 * loss / sent, loss, sent, recd)
          ofile.write ('%d %.2f %.3f %.3f %.3f\n' % (t / MS, 100.0 * loss / sent, loss, sent, recd))
        else:
          sent = sent / 1000.0
          recd = recd / 1000.0
          loss = sent - recd
          ofile.write ('%d %.2f %.3f %.3f %.3f\n' % (t / MS, 100.0 * 0, loss, sent, recd))
        sent = 0
        recd = 0
        
      elif event[TYPE] == BANDWIDTH:
        for i in range (0, nodes):
          data_rate =(data[i] / 1000.0) / float(BANDWIDTH_PERIOD)
          if data_rate > MAX_RATE:
            MAX_RATE = data_rate
          odataRate.write ('%d %d %.4f\n' % (t / MS, i, data_rate))
          data[i] = 0

      else:
        print 'Unknown event!'
        raise SystemExit

def plot_loss ():
  ofile.close ()
  odataRate.write ('MAX_RATE %.4f\n' % (MAX_RATE))
  odataRate.close ()
  scriptfile = open ('rapidnet/bandwidth/losses.gnuplot', 'r').read ()
  plot_script = scriptfile % (stats_file_loss)
  open ('temp.gnuplot', 'w').write (plot_script)
  print ('Generating plot %s' % plot_file_loss)
  os.system ('gnuplot temp.gnuplot > %s' % plot_file_loss)


parse_link_events ()
parse_trace_files ()
add_compute_events ()
process_events ()
plot_loss ()

