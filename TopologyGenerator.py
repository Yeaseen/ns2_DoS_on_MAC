#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 16 05:24:08 2019

@author: yeaseen
"""

import random
import math
#inp=int(input("Enter number of Node: "))
#length=int(input("Enter Length: "))
inp=50
length=625
lstX=random.sample(range(length*length), inp)

cx=[]
cy=[]
for i in range(len(lstX)):
    #print(int(lstX[i])//length)
    cx.append(int(lstX[i])//length)
    #print(int(lstX[i])%length)
    cy.append(int(lstX[i])%length)

f= open("SC1/scn.tcl","w+")

for i in range(len(cx)):
    #print(i,lstX[i])
    f.write("$node_("+str(i)+") set X_ "+str(cx[i])+"\n")
    f.write("$node_("+str(i)+") set Y_ "+str(cy[i])+"\n")
    f.write("$node_("+str(i)+") set Z_ "+'0'+"\n")

f.close()


print("hello")


source=inp//4
lstsource=[]
lstrecvr=[]
listofzeros = [0] * inp

listvalues=list(range(0,inp-1))

while(len(listvalues)>0 and len(lstsource)<source):
    ransrc=random.choice(listvalues)
    if listofzeros[ransrc]==0:
        #print(ransrc)
        temprecv=[]
        for i in range(inp):
            if i!=ransrc:
                dist = math.sqrt((cx[i]-cx[ransrc])*(cx[i]-cx[ransrc]) + (cy[i]-cy[ransrc])*(cy[i]-cy[ransrc]))
                if dist<250:
                    if listofzeros[i]==0:
                        temprecv.append(i)
        if len(temprecv)==0:
            listofzeros[ransrc]=1
            continue
        ranrecv=random.choice(temprecv)
        lstrecvr.append(ranrecv)
        listofzeros[ranrecv]=1
        
        lstsource.append(ransrc)
        listofzeros[ransrc]=1
    else:
        listvalues.remove(ransrc)
        
    
   
 
f= open("SC1/cbr-exp1.tcl","w+")

for i in range(len(lstsource)):
    #print(i,lstX[i])
    f.write("set udp_("+str(i)+") [new Agent/UDP]\n")
    f.write("$ns_ attach-agent $node_("+str(lstsource[i])+") $udp_("+str(i)+")\n")
    f.write("set null_("+str(i)+") [new Agent/Null]\n")
    f.write("$ns_ attach-agent $node_("+str(lstrecvr[i])+") $null_("+str(i)+")\n")
    f.write("set cbr_("+str(i)+") [new Application/Traffic/CBR]\n")
    f.write("$cbr_("+str(i)+") set packetSize_ 1024\n")
    f.write("$cbr_("+str(i)+") set interval_ 0.04\n")
    f.write("$cbr_("+str(i)+") set random_ 1\n")
    f.write("$cbr_("+str(i)+") set maxpkts_ 100000\n")
    f.write("$cbr_("+str(i)+") attach-agent $udp_("+str(i)+")\n")
    f.write("$ns_ connect $udp_("+str(i)+") $null_("+str(i)+")\n\n")
    
    
    start=random.randint(0,50)
    end=500
    
    f.write("$ns_ at "+str(start)+  " \"$cbr_("+str(i)+") start\"\n")
    f.write("$ns_ at "+str(end)+" \"$cbr_("+str(i)+") stop\"\n\n\n\n\n")
    

f.close()  
    
    

sx=[]
sy=[]
rx=[]
ry=[]

for i in lstsource:
    sx.append(cx[i])
    sy.append(cy[i])
for i in lstrecvr:
    rx.append(cx[i])
    ry.append(cy[i])
    
    
import matplotlib.pyplot as plt
import networkx as nx
    

G = nx.Graph()

plt.scatter(cx, cy,)
plt.scatter(sx, sy, color='red')
plt.scatter(rx, ry, color='green')

plt.grid(True)
#plt.savefig("points.png")
plt.show()

    
    
    
    
    
    
    
    
    
    
    
    
    
    