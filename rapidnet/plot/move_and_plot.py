#!/usr/bin/python

import os, sys

app_pairs = {'periodic': ('SimLsPeriodic', 'SimHslsPeriodic'), 'triggered': ('SimLsTriggered', 'SimHslsTriggered')}

if len (sys.argv) != 6:
  print 'Usage: ./move_and_plot <nodes> <duration> <speed_low> <speed_up> <timestamp>'
  sys.exit (0)

nodes = sys.argv[1]
duration = sys.argv[2]
speed_low = sys.argv[3]
speed_up = sys.argv[4]
timestamp = sys.argv[5]

def get_app_dir (prefix, suffix):
  entries = os.listdir (".")
  for entry in entries:
    if entry.startswith (prefix) and entry.endswith (suffix):
      return entry
  print 'No output folder found of the form %s*%s' % (prefix, suffix)
  return None

def move_and_plot (timestamp, app_pair, pair_name):
  ls_app = app_pair[0]
  hsls_app = app_pair[1]

  ls_dir = get_app_dir (ls_app, timestamp)
  hsls_dir = get_app_dir (hsls_app, timestamp)

  dest_dir = 'run_%snodes_%s-%s_%ssec_%s' % (nodes, speed_low, speed_up, duration, timestamp)

  # Create dest_dir
  if not os.path.exists (dest_dir):
    os.system ('mkdir %s' % dest_dir)

  # Move to dest_dir
  os.system ('mv %s %s' % (ls_dir, dest_dir))
  os.system ('mv %s %s' % (hsls_dir, dest_dir))

  # Set new path names
  ls_dir = os.path.join (dest_dir, ls_dir)
  hsls_dir = os.path.join (dest_dir, hsls_dir)

  # Plot
  dest_dir = os.path.join (dest_dir, pair_name)
  os.system ('mkdir %s' % dest_dir)
  os.system ('./rapidnet/plot/plot.py %s %s %s' % (ls_dir, hsls_dir, dest_dir))

  # Compute average
  os.system ('./rapidnet/plot/avg.py %s %s %s %s %s' % (ls_dir, hsls_dir, dest_dir, nodes, duration))


for name, pair in app_pairs.iteritems ():
  move_and_plot (timestamp, pair, name)

