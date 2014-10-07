#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 2.603056, 2 fc rgb "blue" fs solid
set object 2 rect from 2.603056, 1 to 2.608500, 2 fc rgb "red" fs solid
set object 3 rect from 2.608500, 1 to 2.713578, 2 fc rgb "blue" fs solid
set object 4 rect from 2.713578, 1 to 2.720646, 2 fc rgb "red" fs solid
set object 5 rect from 2.720646, 1 to 3.541701, 2 fc rgb "blue" fs solid
set object 6 rect from 3.541701, 1 to 3.548409, 2 fc rgb "red" fs solid
set object 7 rect from 3.548409, 1 to 7.334851, 2 fc rgb "blue" fs solid
set object 8 rect from 7.334851, 1 to 7.483495, 2 fc rgb "red" fs solid
set object 9 rect from 7.483495, 1 to 9.418221, 2 fc rgb "blue" fs solid
set object 10 rect from 9.418221, 1 to 9.421017, 2 fc rgb "red" fs solid
set object 11 rect from 9.421017, 1 to 11.128135, 2 fc rgb "blue" fs solid
set object 12 rect from 11.128135, 1 to 11.135014, 2 fc rgb "red" fs solid
set object 13 rect from 11.135014, 1 to 13.149680, 2 fc rgb "blue" fs solid
set object 14 rect from 13.149680, 1 to 13.152692, 2 fc rgb "red" fs solid
set object 15 rect from 13.152692, 1 to 14.921424, 2 fc rgb "blue" fs solid
set object 16 rect from 14.921424, 1 to 15.040742, 2 fc rgb "red" fs solid
set object 17 rect from 15.040742, 1 to 18.008902, 2 fc rgb "blue" fs solid
set object 18 rect from 18.008902, 1 to 18.014275, 2 fc rgb "red" fs solid
set object 19 rect from 18.014275, 1 to 18.714828, 2 fc rgb "blue" fs solid
set object 20 rect from 18.714828, 1 to 18.742907, 2 fc rgb "red" fs solid
plot [0:18.742907] [0:3] 0
---EOF---
