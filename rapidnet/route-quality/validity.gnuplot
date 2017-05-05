set term postscript #png small xffffff
    set data style lines
    set grid
    set yrange [ 0 :  ]
    set title "Route Validity"
    set xlabel "Seconds"
    set ylabel "Route Validity"
    plot       "%s" using ($1):($2)
