set term postscript  #png small xffffff
set size 0.6, 0.6
set output "%(dest_file)s"
set style line 1 lt 1 lw 3
set style line 3 lt 3 lw 3
set yrange [ 0 :  ]
set xlabel "Time (Seconds)"
set ylabel "Per-node Bandwidth Utilization (KBps)"
plot    "%(ls)s/bandwidth-recd.points" using ($1):($2)/1000 title "LS (received)" w l ls 3, \
        "%(hsls)s/bandwidth-recd.points" using ($1):($2)/1000 title "HSLS (received)" w l ls 1
