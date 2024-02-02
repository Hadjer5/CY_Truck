set terminal pngcairo enhanced font 'arial,10' size 800,600
set output './images/graphique_t.png'

set style data histograms
set style histogram cluster gap 1
set boxwidth 1

set xtic rotate by -45

set datafile separator ';'
set xtics nomirror
set ytics nomirror
set xlabel "Noms de villes"
set ylabel "Nombre de trajets"
set title "Graphique traitement T"

plot "./temp/temp_tf.csv" using 2:xtic(1) lc rgb '#7FOOFF' title 'Ville Trajet', '' using 3 lc rgb '#B266FF' title 'Ville Départ'

