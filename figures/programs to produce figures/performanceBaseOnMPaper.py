#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Produce the figure of the numbers of iterations to achieve the maximum probability as a function of M, which is in logarithmic scale.

@author: Haozhan Tang, Haorui Wang
"""
import matplotlib
import matplotlib.pyplot as plt

# Read simulation data.
with open('Output_For_All_M.txt') as f:
    lines = f.readlines()
M = []
P, P1 = [], []
r, r1 = [], []
for i in range(len(lines)):
    if i == 0:
        continue
    print(lines[i])
    print('___')
    elements = lines[i].split()
    M.append(float(elements[0]))
    P.append(float(elements[1]))
    r.append(float(elements[2]))
    P1.append(float(elements[3]))
    r1.append(float(elements[4]))

# # Figure parameters setting.
matplotlib.rc('xtick', labelsize = 24) 
matplotlib.rc('ytick', labelsize = 24)
plt.rcParams.update({'font.size': 15})

# Produce the figure
fig = plt.figure(figsize = (8, 16), dpi = 1200)
ax = plt.subplot(111)
ax.tick_params(direction='in', top=True, right=True)
ax.plot(M, r, '--', marker = "o", label = r"$r$", linewidth = 3, markersize = 10)
ax.plot(M, r1, '--', marker = "s", color = 'orchid', label = r"$r'$", linewidth = 4, markersize = 8)
ax.set_ylabel(r"$Number\ of\ Iterations$", fontsize = 32)
ax.legend(fontsize = 27, loc = 'upper right', fancybox = True, framealpha = 0.83)
plt.xscale("log")
plt.xlabel(r"$M$", fontsize = 32)

fig.savefig('SVG Figures/r_byMAll.svg', bbox_inches = 'tight')
