#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 16 05:24:08 2019

@author: yeaseen
"""

import random
import math
import os
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
        
    
   






attacker=source//4
lstattackers=random.choices(lstsource,k=attacker)




# define the name of the directory to be created
#import os
path = "SC1"+str(inp)
print(path)

try:  
    os.makedirs(path)
except OSError:  
    print ("Creation of the directory %s failed" % path)
else:  
    print ("Successfully created the directory %s" % path)


f= open(path+"/data.txt","w+")
f.write(str(inp)+"\n")
f.write(str(source)+"\n")
for i in lstsource:
    print(str(i))
    f.write(str(i)+"\n")

f.write(str(attacker)+"\n")
for i in lstattackers:
    print(str(i))
    f.write(str(i)+"\n")


f.close()


f= open(path+"/scn.tcl","w+")

for i in range(len(cx)):
    #print(i,lstX[i])
    f.write("$node_("+str(i)+") set X_ "+str(cx[i])+"\n")
    f.write("$node_("+str(i)+") set Y_ "+str(cy[i])+"\n")
    f.write("$node_("+str(i)+") set Z_ "+'0'+"\n")

f.close()



f= open(path+"/cbr-exp1.tcl","w+")

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
    




f= open(path+"/wireless14_1.tcl","w+")

f.write("set val(chan)       Channel/WirelessChannel\n\
set val(prop)       Propagation/TwoRayGround\n\
set val(netif)      Phy/WirelessPhy\n\
set val(mac)        Mac/802_11\n\
set val(ifq)        Queue/DropTail/PriQueue\n\
set val(ll)         LL\n\
set val(ant)        Antenna/OmniAntenna\n\
set val(x)              800   ;# X dimension of the topography\n\
set val(y)              800   ;# Y dimension of the topography\n\
set val(ifqlen)         50            ;# max packet in ifq\n\
set val(seed)           0.0\n\
set val(adhocRouting)   AODV\n\
set val(nn)             "+str(inp)+"\n\
set val(cp)             \"cbr-exp1.tcl\" \n\
set val(sc)             \"scn.tcl\"\n\
set val(stop)           500.0           ;# simulation time\n\
\n\
set ns_     [new Simulator]\n\
\n\
# setup topography object\n\
\n\
set topo    [new Topography]\n\
\n\
# create trace object for ns and nam\n\
\n\
set tracefd [open wireless14-out1.tr w]\n\
$ns_ use-newtrace\n\
$ns_ trace-all $tracefd\n\
\n\
# define topology\n\
$topo load_flatgrid $val(x) $val(y)\n\
\n\
set god_ [create-god $val(nn)]\n\
\n\
$ns_ node-config -adhocRouting $val(adhocRouting) \\\n\
                 -llType $val(ll) \\\n\
                 -macType $val(mac) \\\n\
                 -ifqType $val(ifq) \\\n\
                 -ifqLen $val(ifqlen) \\\n\
                 -antType $val(ant) \\\n\
                 -propType $val(prop) \\\n\
                 -phyType $val(netif) \\\n\
                 -channelType $val(chan) \\\n\
         -topoInstance $topo \\\n\
         -agentTrace ON \\\n\
                 -routerTrace ON \\\n\
                 -macTrace ON \n\
#  Create the specified number of nodes [$val(nn)] and attach them\n\
#  to the channel.\n\
\n\
for {set i 0} {$i < $val(nn) } {incr i} {\n\
    set node_($i) [$ns_ node]   \n\
    $node_($i) random-motion 0      ;# disable random motion\n\
    $node_($i) radius 1         ;# radius of each node....\n}\n")
    
f.write("\n\
puts \"Loading connection pattern...\"\n\
source $val(cp)\n\
\n\
puts \"Loading scenario file...\"\n\
source $val(sc)\n\
\n\
for {set i 0} {$i < $val(nn)} {incr i} {\n\
    $ns_ initial_node_pos $node_($i) 20\n}\n")

f.write("for {set i 0} {$i < $val(nn) } {incr i} {\n\
    $ns_ at $val(stop).0 \"$node_($i) reset\";\n}\n\
$ns_ at  $val(stop).0002 \"puts \\\"NS EXITING... \\\" ; $ns_ halt\"\n\
\n\
puts $tracefd \"M 0.0 nn $val(nn) x $val(x) y $val(y) rp $val(adhocRouting)\"\n\
puts $tracefd \"M 0.0 sc $val(sc) cp $val(cp) seed $val(seed)\"\n\
puts $tracefd \"M 0.0 prop $val(prop) ant $val(ant)\"\n\
\n\
puts \"Starting Simulation...\"\n\
$ns_ run\n")
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

    
    
    
    
    
    
    
    
    
    
    
    
    
    