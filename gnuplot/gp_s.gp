set terminal pngcairo enhanced font 'arial,10' size 1200,600
set output './images/graphique_s.png'

set xtic rotate by -45

set datafile separator ';'
set xlabel "Identifiants trajets"
set ylabel "Distances en km"
set title "Graphique traitement S"

plot "./temp/temp_sf.csv" using 0:3:xtic(1) with lines lc rgb '#7FOOFF' lw 2 title 'Max', "./temp/temp_sf.csv" using 0:4:xtic(1) with lines lw 1 title 'Moy', "./temp/temp_sf.csv" using 0:2:xtic(1) with lines lc rgb '#7FOOFF' lw 2 title 'Min'
