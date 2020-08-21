set terminal pdf enhanced
set output "plot.pdf"

set title "Parallel Patterns (2048-10240-5)" offset 0,-.5
set xlabel "Threads"
set ylabel "Time (ms)"

set key box
set key outside top center vertical maxrows 2 maxcols 6
set key autotitle columnheader
set key samplen 3 spacing 1 font ",9"

set style data histogram
set style histogram cluster gap 2 errorbars linewidth 1
set style fill solid border rgb "black"

set logscale y 3
set auto x

plot for [i=2:12:2] "data.dat" using i:i+1:xticlabels(1) title col(i) linewidth 1
# plot "data.dat" using 2:8:xticlabels(1) title col linewidth 2 linecolor "#ff0000" fillstyle pattern 1, \
#      "data.dat" using 3:9:xticlabels(1) title col linewidth 2 linecolor "#00c000" fillstyle pattern 6, \
#      "data.dat" using 4:10:xticlabels(1) title col linewidth 2 linecolor "#4169e1" fillstyle pattern 4, \
#      "data.dat" using 5:11:xticlabels(1) title col linewidth 2 linecolor "#8b0000" fillstyle pattern 2, \
#      "data.dat" using 6:12:xticlabels(1) title col linewidth 2 linecolor "#00eeee" fillstyle pattern 3, \
#      "data.dat" using 7:13:xticlabels(1) title col linewidth 2 linecolor "#556b2f" fillstyle pattern 5