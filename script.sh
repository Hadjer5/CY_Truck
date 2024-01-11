#!/bin/bash

input_file="$1"

 ###Option d1
 
#gsub supprime le retour à la ligne après le nom du conducteur
#NR>1 permet d'ignorer la premiere ligne du csv
#count[$6] s'incremente a chaque ligne du csv et compte le nombre de trajet de chaque conducteur(compte le nb d'occurence)
#la boucle for affiche chaque conducteur suivi du nombre de trajet qu'il a effectué
awk -F';' 'NR > 1 {
    gsub(/\r/, "", $6)
    count[$6]++
}
END {
    for (driver in count) print driver ";" count[driver]}
' "$input_file" > temp.csv

#tri decroissant en fonction de la deuxième colonne
sort -t ';' -k2 -n -r -o temp.csv temp.csv

#10 premiers conducteurs
head -n 10 temp.csv > temp_d1.csv
#supp fichier temporaire
rm temp.csv
cat temp_d1.csv

