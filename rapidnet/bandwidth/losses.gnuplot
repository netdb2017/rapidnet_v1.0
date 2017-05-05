set term postscript
    set data style lines
    set grid
    set yrange [ 0 : 100 ]
    set xlabel "Seconds"
    set ylabel "Loss (%%)"
    plot    "%s" using 1:($2) title "Loss (%%)"
