import os

# Global variables for aggregating stats from each node
# Sent and received packets are aggregated separately
sent_stats = {}
recd_stats = {}

# Global constant for stats file names
STATS_FILE_SENT = 'bandwidth-sent.points'
STATS_FILE_RECD = 'bandwidth-recd.points'

# Global constant for plot file names
PLOT_FILE_SENT = 'bandwidth-sent.ps'
PLOT_FILE_RECD = 'bandwidth-recd.ps'

# Global constant for gnuplot script
GNUPLOT_SCRIPT = 'rapidnet/bandwidth/sim_bandwidth.gnuplot'

# Global constants for plot title
TITLE_SENT = 'Bandwidth (sent)'
TITLE_RECD = 'Bandwidth (received)'

# Aggregates the statistics for the given node
# The node's IP address, neighbor list and corresponding pcap file
# is provided as arguments
def aggregate_stats (ipaddr, pcap_file):

  print ('Processing ' + pcap_file + ' for node ' + ipaddr)

  # Trace file for dumping individual node IP packet traces
  trace_file = pcap_file + '.trace'

# Only dumping sender and destination address is always broadcast
  print ('Creating ' + trace_file)
  # IMPORTANT: IF DISCOVERY IS USING PORT 520 THEN YOU WILL NEED TO CHANGE CUT TO SELECT
  # FIELD 1,3,11 ELSE FIELDS SHOULD BE 1,3,8 (Run tcpdump and you will know why).
  os.system ('sudo tcpdump -nn -tt -r %s | grep "4000: UDP" | cut -d \' \' -f 1,3,8 > %s' % (pcap_file, trace_file))

  # Create a map of sender to (time, length) pairs
  senders = {}
  file = open (trace_file, 'r')
  for line in file:
    words = line.split ()
    time = int (round (float (words[0])))
    sender = words[1].rpartition ('.')[0]
    bytes = int (words[2])
    if sender not in senders.keys ():
      senders[sender] = [(time, bytes)]
    else:
      senders[sender] += [(time, bytes)]

  # Aggregate sent packets
  if ipaddr in senders:
    for packet in senders[ipaddr]:
      time = packet[0]
      bytes = packet[1]
      if time not in sent_stats.keys ():
        sent_stats[time] = bytes
      else:
        sent_stats[time] += bytes

  # Aggregate recd packets
  for sender in senders.keys ():
    if sender == ipaddr:
      #print ('Self %s' % sender)
      continue
    #print ('Neighbor: %s' % sender)
    for packet in senders[sender]:
      time = packet[0]
      bytes = packet[1]
      if time not in recd_stats.keys ():
        recd_stats[time] = bytes
      else:
        recd_stats[time] += bytes

# Computes the average bytes per second
def compute_average (stats, count):
  for time, bytes in stats.iteritems ():
    stats[time] = int (round (float(bytes) / count))

# Dumps the stats to a file
def dump_stats (stats, dir, stats_file, count):
  print ('Dumping to stats file: ' + stats_file)
  compute_average (stats, count)
  file = open (os.path.join (dir, stats_file), 'w')
  times = stats.keys()
  times.sort ()
  #print times
  for k in times:
    file.write (str (k) + ' ' + str(stats[k]) + '\n')
  #print stats
  file.close ()

# Computes the bandwidth statistic: Average bytes per second (Bps) per node
def compute_stats (dir, app, nodes):

  # Data is the map of IP with map of {
  # 'sim-pcap': <simulation-pcapfilename>
  # }
  data = {}

  # Map neighbor list representation to IPs and populate data
  for nodeid, nodeip in nodes.iteritems ():
    data[nodeip] = {
      'sim-pcap': os.path.join (dir, 'pcaps', app + '-' + str (nodeid-1) + '-0.pcap'),
      }

  #print data

  # Iterate over every node's data and aggregate stats
  for nodeip,data in data.iteritems ():
    aggregate_stats (nodeip, data['sim-pcap'])

  # Finally, dump the aggregated stats
  dump_stats (sent_stats, dir, STATS_FILE_SENT, len (nodes))
  dump_stats (recd_stats, dir, STATS_FILE_RECD, len (nodes))

# Plots the computed stats data
# app: application name string
# nodes: map of integer nodeid (1-based) to IP address string
def plot_stats (dir, app, nodes):

  # Invoke function to compute stats
  compute_stats (dir, app, nodes)

  # Open parameterized gnuplot script
  scriptfile = open (GNUPLOT_SCRIPT, 'r').read ()

  # Plot sent stats
  plot_file_sent = os.path.join (dir, PLOT_FILE_SENT)
  sentplot_script = scriptfile % (os.path.join (dir, STATS_FILE_SENT), TITLE_SENT)
  open ('temp.gnuplot', 'w').write (sentplot_script)
  print ('Generating plot %s' % plot_file_sent)
  os.system ('gnuplot temp.gnuplot > %s' % plot_file_sent)

  # Plot recd stats
  plot_file_recd = os.path.join (dir, PLOT_FILE_RECD)
  recdplot_script = scriptfile % (os.path.join (dir, STATS_FILE_RECD), TITLE_RECD)
  open ('temp.gnuplot', 'w').write (recdplot_script)
  print ('Generating plot %s' % plot_file_recd)
  os.system ('gnuplot temp.gnuplot > %s' % plot_file_recd)

  # Remove the temp file
  os.remove ('temp.gnuplot')

