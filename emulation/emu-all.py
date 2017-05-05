#!/usr/bin/python
import sys,os,threading,time
from time import strftime, gmtime

X_MIN=1
X_MAX=24

blacklisted_nodes=[]

application=['EmuLsPeriodic','EmuLsTriggered','EmuHslsPeriodic','EmuHslsTriggered']

app = 'EmuLsPeriodic'
testbed = 'cluster'
device = 'eth1'
duration =200


nodesNum=24
path='/home/DBGROUP/xiaozhou/RapidNet/trunk/'
topofile='topo/Discovery_randomwaypoint_24nodes_wifi_speed0.2-0.5_20091013175909/apptable/apptable_cluster_Discovery_randomwaypoint_24nodes_wifi_speed0.2-0.5_20091013175909'

dir = '%s_%.2dnodes_%s_%s' % (app, nodesNum,'clust_speed0.2-0.5_200',strftime ("%Y%m%d%H%M%S", gmtime ()))

def main():
	os.system ('mkdir %s%s' % (path,dir))
	for x in range(X_MIN ,X_MAX+1):
		currentNode='dbclust%d'%x
		if currentNode not in blacklisted_nodes:
			print currentNode
			t = threading.Thread(target=runlinkstate, args=(x,1))
			t.start()
	
def runlinkstate(x,y):	
	os.system('ssh xiaozhou@dbclust%d "cd %s ; python emulation/run_emulation.py %s %s %s %s %s %s %s"'%(x,path,app,dir,path,topofile,testbed,device,duration))
#	os.system('ssh xiaozhou@dbclust%d "cd %s ; python emulation/emu-hsls-triggered-clust.py %s %s" '%(x,path,dir,topofile))


main()

