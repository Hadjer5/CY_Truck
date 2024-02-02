set terminal pngcairo enhanced font 'arial,10' size 800,600
set output './images/graphique_d1.png'

set style data histograms
set style histogram rowstacked
set boxwidth 0.75 relative 

set xtic rotate by -90

set datafile separator ';'
set xtics nomirror
set ytics nomirror
set xlabel "Noms des conducteurs"
set ylabel "Nombre de trajets"
set title "Graphique traitement D1"

plot "./temp/temp_d1.csv" using 2:xtic(1) with boxes notitle
