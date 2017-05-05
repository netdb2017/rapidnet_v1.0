#!/usr/bin/python
import sys, os

if len (sys.argv) != 6:
  print 'Usage: avg.py <ls_dir> <hsls_dir> <dest_dir> <nodes> <duration>'
  sys.exit (0)

ls_dir = sys.argv[1]
hsls_dir = sys.argv[2]
dest_dir = sys.argv[3]
nodes = int (sys.argv[4])
duration = int (sys.argv[5])
avgfile = open (os.path.join (dest_dir, 'averages.txt'), 'w')
app_period = 5

# Sent bandwidth
def avgBandwidth (filename, duration, r_s):
  file = open (filename, 'r')
  total = 0.0
  for line in file:
    value = float (line.split (' ')[1])
    total += value
  avg = (total / duration) / 1000
  avgfile.write ('Average %s bandwidth: %.3f kBps (%d secs)\n' % (r_s, avg, duration))
  return avg

def avgStretch (filename):
  total = 0.0
  count = 0
  file = open (filename, 'r')
  for line in file:
    value = float (line.split (' ')[1])
    if value != 0.0:
      total += value
      count += 1
  avg = total / count
  avgfile.write ('Average stretch: %.3f\n' % avg)
  return avg

def avgValidity (filename):
  total = 0.0
  count = 0
  file = open (filename, 'r')
  for line in file:
    value = float (line.split (' ')[1])
    total += value
    count += 1
  avg = total / count
  avgfile.write ('Average validity: %.3f\n' % avg)
  return avg

def avgLossRate (sentAvg, recdAvg, avgNodeDeg):
  loss_rate = 100 * (1.0 - float (recdAvg) / (sentAvg * avgNodeDeg))
  avgfile.write ('Average loss rate (%%): %.2f\n' % loss_rate)
  return loss_rate

def avgLossRateNew (dir):
  file = open (os.path.join (dir, 'losses.points'), 'r')
  losses = {}
  for line in file:
    words = line.strip ().split (' ')
    values = []
    for word in words:
      values.append (float (word))
    losses[values[0]] = values[1:]
  sent = 0
  recd = 0
  for k, v in losses.iteritems ():
    sent += v[2]
    recd += v[3]
  loss_rate = 100.0 * (sent - recd) / sent
  avgfile.write ('Average loss rate new (%%): %.2f\n' % loss_rate)
  return loss_rate

def write_stats (dir, name, avg_node_deg):
  avgfile.write ('=== %s Stats ===\n' % name)
  sent = avgBandwidth (os.path.join (dir, 'bandwidth-sent.points'), duration, 'sent')
  recd = avgBandwidth (os.path.join (dir, 'bandwidth-recd.points'), duration, 'recd')
  stretch = avgStretch (os.path.join (dir, 'stretch.points'))
  validity = avgValidity (os.path.join (dir, 'validity.points'))
  avgfile.write ('Avg. node deg: %.1f\n' % avg_node_deg)
  loss_rate = avgLossRate (sent, recd, avg_node_deg)
  loss_rate = avgLossRateNew (dir)
  avgfile.write ('\n\n')

def get_avg_node_deg (dir, duration):
  avg_node_deg = 0
  filenames = os.listdir (dir)
  for filename in filenames:
    if filename.startswith ('output') and filename.endswith ('.log'):
#      print 'Reading %s for avg node degree' % filename
      file = open (os.path.join (dir, filename), 'r')
      for line in file:
        if line.startswith ('time=') and 'tLink' in line:
          avg_node_deg += 1
  return float (avg_node_deg) / (nodes * (duration/app_period))

write_stats (ls_dir, "LS", get_avg_node_deg (ls_dir, duration))
write_stats (hsls_dir, "HSLS", get_avg_node_deg (hsls_dir, duration))

