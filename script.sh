#!/bin/bash

input_file="$1"
shift 1

function compil {
  local arg1=$1
  local arg2=$2

  make -f Makefile_1

  ./main "$arg1" "$arg2"

  if [ $? -eq 1 ]; then
    echo "Erreur fichier"
  elif [ $? -eq 2 ]; then
    echo "Erreur allocation mémoire"
  elif [ $? -eq 0 ]; then
    echo "Exécution réussie"
  fi

  make -f Makefile_1 clean
}



# Vérifie si le dossier existe
if [ -d "temp" ]; then
    # Supprime les fichiers présents dans le dossier
    rm -rf temp
fi
if [ ! -d "temp" ]; then
    # Crée le dossier s'il n'existe pas
    mkdir "temp"
fi
if [ -d "images" ]; then
    # Supprime les fichiers présents dans le dossier
    rm -rf images
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

deb=$(date +%s)

while [ True ]; do
	if [ "$1" = "-h" ]; then 
    	echo "AIDE"
     	echo "Commande : bash script.sh data/data.csv [ options ]"   
      	echo "Vous pouvez entrer plusieurs options"
       	echo "Options :"
	echo "-d1 : Conducteurs avec le plus de trajets"
 	echo "Sortie : Histogramme Noms conducteurs / Nombre de trajets effectués"
  	echo "-d2 : Conducteurs avec les plus grandes distances parcourues"
   	echo "Sortie : Histogramme Noms conducteurs / Distance totale parcourue"
    	echo "-l : 10 plus longs trajets"
     	echo "Sortie : Histogramme Distances en km / Identifiants trajets"
 	echo "-t : 10 villes plus traversées"
     	echo "Sortie : Histogramme Nombre de trajets / Nom villes"
      	echo "Avec pour chaque ville 2 barres : Nb. Trajets tot la traversant / Nb. fois ville de départ"
    	echo "-s : Statistiques étapes"
	echo "Sortie : Courbes min/max/moy Distances en km / Identifiants trajets"
 	echo "FIN AIDE"
 	break
  
  elif [ "$1" = "-d1" ]; then 
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
    #temps exécution
    fin_d1=$(date +%s)
    temps_exec_d1=$((fin_d1 - deb))
    #supp fichier temporaire
    rm temp/temp.csv

    shift 1
    echo "Temps d'exécution pour le traitement D1 : $temps_exec_d1 seconde(s)"
  
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
    #temps exécution
    fin_d2=$(date +%s)
    temps_exec_d2=$((fin_d2 - deb))
    #supp fichier temporaire
    rm temp/temp.csv


    shift 1
    echo "Temps d'exécution pour le traitement D2 : $temps_exec_d2 seconde(s)"

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
    #temps exécution
    fin_l=$(date +%s)
    temps_exec_l=$((fin_d2 - deb))
    #supp fichier temporaire
    rm temp/temp.csv
    
    shift 1
    echo "Temps d'exécution pour le traitement L : $temps_exec_l seconde(s)"

  elif [ "$1" = "-t" ]; then
    awk -F';' 'NR > 1 {
      printf("%s;%s;%s\n", $2, $3, $4)
  }' "$input_file" > temp/temp_t.csv
  

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


    #compilation
    cd exec
    compil "../temp/temp_s.csv" "../temp/temp_sf.csv"
    cd ..
    rm temp/temp_s.csv temp/temp_s1.csv
    

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


### Parcourt le dossier temp et pour chaque traitement produit à l'aide de GnuPlot le graphique associé qui est ensuite stocké dans le dossier "images"



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
	if [ "$(basename "$fichier")" == "temp_tf.csv" ]; then
		gnuplot -persist gnuplot/gp_t.gp
	fi
	if [ "$(basename "$fichier")" == "temp_sf.csv" ]; then
		gnuplot -persist gnuplot/gp_s.gp
	fi
done


