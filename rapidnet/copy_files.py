#!/usr/bin/python
import os, sys

if len (sys.argv) != 3:
  print 'Usage: <run_folder> <user>'
  raise SystemExit

user = sys.argv[2]
host = 'dbclust%d.cis.upenn.edu'
path = '/home/DBGROUP/mshivk/temp/trunk'
run_folder = sys.argv[1]

app_files =  ['events.log'] #('*.points', 'decorator.log', '*.ps', '*.pcap.trace')

folders = {
  'SimLsPeriodic': app_files,
  'SimHslsPeriodic': app_files,
  'SimLsTriggered': app_files,
  'SimHslsTriggered': app_files,
  'periodic': ('*'),
  'triggered': ('*'),
}

os.system ('mkdir %s' % run_folder)
source = '%s@%s:%s/%s' % (user, host, path, run_folder)

for folder in folders:
  target = os.path.join (run_folder, folder)
  os.system ('mkdir %s' % target)
  clust_id = 0
  for file in folders[folder]:
    clust_id = 15 + (clust_id + 1) % 24
    os.system ('scp %s/%s*/%s %s' % (source % clust_id, folder, file, target))

