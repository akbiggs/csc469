#!/bin/sh
gnuplot << ---EOF---
set title "Active and Inactive Periods"
set xlabel "Time (ms)"
set nokey
set noytics
set label "Inactive" at screen 0.2, screen 1 tc lt 1
set label "Child 1 Active" at screen 0.5, screen 0.5 left tc lt 3
set label "Child 2 Active" at screen -0.01, screen -0.01 left tc lt 2
set term postscript eps 10
set size 0.45,0.35
set output "forker.eps"
set object 1 rect from 0.254843, 2 to 0.254843, 3 fc rgb "blue" fs solid noborder
set object 2 rect from 0.254843, 2 to 0.255075, 3 fc rgb "red" fs solid noborder
set object 3 rect from 0.255075, 2 to 3.548315, 3 fc rgb "blue" fs solid noborder
set object 4 rect from 3.548315, 2 to 7.159985, 3 fc rgb "red" fs solid noborder
set object 5 rect from 7.159985, 2 to 10.688174, 3 fc rgb "blue" fs solid noborder
set object 6 rect from 10.688174, 2 to 10.695210, 3 fc rgb "red" fs solid noborder
set object 7 rect from 10.695210, 2 to 14.257655, 3 fc rgb "blue" fs solid noborder
set object 8 rect from 14.257655, 2 to 14.263725, 3 fc rgb "red" fs solid noborder
set object 9 rect from 14.263725, 2 to 17.827477, 3 fc rgb "blue" fs solid noborder
set object 10 rect from 17.827477, 2 to 17.917547, 3 fc rgb "red" fs solid noborder
set object 11 rect from 17.917547, 2 to 21.397326, 3 fc rgb "blue" fs solid noborder
set object 12 rect from 21.397326, 2 to 21.418831, 3 fc rgb "red" fs solid noborder
set object 13 rect from 21.418831, 2 to 24.967176, 3 fc rgb "blue" fs solid noborder
set object 14 rect from 24.967176, 2 to 24.976063, 3 fc rgb "red" fs solid noborder
set object 15 rect from 24.976063, 2 to 28.536961, 3 fc rgb "blue" fs solid noborder
set object 16 rect from 28.536961, 2 to 28.555099, 3 fc rgb "red" fs solid noborder
set object 17 rect from 28.555099, 2 to 32.106808, 3 fc rgb "blue" fs solid noborder
set object 18 rect from 32.106808, 2 to 32.114220, 3 fc rgb "red" fs solid noborder
set object 19 rect from 32.114220, 2 to 35.676682, 3 fc rgb "blue" fs solid noborder
set object 20 rect from 35.676682, 2 to 35.676941, 3 fc rgb "red" fs solid noborder
set object 21 rect from 35.676941, 2 to 39.246529, 3 fc rgb "blue" fs solid noborder
set object 22 rect from 39.246529, 2 to 39.246788, 3 fc rgb "red" fs solid noborder
set object 23 rect from 39.246788, 2 to 39.316322, 3 fc rgb "blue" fs solid noborder
set object 24 rect from 39.316322, 2 to 39.336544, 3 fc rgb "red" fs solid noborder
set object 25 rect from 39.336544, 2 to 42.816292, 3 fc rgb "blue" fs solid noborder
set object 26 rect from 42.816292, 2 to 42.841770, 3 fc rgb "red" fs solid noborder
set object 27 rect from 42.841770, 2 to 46.386070, 3 fc rgb "blue" fs solid noborder
set object 28 rect from 46.386070, 2 to 46.393566, 3 fc rgb "red" fs solid noborder
set object 29 rect from 46.393566, 2 to 49.956116, 3 fc rgb "blue" fs solid noborder
set object 30 rect from 49.956116, 2 to 49.974159, 3 fc rgb "red" fs solid noborder
set object 31 rect from 49.974159, 2 to 53.525818, 3 fc rgb "blue" fs solid noborder
set object 32 rect from 53.525818, 2 to 53.533756, 3 fc rgb "red" fs solid noborder
set object 33 rect from 53.533756, 2 to 57.095619, 3 fc rgb "blue" fs solid noborder
set object 34 rect from 57.095619, 2 to 57.106567, 3 fc rgb "red" fs solid noborder
set object 35 rect from 57.106567, 2 to 60.665375, 3 fc rgb "blue" fs solid noborder
set object 36 rect from 60.665375, 2 to 60.679649, 3 fc rgb "red" fs solid noborder
set object 37 rect from 60.679649, 2 to 61.451157, 3 fc rgb "blue" fs solid noborder
set object 38 rect from 61.451157, 2 to 61.452198, 3 fc rgb "red" fs solid noborder
set object 39 rect from 61.452198, 2 to 64.235252, 3 fc rgb "blue" fs solid noborder
set object 40 rect from 64.235252, 2 to 64.240700, 3 fc rgb "red" fs solid noborder
set object 41 rect from 64.240700, 2 to 64.305641, 3 fc rgb "blue" fs solid noborder
set object 42 rect from 64.305641, 2 to 64.305916, 3 fc rgb "red" fs solid noborder
set object 43 rect from 64.305916, 2 to 64.313042, 3 fc rgb "blue" fs solid noborder
set object 44 rect from 64.313042, 2 to 64.322227, 3 fc rgb "red" fs solid noborder
set object 45 rect from 64.322227, 2 to 64.343857, 3 fc rgb "blue" fs solid noborder
set object 46 rect from 64.343857, 2 to 64.344254, 3 fc rgb "red" fs solid noborder
set object 47 rect from 64.344254, 2 to 67.805244, 3 fc rgb "blue" fs solid noborder
set object 48 rect from 67.805244, 2 to 67.811745, 3 fc rgb "red" fs solid noborder
set object 49 rect from 67.811745, 2 to 71.374847, 3 fc rgb "blue" fs solid noborder
set object 50 rect from 71.374847, 2 to 71.387009, 3 fc rgb "red" fs solid noborder
set object 51 rect from 71.387009, 2 to 71.404320, 3 fc rgb "blue" fs solid noborder
set object 52 rect from 71.404320, 2 to 71.406136, 3 fc rgb "red" fs solid noborder
set object 53 rect from 71.406136, 2 to 71.437187, 3 fc rgb "blue" fs solid noborder
set object 54 rect from 71.437187, 2 to 71.438995, 3 fc rgb "red" fs solid noborder
set object 55 rect from 71.438995, 2 to 72.554367, 3 fc rgb "blue" fs solid noborder
set object 56 rect from 72.554367, 2 to 72.556114, 3 fc rgb "red" fs solid noborder
set object 57 rect from 72.556114, 2 to 74.944901, 3 fc rgb "blue" fs solid noborder
set object 58 rect from 74.944901, 2 to 74.950890, 3 fc rgb "red" fs solid noborder
set object 59 rect from 74.950890, 2 to 78.514557, 3 fc rgb "blue" fs solid noborder
set object 60 rect from 78.514557, 2 to 78.519684, 3 fc rgb "red" fs solid noborder
set object 61 rect from 3.643683, 1 to 7.118021, 2 fc rgb "green" fs solid noborder
set object 62 rect from 7.118021, 1 to 17.839830, 2 fc rgb "red" fs solid noborder
set object 63 rect from 17.839830, 1 to 17.909491, 2 fc rgb "green" fs solid noborder
set object 64 rect from 17.909491, 1 to 17.910162, 2 fc rgb "red" fs solid noborder
set object 65 rect from 17.910162, 1 to 17.910202, 2 fc rgb "green" fs solid noborder
set object 66 rect from 17.910202, 1 to 17.911350, 2 fc rgb "red" fs solid noborder
set object 67 rect from 17.911350, 1 to 17.911642, 2 fc rgb "green" fs solid noborder
set object 68 rect from 17.911642, 1 to 17.911932, 2 fc rgb "red" fs solid noborder
set object 69 rect from 17.911932, 1 to 17.911999, 2 fc rgb "green" fs solid noborder
set object 70 rect from 17.911999, 1 to 17.912230, 2 fc rgb "red" fs solid noborder
set object 71 rect from 17.912230, 1 to 17.915594, 2 fc rgb "green" fs solid noborder
set object 72 rect from 17.915594, 1 to 17.915840, 2 fc rgb "red" fs solid noborder
set object 73 rect from 17.915840, 1 to 17.916012, 2 fc rgb "green" fs solid noborder
set object 74 rect from 17.916012, 1 to 17.916370, 2 fc rgb "red" fs solid noborder
set object 75 rect from 17.916370, 1 to 21.397366, 2 fc rgb "green" fs solid noborder
set object 76 rect from 21.397366, 1 to 21.397635, 2 fc rgb "red" fs solid noborder
set object 77 rect from 21.397635, 1 to 24.967213, 2 fc rgb "green" fs solid noborder
set object 78 rect from 24.967213, 1 to 24.975250, 2 fc rgb "red" fs solid noborder
set object 79 rect from 24.975250, 1 to 28.536997, 2 fc rgb "green" fs solid noborder
set object 80 rect from 28.536997, 1 to 28.537262, 2 fc rgb "red" fs solid noborder
set object 81 rect from 28.537262, 1 to 32.106766, 2 fc rgb "green" fs solid noborder
set object 82 rect from 32.106766, 1 to 32.114494, 2 fc rgb "red" fs solid noborder
set object 83 rect from 32.114494, 1 to 35.676624, 2 fc rgb "green" fs solid noborder
set object 84 rect from 35.676624, 1 to 35.684170, 2 fc rgb "red" fs solid noborder
set object 85 rect from 35.684170, 1 to 39.246487, 2 fc rgb "green" fs solid noborder
set object 86 rect from 39.246487, 1 to 39.261017, 2 fc rgb "red" fs solid noborder
set object 87 rect from 39.261017, 1 to 39.316360, 2 fc rgb "green" fs solid noborder
set object 88 rect from 39.316360, 1 to 39.316628, 2 fc rgb "red" fs solid noborder
set object 89 rect from 39.316628, 1 to 39.321098, 2 fc rgb "green" fs solid noborder
set object 90 rect from 39.321098, 1 to 39.321301, 2 fc rgb "red" fs solid noborder
set object 91 rect from 39.321301, 1 to 39.321819, 2 fc rgb "green" fs solid noborder
set object 92 rect from 39.321819, 1 to 39.322018, 2 fc rgb "red" fs solid noborder
set object 93 rect from 39.322018, 1 to 39.322269, 2 fc rgb "green" fs solid noborder
set object 94 rect from 39.322269, 1 to 39.322666, 2 fc rgb "red" fs solid noborder
set object 95 rect from 39.322666, 1 to 39.324013, 2 fc rgb "green" fs solid noborder
set object 96 rect from 39.324013, 1 to 39.324211, 2 fc rgb "red" fs solid noborder
set object 97 rect from 39.324211, 1 to 39.335762, 2 fc rgb "green" fs solid noborder
set object 98 rect from 39.335762, 1 to 39.335995, 2 fc rgb "red" fs solid noborder
set object 99 rect from 39.335995, 1 to 39.336983, 2 fc rgb "green" fs solid noborder
set object 100 rect from 39.336983, 1 to 39.337242, 2 fc rgb "red" fs solid noborder
set object 101 rect from 39.337242, 1 to 39.340584, 2 fc rgb "green" fs solid noborder
set object 102 rect from 39.340584, 1 to 39.340809, 2 fc rgb "red" fs solid noborder
set object 103 rect from 39.340809, 1 to 39.343246, 2 fc rgb "green" fs solid noborder
set object 104 rect from 39.343246, 1 to 39.343437, 2 fc rgb "red" fs solid noborder
set object 105 rect from 39.343437, 1 to 39.350857, 2 fc rgb "green" fs solid noborder
set object 106 rect from 39.350857, 1 to 39.351234, 2 fc rgb "red" fs solid noborder
set object 107 rect from 39.351234, 1 to 42.816219, 2 fc rgb "green" fs solid noborder
set object 108 rect from 42.816219, 1 to 42.822102, 2 fc rgb "red" fs solid noborder
set object 109 rect from 42.822102, 1 to 46.386105, 2 fc rgb "green" fs solid noborder
set object 110 rect from 46.386105, 1 to 46.391312, 2 fc rgb "red" fs solid noborder
set object 111 rect from 46.391312, 1 to 49.956112, 2 fc rgb "green" fs solid noborder
set object 112 rect from 49.956112, 1 to 49.967415, 2 fc rgb "red" fs solid noborder
set object 113 rect from 49.967415, 1 to 49.970161, 2 fc rgb "green" fs solid noborder
set object 114 rect from 49.970161, 1 to 49.971630, 2 fc rgb "red" fs solid noborder
set object 115 rect from 49.971630, 1 to 53.525932, 2 fc rgb "green" fs solid noborder
set object 116 rect from 53.525932, 1 to 53.531498, 2 fc rgb "red" fs solid noborder
set object 117 rect from 53.531498, 1 to 57.095787, 2 fc rgb "green" fs solid noborder
set object 118 rect from 57.095787, 1 to 57.105923, 2 fc rgb "red" fs solid noborder
set object 119 rect from 57.105923, 1 to 60.665642, 2 fc rgb "green" fs solid noborder
set object 120 rect from 60.665642, 1 to 60.677227, 2 fc rgb "red" fs solid noborder
plot [0.254843:60.677227] [0:4] 0
---EOF---
