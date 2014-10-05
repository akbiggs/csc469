#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set term postscript eps 10
set size 0.45,0.35
set output "activeperiods.eps"
set object 1 rect from 0.000000, 1 to 0.004340, 2 fc rgb "blue" fs solid
set object 2 rect from 0.004340, 1 to 0.005810, 2 fc rgb "red" fs solid
plot [0:0.005810] [0:3] 0
---EOF---
