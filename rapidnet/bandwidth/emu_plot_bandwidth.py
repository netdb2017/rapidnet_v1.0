import os

# Global variables for aggregating stats from each node
# Sent and received packets are aggregated separately
sent_stats = {}
recd_stats = {}

# Global time varying apptable
apptable = {}

# Global constant for stats file names
STATS_FILE_SENT = 'bandwidth-sent.points'
STATS_FILE_RECD = 'bandwidth-recd.points'

# Global constant for plot file names
PLOT_FILE_SENT = 'bandwidth-sent.ps'
PLOT_FILE_RECD = 'bandwidth-recd.ps'

# Global constant for gnuplot script
GNUPLOT_SCRIPT = 'rapidnet/bandwidth/emu_bandwidth.gnuplot'

# Global constants for plot title
TITLE_SENT = 'Bandwidth (sent)'
TITLE_RECD = 'Bandwidth (received)'

def process_pcap (pcap_file):
  print ('Processing ' + pcap_file)

  # Trace file for dumping individual node IP packet traces
  trace_file = pcap_file + '.trace'

  # Only dumping sender and destination address is always broadcast
  print ('Creating ' + trace_file)
  os.system ('tcpdump -tt -nn -r %s broadcast | grep "IP" | grep ".4000: UDP, length" | cut -d \' \' -f 1,3,8 > %s' % (pcap_file, trace_file))


# Aggregates the statistics for the given node
# The node's IP address, neighbor list and corresponding pcap file
# is provided as arguments
def aggregate_stats (ipaddr, pcap_file):
  print pcap_file
  trace_file = pcap_file + '.trace'

  # Create a map of sender to list of (time, length) pairs
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
  for packet in senders[ipaddr]:
    time = packet[0]
    bytes = packet[1]
    if time not in sent_stats.keys ():
      sent_stats[time] = bytes
    else:
      sent_stats[time] += bytes

  # Aggregate recd packets
  for sender, packets in senders.iteritems ():
    for packet in packets:
      #print 'begin, for packet: ', packet
      time = packet[0]
      bytes = packet[1]
      interval = int (time / 5) * 5
      #print 'interval: ', interval, apptable [interval]
      #print '[ipaddr, sender]: ', [ipaddr, sender]
      if [ipaddr, sender] in apptable [interval]:
        #print 'accepting'
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
  for i in range (0, max (stats.keys ()) + 10):
    if i not in stats.keys ():
      stats[i] = 0;
  print ('Dumping to stats file: ' + stats_file)
  compute_average (stats, count)
  file = open (dir + stats_file, 'w')
  times = stats.keys()
  times.sort ()
  for k in times:
    file.write (str (k) + ' ' + str(stats[k]) + '\n')
  file.close ()

def get_fileinfo (dir, app, nodes, blacklist):
  if not dir.endswith ('/'):
    dir = dir + '/'

  # Data is the map of IP with map of {
  # 'emu-pcap': <emulation-pcapfilename> # only for emulation
  # }
  data = {}

  for nodeid in nodes.keys ():
    if nodeid not in blacklist:
      data[nodes[nodeid]] = {
        'emu-pcap': os.path.join (dir, 'pcaps', app + '_' + nodes[nodeid] + '-0-0.pcap'),
        }
  return data

# Computes the bandwidth statistic: Average bytes per second (Bps) per node
def compute_stats (dir, app, nodes, apptable_file, blacklist):

  data = get_fileinfo (dir, app, nodes, blacklist)

  # loads the apptable global dict
  execfile (apptable_file, globals ())

  # Iterate over every node's data and aggregate stats
  for nodeip, data in data.iteritems ():
    aggregate_stats (nodeip, data['emu-pcap'])

  # Finally, dump the aggregated stats
  dump_stats (sent_stats, dir, STATS_FILE_SENT, len (nodes))
  dump_stats (recd_stats, dir, STATS_FILE_RECD, len (nodes))

def process_all_pcap (dir):
  files = os.listdir (dir)
  for file in files:
    if file.endswith ('.pcap'):
      process_pcap (os.path.join (dir, file))

# Plots the computed stats data
# app: application name string
# nodes: map of integer nodeid (1-based) to IP address string
# sim_outputlog: output.log file from simulation
def plot_stats (dir, app, nodes, apptable_file, blacklist):

  # Process pcap files if present (in case of cluster)
  # For orbit, trace files are already present
  # process_all_pcap (dir)

  if not dir.endswith ('/'):
    dir = dir + '/'

  # Invoke function to compute stats
  compute_stats (dir, app, nodes, apptable_file, blacklist)

  # Open parameterized gnuplot script
  scriptfile = open (GNUPLOT_SCRIPT, 'r').read ()

  # Plot sent stats
  plot_file_sent = dir + PLOT_FILE_SENT
  sentplot_script = scriptfile % (dir + STATS_FILE_SENT, TITLE_SENT)
  open ('temp.gnuplot', 'w').write (sentplot_script)
  print ('Generating plot %s' % plot_file_sent)
  os.system ('gnuplot temp.gnuplot > %s' % plot_file_sent)

  # Plot recd stats
  plot_file_recd = dir + PLOT_FILE_RECD
  recdplot_script = scriptfile % (dir + STATS_FILE_RECD, TITLE_RECD)
  open ('temp.gnuplot', 'w').write (recdplot_script)
  print ('Generating plot %s' % plot_file_recd)
  os.system ('gnuplot temp.gnuplot > %s' % plot_file_recd)

  # Remove the temp file
  os.remove ('temp.gnuplot')

