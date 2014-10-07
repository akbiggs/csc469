#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 3.763998, 2 fc rgb "blue" fs solid
set object 2 rect from 3.763998, 1 to 3.769041, 2 fc rgb "red" fs solid
set object 3 rect from 3.769041, 1 to 7.559436, 2 fc rgb "blue" fs solid
set object 4 rect from 7.559436, 1 to 7.561602, 2 fc rgb "red" fs solid
set object 5 rect from 7.561602, 1 to 11.354785, 2 fc rgb "blue" fs solid
set object 6 rect from 11.354785, 1 to 11.369248, 2 fc rgb "red" fs solid
set object 7 rect from 11.369248, 1 to 15.150197, 2 fc rgb "blue" fs solid
set object 8 rect from 15.150197, 1 to 15.157317, 2 fc rgb "red" fs solid
set object 9 rect from 15.157317, 1 to 18.945583, 2 fc rgb "blue" fs solid
set object 10 rect from 18.945583, 1 to 18.947601, 2 fc rgb "red" fs solid
set object 11 rect from 18.947601, 1 to 22.740967, 2 fc rgb "blue" fs solid
set object 12 rect from 22.740967, 1 to 22.748672, 2 fc rgb "red" fs solid
set object 13 rect from 22.748672, 1 to 26.536379, 2 fc rgb "blue" fs solid
set object 14 rect from 26.536379, 1 to 26.538910, 2 fc rgb "red" fs solid
set object 15 rect from 26.538910, 1 to 30.331808, 2 fc rgb "blue" fs solid
set object 16 rect from 30.331808, 1 to 30.333738, 2 fc rgb "red" fs solid
set object 17 rect from 30.333738, 1 to 34.127178, 2 fc rgb "blue" fs solid
set object 18 rect from 34.127178, 1 to 34.131935, 2 fc rgb "red" fs solid
set object 19 rect from 34.131935, 1 to 37.922604, 2 fc rgb "blue" fs solid
set object 20 rect from 37.922604, 1 to 37.924999, 2 fc rgb "red" fs solid
plot [0:37.924999] [0:3] 0
---EOF---
