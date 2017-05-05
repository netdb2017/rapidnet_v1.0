import os, sys

# Parse and dump the tLinks and tLSUs graph data structures in python scripts
# dir: directory where to find output.log and place the generated files
# nodecount: number of nodes
def parse_output (dir, nodecount):

  output_file = os.path.join (dir, 'output.log')
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
        routefile = open (routefilename, 'w')
        print 'Dumping links for time %3d seconds in %s' % (time, routefilename)
        routefile.write ('tLink = {}\n')
        routefile.write ('tLSU = {}\n')
        routefile.write ('for i in range (1, %d + 1):\n' % nodecount)
        routefile.write ('  tLink[i] = {}\n')
        routefile.write ('  tLSU[i] = {}\n')
        routefile.write ('  for j in range (1, %d + 1):\n' % nodecount)
        routefile.write ('    tLSU[i][j] = {}\n\n')

      tuplename = words[2].split('=')[1]
      if tuplename == 'tLink':
        #split to get the value and pick only x from '192.168.0.x'
        from_id = int (words[3].split('=')[1].rpartition ('.')[2])
        to_id = int (words[4].split('=')[1].rpartition ('.')[2])
        cost = int (words[5].split('=')[1].rpartition ('.')[2])
        routefile.write ('tLink[%d][%d] = %d\n' % (from_id, to_id, cost))

      if tuplename == 'tLSU':
        src_id = int (words[3].split('=')[1].rpartition ('.')[2])
        from_id = int (words[4].split('=')[1].rpartition ('.')[2])
        to_id = int (words[5].split('=')[1].rpartition ('.')[2])
        cost = int (words[6].split('=')[1].rpartition ('.')[2])
        routefile.write ('tLSU[%d][%d][%d] = %d\n' % (src_id, from_id, to_id, cost))

