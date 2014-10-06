#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 86.410591, 2 fc rgb "blue" fs solid
set object 2 rect from 86.410591, 1 to 89.216820, 2 fc rgb "red" fs solid
set object 3 rect from 89.216820, 1 to 136.763824, 2 fc rgb "blue" fs solid
set object 4 rect from 136.763824, 1 to 138.384323, 2 fc rgb "red" fs solid
set object 5 rect from 138.384323, 1 to 179.511642, 2 fc rgb "blue" fs solid
set object 6 rect from 179.511642, 1 to 179.818039, 2 fc rgb "red" fs solid
set object 7 rect from 179.818039, 1 to 210.476364, 2 fc rgb "blue" fs solid
set object 8 rect from 210.476364, 1 to 419.564972, 2 fc rgb "red" fs solid
set object 9 rect from 419.564972, 1 to 458.805847, 2 fc rgb "blue" fs solid
set object 10 rect from 458.805847, 1 to 459.147766, 2 fc rgb "red" fs solid
set object 11 rect from 459.147766, 1 to 551.906311, 2 fc rgb "blue" fs solid
set object 12 rect from 551.906311, 1 to 551.979126, 2 fc rgb "red" fs solid
set object 13 rect from 551.979126, 1 to 645.003601, 2 fc rgb "blue" fs solid
set object 14 rect from 645.003601, 1 to 645.077271, 2 fc rgb "red" fs solid
set object 15 rect from 645.077271, 1 to 738.103027, 2 fc rgb "blue" fs solid
set object 16 rect from 738.103027, 1 to 738.678467, 2 fc rgb "red" fs solid
set object 17 rect from 738.678467, 1 to 831.201660, 2 fc rgb "blue" fs solid
set object 18 rect from 831.201660, 1 to 831.450500, 2 fc rgb "red" fs solid
set object 19 rect from 831.450500, 1 to 924.301270, 2 fc rgb "blue" fs solid
set object 20 rect from 924.301270, 1 to 924.355774, 2 fc rgb "red" fs solid
set object 21 rect from 924.355774, 1 to 1017.399414, 2 fc rgb "blue" fs solid
set object 22 rect from 1017.399414, 1 to 1017.749756, 2 fc rgb "red" fs solid
set object 23 rect from 1017.749756, 1 to 1110.498535, 2 fc rgb "blue" fs solid
set object 24 rect from 1110.498535, 1 to 1110.716675, 2 fc rgb "red" fs solid
set object 25 rect from 1110.716675, 1 to 1203.598267, 2 fc rgb "blue" fs solid
set object 26 rect from 1203.598267, 1 to 1203.669067, 2 fc rgb "red" fs solid
set object 27 rect from 1203.669067, 1 to 1296.697388, 2 fc rgb "blue" fs solid
set object 28 rect from 1296.697388, 1 to 1297.021118, 2 fc rgb "red" fs solid
set object 29 rect from 1297.021118, 1 to 1389.796143, 2 fc rgb "blue" fs solid
set object 30 rect from 1389.796143, 1 to 1389.926270, 2 fc rgb "red" fs solid
set object 31 rect from 1389.926270, 1 to 1482.894531, 2 fc rgb "blue" fs solid
set object 32 rect from 1482.894531, 1 to 1482.942383, 2 fc rgb "red" fs solid
set object 33 rect from 1482.942383, 1 to 1575.993774, 2 fc rgb "blue" fs solid
set object 34 rect from 1575.993774, 1 to 1576.321167, 2 fc rgb "red" fs solid
set object 35 rect from 1576.321167, 1 to 1669.093140, 2 fc rgb "blue" fs solid
set object 36 rect from 1669.093140, 1 to 1669.241943, 2 fc rgb "red" fs solid
set object 37 rect from 1669.241943, 1 to 1762.191895, 2 fc rgb "blue" fs solid
set object 38 rect from 1762.191895, 1 to 1762.256836, 2 fc rgb "red" fs solid
set object 39 rect from 1762.256836, 1 to 1855.291260, 2 fc rgb "blue" fs solid
set object 40 rect from 1855.291260, 1 to 1855.707153, 2 fc rgb "red" fs solid
plot [0:1855.707153] [0:3] 0
---EOF---
