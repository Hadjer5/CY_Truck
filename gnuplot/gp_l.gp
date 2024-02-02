set terminal pngcairo enhanced font 'arial,10' size 800,600
set output './images/graphique_l.png'

set style data histograms
set style histogram rowstacked
set boxwidth 0.75 relative 

set xtic rotate by -45

set datafile separator ';'
set xtics nomirror
set ytics nomirror
set xlabel "N° id"
set ylabel "Distance en km"
set title "Graphique traitement L"

plot "./temp/temp_l.csv" using 2:xtic(1) lc rgb '#7FOOFF' with boxes notitle

