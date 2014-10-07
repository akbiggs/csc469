#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 3.644264, 2 fc rgb "blue" fs solid
set object 2 rect from 3.644264, 1 to 3.652486, 2 fc rgb "red" fs solid
set object 3 rect from 3.652486, 1 to 5.448017, 2 fc rgb "blue" fs solid
set object 4 rect from 5.448017, 1 to 5.478938, 2 fc rgb "red" fs solid
set object 5 rect from 5.478938, 1 to 7.441473, 2 fc rgb "blue" fs solid
set object 6 rect from 7.441473, 1 to 7.444870, 2 fc rgb "red" fs solid
set object 7 rect from 7.444870, 1 to 8.585810, 2 fc rgb "blue" fs solid
set object 8 rect from 8.585810, 1 to 16.255150, 2 fc rgb "red" fs solid
set object 9 rect from 16.255150, 1 to 18.833008, 2 fc rgb "blue" fs solid
set object 10 rect from 18.833008, 1 to 18.838045, 2 fc rgb "red" fs solid
set object 11 rect from 18.838045, 1 to 22.630259, 2 fc rgb "blue" fs solid
set object 12 rect from 22.630259, 1 to 22.632597, 2 fc rgb "red" fs solid
set object 13 rect from 22.632597, 1 to 25.767509, 2 fc rgb "blue" fs solid
set object 14 rect from 25.767509, 1 to 25.797821, 2 fc rgb "red" fs solid
set object 15 rect from 25.797821, 1 to 26.427420, 2 fc rgb "blue" fs solid
set object 16 rect from 26.427420, 1 to 26.435169, 2 fc rgb "red" fs solid
set object 17 rect from 26.435169, 1 to 30.224628, 2 fc rgb "blue" fs solid
set object 18 rect from 30.224628, 1 to 30.227261, 2 fc rgb "red" fs solid
set object 19 rect from 30.227261, 1 to 34.021790, 2 fc rgb "blue" fs solid
set object 20 rect from 34.021790, 1 to 34.023861, 2 fc rgb "red" fs solid
plot [0:34.023861] [0:3] 0
---EOF---
