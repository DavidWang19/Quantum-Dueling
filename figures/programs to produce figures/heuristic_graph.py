#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Produce the probability curves of the algorithm under near-optimal parameters.

@author: Haozhan Tang, Haorui Wang
"""
import matplotlib
import matplotlib.pyplot as plt
import os

path = "./"
listDir = os.listdir(path)
fileNames, fileContents = [], []
for fileName in listDir:
    if "Heuristics" in fileName:
        fileNames.append(fileName.split('.')[0])
        with open(fileName) as f:
            lines = f.readlines()
            fileContents.append(lines)
# Manually choose 4 different sets of data.
graphNum = 3
curName, curData = fileNames[graphNum], fileContents[graphNum]

# Read simulation data.
problemsDataClassified = []
curProblem = []
for line in curData:
    if ("Problem" in line) and (len(curProblem) != 0):
        problemsDataClassified.append(curProblem)
        curProblem = []       
    if "G" in line and "." in line:
        curProblem.append(line)
problemsDataClassified.append(curProblem)

posibAll, gateAll = [], []
i = 0
limitCut = 100
for problem in problemsDataClassified:
    posibAll.append([])
    gateAll.append([])
    for line in problem[:limitCut]:
        gAndPo = line.split(' ')
        g = int(gAndPo[0].strip()[-1])
        pos = float(gAndPo[1].strip())
        posibAll[i].append(pos)
        gateAll[i].append(g) 
    i += 1

# The table of legends for 4 data sets.
legendNames = {
    'Heuristics_Output_for_all_N-18' : [r"$N\ =\ 256$", r"$N\ =\ 1024$", r"$N\ =\ 4096$", r"$N\ =\ 16384$"],
    'Heuristics_Output_1' : [r"$t\ =\ 1$", r"$t\ =\ 8$", r"$t\ =\ 0$"],
    'Heuristics_Output_2-18' : [r"$M\ =\ 4$", r"$M\ =\ 16$", r"$M\ =\ 32$", r"$M\ =\ 86$", r"$M\ =\ 256$"],
    'Heuristics_Output_3' : [r"$case\ 1$", r"$case\ 2$", r"$case\ 3$"]
    }

# Colors and figure parameters setting.
colorChoice = ['red', 'orchid', 'gray', 'peru', 'steelblue']
matplotlib.rc('xtick', labelsize = 30) 
matplotlib.rc('ytick', labelsize = 30)
plt.rcParams.update({'font.size': 15})

lgNames = legendNames[curName]
fig = plt.figure(figsize = (24, 24), dpi = 700)
ax = plt.subplot(111)
ax.tick_params(direction='in', top=True, right=True)
for prNum in range(len(posibAll)):
    problemNum = prNum
    legendName = lgNames[prNum]
    curPosib, curGate = posibAll[problemNum], gateAll[problemNum]
    g1Posib, g2Posib = [], []
    g1PosibIndex, g2PosibIndex, allIndex = [], [], []
    for i in range(len(curGate)):
        allIndex.append(i)
        if curGate[i] == 1:
            g1PosibIndex.append(i)
            g1Posib.append(curPosib[i])
        else:
            g2PosibIndex.append(i)
            g2Posib.append(curPosib[i])
    curColor = colorChoice[problemNum]
    ax.scatter(g1PosibIndex, g1Posib, marker = "8", color = curColor, s = 80)
    ax.scatter(g2PosibIndex, g2Posib, marker = "^", color = curColor, s = 150)
    ax.plot(allIndex, curPosib, '--', label = legendName, color = curColor, linewidth = 3)
    ax.set_ylabel(r"$Posibility$", fontsize = 38)
    ax.legend(fontsize = 27, loc = 'upper right', fancybox = True, framealpha = 0.83)
    #plt.xscale("log")
    plt.xlabel(r"$Number\ of\ Accesses\ to\ Oracle$", fontsize = 38)

fig.savefig(f'SVG Figures/{curName}.svg', bbox_inches = 'tight')
