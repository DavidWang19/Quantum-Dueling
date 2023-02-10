#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Produce a bar plot of the (local) maximum success probability of measuring the most optimized solution.

@author: Haozhan Tang, Haorui Wang
"""

import matplotlib
import matplotlib.pyplot as plt

# Read simulation data.
with open('First_Output.txt') as f:
    lines = f.readlines()
probLine = []
probi = []
for i in range(len(lines)):
    if 'No.' in lines[i]:
        print(lines[i])
        print(lines[i + 1])
        print('___')
        probLine.append(lines[i])
        probi.append(lines[i + 1].split(' '))
for line in probi:
    for i in range(len(line)):
        line[i] = float(line[i].strip())

# Figure parameters setting
matplotlib.rc('xtick', labelsize = 24) 
matplotlib.rc('ytick', labelsize = 24)
plt.rcParams.update({'font.size': 15})

firstMaxLoc = int(lines[-1].strip())
firstMax = probi[firstMaxLoc]

x = []
for i in range(len(firstMax)):
    x.append(i + 1)

# Produce the figure.
fig = plt.figure(figsize = (16, 4), dpi = 1200)
plt.rcParams['text.usetex'] = False
ax = plt.subplot(111)
toNum = 100
ax.tick_params(direction='in', top=True, right=True)
ax.bar(x[:toNum], firstMax[:toNum], color = 'orchid')
ax.set_yscale('log')
plt.ylabel(r"$Probability$", fontsize = 32)
plt.xlabel(r"$Solution\ Index$", fontsize = 32)

fig.savefig('SVG Figures/first_output_bar.svg', bbox_inches = 'tight')
