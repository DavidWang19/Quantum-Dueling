#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Produce the figure of the success probabilities of finding the most optimized solution as a function of number of iterations performed.

@author: Haozhan Tang, Haorui Wang
"""
import matplotlib
import matplotlib.pyplot as plt

# Read simulation data.
with open('Combined_Output.txt') as f:
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

x = []
for i in range(len(probi)):
    x.append(i)

# Data selection.
P_1 = [line[0] for line in probi]
P_17 = [line[16] for line in probi]
P_33 = [line[32] for line in probi]

# Figure parameters setting.
matplotlib.rc('xtick', labelsize = 24) 
matplotlib.rc('ytick', labelsize = 24)
plt.rcParams.update({'font.size': 15})
left, width = 0.1, 0.65
bottom, height = 0.1, 0.65
spacing = 0.005
rect_scatter = [left, bottom, width, height]

# Produce the figure.
fig = plt.figure(figsize = (16, 8), dpi = 1200)
plt.rcParams['text.usetex'] = False
ax = plt.subplot(111)
ax.tick_params(direction='in', top=True, right=True)
ax.plot(x, P_1, '--', marker = "o", label = r"$P_1$", linewidth = 3, markersize = 9)
ax.plot(x, P_17, '--', marker = "s", color = 'orchid', label = r"$P_{17}$", linewidth = 4, markersize = 8)
ax.plot(x, P_33, '--', marker = "^", label = r"$P_{33}$", linewidth = 3, markersize = 10)
plt.ylabel(r"$Probability$", fontsize = 32)
plt.xlabel(r"$Iteration\ Number$", fontsize = 32)
ax.legend(fontsize = 27, loc = 'upper right', fancybox = True, framealpha = 0.83)

fig.savefig('SVG Figures/combined_output.svg')
