set term postscript #png small xffffff
    set data style lines
    set grid
    set yrange [ 0 :  ]
    set xlabel "Seconds"
    set ylabel "Bytes/Second"
    plot    "%s" using ($1):($2) title "%s"
