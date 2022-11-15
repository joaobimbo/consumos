# ----- sender.py ------

#!/usr/bin/env python

from socket import *
import sys
import numpy as np
import time
import math

s = socket(AF_INET,SOCK_DGRAM)
host = "127.0.0.1"
port = 8543
buf =2048*5
addr = (host,port)

#file_name=sys.argv[2]

#s.sendto(file_name,addr)

#f=open(file_name,"rb")
#data = f.read(buf)
while True:
    # sampling rate
    sr = 2048
    # sampling interval
    ts = 1.0/sr
    t = np.arange(0,1,ts)

    freq = 50
    data = 3*np.sin(2*np.pi*freq*t)
    #data=np.random.randn(5)   
    print(len(data))
    s.sendto(data,addr)
    time.sleep(1)
    #if(s.sendto(data,addr)):
    #    data = f.read(buf)
    #s.close()
    #f.close()
