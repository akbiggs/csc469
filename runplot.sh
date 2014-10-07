#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 3.513222, 2 fc rgb "blue" fs solid noborder
set object 2 rect from 3.513222, 1 to 3.516594, 2 fc rgb "red" fs solid noborder
set object 3 rect from 3.516594, 1 to 7.050795, 2 fc rgb "blue" fs solid noborder
set object 4 rect from 7.050795, 1 to 7.053007, 2 fc rgb "red" fs solid noborder
set object 5 rect from 7.053007, 1 to 10.588362, 2 fc rgb "blue" fs solid noborder
set object 6 rect from 10.588362, 1 to 10.593687, 2 fc rgb "red" fs solid noborder
set object 7 rect from 10.593687, 1 to 14.125911, 2 fc rgb "blue" fs solid noborder
set object 8 rect from 14.125911, 1 to 14.128172, 2 fc rgb "red" fs solid noborder
set object 9 rect from 14.128172, 1 to 17.663469, 2 fc rgb "blue" fs solid noborder
set object 10 rect from 17.663469, 1 to 17.665590, 2 fc rgb "red" fs solid noborder
set object 11 rect from 17.665590, 1 to 21.201075, 2 fc rgb "blue" fs solid noborder
set object 12 rect from 21.201075, 1 to 21.206993, 2 fc rgb "red" fs solid noborder
set object 13 rect from 21.206993, 1 to 24.738604, 2 fc rgb "blue" fs solid noborder
set object 14 rect from 24.738604, 1 to 24.740835, 2 fc rgb "red" fs solid noborder
set object 15 rect from 24.740835, 1 to 28.276173, 2 fc rgb "blue" fs solid noborder
set object 16 rect from 28.276173, 1 to 28.278275, 2 fc rgb "red" fs solid noborder
set object 17 rect from 28.278275, 1 to 31.813808, 2 fc rgb "blue" fs solid noborder
set object 18 rect from 31.813808, 1 to 31.818861, 2 fc rgb "red" fs solid noborder
set object 19 rect from 31.818861, 1 to 35.351330, 2 fc rgb "blue" fs solid noborder
set object 20 rect from 35.351330, 1 to 35.353561, 2 fc rgb "red" fs solid noborder
plot [0.000046:35.353561] [0:3] 0
---EOF---
