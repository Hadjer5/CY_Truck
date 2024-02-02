#include "head.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    exit(1);
  }
  // fichier de sortie en argv[2]

  FILE *fichier = fopen(argv[1], "r");
  FILE* sortie=fopen(argv[2],"w");
  if (fichier == NULL) {
    exit(1);
  }
  if (sortie == NULL) {
    exit(1);
  }
  if(strcmp(argv[1],"../temp/temp_t.csv")==0){
    ville_t(fichier);
    //remettre dans l'ordre alphabetique
    alphabetique(sortie);
  }
  if(strcmp(argv[1],"../temp/temp_s.csv")==0){
    dist(fichier,sortie);
  }

  fclose(fichier);
  fclose(sortie);
  //erreur 0:ok 1: fichier 2:memoire allouee echec

  return 0;
}