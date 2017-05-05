#!/usr/bin/python
import sys,os

X_MIN=1
X_MAX=24

for x in range(X_MIN ,X_MAX+1):
	print "dbclust",x
	os.system("ssh xiaozhou@dbclust%d sudo ifconfig eth1 promisc"%x)
