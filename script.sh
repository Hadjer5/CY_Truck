#!/bin/bash

input_file="$1"
shift 1



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
###   pas forcement nécessaire on peut prendre le contenu du repertoire temp (ne pas oublier de le supp après)


###Options avec C
##c et makefile


##ajouter le temps d'exec

chemin_e="/temp"
chemin_s="/images"
verif=("temp_d1.csv, temp_d2.csv, temp_l.csv, temp_t.csv, temp_s.csv")

while [ True ]; do 
	for fichier in "${!graphique[@]}"; do
		chemin_fichier="$chemin_e/$fichier"
		
		if [ "$chemin_fichier" == "temp_d1.csv" ]; then 
			options_d1="set title 'Graphique traitement D1'; set xlabel 'Nombre de trajets'; set ylabel 'Conducteurs';"
			gnuplot -persist <<-EOFMarker
				$options_d1
				set terminal pngcairo enhanced 'arial,10' size 800,600
				set output '$chemin_s/image_d1.png'
				set style data histograms
				set style histogram rowstacked 
				set boxwidth 0.5 
				set format x "%d"
				set ytics out 
				set yrange [0:*]
				plot '$chemin_fichier' using 2:yticslabels(1) with boxes title 'Nombre de  trajets'
			EOFMarker
		fi
		if [ "$chemin_fichier" == "temp_d2.csv" ]; then 
			# gnuplot d2
		fi
		if [ "$chemin_fichier" == "temp_l.csv" ]; then 
			# gnuplot l
		fi
		if [ "$chemin_fichier" == "temp_t.csv" ]; then 
			# gnuplot t
		fi
		if [ "$chemin_fichier" == "temp_s.csv" ]; then 
			# gnuplot s
		fi
	done
done










