set term postscript #png small xffffff
    set data style lines
    set grid
    set yrange [ 1 :  ]
    set xrange [ 0 :  ]
    set title "Route Stretch"
    set xlabel "Seconds"
    set ylabel "Route Stretch"
    plot       "%s" using ($1):($2) 
