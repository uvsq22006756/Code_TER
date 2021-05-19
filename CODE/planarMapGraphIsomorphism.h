#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "limits.h"
#include "string.h"
#include "stdbool.h"
#include "assert.h"
#include "ctype.h"
#include "time.h"
#include "unistd.h"



typedef struct sommet
{
    int valeur_sommet;
    int degre;
    bool marque;
    struct sommet *listeDAdjacence;
    int nbre_voisins_ajoutes;
} Sommet;

typedef struct stat
{
    int nbOccDeg;
    int somet; //Premier sommet du graphe ayant comme degre "DEG" telque nbre occurrence(DEG)= nbOccDeg
    int* sommets;
} Stat;


void ajout(Sommet *liste, int val);
bool exist(Sommet *liste, int val, int nbre_sommets);
void initialiser_ListeDesVoisinsLibres(int *l, int taille);
int *initialiser_Ensemble_Liste_DAjacences(Sommet *Ensemble_Listes_DAdjacences, int *ordreDeVisite , int *Liste_Voisins_Libres, int nbre_sommets);
Sommet *generate_Adjacence_Lists(int nbre_sommets, Stat *statsDegres, int *ordreDeVisite ,int *somme_deg);
Stat *initStats(int taille);
int premierSommetNbOccDEGMin(Stat *stats, int taille);
int nextNeighbours(Sommet *Ensemble_Listes_DAdjacences, int previous, int current);
void markEdge(Sommet *Ensemble_Listes_DAdjacences, int current, int nextAvailableNeighbour);
void affecterNumordre(int *ordreDeVisite, Sommet *Ensemble_Listes_DAdjacences, int sommet, int *numActuel);
void affichageCode(int *codeArray, int indexCode, int *ordreDeVisite, int nbre_sommet, int nbre_arcs, Sommet *Ensemble_Listes_DAdjacences);
int *deepFirstSearch(Sommet *Ensemble_Listes_DAdjacences, int *ordreDeVisite, int begin_value, int premierVoisin , int nbre_sommets, int nbre_arcs, int *tailleCode);
bool compareStats(Stat * stats_G1, Stat * stats_G2, int taille);
bool isIsomorph(Sommet *G1, Sommet *G2, Stat *stats_G1, Stat *stats_G2, int *CodeArray1, int * OrdreVisite1, int nbre_sommets_G1, int nbre_sommets_G2, int tailleCode1,int nbre_arcs1, int nbre_arcs2);
bool compareCode(int *CodeArray1, int *CodeArray2, Sommet *G1, Sommet *G2,int *OrdreVisite1, int *OrdreVisite2 ,int tailleCode1, int tailleCode2, int nbre_sommets );
bool compareListeAdjacence(Sommet *G1, Sommet *G2, int *OrdreVisite1, int* OrdreVisite2 , int sommetActuel);
bool compareListeAdjacences(Sommet *G1, Sommet *G2, int *OrdreVisite1, int* OrdreVisite2 , int sommetActuel_G1,  int sommetActuel_G2, int nbre_sommets);
int* sommet_deg_i(Sommet *G2,int deg, int taille, int* nbre);
void initOrdreVisite(int *OrdreVisite, int taille);