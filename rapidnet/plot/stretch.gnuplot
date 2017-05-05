set term postscript enhanced #png small xffffff
set size 0.6, 0.6
set output "%(dest_file)s"
set style line 1 lt 1 lw 3
set style line 3 lt 3 lw 3
set xrange [ 0 :  ]
set yrange [ 1 :  ]
set xlabel "Time (Seconds)"
set ylabel "Route Stretch"
plot    "%(ls)s/stretch.points" using ($1):($2) title "LS" w l ls 3, \
        "%(hsls)s/stretch.points" using ($1):($2) title "HSLS" w l ls 1
