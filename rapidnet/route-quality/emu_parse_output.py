import os, sys

# Parse and dump the tLinks and tLSUs graph data structures in python scripts
# dir: directory where to find output.log and place the generated files
# nodecount: number of nodes
# invnodesmap: inverse nodes map
# ouputfile_name: output log file
# mode: 'w' for write or 'a' for append
def parse_output (dir, nodecount, invnodesmap, output_filename = 'output.log', mode = 'w'):

  output_file = os.path.join (dir, output_filename)
  os.system ('mkdir %s' % os.path.join (dir, 'routes'))

  prev_time = 0
  ofile = open (output_file, 'r')

  for line in ofile:
    if line.startswith ('time'):
      words = line.split (' ')
      time = int (words[0].split('=')[1])

      #Moving to the next time slot
      if time != prev_time:
        prev_time = time
        routefilename = os.path.join (dir, 'routes/route_%3.3d.py' % time)
        if not os.path.exists (routefilename):
          # Write mode, create a fresh file and initialize values
          routefile = open (routefilename, 'w')
          print 'Creating dump file for time %3d seconds: %s' % (time, routefilename)
          routefile.write ('tLink = {}\n')
          routefile.write ('tLSU = {}\n')
          routefile.write ('for i in range (1, %d + 1):\n' % nodecount)
          routefile.write ('  tLink[i] = {}\n')
          routefile.write ('  tLSU[i] = {}\n')
          routefile.write ('  for j in range (1, %d + 1):\n' % nodecount)
          routefile.write ('    tLSU[i][j] = {}\n\n')
        else:
          # Append mode, append to already created file
          print 'Appending to dump file for time %3d seconds: %s' % (time, routefilename)
          routefile = open (routefilename, 'a')

      tuplename = words[2].split('=')[1]
      if tuplename == 'tLink':
        #split to get the value and pick only x from '192.168.0.x'
        from_id = invnodesmap[words[3].split('=')[1]]
        to_id = invnodesmap[words[4].split('=')[1]]
        cost = int (words[5].split('=')[1].rpartition ('.')[2])
        routefile.write ('tLink[%d][%d] = %d\n' % (from_id, to_id, cost))

      if tuplename == 'tLSU':
        #split to get the value and pick only x from '192.168.0.x'
        src_id = invnodesmap[words[3].split('=')[1]]
        from_id = invnodesmap[words[4].split('=')[1]]
        to_id = invnodesmap[words[5].split('=')[1]]
        cost = int (words[6].split('=')[1].rpartition ('.')[2])
        routefile.write ('tLSU[%d][%d][%d] = %d\n' % (src_id, from_id, to_id, cost))

# Parse all output log files from emulation run
def parse_all_outputs (dir, nodecount, nodesmap, blacklist):

  # Create inverse nodesmap
  invnodesmap = {}
  for key, value in nodesmap.iteritems ():
    invnodesmap[value] = key

  mode = 'w'
  for nodeid in range (1, nodecount + 1):
    if nodeid not in blacklist:
      output_filename = 'output_%s.log' % nodesmap [nodeid]
      #os.system ('ls -l %s' % output_filename)
      parse_output (dir, nodecount, invnodesmap, output_filename, mode)
      mode = 'a'


#dir = '/home/DBGROUP/xiaozhou/repos/ws_xz_mshivk_wintech/EmuLsPeriodic_23nodes_high_mobility_20090610011536'
#period = 5
#duration = 300
#nodecount = 24
#import sys
#sys.path.append ('./emulations')
#from nodesmap_clust import *
#blacklist = [4]
#parse_all_outputs (dir, nodecount, nodes, blacklist)

