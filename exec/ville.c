#include "head.h"

Trajet *build_t(FILE *csv, long *pos) {
  Trajet *t = malloc(sizeof(Trajet));
  if (t == NULL) {
    exit(2);
  }
  char *s = malloc(sizeof(char) * 50);
  if (s == NULL) {
    exit(2);
  }
  if (fscanf(csv, "%99[^;];%99[^;];%99[^\n]", s, t->ville_depart,
             t->ville_arrivee) != 3) {
    free(t);
    exit(1);
  }
  t->step = atoi(s);
  free(s);
  fgetc(csv);
  *pos = ftell(csv);
  return t;
}

void ville_t(FILE *csv) {
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

  Trajet *line = malloc(sizeof(Trajet));
  int *dep = malloc(sizeof(int));
  *dep = 0;

  line = build_t(csv, pos);
  avl *a = nouveauNoeud(build_v(line->ville_arrivee, dep));

  while ((*pos) != (*posfin)) {
    line = build_t(csv, pos);
    if (line->step == 1) {
      *dep = 1;
      a = modif_avl(a, line->ville_depart, dep);

      *dep = 0;
      a = modif_avl(a, line->ville_arrivee, dep);

    } else {
      a = modif_avl(a, line->ville_arrivee, dep);
    }
  }
  free(line);
  free(pos);
  free(posfin);
  free(dep);

  FILE *fichier = fopen("../temp/temp_t2.csv", "w");
  if (fichier == NULL) {
    exit(1);
  }
  infixe(a, fichier);
  fclose(fichier);
  liberer(a);

  FILE *fichier2 = fopen("../temp/temp_t2.csv", "r");
  if (fichier == NULL) {
    exit(1);
  }
  dixVilles(fichier2);
  fclose(fichier2);
}

avl *nouveauNoeud(Ville *ville) {
  avl *nouveau = malloc(sizeof(avl));
  if (nouveau == NULL) {
    exit(2);
  }

  nouveau->ville = ville;
  nouveau->gauche = NULL;
  nouveau->droite = NULL;
  nouveau->hauteur = 1;

  return nouveau;
}

avl *modif_avl(avl *a, char *ville, int *dep) {
  int tmp = recherche(a, ville, dep);
  if (tmp == 0) {
    Ville *v = build_v(ville, dep);
    a = insereravl(a, v);
  }
  return a;
}

Ville *build_v(char *nom, int *dep) {
  Ville *new = malloc(sizeof(Ville));
  strcpy(new->nom, nom);
  new->compteur = 1;
  new->compt_dep = *dep;
  return new;
}

int recherche(avl *a, char *ville, int *dep) {
  while (a != NULL) {
    int cmp = strcmp(ville, a->ville->nom);

    if (cmp == 0) {
      a->ville->compteur = a->ville->compteur + 1;
      if (*dep == 1) {
        a->ville->compt_dep = a->ville->compt_dep + 1;
      }
      return 1;
    } else if (cmp < 0) {
      a = a->gauche;
    } else {
      a = a->droite;
    }
  }
  return 0;
}

int hauteur(avl *noeud) {
  if (noeud == NULL) {
    return 0;
  } else {
    return noeud->hauteur;
  }
}

void majHauteur(avl *noeud) {
  int h_G = hauteur(noeud->gauche);
  int h_D = hauteur(noeud->droite);

  if (h_G > h_D) {
    noeud->hauteur = h_G + 1;
  } else {
    noeud->hauteur = h_D + 1;
  }
}

int equilibre(avl *noeud) {
  if (noeud == NULL) {
    return 0;
  } else {
    return hauteur(noeud->gauche) - hauteur(noeud->droite);
  }
}

avl *rotationGauche(avl *noeud) {
  avl *tmp = noeud->droite;
  noeud->droite = tmp->gauche;
  tmp->gauche = noeud;
  majHauteur(noeud);
  majHauteur(tmp);
  return tmp;
}

avl *rotationDroite(avl *noeud) {
  avl *tmp = noeud->gauche;
  noeud->gauche = tmp->droite;
  tmp->droite = noeud;
  majHauteur(noeud);
  majHauteur(tmp);
  return tmp;
}


avl *insereravl(avl *a, Ville *ville){
  if (a == NULL) {
    a = nouveauNoeud(ville);
    return a;
  } else {
    if (strcmp(ville->nom, a->ville->nom) < 0) {
      a->gauche = insereravl(a->gauche, ville);
    } else if (strcmp(ville->nom, a->ville->nom) > 0) {
      a->droite = insereravl(a->droite, ville);
    } else {
      return a;
    }
  }

  majHauteur(a);
  int equilibre_noeud = equilibre(a);

  if (equilibre_noeud > 1) {
    if (strcmp(ville->nom, a->gauche->ville->nom) < 0) {
      return rotationDroite(a);
    } else {
      a->gauche = rotationGauche(a->gauche);
      return rotationDroite(a);
    }
  } else if (equilibre_noeud < -1) {
    if (strcmp(ville->nom, a->droite->ville->nom) > 0) {
      return rotationGauche(a);
    } else {
      a->droite = rotationDroite(a->droite);
      return rotationGauche(a);
    }
  }
  return a;
}

// A ajuster pour ecrire dans un fichier
void infixe(avl *a, FILE *csv){
  if (a != NULL) {
    infixe(a->gauche, csv);
    fprintf(csv, "%s;%d;%d\n", a->ville->nom, a->ville->compteur,
            a->ville->compt_dep);
    infixe(a->droite, csv);
  }
}

// liberer la memoire
void liberer(avl *a) {
  if (a != NULL) {
    liberer(a->gauche);
    liberer(a->droite);
    free(a->ville);
    free(a);
  }
}

Ville *build_ville(FILE *csv, long *pos) {
  Ville *v = malloc(sizeof(Ville));
  if (v == NULL) {
    exit(2);
  }
  char *s = malloc(sizeof(char) * 50);
  if (s == NULL) {
    exit(2);
  }
  char *s1 = malloc(sizeof(char) * 50);
  if (s == NULL) {
    exit(2);
  }
  if (fscanf(csv, "%99[^;];%99[^;];%99[^\n]", v->nom, s, s1) != 3) {
    free(v);
    exit(1);
  }
  v->compteur = atoi(s);
  v->compt_dep = atoi(s1);
  if (strcmp(v->nom, "") == 0 || strcmp(v->nom, ";") == 0) {
    free(v);
    free(s);
    free(s1);
    return v;
  }
  free(s);
  free(s1);
  fgetc(csv);
  /*fgetc(csv);
  fgetc(csv);*/
  *pos = ftell(csv);
  return v;
}

int max(int a, int b){
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int n_hauteur(n_avl *a){
  if (a == NULL) {
    return 0;
  } else {
    return a->hauteur;
  }
}

int n_equilibre(n_avl *a){
  if (a == NULL) {
    return 0;
  } else {
    return n_hauteur(a->gauche) - n_hauteur(a->droite);
  }
}

n_avl *nouveauNoeud_n(Ville *ville){
  n_avl *nouveau = malloc(sizeof(n_avl));
  if (nouveau == NULL) {
    exit(2);
  }
  nouveau->ville = ville;
  nouveau->gauche = NULL;
  nouveau->droite = NULL;
  nouveau->hauteur = 1;
  return nouveau;
}

n_avl *n_rotationGauche(n_avl *noeud){
  n_avl *tmp = noeud->droite;
  noeud->droite = tmp->gauche;
  tmp->gauche = noeud;
  noeud->hauteur = max(n_hauteur(noeud->gauche), n_hauteur(noeud->droite)) + 1;
  tmp->hauteur = max(n_hauteur(tmp->gauche), n_hauteur(tmp->droite)) + 1;
  return tmp;
}

n_avl *n_rotationDroite(n_avl *noeud){
  n_avl *tmp = noeud->gauche;
  noeud->gauche = tmp->droite;
  tmp->droite = noeud;
  noeud->hauteur = max(n_hauteur(noeud->gauche), n_hauteur(noeud->droite)) + 1;
  tmp->hauteur = max(n_hauteur(tmp->gauche), n_hauteur(tmp->droite)) + 1;
  return tmp;
}

n_avl *n_equilibrer(n_avl *t){
  if (n_equilibre(t) == 2) {
    if (n_equilibre(t->gauche) < 0) {
      t->gauche = n_rotationGauche(t->gauche);
    }
    t = n_rotationDroite(t);
  }
  if (n_equilibre(t) == -2) {
    if (n_equilibre(t->droite) > 0) {
      t->droite = n_rotationDroite(t->droite);
    }
    t = n_rotationGauche(t);
  }
  return t;
}

n_avl *n_inserer(n_avl *t, Ville *v){
  if (v == NULL) {
    return t;
  }
  if (t == NULL) {
    n_avl *nouveau = malloc(sizeof(n_avl));
    nouveau->ville = v;
    nouveau->gauche = NULL;
    nouveau->droite = NULL;
    nouveau->hauteur = 1;
    return nouveau;
  } else {
    if (v->compteur < t->ville->compteur) {
      t->gauche = n_inserer(t->gauche, v);
    } else {
      t->droite = n_inserer(t->droite, v);
    }
    t->hauteur = 1 + max(n_hauteur(t->gauche), n_hauteur(t->droite));
    t = n_equilibrer(t);
  }
  return t;
}

void affiche_n(n_avl *a, FILE *csv, int *i){
  if (a != NULL && *i < 10) {
    affiche_n(a->droite, csv, i);

    if (*i < 10) {
      fprintf(csv, "%s;%d;%d\n", a->ville->nom, a->ville->compteur,
              a->ville->compt_dep);
      *i = *i + 1;
    }

    affiche_n(a->gauche, csv, i);
  }
}

void dixVilles(FILE *csv) {
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

  Ville *line = malloc(sizeof(Ville));
  if (line == NULL) {
    exit(2);
  }

  line = build_ville(csv, pos);
  n_avl *a = nouveauNoeud_n(line);
  while ((*pos) != (*posfin)) {
    line = build_ville(csv, pos);
    a = n_inserer(a, line);
  }

  FILE *fichier2 = fopen("../temp/temp_t3.csv", "w");
  if (fichier2 == NULL) {
    exit(1);
  }
  int i = 0;
  affiche_n(a, fichier2, &i);
  fclose(fichier2);
  liberer2(a);

  free(pos);
  free(posfin);
}

//!!!!! Il faut remettre dans l'ordre alphabetique (remettre dans temp_t.csv)
void alphabetique(FILE *sortie) {
  FILE *csv;
  csv = fopen("../temp/temp_t3.csv", "r");
  if (csv == NULL) {
    exit(1);
  }
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

  Ville *line = malloc(sizeof(Ville));
  if (line == NULL) {
    exit(2);
  }

  line = build_ville(csv, pos);
  avl *a = nouveauNoeud(line);
  while ((*pos) != (*posfin)) {
    line = build_ville(csv, pos);
    a = insereravl(a, line);
  }
  fclose(csv);
  infixe(a, sortie);
  liberer(a);
  free(pos);
  free(posfin);
}

void liberer2(n_avl *a) {
  if (a != NULL) {
    liberer2(a->gauche);
    liberer2(a->droite);
    free(a->ville);
    free(a);
  }
}