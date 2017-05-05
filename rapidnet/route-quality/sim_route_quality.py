#!/usr/bin/python
import os, sys
from sim_parse_output import *
from compute_route_quality import *

# Global variables
VALIDITY_GNUPLOT = './rapidnet/route-quality/validity.gnuplot'
STRETCH_GNUPLOT = './rapidnet/route-quality/stretch.gnuplot'

# Dir should contain dir/output.log - the log file for parsing
# Check arguments
if len (sys.argv) != 5:
  print 'Usage: sim_route_quality <dir> <apptable-period> <duration> <node-count>'
  sys.exit (0)


# Read command line arguments
dir = sys.argv[1]
period = int (sys.argv[2])
base = period
duration = int (sys.argv[3])
nodecount = int (sys.argv[4])

# Parse the output file
parse_output (dir, nodecount)
times = range (base+period, duration+period, period)


route_quality = {}
# Compute route quality for every time slot
for time in times:
  routes_file = os.path.join (dir, 'routes/route_%3.3d.py' % time)
  if os.path.exists (routes_file):
    execfile (routes_file)
    #print 'Processing for time:', time
    route_quality[time] = computeRouteQuality (tLink, tLSU, nodecount)
  else:
    print 'Route file', routes_file, 'not found for time:', time


# Dump validity to files for plotting
validity_filename = os.path.join (dir, 'validity.points')
validity_file = open (validity_filename, 'w')
stretch_filename = os.path.join (dir, 'stretch.points')
stretch_file = open (stretch_filename, 'w')

print 'Writing validity data to ', validity_filename
print 'Writing stretch data to ', stretch_filename
for time in times:
  validity_file.write ('%d %f %d %d %d %d %.1f %d\n' % (time, route_quality[time][0], route_quality[time][2], \
    route_quality[time][3], route_quality[time][4], route_quality[time][5], route_quality[time][6], \
    route_quality[time][7]))
  stretch_file.write ('%d %f\n' % (time, route_quality[time][1]))

validity_file.close ()
stretch_file.close ()

# Plot validity using gnuplot
gnuplot_script = open (VALIDITY_GNUPLOT, 'r').read ()
open ('temp.gnuplot', 'w').write (gnuplot_script % validity_filename)
validity_imagefile = os.path.join (dir, 'validity.ps')
print 'Plotting validity to file: ', validity_imagefile
os.system ('gnuplot temp.gnuplot > %s' % validity_imagefile)


# Plot stretch using gnuplot
gnuplot_script = open (STRETCH_GNUPLOT, 'r').read ()
open ('temp.gnuplot', 'w').write (gnuplot_script % stretch_filename)
stretch_imagefile = os.path.join (dir, 'stretch.ps')
print 'Plotting stretch to file: ', stretch_imagefile
os.system ('gnuplot temp.gnuplot > %s' % stretch_imagefile)

os.system ('rm temp.gnuplot')

