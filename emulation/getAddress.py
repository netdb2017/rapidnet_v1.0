import os,sys


def getAddress(interface):
  address={}

  p = os.popen("sudo ifconfig %s" %interface)
  p = os.popen("sudo ifconfig %s" %interface)
  macline = p.readline()
  ipline = p.readline()
	
  macline = macline.strip()
  ipline = ipline.strip()
  mac = macline.split(" ")[-1]
  ipaddr = ipline.split(" ")[1].split(":")[1]
  ipmask = ipline.split(" ")[5].split(":")[1]
	
  addr=ipaddr.split(".")
  mask=ipmask.split(".")
	
  ipnet=str(int(addr[0])&int(mask[0]))+"."+str(int(addr[1])&int(mask[1]))+"."+str(int(addr[2])&int(mask[2]))+"."+str(int(addr[3])&int(mask[3]))
  net=ipnet.split(".")
  ipbase=str(int(addr[0])^int(net[0]))+"."+str(int(addr[1])^int(net[1]))+"."+str(int(addr[2])^int(net[2]))+"."+str(int(addr[3])^int(net[3]))		
	
  address["mac"]=mac
  address["ipaddr"]=ipaddr
  address["ipnet"]=ipnet
  address["ipmask"]=ipmask
  address["ipbase"]=ipbase
		
  return address

if __name__ == "__main__":
  
  if len (sys.argv) != 2:
    print 'usage: python getAggress.py <interface (example: eth0)>'
    exit()

  interface = sys.argv[1]
  address = getAddress(interface) 
  print address
