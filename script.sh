#!/bin/bash

input_file="$1"
opt="$2"

#Options sans C (-d1,-d2,-L)
 
#gsub supprime le retour à la ligne après le nom du conducteur
#NR>1 permet d'ignorer la premiere ligne du csv
#count[$6] s'incremente a chaque ligne du csv et compte le nombre de trajet de chaque conducteur(compte le nb d'occurence)
#la boucle for affiche chaque conducteur suivi du nombre de trajet qu'il a effectué
if [ "$opt" = "-d1" ]; then 
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
  
elif [ "$opt" = "-d2" ]; then
  awk -F';' 'NR > 1 {
      gsub(/\r/, "", $6)
      somme[$6] += $5
  }
  END {
      for (driver in somme) print driver ";" somme[driver]}
  ' "$input_file" > temp.csv
  #tri decroissant en fonction de la deuxième colonne
  sort -t ';' -k2 -n -r -o temp.csv temp.csv
  #10 premiers conducteurs
  head -n 10 temp.csv > temp_d2.csv
  #supp fichier temporaire
  rm temp.csv

elif [ "$opt" = "-l" ]; then
  awk -F';' 'NR > 1 {
      gsub(/\r/, "", $6)
      somme[$1] += $5
  }
  END {
      for (id in somme) print id ";" somme[id]}
  ' "$input_file" > temp.csv
  #tri decroissant en fonction de la deuxième colonne
  sort -t ';' -k2 -n -r -o temp.csv temp.csv
  #10 premiers conducteurs
  head -n 10 temp.csv > temp_l.csv
  #classer les id dans l'odre croissant
  sort -t ';' -k1 -n -o temp_l.csv temp_l.csv
  #supp fichier temporaire
  rm temp.csv
fi


###Verification validite (csv existe, nombre d'arguments etc)

###h

###Faire une liste? de tous les arguments d'options entrés pour avoir tous les traitements à faire (A faire en début de script apres verifications)


#fonction pour les 10 premiers, paramètre temp.csv(sort et rm)

###Options avec C
##c et makefile


##ajouter le temps d'exec

