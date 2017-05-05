#!/usr/bin/python

import os, sys

scripts_dir = 'rapidnet/plot'

scripts = ['bandwidth-recd.gnuplot', 'bandwidth-sent.gnuplot', 'stretch.gnuplot', 'validity.gnuplot']

if len (sys.argv) != 4:
  print 'Usage: ./plot <ls_dir> <hsls_dir> <dest_dir>'
  sys.exit (0)

args = {}
args['ls'] = sys.argv[1]
args['hsls'] = sys.argv[2]
dest_dir = sys.argv[3]

def plot (script_file, args, dest_dir):
  args ['dest_file'] = os.path.join (dest_dir, script_file.replace ('.gnuplot', '.ps'))
  script = open (os.path.join (scripts_dir, script_file), 'r').read ()
  script = script % args
  tempfile = open ('temp.gnuplot', 'w')
  tempfile.write (script)
  tempfile.close ()
  os.system ('gnuplot temp.gnuplot')
  os.system ('rm temp.gnuplot')

for script_file in scripts:
  plot (script_file, args, dest_dir)

