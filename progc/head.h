#ifndef HEAD
#define HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int step;
  char ville_arrivee[50];
  char ville_depart[50];
} Trajet;

typedef struct {
  char nom[50];
  int compteur;
  int compt_dep;
} Ville;

typedef struct avl avl;
struct avl {
  Ville *ville;
  avl *gauche;
  avl *droite;
  int hauteur;
};

typedef struct n_avl n_avl;
struct n_avl {
  Ville *ville;
  n_avl *gauche;
  n_avl *droite;
  int hauteur;
};


typedef struct s_line s_line;
struct s_line {
  int id;
  float distance;
};

typedef struct s_trajet s_trajet;
struct s_trajet {
  int id;
  float min;
  float max;
  float sum;
  int compt;
};

typedef struct s_avl s_avl;
struct s_avl {
  s_trajet *trajet;
  s_avl *gauche;
  s_avl *droite;
  int hauteur;
};


typedef struct s_trajet1 s_trajet1;
struct s_trajet1 {
  int id;
  float min;
  float max;
  float moy;
  float nb;
};

typedef struct s_avl1 s_avl1;
struct s_avl1 {
  s_trajet1 *trajet;
  s_avl1 *gauche;
  s_avl1 *droite;
  int hauteur;
};

Trajet *build_t(FILE *csv, long *pos);
void ville_t(FILE *csv);
avl *nouveauNoeud(Ville *ville);
avl *modif_avl(avl *a, char *ville, int *dep);
Ville *build_v(char *nom, int *dep);
int recherche(avl *a, char *ville, int *dep);
int hauteur(avl *noeud);
void majHauteur(avl *noeud);
int equilibre(avl *noeud);
avl *rotationGauche(avl *noeud);
avl *rotationDroite(avl *noeud);
avl *insereravl(avl *a, Ville *ville);
void infixe(avl *a, FILE *csv);
void liberer(avl *a);

Ville *build_ville(FILE *csv, long *pos);
int max(int a, int b);
int n_hauteur(n_avl *a);
int n_equilibre(n_avl *a);
n_avl *nouveauNoeud_n(Ville *ville);
n_avl *n_rotationGauche(n_avl *noeud);
n_avl *n_rotationDroite(n_avl *noeud);
n_avl *n_equilibrer(n_avl *t);
n_avl *n_inserer(n_avl *t, Ville *v);
void affiche_n(n_avl *a, FILE *csv, int *i);
void dixVilles(FILE *csv);
void alphabetique(FILE *sortie);
void liberer2(n_avl *a);

float f_min(float a, float b);
float f_max(float a, float b);
int s_hauteur(s_avl *a);
s_avl *s_rotationGauche(s_avl *noeud);
s_avl *s_rotationDroite(s_avl *noeud);
int s_equilibre(s_avl *t);
s_line *s_build(FILE *csv, long *pos);
s_avl *s_equilibrer(s_avl *a);
s_avl *noeud(s_line *l);
s_avl *inserer_s(s_avl *a, s_line *l);
void s_affiche(s_avl *a, FILE *csv);
void dist(FILE *csv,FILE *sortie);
void s_liberer(s_avl *a);

s_trajet1 *s_build1(FILE *csv, long *pos);
s_avl1 *noeud1(s_trajet1 *t);
int s_hauteur1(s_avl1 *a);
int s_equilibre1(s_avl1* a);
s_avl1* s_rotationgauche1(s_avl1* noeud);
s_avl1* s_rotationdroite1(s_avl1* noeud);
s_avl1* s_equilibrer1(s_avl1* a);
s_avl1 *inserer_s1(s_avl1 *a, s_trajet1 *t);
void s_affiche1(s_avl1 *a, FILE *csv,int *i);
void s_liberer1(s_avl1 *a);
void order(FILE *csv,FILE *sortie);

int main(int argc, char *argv[]);
void viderTampon();

#endif