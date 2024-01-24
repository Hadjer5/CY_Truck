#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/*
Faire un avl dans l'ordre alphabetique des villes
avl d'une struct (ville, compteur, ville de départ)
Compter uniquement les villes d'arrivée sauf quand etape = 1 
*/

//Verfier malloc et libérer


typedef struct{
  int step;
  char ville_arrivee[30];
  char ville_depart[30];
} Trajet;

typedef struct{
  char nom[30];
  int compteur;
  int compt_dep;
} Ville;

typedef struct avl avl;
struct avl{
  Ville* ville;
  avl *gauche;
  avl *droite;
  int hauteur;
};

Ville* build_v(char* nom,int* dep);
int recherche(avl* a, char* ville,int* f);
avl* modif_avl(avl* a,char* ville,int* dep);
avl* insereravl(avl* a,Ville* ville);
void infixe(avl* a,FILE* csv);
avl* nouveauNoeud(Ville *ville);
void infixe2(avl* a);

//lire le trajet dans le csv
Trajet* build_t(FILE* csv,long* pos) {
    Trajet* t = malloc(sizeof(Trajet));
    char* s=malloc(sizeof(char)*10);
  if (fscanf(csv, "%99[^;];%99[^;];%99[^\n]", s, t->ville_depart, t->ville_arrivee) != 3) {
      free(t);
      exit(EXIT_FAILURE);
  }
  t->step=atoi(s);
  free(s);
    fgetc(csv);
    *pos=ftell(csv);
    return t;
}


void ville_t(FILE* csv){
  long* pos=malloc(sizeof(long));
  if (pos == NULL) {
      exit(4);
  }
  long* posfin=malloc(sizeof(long));
  if (posfin == NULL) {
      exit(4);
  }
  fseek(csv,0,2);
  *posfin=ftell(csv);
  rewind(csv);
  *pos=ftell(csv);

  Trajet* line=malloc(sizeof(Trajet));
  int* dep=malloc(sizeof(int));
  *dep=0;

  line=build_t(csv,pos);
  avl* a=nouveauNoeud(build_v(line->ville_arrivee,dep));

  while((*pos) != (*posfin)){
    line=build_t(csv,pos);  
    if(line->step==1){
      *dep=1;
      a=modif_avl(a,line->ville_depart,dep);

      *dep=0;
      a=modif_avl(a,line->ville_arrivee,dep);

    }
    else{
      a=modif_avl(a,line->ville_arrivee,dep);
    }
  }
  free(line);
  free(pos);
  free(posfin);
  free(dep);

  FILE* fichier = fopen("temp/temp_t2.csv", "w");
  if (fichier == NULL) {
     //erreur
  }
  infixe(a, fichier);
  fclose(fichier);

}

avl* nouveauNoeud(Ville *ville) {
    avl* nouveau = malloc(sizeof(avl));
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }

    nouveau->ville = ville;
    nouveau->gauche = NULL;
    nouveau->droite = NULL;
    nouveau->hauteur = 1;

    return nouveau;
}


avl* modif_avl(avl* a,char* ville,int* dep){
    int tmp=recherche(a,ville,dep);
  if(tmp==0){
      Ville* v=build_v(ville,dep);
      a=insereravl(a,v);
    }
  return a;
}


Ville* build_v(char* nom,int* dep){
  Ville* new=malloc(sizeof(Ville));
  strcpy(new->nom, nom);
    new->compteur=1;
    new->compt_dep=*dep;
  return new;
}

int recherche(avl* a, char *ville, int* dep) {
    while (a != NULL) {
        int cmp = strcmp(ville, a->ville->nom);
      
        if (cmp == 0){
            a->ville->compteur=a->ville->compteur+1;
            if(*dep==1){
              a->ville->compt_dep=a->ville->compt_dep+1;
            }
            return 1; 
        }
        else if (cmp < 0){
            a = a->gauche;
        }
        else{
            a = a->droite;
        }  
    }
    return 0;
}

// Fonction pour obtenir la hauteur
int hauteur(avl *noeud) {
    if (noeud == NULL) {
        return 0;
    } else {
        return noeud->hauteur;
    }
}

// Fonction pour mettre à jour la hauteur d'un nœud AVL
void majHauteur(avl *noeud) {
    int h_G = hauteur(noeud->gauche);
    int h_D = hauteur(noeud->droite);

    if (h_G > h_D) {
        noeud->hauteur = h_G + 1;
    } else {
        noeud->hauteur = h_D + 1;
    }
}

// Fonction pour obtenir le facteur d'équilibre
int equilibre(avl *noeud){
  if(noeud==NULL){
    return 0;
  }
  else{
    return hauteur(noeud->gauche)-hauteur(noeud->droite);
  }
}

//Rotation gauche
avl* rotationGauche(avl* noeud){
  avl* tmp=noeud->droite;
  noeud->droite=tmp->gauche;
  tmp->gauche=noeud;
  majHauteur(noeud);
  majHauteur(tmp);
  return tmp;
}
//Rotation droite
avl* rotationDroite(avl* noeud){
  avl* tmp=noeud->gauche;
  noeud->gauche=tmp->droite;
  tmp->droite=noeud;
  majHauteur(noeud);
  majHauteur(tmp);
  return tmp;
}
//Fonction pour inserer un noeud dans l'arbre AVL
avl* insereravl(avl* a,Ville* ville){
  if(a==NULL){
    a=nouveauNoeud(ville);
    return a;
  }
  else{
    if(strcmp(ville->nom,a->ville->nom)<0){
      a->gauche=insereravl(a->gauche,ville);
    }
    else if(strcmp(ville->nom,a->ville->nom)>0){
      a->droite=insereravl(a->droite,ville);
    }
    else{
      return a;
    }
  }

  majHauteur(a);
  int equilibre_noeud=equilibre(a);

  if(equilibre_noeud>1){
    if(strcmp(ville->nom,a->gauche->ville->nom)<0){
      return rotationDroite(a);
    }
    else{
      a->gauche=rotationGauche(a->gauche);
      return rotationDroite(a);
    }
  }
  else if(equilibre_noeud<-1){
    if(strcmp(ville->nom,a->droite->ville->nom)>0){
      return rotationGauche(a);
    }
    else{
      a->droite=rotationDroite(a->droite);
      return rotationGauche(a);
    }
  }
  return a;
}

//A ajuster pour ecrire dans un fichier
void infixe(avl* a,FILE* csv){
  if(a!=NULL){
    infixe(a->gauche,csv);
    fprintf(csv, "%s;%d;%d\n", a->ville->nom, a->ville->compteur, a->ville->compt_dep);
    infixe(a->droite,csv);
  }
}

//liberer la memoire
void liberer(avl* a) {
  if (a != NULL) {
    liberer(a->gauche);
    liberer(a->droite);
    free(a->ville);
    free(a);
  }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        //erreur
    }

    FILE* fichier = fopen(argv[1], "r");

    if (fichier == NULL) {
        perror("Erreur ouverture");
        exit(1);
    }

    ville_t(fichier);

    fclose(fichier);

    return 0;
}

