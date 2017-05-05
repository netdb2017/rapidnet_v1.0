set term postscript  #png small xffffff
set size 0.6, 0.6
set output "%(dest_file)s"
set style line 1 lt 1 lw 3
set style line 3 lt 3 lw 3
set yrange [ 0 :  1.1]
set xlabel "Time (Seconds)"
set ylabel "Route Validity"
plot    "%(ls)s/validity.points" using ($1):($2) title "LS" w l ls 3, \
        "%(hsls)s/validity.points" using ($1):($2) title "HSLS" w l ls 1
