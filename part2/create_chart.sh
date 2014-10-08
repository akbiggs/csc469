#!/bin/sh

gnuplot << ---EOF---
set boxwidth 0.5
set style fill solid
set ylabel "Time (microseconds)"
set xlabel "CPU"
set nokey
set yrange [0:]
set size 0.65,0.45
set term postscript eps 10
set title "Average $1 Time"
set output "plot$1.eps"
plot "data$1.dat" using 1:3:xtic(2) with boxes
---EOF---
