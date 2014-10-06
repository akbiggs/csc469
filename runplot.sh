#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 88.911621, 2 fc rgb "blue" fs solid
set object 2 rect from 88.911621, 1 to 89.175797, 2 fc rgb "red" fs solid
set object 3 rect from 89.175797, 1 to 182.013245, 2 fc rgb "blue" fs solid
set object 4 rect from 182.013245, 1 to 182.074066, 2 fc rgb "red" fs solid
set object 5 rect from 182.074066, 1 to 210.501556, 2 fc rgb "blue" fs solid
set object 6 rect from 210.501556, 1 to 407.792084, 2 fc rgb "red" fs solid
set object 7 rect from 407.792084, 1 to 461.310364, 2 fc rgb "blue" fs solid
set object 8 rect from 461.310364, 1 to 461.579803, 2 fc rgb "red" fs solid
set object 9 rect from 461.579803, 1 to 534.578247, 2 fc rgb "blue" fs solid
set object 10 rect from 534.578247, 1 to 597.478699, 2 fc rgb "red" fs solid
set object 11 rect from 597.478699, 1 to 647.505798, 2 fc rgb "blue" fs solid
set object 12 rect from 647.505798, 1 to 647.732239, 2 fc rgb "red" fs solid
set object 13 rect from 647.732239, 1 to 719.973816, 2 fc rgb "blue" fs solid
set object 14 rect from 719.973816, 1 to 728.764648, 2 fc rgb "red" fs solid
set object 15 rect from 728.764648, 1 to 740.605896, 2 fc rgb "blue" fs solid
set object 16 rect from 740.605896, 1 to 740.802795, 2 fc rgb "red" fs solid
set object 17 rect from 740.802795, 1 to 823.310486, 2 fc rgb "blue" fs solid
set object 18 rect from 823.310486, 1 to 824.532959, 2 fc rgb "red" fs solid
set object 19 rect from 824.532959, 1 to 833.703674, 2 fc rgb "blue" fs solid
set object 20 rect from 833.703674, 1 to 834.153503, 2 fc rgb "red" fs solid
plot [0:834.153503] [0:3] 0
---EOF---
