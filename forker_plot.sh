#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set label "Inactive" at graph 0.95, graph 0.95 right tc lt 1 font "Arial,7"
set label "Child 1 Active" at graph 0.95, graph 0.9 right tc lt 3 font "Arial,7"
set label "Child 2 Active" at graph 0.95, graph 0.85 right tc lt 2 font "Arial,7"
set term postscript eps 10
set size 0.45,0.35
set output "forker.eps"
set object 1 rect from 3.855821, 2 to 7.619369, 3 fc rgb "blue" fs solid noborder
set object 2 rect from 7.619369, 2 to 7.622725, 3 fc rgb "red" fs solid noborder
set object 3 rect from 7.622725, 2 to 11.444450, 3 fc rgb "blue" fs solid noborder
set object 4 rect from 11.444450, 2 to 22.925514, 3 fc rgb "red" fs solid noborder
set object 5 rect from 22.925514, 2 to 23.090155, 3 fc rgb "blue" fs solid noborder
set object 6 rect from 23.090155, 2 to 23.110960, 3 fc rgb "red" fs solid noborder
set object 7 rect from 23.110960, 2 to 26.744749, 3 fc rgb "blue" fs solid noborder
set object 8 rect from 26.744749, 2 to 26.747950, 3 fc rgb "red" fs solid noborder
set object 9 rect from 26.747950, 2 to 30.569868, 3 fc rgb "blue" fs solid noborder
set object 10 rect from 30.569868, 2 to 32.767666, 3 fc rgb "red" fs solid noborder
set object 11 rect from 32.767666, 2 to 34.394886, 3 fc rgb "blue" fs solid noborder
set object 12 rect from 34.394886, 2 to 42.047909, 3 fc rgb "red" fs solid noborder
set object 13 rect from 42.047909, 2 to 42.374786, 3 fc rgb "blue" fs solid noborder
set object 14 rect from 42.374786, 2 to 42.401321, 3 fc rgb "red" fs solid noborder
set object 15 rect from 42.401321, 2 to 45.870159, 3 fc rgb "blue" fs solid noborder
set object 16 rect from 45.870159, 2 to 45.876987, 3 fc rgb "red" fs solid noborder
set object 17 rect from 45.876987, 2 to 49.695286, 3 fc rgb "blue" fs solid noborder
set object 18 rect from 49.695286, 2 to 49.697605, 3 fc rgb "red" fs solid noborder
set object 19 rect from 49.697605, 2 to 52.008766, 3 fc rgb "blue" fs solid noborder
set object 20 rect from 52.008766, 2 to 57.351082, 3 fc rgb "red" fs solid noborder
set object 21 rect from 0.217404, 1 to 3.779872, 2 fc rgb "green" fs solid noborder
set object 22 rect from 3.779872, 1 to 11.448641, 2 fc rgb "red" fs solid noborder
set object 23 rect from 11.448641, 1 to 13.423556, 2 fc rgb "green" fs solid noborder
set object 24 rect from 13.423556, 1 to 13.484150, 2 fc rgb "red" fs solid noborder
set object 25 rect from 13.484150, 1 to 15.269468, 2 fc rgb "green" fs solid noborder
set object 26 rect from 15.269468, 1 to 15.279020, 2 fc rgb "red" fs solid noborder
set object 27 rect from 15.279020, 1 to 19.094595, 2 fc rgb "green" fs solid noborder
set object 28 rect from 19.094595, 1 to 19.096987, 2 fc rgb "red" fs solid noborder
set object 29 rect from 19.096987, 1 to 22.919689, 2 fc rgb "green" fs solid noborder
set object 30 rect from 22.919689, 1 to 30.573008, 2 fc rgb "red" fs solid noborder
set object 31 rect from 30.573008, 1 to 32.718472, 2 fc rgb "green" fs solid noborder
set object 32 rect from 32.718472, 1 to 34.402336, 2 fc rgb "red" fs solid noborder
set object 33 rect from 34.402336, 1 to 38.220001, 2 fc rgb "green" fs solid noborder
set object 34 rect from 38.220001, 1 to 38.222378, 2 fc rgb "red" fs solid noborder
set object 35 rect from 38.222378, 1 to 42.045090, 2 fc rgb "green" fs solid noborder
set object 36 rect from 42.045090, 1 to 52.028900, 2 fc rgb "red" fs solid noborder
set object 37 rect from 52.028900, 1 to 53.520302, 2 fc rgb "green" fs solid noborder
set object 38 rect from 53.520302, 1 to 53.523022, 2 fc rgb "red" fs solid noborder
set object 39 rect from 53.523022, 1 to 57.345432, 2 fc rgb "green" fs solid noborder
set object 40 rect from 57.345432, 1 to 57.466881, 2 fc rgb "red" fs solid noborder
plot [3.855821:57.466881] [0:4] 0
---EOF---
