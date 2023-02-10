#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Produce the figure of the estimation of time complexity.

@author: Haorui Wang, Haozhan Tang
"""
from scipy.optimize import curve_fit
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os

# Read simulation data.
path = "./"
listDir = os.listdir(path)
fileContents = []
for fileName in listDir:
    if "Complexity_data" in fileName:
        with open(fileName) as f:
            lines = f.readlines()
            fileContents.append(lines)
N, NumAccessOracle = [], []
for fileContent in fileContents:
    for curLine in fileContent:
        if "N: " in curLine:
            N.append(int(curLine.split(",")[0][3:]))
        if "Target probability" in curLine:
            NumAccessOracle.append(int(curLine.split(" ")[5]))
print(N)
print(NumAccessOracle)

# Colors and figure parameters setting.
colorChoice = ['red', 'orchid', 'gray', 'peru', 'steelblue']
matplotlib.rc('xtick', labelsize = 30) 
matplotlib.rc('ytick', labelsize = 30)
plt.rcParams.update({'font.size': 15})
fig = plt.figure(figsize = (15, 10), dpi = 700)
ax = plt.subplot(111)
ax.tick_params(direction = 'in', top = True, right = True, width = 2, length = 6)
ax.scatter(N, NumAccessOracle, marker = "^", color = "orchid", s = 150)

# Fit the line with function a*sqrt(x) + b.
def fit(x, a, b):
    return a*np.sqrt(x) + b


popt, pcov = curve_fit(fit, N, NumAccessOracle)
perr = np.sqrt(np.diag(pcov))
print(popt)
print(perr)

# Produce the figure
Nspace = np.linspace(1, 65536, 65536)
ax.plot(Nspace, fit(Nspace, *popt), '-', label=r'~$\sqrt{N}$', color = 'orchid', linewidth=2)
ax.set_ylabel(r"$Number\ of\ Accesses\ to\ Oracle$", fontsize = 30)
ax.legend(fontsize = 27, loc = 'upper right', fancybox = True, framealpha = 0.83)
ax.set_xticks(np.arange(0, 70000, step=15000))
#plt.xscale("log")
plt.xlabel(r"$N$", fontsize = 38)

saved_file_name = "NIterComplex"
fig.savefig(f'SVG Figures/{saved_file_name}.svg', bbox_inches = 'tight')
