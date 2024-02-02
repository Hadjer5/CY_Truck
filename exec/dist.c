#include "head.h"

float f_min(float a, float b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

float f_max(float a, float b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int s_hauteur(s_avl *a) {
  if (a == NULL) {
    return 0;
  } else {
    return a->hauteur;
  }
}

s_avl *s_rotationGauche(s_avl *noeud) {
  s_avl *tmp = noeud->droite;
  noeud->droite = tmp->gauche;
  tmp->gauche = noeud;
  noeud->hauteur = max(s_hauteur(noeud->gauche), s_hauteur(noeud->droite)) + 1;
  tmp->hauteur = max(s_hauteur(tmp->gauche), s_hauteur(tmp->droite)) + 1;
  return tmp;
}

s_avl *s_rotationDroite(s_avl *noeud) {
  s_avl *tmp = noeud->gauche;
  noeud->gauche = tmp->droite;
  tmp->droite = noeud;
  noeud->hauteur = max(s_hauteur(noeud->gauche), s_hauteur(noeud->droite)) + 1;
  tmp->hauteur = max(s_hauteur(tmp->gauche), s_hauteur(tmp->droite)) + 1;
  return tmp;
}

int s_equilibre(s_avl *t) {
  if (t == NULL) {
    return 0;
  } else {
    return s_hauteur(t->gauche) - s_hauteur(t->droite);
  }
}

s_line *s_build(FILE *csv, long *pos) {
  s_line *s = malloc(sizeof(s_trajet));
  if (s == NULL) {
    exit(2);
  }
  char *s1 = malloc(sizeof(char) * 10);
  if (s1 == NULL) {
    exit(2);
  }
  char *s2 = malloc(sizeof(char) * 10);
  if (s2 == NULL) {
    exit(2);
  }
  if (fscanf(csv, "%99[^;];%99[^\n]", s1, s2) != 2) {
    free(s);
    exit(1);
  }
  s->id = atoi(s1);
  s->distance = atof(s2);
  free(s1);
  free(s2);
  fgetc(csv);
  *pos = ftell(csv);
  return s;
}

s_avl *s_equilibrer(s_avl *a) {
  int equilibre = s_equilibre(a);
  if (equilibre > 1) {
    if (s_equilibre(a->gauche) >= 0) {
      return s_rotationDroite(a);
    } else {
      a->gauche = s_rotationGauche(a->gauche);
      return s_rotationDroite(a);
    }
  }
  if (equilibre < -1) {
    if (s_equilibre(a->droite) <= 0) {
      return s_rotationGauche(a);
    } else {
      a->droite = s_rotationDroite(a->droite);
      return s_rotationGauche(a);
    }
  }
  return a;
}

s_avl *noeud(s_line *l) {
  s_avl *a = malloc(sizeof(s_avl));
  if (a == NULL) {
    exit(2);
  }
  s_trajet *t = malloc(sizeof(s_trajet));
  if (t == NULL) {
    exit(2);
  }
  t->id = l->id;
  t->min = l->distance;
  t->max = l->distance;
  t->sum = l->distance;
  t->compt = 1;

  a->trajet = t;
  a->gauche = NULL;
  a->droite = NULL;
  a->hauteur = 1;
  return a;
}

s_avl *inserer_s(s_avl *a, s_line *l) {
  if (a == NULL) {
    return noeud(l);
  } else {
    if (l->id < a->trajet->id) {
      a->gauche = inserer_s(a->gauche, l);
    } else if (l->id > a->trajet->id) {
      a->droite = inserer_s(a->droite, l);
    } else {
      a->trajet->min = f_min(a->trajet->min, l->distance);
      a->trajet->max = f_max(a->trajet->max, l->distance);
      a->trajet->sum += l->distance;
      a->trajet->compt++;
    }
    a->hauteur = 1 + max(s_hauteur(a->gauche), s_hauteur(a->droite));
    a = s_equilibrer(a);
  }
  return a;
}

void s_affiche(s_avl *a, FILE *csv) {
  if (a != NULL) {
    s_affiche(a->gauche, csv);
    fprintf(csv, "%d;%f;%f;%f\n", a->trajet->id, a->trajet->min, a->trajet->max,
            (a->trajet->sum) / (a->trajet->compt));
    s_affiche(a->droite, csv);
  }
}

void s_liberer(s_avl *a) {
  if (a != NULL) {
    s_liberer(a->gauche);
    s_liberer(a->droite);
    free(a->trajet);
    free(a);
  }
}

void dist(FILE *csv,FILE* sortie) {
  long *pos = malloc(sizeof(long));
  if (pos == NULL) {
    exit(2);
  }
  long *posfin = malloc(sizeof(long));
  if (posfin == NULL) {
    exit(2);
  }
  fseek(csv, 0, 2);
  *posfin = ftell(csv);
  rewind(csv);
  *pos = ftell(csv);

  s_line *line = s_build(csv, pos);
  s_avl *a = noeud(line);

  while ((*pos) != (*posfin)) {
    line = s_build(csv, pos);
    a = inserer_s(a, line);
  }

  FILE *fichier = fopen("../temp/temp_s1.csv", "w");
  if (fichier == NULL) {
    exit(1);
  }
  s_affiche(a, fichier);
  fclose(fichier);
  free(pos);
  free(posfin);
  s_liberer(a);

  FILE *fichier2 = fopen("../temp/temp_s1.csv", "r");
  if (fichier2 == NULL) {
    exit(1);
  }
  order(fichier2,sortie);
  fclose(fichier2);
}

/*------------------------------------------------------------*/

s_trajet1 *s_build1(FILE *csv, long *pos) {
  s_trajet1 *s = malloc(sizeof(s_trajet1));
  if (s == NULL) {
    exit(2);
  }
  char *s1 = malloc(sizeof(char) * 10);
  if (s1 == NULL) {
    exit(2);
  }
  char *s2 = malloc(sizeof(char) * 10);
  if (s2 == NULL) {
    exit(2);
  }
  char *s3 = malloc(sizeof(char) * 10);
  if (s3 == NULL) {
    exit(2);
  }
  char *s4 = malloc(sizeof(char) * 10);
  if (s4 == NULL) {
    exit(2);
  }
  if (fscanf(csv, "%99[^;];%99[^;];%99[^;];%99[^\n]", s1, s2, s3, s4) != 4) {
    free(s);
    exit(1);
  }
  s->id = atoi(s1);
  s->min = atof(s2);
  s->max = atof(s3);
  s->moy = atof(s4);
  s->nb = (s->max) - (s->min);
  free(s1);
  free(s2);
  free(s3);
  free(s4);
  fgetc(csv);
  *pos = ftell(csv);
  return s;
}

s_avl1 *noeud1(s_trajet1 *t) {
  s_avl1 *a = malloc(sizeof(s_avl1));
  if (a == NULL) {
    exit(2);
  }
  a->trajet = t;
  a->gauche = NULL;
  a->droite = NULL;
  a->hauteur = 1;
  return a;
}

int s_hauteur1(s_avl1 *a){
  if(a==NULL){
    return 0;
  }
  else{
    return a->hauteur;
  }
}

int s_equilibre1(s_avl1* a){
  if (a == NULL) {
    return 0;
  } else {
    return s_hauteur1(a->gauche) - s_hauteur1(a->droite);
  }
}

s_avl1* s_rotationgauche1(s_avl1* noeud){
  s_avl1* tmp = noeud->droite;
  noeud->droite = tmp->gauche;
  tmp->gauche = noeud;
  noeud->hauteur = max(s_hauteur1(noeud->gauche), s_hauteur1(noeud->droite)) + 1;
  tmp->hauteur = max(s_hauteur1(tmp->gauche), s_hauteur1(tmp->droite)) + 1;
  return tmp;
}

s_avl1* s_rotationdroite1(s_avl1* noeud){
  s_avl1* tmp = noeud->gauche;
  noeud->gauche = tmp->droite;
  tmp->droite = noeud;
  noeud->hauteur = max(s_hauteur1(noeud->gauche), s_hauteur1(noeud->droite)) + 1;
  tmp->hauteur = max(s_hauteur1(tmp->gauche), s_hauteur1(tmp->droite)) + 1;
  return tmp;
}

s_avl1* s_equilibrer1(s_avl1* a){
  int equilibre = s_equilibre1(a);
  if(equilibre>1){
    if(s_equilibre1(a->gauche)>=0){
      return s_rotationdroite1(a);
    }
    else{
      a->gauche = s_rotationgauche1(a->gauche);
      return s_rotationdroite1(a);
    }    
  }
  if(equilibre<-1){
    if(s_equilibre1(a->droite)<=0){
      return s_rotationgauche1(a);
    }
    else{
      a->droite = s_rotationdroite1(a->droite);
      return s_rotationgauche1(a);
    }
  }
  return a;
}



s_avl1 *inserer_s1(s_avl1 *a, s_trajet1 *t) {
  if (a == NULL) {
    return noeud1(t);
  } else {
    if (t->nb <= a->trajet->nb) {
      a->gauche = inserer_s1(a->gauche, t);
    } else if (t->nb > a->trajet->nb) {
      a->droite = inserer_s1(a->droite, t);
    }
  }
  a->hauteur = 1 + max(s_hauteur1(a->gauche),s_hauteur1(a->droite));
  a=s_equilibrer1(a);
  return a;
}

void s_affiche1(s_avl1 *a, FILE *csv,int *i){
  if(a!=NULL && * i <50){
    s_affiche1(a->droite, csv,i);
    if(*i < 50){
      fprintf(csv, "%d;%f;%f;%f\n", a->trajet->id, a->trajet->min, a->trajet->max, a->trajet->moy);
      *i=*i+1;
    }
    s_affiche1(a->gauche, csv,i);
  }
}


void s_liberer1(s_avl1 *a){
  if(a!=NULL){
    s_liberer1(a->gauche);
    s_liberer1(a->droite);
    free(a->trajet);
    free(a);
  }
}

void order(FILE *csv,FILE* sortie) {
  long *pos = malloc(sizeof(long));
  if (pos == NULL) {
    exit(2);
  }
  long *posfin = malloc(sizeof(long));
  if (posfin == NULL) {
    exit(2);
  }
  fseek(csv, 0, 2);
  *posfin = ftell(csv);
  rewind(csv);
  *pos = ftell(csv);

  s_trajet1 *line = s_build1(csv, pos);
  s_avl1 *a = noeud1(line);
  int i=0;
  
  while ((*pos) != (*posfin)){
    line = s_build1(csv, pos);
    a = inserer_s1(a, line);
  }

  s_affiche1(a, sortie,&i);
  free(pos);
  free(posfin);
  s_liberer1(a);
}
