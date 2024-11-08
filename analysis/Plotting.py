#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 16 13:34:25 2024

@author: Haoyuan Shi
Contact: hshi254@uwo.ca
"""

import matplotlib.pyplot as plt
import numpy as np

def get_cell_area():
    area_d = {}
    with open("../data/output/area.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        while line != "":
            while lst[0] != "ITER" and line!="":
                if lst[0] not in area_d:
                    area_d[lst[0]]=[float(lst[1])]
                else:
                    area_d[lst[0]].append(float(lst[1]))
                num_cells = int(lst[0][-1])+1
                line = file.readline()
                lst = line.strip("\n").split(" ")
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return area_d,num_cells

def get_cell_volume():
    volume_d = {}
    with open("../data/output/volume.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        while line != "":
            while lst[0] != "ITER" and line!="":
                if lst[0] not in volume_d:
                    volume_d[lst[0]]=[float(lst[1])]
                else:
                    volume_d[lst[0]].append(float(lst[1]))
                line = file.readline()
                lst = line.strip("\n").split(" ")
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return volume_d

def get_ave_volume():
    volume_l = []
    with open("../data/output/volume.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        tot_v = 0
        while line != "":
            if lst[0] == "ITER":
                if tot_v != 0:
                    volume_l.append(tot_v/64)
                    tot_v = 0
            else:
                tot_v += float(lst[1])
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return volume_l
    
def get_ave_area():
    area_l = []
    with open("../data/output/area.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        tot_a = 0
        while line != "":
            if lst[0] == "ITER":
                if tot_a != 0:
                    area_l.append(tot_a/64)
                    tot_a = 0
            else:
                tot_a += float(lst[1])
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return area_l

def get_cell_force():
    force_l = []
    with open("../data/output/force.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        iterf = []
        while line != "":
            if lst[0] == "ITER":
                if iterf != []:
                    force_l.append(np.average(iterf))
                iterf = []
            else:
                iterf.append(float(lst[1]))
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return force_l

def get_var():
    var_d = {}
    with open("../conf") as file:
        line = file.readline()
        while line != "":
            lst = line.strip().split(",")
            if lst[0] == "dt":
                var_d["dt"] = float(lst[1])
            # elif lst[0] == "kv"
            #     var_d["kv"] = float(lst[1])
            elif lst[0] == "v0":
                var_d["v0"] = float(lst[1])
            elif lst[0] == "s0":
                var_d["s0"] = float(lst[1])
            line = file.readline()
    return var_d
                        
                
            

vol = get_ave_volume()
are = get_ave_area()
force = get_cell_force()



var_d = get_var()
dt = var_d["dt"]

fig, ax=plt.subplots(2,1,sharex=True)

length = len(vol)
v0 = float(var_d["v0"])
v0_l = np.full((1,length),v0)[0]
s0 = float(var_d["s0"])
s0_l = np.full((1,length),s0)[0]
t = np.arange(0,length,1)

ax[0].set_title("Change in Volume dt="+str(dt))
#plt.xlabel("time(s)")
ax[0].set_ylabel("rel. volume")
ax[0].plot(t,vol)
#ax[0].plot(t,v0_l,label="$v_0$",ls="dashed")
ax[0].legend()

ax[1].set_title("Change in Surface Area dt="+str(dt))
ax[1].set_xlabel("N")
ax[1].set_ylabel("rel. surf. area")
plt.plot(t,are)
#plt.plot(t,s0_l,label="$s_0$",ls="dashed")
ax[1].legend()

fig.savefig("../data/output/plot0.png")

plt.figure()
plt.title("Max Force")
plt.xlabel("N")
plt.ylabel("force")
plt.plot(np.arange(0,500,1),force)
#plt.legend()
plt.savefig("../data/output/plot1.png")
    
    
    


