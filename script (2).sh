#!/bin/bash

input_file="$1"
shift 1



# Vérifie si le dossier existe
if [ ! -d "temp" ]; then
    # Crée le dossier s'il n'existe pas
    mkdir "temp"
fi

if [ ! -d "image" ]; then
    mkdir "image"
fi



###Options sans C (-d1,-d2,-L)

##gestion du nombre d'argument -> shift 1
 
#gsub supprime le retour à la ligne après le nom du conducteur
#NR>1 permet d'ignorer la premiere ligne du csv
#count[$6] s'incremente a chaque ligne du csv et compte le nombre de trajet de chaque conducteur(compte le nb d'occurence)
#la boucle for affiche chaque conducteur suivi du nombre de trajet qu'il a effectué


i=0
while [ True ]; do
  if [ "$1" = "-d1" ]; then 
    awk -F';' 'NR > 1 {
        gsub(/\r/, "", $6)
        count[$6]++
    }
    END {
        for (driver in count) print driver ";" count[driver]}
    ' "$input_file" > temp/temp.csv
   #tri decroissant en fonction de la deuxième colonne
    sort -t ';' -k2 -n -r -o temp/temp.csv temp/temp.csv
    #10 premiers conducteurs
    head -n 10 temp/temp.csv > temp/temp_d1.csv
    #supp fichier temporaire
    rm temp/temp.csv

    file[i]="temp_d1.csv"
    i=$((i+1))
    shift 1
  
  elif [ "$1" = "-d2" ]; then
    awk -F';' 'NR > 1 {
        gsub(/\r/, "", $6)
        somme[$6] += $5
    }
    END {
        for (driver in somme) print driver ";" somme[driver]}
    ' "$input_file" > temp/temp.csv
    #tri decroissant en fonction de la deuxième colonne
    sort -t ';' -k2 -n -r -o temp/temp.csv temp/temp.csv
    #10 premiers conducteurs
    head -n 10 temp/temp.csv > temp/temp_d2.csv
    #supp fichier temporaire
    rm temp/temp.csv

    file[i]="temp_d2.csv"
    i=$((i+1))
    shift 1

  elif [ "$1" = "-l" ]; then
    awk -F';' 'NR > 1 {
        gsub(/\r/, "", $6)
        somme[$1] += $5
    }
    END {
        for (id in somme) print id ";" somme[id]}
    ' "$input_file" >temp/temp.csv
    #tri decroissant en fonction de la deuxième colonne
    sort -t ';' -k2 -n -r -o temp/temp.csv temp/temp.csv
    #10 premiers conducteurs
    head -n 10 temp/temp.csv > temp/temp_l.csv
    #classer les id dans l'odre croissant
    sort -t ';' -k1 -n -o temp/temp_l.csv temp/temp_l.csv
    #supp fichier temporaire
    rm temp/temp.csv
    
    file[i]="temp_l.csv"
    i=$((i+1))
    shift 1

  elif [ "$1" = "-t" ]; then
    awk -F';' 'NR > 1 {
      printf("%s;%s;%s\n", $2, $3, $4)
  }' "$input_file" > temp/temp_t.csv
  
    file[i]="temp_t.csv"
    i=$((i+1))
    shift 1
  else
    break
  fi
  
done

for element in "${file[@]}"; do
    echo "$element"
done

###Verification validite (csv existe, nombre d'arguments etc)

###h

###Faire une liste? de tous les arguments d'options entrés pour avoir tous les traitements à faire (A faire en début de script apres verifications)


#fonction pour les 10 premiers, paramètre temp.csv(sort et rm)

###Options avec C
##c et makefile


##ajouter le temps d'exec