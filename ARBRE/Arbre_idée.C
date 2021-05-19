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


#define MARKER ')'
const int N=5;

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

typedef struct arbre {
   int key;
   arbre *child[N];
} arbre;

arbre* newArbre(int key){
    arbre *temp = new arbre;
    temp->key = key;
    for (int i = 0; i < N; i++)
        temp->child[i] = NULL;
    return temp;
}

void ajout(Sommet *liste, int val)
{

    Sommet *s = (Sommet *)malloc(sizeof(Sommet));
    s->listeDAdjacence = NULL;
    s->valeur_sommet = val;
    s->marque = false;

    bool firsAdding = false;

    if (liste->listeDAdjacence == NULL)
    {
        firsAdding = true;
    }

    s->listeDAdjacence = liste->listeDAdjacence;
    liste->listeDAdjacence = s;

    if (firsAdding)
    {
        s->listeDAdjacence = liste;
    }
}


int* sommet_deg_i(Sommet *G2,int deg, int taille, int* nbre){
    int *liste = (int*)malloc(taille*sizeof(int));
    int nb=0;
    for (int i = 0; i < taille; i++)
    {
        if((G2+i)->degre ==deg){
            liste[nb++]=(G2+i)->valeur_sommet;
        }
    }
    *nbre=nb;
    return liste;
}

void construction_arbre(arbre *Arbre,Sommet *G1, Sommet *G2,
int *ordreDeVisite1_inverse,
int nbre_sommets,int indice){


    int *voisin_possible;
    int *taille_voisin_possible=(int* ) malloc (sizeof(int)*1);

    if(indice==nbre_sommets){
      
        printf("FINI");
        return;
    }else{
        voisin_possible=sommet_deg_i(G2, 
        (G1+ordreDeVisite1_inverse[indice] -1)->degre,nbre_sommets,taille_voisin_possible);
        printf("La liste de degres %d de l'incide  %d soit %d est \n :" ,(
        G1+ordreDeVisite1_inverse[indice] -1)->degre, 
        indice +1 , 
        ordreDeVisite1_inverse[indice] );

        for(int i=0;i<*(taille_voisin_possible);i++){
            printf("%d ,", voisin_possible[i]+1);
        }
        printf("\n :");



        for(int i=0;i<*(taille_voisin_possible);i++){
            Arbre->child[i]=newArbre(voisin_possible[i]);
            construction_arbre(Arbre->child[i],G1,G2,ordreDeVisite1_inverse,nbre_sommets,indice+1);
        }
        free(voisin_possible);
        free(taille_voisin_possible);
    }

 }

void serialize(arbre *root)
{
    // Base case
    if (root == NULL) return;
  
    // Else, store current node and recur for its children
    printf("  %d  ", root->key+1);
    for (int i = 0; i < N && root->child[i]; i++)
         serialize(root->child[i]);
  
    // Store marker at the end of children
    printf( "  ) ");
}

void serialize1(arbre *root, FILE *fp)
{
// Base case
    if (root == NULL) return;
  
    // Else, store current node and recur for its children
    fprintf(fp, "%d ", root->key+1);
    for (int i = 0; i < N && root->child[i]; i++)
         serialize1(root->child[i], fp);
  
    // Store marker at the end of children
    fprintf(fp, "%c ", MARKER);
}

void traverse(arbre *root)
{
    if (root)
    {
        printf("%d ", root->key+1);
        for (int i = 0; i < N; i++)
            traverse(root->child[i]);
    }
}

char* traverse_fichier(FILE *fichier, int taille){
    FILE *fp = fopen("tree.txt", "r");
    if (fp == NULL)
    {
        puts("Could not open file");
        return 0;
    }
    
    int MAX= taille*taille*taille;
    char *tab_char= (char*)malloc( MAX * sizeof(char));
    fgets(tab_char,MAX,fichier);
    fclose(fp);
    return tab_char;
}



 int main (){
     int nbre_sommets=5;
    Sommet *G1=(Sommet*)malloc(sizeof(Sommet) * 5);
    for (int i = 0; i < 5; i++)
    {
        (G1 +i)->valeur_sommet = i;
        (G1 +i)->listeDAdjacence = NULL; 
        (G1 +i)->marque=false;
        (G1 +i)->nbre_voisins_ajoutes=0;
    }
    
    //SOMMET 1
    G1->degre=3;
    ajout(G1,4); 
    ajout(G1,2);
    ajout(G1,1);
    
    //SOMMET 2
    (G1+1)->degre=4;
    ajout(G1+1,3);
    ajout(G1+1,4);
    ajout(G1+1,0);
    ajout(G1+1,2);
    
    //SOMMET 3
    (G1+2)->degre=4;
    ajout(G1+2,4);
    ajout(G1+2,3);
    ajout(G1+2,1);
    ajout(G1+2,0);
    
    //SOMMET 4
    (G1+3)->degre=2;
    ajout(G1+3,1);
    ajout(G1+3,2);


    //SOMMET 5
    (G1+4)->degre=3;
    ajout(G1+4,2);
    ajout(G1+4,1);
    ajout(G1+4,0);

    int *ordreDeVisite1 = (int *)calloc(nbre_sommets,sizeof(int));
    int *ordreDeVisite1_inverse = (int *)calloc(nbre_sommets,sizeof(int));


    Sommet *G2=(Sommet*)malloc(sizeof(Sommet) * 5);
    for (int i = 0; i < 5; i++)
    {
        (G2 +i)->valeur_sommet = i;
        (G2 +i)->listeDAdjacence = NULL; 
        (G2 +i)->marque=false;
        (G2 +i)->nbre_voisins_ajoutes=0;
    }
    
    //SOMMET 1
    G2->degre=4;
    ajout(G2,4); 
    ajout(G2,2);
    ajout(G2,1);
    ajout(G2,3);

    //SOMMET 2
    (G2+1)->degre=3;
    ajout(G2+1,3);
    ajout(G2+1,4);
    ajout(G2+1,0);
    
    //SOMMET 3
    (G2+2)->degre=2;
    ajout(G2+2,4);
    ajout(G2+2,0);
    
    //SOMMET 4
    (G2+3)->degre=3;
    ajout(G2+3,4);
    ajout(G2+3,0);
    ajout(G2+3,1);



    //SOMMET 5
    (G2+4)->degre=4;
    ajout(G2+4,2);
    ajout(G2+4,1);
    ajout(G2+4,0);
    ajout(G2+1,3);


    int *ordreDeVisite2 = (int *)calloc(nbre_sommets,sizeof(int));
    int *ordreDeVisite2_inverse = (int *)calloc(nbre_sommets,sizeof(int));

    ordreDeVisite1[3]=1;
    ordreDeVisite1[2]=2;
    ordreDeVisite1[0]=3;
    ordreDeVisite1[4]=4;
    ordreDeVisite1[1]=5;

    ordreDeVisite1_inverse[0]=4;
    ordreDeVisite1_inverse[1]=3;
    ordreDeVisite1_inverse[2]=1;
    ordreDeVisite1_inverse[3]=5;
    ordreDeVisite1_inverse[4]=2;

    arbre *Arbre=newArbre(4);
    construction_arbre(Arbre,G1,G2,ordreDeVisite1_inverse,
    nbre_sommets,2);
    serialize(Arbre);
    FILE *fp = fopen("tree.txt", "w");
    if (fp == NULL)
    {
        puts("Could not open file");
        return 0;
    }
    serialize1(Arbre, fp);

    fclose(fp);
    // Let us deserialize the storeed tree into root1

    printf("\n");
    printf("Constructed N-Ary Tree from file is \n");
    char * temp=traverse_fichier(fp,5);
    printf("%s",temp);
    
    

 }