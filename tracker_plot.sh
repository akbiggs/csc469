#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set label "Active Periods" at screen 0.5,0.5
set label "Inactive" at 0.5,2.8 tc lt 1
set label "Active" at 0.5,2.5 tc lt 3
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "tracker.eps"
set object 1 rect from 0.000000, 1 to 3.467425, 2 fc rgb "blue" fs solid
set object 2 rect from 3.467425, 1 to 3.472302, 2 fc rgb "red" fs solid
set object 3 rect from 3.472302, 1 to 7.294680, 2 fc rgb "blue" fs solid
set object 4 rect from 7.294680, 1 to 7.296787, 2 fc rgb "red" fs solid
set object 5 rect from 7.296787, 1 to 8.580172, 2 fc rgb "blue" fs solid
set object 6 rect from 8.580172, 1 to 8.582322, 2 fc rgb "red" fs solid
set object 7 rect from 8.582322, 1 to 9.669817, 2 fc rgb "blue" fs solid
set object 8 rect from 9.669817, 1 to 16.552505, 2 fc rgb "red" fs solid
set object 9 rect from 16.552505, 1 to 18.776464, 2 fc rgb "blue" fs solid
set object 10 rect from 18.776464, 1 to 18.781996, 2 fc rgb "red" fs solid
set object 11 rect from 18.781996, 1 to 22.603691, 2 fc rgb "blue" fs solid
set object 12 rect from 22.603691, 1 to 23.233747, 2 fc rgb "red" fs solid
set object 13 rect from 23.233747, 1 to 24.233063, 2 fc rgb "blue" fs solid
set object 14 rect from 24.233063, 1 to 24.236546, 2 fc rgb "red" fs solid
set object 15 rect from 24.236546, 1 to 26.430882, 2 fc rgb "blue" fs solid
set object 16 rect from 26.430882, 1 to 26.435081, 2 fc rgb "red" fs solid
set object 17 rect from 26.435081, 1 to 30.258137, 2 fc rgb "blue" fs solid
set object 18 rect from 30.258137, 1 to 30.651892, 2 fc rgb "red" fs solid
set object 19 rect from 30.651892, 1 to 34.085403, 2 fc rgb "blue" fs solid
set object 20 rect from 34.085403, 1 to 34.095951, 2 fc rgb "red" fs solid
plot [0.000204:34.095951] [0:3] 0
---EOF---
