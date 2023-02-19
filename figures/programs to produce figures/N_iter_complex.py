#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Produce the figure of the estimation of time complexity.

@author: Haorui Wang, Haozhan Tang
"""
from scipy.stats import linregress
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

logN = np.log2(N)
logNumAccessOracle = np.log2(NumAccessOracle)

# Colors and figure parameters setting.
colorChoice = ['red', 'orchid', 'gray', 'peru', 'steelblue']
matplotlib.rc('xtick', labelsize = 30)
matplotlib.rc('ytick', labelsize = 30)
plt.rcParams.update({'font.size': 15})
fig = plt.figure(figsize = (15, 10), dpi = 700)
ax = plt.subplot(111)
ax.tick_params(direction = 'in', top = True, right = True, width = 2, length = 6)
ax.scatter(logN, logNumAccessOracle, marker = ".", color = "orchid", s = 150)

# Using log scale, fit the line with function k * log(x) + b.
result = linregress(logN, logNumAccessOracle)
intercept = result.intercept
intercept_stderr = result.intercept_stderr
slope = result.slope
slope_stderr = result.stderr
print(slope, intercept)
print(result.rvalue ** 2)
print(slope_stderr, intercept_stderr)

# Uncertainty processing
slope_mult = 1
round_slope = slope
while slope_stderr < 10:
    round_slope *= 10
    slope_stderr *= 10
    slope_mult *= 10
int_slope = int(round(round_slope))
int_slope_stderr = int(round(slope_stderr))
slope_str = f"{int_slope / slope_mult}({int_slope_stderr})"
intercept_mult = 1
round_intercept = intercept
while intercept_stderr < 10:
    round_intercept *= 10
    intercept_stderr *= 10
    intercept_mult *= 10
int_intercept = int(round(round_intercept))
int_intercept_stderr = int(round(intercept_stderr))
intercept_str = f"{int_intercept / intercept_mult}({int_intercept_stderr})"
if int_intercept < 0:
    eqn_str = f"$\log_2T={slope_str}\log_2N{intercept_str}$"
else:
    eqn_str = f"$\log_2T={slope_str}\log_2N+{intercept_str}$"

# Produce the figure
Nspace = np.linspace(3, 16.5, 1000)
ax.plot(Nspace, slope * Nspace + intercept, '-', label=eqn_str, color = 'orchid', linewidth=2)
ax.set_ylabel(r"$\log_2(Number\ of\ Accesses\ to\ Oracle)$", fontsize = 30)
ax.legend(fontsize = 27, loc = 'upper left', fancybox = True, framealpha = 0.83)
ax.set_xticks(np.arange(1, 17, step=1))
ticklabels = [r"$2^{" + str(i) + "}$" for i in range(1, 17)]
ax.set_xticklabels(ticklabels, fontsize = 24)
ax.tick_params(axis = 'x', pad = 10)
plt.xlabel(r"$N$", fontsize = 38)

saved_file_name = "NIterComplex"
fig.savefig(f'SVG Figures/{saved_file_name}.svg', bbox_inches = 'tight')
