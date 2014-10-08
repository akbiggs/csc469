#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "tracker.eps"
set object 1 rect from 0.000000, 1 to 0.268126, 2 fc rgb "blue" fs solid noborder
set object 2 rect from 0.268126, 1 to 0.275076, 2 fc rgb "red" fs solid noborder
set object 3 rect from 0.275076, 1 to 4.067688, 2 fc rgb "blue" fs solid noborder
set object 4 rect from 4.067688, 1 to 4.070208, 2 fc rgb "red" fs solid noborder
set object 5 rect from 4.070208, 1 to 4.787818, 2 fc rgb "blue" fs solid noborder
set object 6 rect from 4.787818, 1 to 4.800037, 2 fc rgb "red" fs solid noborder
set object 7 rect from 4.800037, 1 to 7.867303, 2 fc rgb "blue" fs solid noborder
set object 8 rect from 7.867303, 1 to 7.874134, 2 fc rgb "red" fs solid noborder
set object 9 rect from 7.874134, 1 to 11.666928, 2 fc rgb "blue" fs solid noborder
set object 10 rect from 11.666928, 1 to 11.684844, 2 fc rgb "red" fs solid noborder
set object 11 rect from 11.684844, 1 to 15.466472, 2 fc rgb "blue" fs solid noborder
set object 12 rect from 15.466472, 1 to 15.474828, 2 fc rgb "red" fs solid noborder
set object 13 rect from 15.474828, 1 to 19.266081, 2 fc rgb "blue" fs solid noborder
set object 14 rect from 19.266081, 1 to 19.268688, 2 fc rgb "red" fs solid noborder
set object 15 rect from 19.268688, 1 to 19.986675, 2 fc rgb "blue" fs solid noborder
set object 16 rect from 19.986675, 1 to 20.006285, 2 fc rgb "red" fs solid noborder
set object 17 rect from 20.006285, 1 to 23.065695, 2 fc rgb "blue" fs solid noborder
set object 18 rect from 23.065695, 1 to 23.076847, 2 fc rgb "red" fs solid noborder
set object 19 rect from 23.076847, 1 to 26.567385, 2 fc rgb "blue" fs solid noborder
set object 20 rect from 26.567385, 1 to 26.573019, 2 fc rgb "red" fs solid noborder
plot [0.000154:26.573019] [0:3] 0
---EOF---
