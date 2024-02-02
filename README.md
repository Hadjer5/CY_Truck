# CY_Truck

1) Prendre tout le contenu :
     - Dossiers : data, gnuplot, exec  (les dossiers "temp" et "images" sont crées avec le script lors de l'exécution, le dossier "exec" est le dossier "progc" demandé dans la consigne)
     - Fichiers : script.sh

       -> Les mettre tels quels dans un dossier sur votre machine (dossier de nom           quelconque)

       
2) Télécharger le fichier brut "data.csv" (s'il n'est pas nommé ainsi, il faut le     faire) et placer ce fichier dans le dossier "data"

   -> Le fichier est trop lourd pour être déposé sur GitHub


3) SCRIPT SHELL :
    - Plusieurs traitements peuvent être faits dans la même commande
  -> ex : bash script.sh data/data.csv -d1 -l -d2
    - On peut bien évidemment faire un seul traitement si on le souhaite
  -> ex : bash script.sh data/data.csv -t
    - Si la commande suivante est saisie dans le terminal : bash data/data.csv -h  
  -> Dans le terminal s'affiche la liste des commandes pouvant être utilisées


 4) Accès aux graphiques :
    - Les graphiques sont disponibles (si le traitement a été effectué) dans le dossier "images"
      -> Ils sont nommés ainsi : graphique_d1.png, graphique_d2.png, graphique_l.png...


5) Accès aux fichiers temporaires :
    - Seuls les fichiers temporaires "finaux" seront disponibles dans le dossier "temp"
      -> De cette manière, le contrôle plus précis des données pourra être possible

