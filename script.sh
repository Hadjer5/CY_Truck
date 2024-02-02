#!/bin/bash

input_file="$1"
shift 1

function compil {
  local arg1=$1
  local arg2=$2

  make -f Makefile

  ./main "$arg1" "$arg2"

  if [ $? -eq 1 ]; then
    echo "Erreur fichier"
  elif [ $? -eq 2 ]; then
    echo "Erreur allocation mémoire"
  elif [ $? -eq 0 ]; then
    echo "Exécution réussie"
  fi

  make -f Makefile clean
}


# Vérifie si le dossier existe
if [ ! -d "temp" ]; then
    # Crée le dossier s'il n'existe pas
    mkdir "temp"
fi

if [ ! -d "images" ]; then
    mkdir "images"
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

    #compilation
    cd exec
    compil "../temp/temp_t.csv" "../temp/temp_tf.csv"
    cd ..
    rm temp/temp_t.csv temp/temp_t2.csv temp/temp_t3.csv
    shift 1

  elif [ "$1" = "-s" ]; then
    awk -F';' 'NR > 1 {
      printf("%s;%s\n", $1, $5)
  }' "$input_file" > temp/temp_s.csv

    file[i]="temp_s.csv"

    #compilation
    cd exec
    compil "../temp/temp_s.csv" "../temp/temp_sf.csv"
    cd ..
    rm temp/temp_s.csv temp/temp_s1.csv
    
    i=$((i+1))
    shift 1
  else
    break
  fi
done

###Verification validite (csv existe, nombre d'arguments etc)

###h

###Faire une liste? de tous les arguments d'options entrés pour avoir tous les traitements à faire (A faire en début de script apres verifications)
###   pas forcement nécessaire on peut prendre le contenu du repertoire temp (ne pas oublier de le supp après)


###Options avec C
##c et makefile


##ajouter le temps d'exec

for fichier in ./temp/*.csv; do
	if [ "$(basename "$fichier")" == "temp_d1.csv" ]; then
		gnuplot -persist gnuplot/gp_d1.gp
	fi
	if [ "$(basename "$fichier")" == "temp_d2.csv" ]; then
		gnuplot -persist gnuplot/gp_d2.gp
	fi
	if [ "$(basename "$fichier")" == "temp_l.csv" ]; then
		gnuplot -persist gnuplot/gp_l.gp
	fi
	if [ "$(basename "$fichier")" == "temp_t3.csv" ]; then
		gnuplot -persist gnuplot/gp_t.gp
	fi
	if [ "$(basename "$fichier")" == "temp_s1.csv" ]; then
		gnuplot -persist gnuplot/gp_s.gp
	fi
done










