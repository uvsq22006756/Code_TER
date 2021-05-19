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


//________________________________________GENERER GRAPHE_____________________________________________________________

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

bool exist(Sommet *liste, int val, int nbre_sommets)
{

    if (val != liste->valeur_sommet && ((liste->degre) > (liste->nbre_voisins_ajoutes)) && nbre_sommets > val)
    {
        Sommet *p = liste->listeDAdjacence;

        if (p == NULL)
        {
            return false;
        }
        while (p != liste && p->valeur_sommet != val)
        {
            p = p->listeDAdjacence;
        }

        return (p != liste);
    }
    return true;
}

int *initialiser_Ensemble_Liste_DAjacences(Sommet *Ensemble_Listes_DAdjacences, int *ordreDeVisite, int *Liste_Voisins_Libres, int nbre_sommets)
{

    int k;
    int min_des_degre_choisi = nbre_sommets;
    int *tampon = (int *)malloc(sizeof(int) * nbre_sommets);
    //CONTRAINTE RELACHÉE!!!!
    //Tout graphe planaire contient
    //au moins un sommet de degré au plus 5

    //printf("\n DEGRES CHOISIS: : ");

    for (int i = 0; i < nbre_sommets; i++)
    {

        (Ensemble_Listes_DAdjacences + i)->valeur_sommet = i;
        (Ensemble_Listes_DAdjacences + i)->listeDAdjacence = NULL;
        (Ensemble_Listes_DAdjacences + i)->marque = false;
        (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes = 0;
        k = 1 + rand() % (nbre_sommets - 1);
        (Ensemble_Listes_DAdjacences + i)->degre = k;

        *(Liste_Voisins_Libres + i) = i;
        *(tampon + i) = i;
        *(ordreDeVisite + i) = 0;
    }

    return tampon;
}

Sommet *generate_Adjacence_Lists(int nbre_sommets, Stat *statsDegres, int *ordreDeVisite, int *somme_deg)
{

    Sommet *Ensemble_Listes_DAdjacences = (Sommet *)malloc(sizeof(Sommet) * nbre_sommets);
    int *Liste_Voisins_Libres = (int *)malloc(sizeof(int) * nbre_sommets);
    int valeur_Du_Nouv_Voisin;
    int domaineDeChoix, x, temp, temp1;

    int *tampon = initialiser_Ensemble_Liste_DAjacences(Ensemble_Listes_DAdjacences, ordreDeVisite, Liste_Voisins_Libres, nbre_sommets);

    for (int i = 0; i < nbre_sommets; i++)
    {
        domaineDeChoix = nbre_sommets - 1;
        x = *(Liste_Voisins_Libres + tampon[i]);
        temp = tampon[i]; //place de i dans liste
        temp1 = *(Liste_Voisins_Libres + domaineDeChoix);

        *(Liste_Voisins_Libres + tampon[i]) = *(Liste_Voisins_Libres + domaineDeChoix);
        *(Liste_Voisins_Libres + domaineDeChoix) = x;

        tampon[i] = tampon[temp1];
        tampon[temp1] = temp;
        domaineDeChoix--;

        for (int j = 0; j < (Ensemble_Listes_DAdjacences + i)->degre; j++)
        {

            valeur_Du_Nouv_Voisin = *(Liste_Voisins_Libres + rand() % (domaineDeChoix + 1));
            //valeur_Du_Nouv_Voisin =rand()%(domaineDeChoix+1);
            if (!exist((Ensemble_Listes_DAdjacences + i), valeur_Du_Nouv_Voisin, nbre_sommets))
            {
                //POUR EVITER DES CYCLES DE DEUX SOMMETS DÉTACHÉS DU RESTE DU GRAPHE
                if ((Ensemble_Listes_DAdjacences + i)->degre == 1 && (Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin)->degre == 1)
                {
                    (Ensemble_Listes_DAdjacences + i)->degre += 1;
                    (Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin)->degre += 1;
                }

                ajout(Ensemble_Listes_DAdjacences + i, valeur_Du_Nouv_Voisin);

                //ECHANGE
                x = *(Liste_Voisins_Libres + tampon[valeur_Du_Nouv_Voisin]); //la place du nouveau voisin
                temp = tampon[valeur_Du_Nouv_Voisin];
                temp1 = *(Liste_Voisins_Libres + domaineDeChoix);

                *(Liste_Voisins_Libres + tampon[valeur_Du_Nouv_Voisin]) = *(Liste_Voisins_Libres + domaineDeChoix);
                *(Liste_Voisins_Libres + domaineDeChoix) = x;
                //SAVEGARDE INDICE PERMUTATION
                x = tampon[valeur_Du_Nouv_Voisin];
                tampon[valeur_Du_Nouv_Voisin] = tampon[temp1];
                tampon[temp1] = temp;

                (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes += 1;
                domaineDeChoix--;
                // ****************Si (i,j) £ E alors (j,i) £ E  *********************
                ajout(Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin, i);
                (Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin)->nbre_voisins_ajoutes += 1;
            }
        }
        //initialiser_ListeDesVoisinsLibres(Liste_Voisins_Libres, nbre_sommets);
    }

    //FILE *F = fopen("Graph.data", "w");
    //if (F == NULL)
    //    exit(4);
    *somme_deg = 0;
    int k;
    for (int i = 0; i < nbre_sommets; i++)
    {
        k = 0;
        Sommet *q = (Ensemble_Listes_DAdjacences + i)->listeDAdjacence;
        printf("\n%d===>", i+1 );
        //fprintf(F, "%d\t", i + 1);

        while (q != (Ensemble_Listes_DAdjacences + i))
        {
            printf("%d\t",q->valeur_sommet+1);
            //fprintf(F, "%d ", q->valeur_sommet + 1);
            q = q->listeDAdjacence;
            *(somme_deg) += 1;
            k++;
        }

        (Ensemble_Listes_DAdjacences + i)->degre = k;
        (statsDegres[k].nbOccDeg) += 1;
        if ((statsDegres + k)->somet == -1)
        {
            (statsDegres + k)->somet = i;
        }
        (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes = 0;
        //fprintf(F, "\n");
    }

    free(Liste_Voisins_Libres);
    return Ensemble_Listes_DAdjacences;
}

Stat *initStats(int taille)
{
    Stat *stats = (Stat *)malloc(sizeof(Stat) * taille);
    for (int i = 0; i < taille; i++)
    {
        stats[i].nbOccDeg = 0;
        stats[i].somet = -1;
        //stats[i].sommets = (int *)malloc(sizeof(int));
    }
    return stats;
}

int premierSommetNbOccDEGMin(Stat *stats, int taille)
{
    int degWithMinOcc = taille;
    int somet = 0;
    int nbre_Min_codes = taille * taille;
    //printf("\n");
    for (int i = 1; i < taille; i++)
    {
        //printf("%d\t", stats[i].nbOccDeg);
        /*
        if (stats[i].nbOccDeg > 0 && stats[i].nbOccDeg < degWithMinOcc)
        {
            degWithMinOcc = stats[i].nbOccDeg;
            somet = stats[i].somet;
            //somet = i;
            
        }
        */

        int x = stats[i].nbOccDeg * i;
        if (stats[i].nbOccDeg > 0 && (x < nbre_Min_codes))
        {
            nbre_Min_codes = x;
            somet = stats[i].somet;
        }
    }
    return somet;
}

//_________________________________________ PARCOURS _________________________________________________________________

//_______________________________    Marquage    _______________________________

bool marquage_total_sommet(Sommet *Ensemble_Listes_DAdjacences, int current, int* marquage_sommet){
    Sommet *p=(Ensemble_Listes_DAdjacences + current)->listeDAdjacence;
    int nb = (Ensemble_Listes_DAdjacences + current)->degre;
    while (nb > 0){
        if(marquage_sommet[p->valeur_sommet]==0){
            return true;
        }
        p=p->listeDAdjacence;
        nb--;
    }
    return false;

}

bool tous_marque(int taille, int* marquage_sommet){
    for(int i=0; i<taille;i++){
        if(marquage_sommet[i]==0){
            return false;
        }
    }
    return true;
}
//_____________________________    Fonctions   _________________________________

void markEdge(Sommet *Ensemble_Listes_DAdjacences, int current, int nextAvailableNeighbour,int *marquage_sommet)
{
    
    (Ensemble_Listes_DAdjacences + current)->marque = true;
    (Ensemble_Listes_DAdjacences + nextAvailableNeighbour)->marque = true;
  
    Sommet *p = (Ensemble_Listes_DAdjacences + current)->listeDAdjacence;
    while (p != (Ensemble_Listes_DAdjacences + current) && p->valeur_sommet != nextAvailableNeighbour)
    {
        p = p->listeDAdjacence;
    }
    p->marque = true;
}

void affecterNumordre(int *ordreDeVisite, int sommet, int *numActuel)
{


    if (ordreDeVisite[sommet] == 0)
    {
        ordreDeVisite[sommet] = *numActuel;
        *(numActuel) += 1;
    }
}

void affecterNumordre_inverse(int *ordreDeVisite_inverse, int *ordreDeVisite, int sommet)
{
    if (ordreDeVisite_inverse[ordreDeVisite[sommet]-1] == 0)
    {
        ordreDeVisite_inverse[ordreDeVisite[sommet]-1] = sommet +1 ;
    }
}


void affichageCode(int *codeArray, int indexCode, int *ordreDeVisite, int nbre_sommet, int nbre_arcs, Sommet *Ensemble_Listes_DAdjacences)
{
    printf(" CODE: ");

    for (int i = 0; i < indexCode; i++)
    {
        printf("%d",  codeArray[i] +1);

        Sommet *p = (Ensemble_Listes_DAdjacences + codeArray[i])->listeDAdjacence;
        printf("(");
        while (p != (Ensemble_Listes_DAdjacences + codeArray[i]))
        {
            printf(" %d ",p->valeur_sommet+1);
            //finalCode[k++]=ordreDeVisite[p->valeur_sommet];   
            p = p->listeDAdjacence;
        }
        printf(")\t");
    }
}

void affichageCode_numordre(int *codeArray, int indexCode, int *ordreDeVisite, int nbre_sommet, int nbre_arcs, Sommet *Ensemble_Listes_DAdjacences)
{
    printf(" CODE: ");

    for (int i = 0; i < indexCode; i++)
    {
        printf("%d",  ordreDeVisite[codeArray[i] ]);


        Sommet *p = (Ensemble_Listes_DAdjacences + codeArray[i])->listeDAdjacence;
        printf("(");
        while (p != (Ensemble_Listes_DAdjacences + codeArray[i]))
        {
            printf(" %d ",ordreDeVisite[p->valeur_sommet]);
            //finalCode[k++]=ordreDeVisite[p->valeur_sommet];            
            p = p->listeDAdjacence;
        }
        printf(")\t");
    }
}

void reinitialiser(Sommet *Ensemble_Listes_DAdjacences, int nbr_sommet){
    int temp;
    for(Sommet *p=Ensemble_Listes_DAdjacences; p<Ensemble_Listes_DAdjacences+nbr_sommet ; p++){
        temp=p->degre;
        p->marque=false;
        //printf( " \n         ( tab sommet : %d , marquage : %d ) \n", p->valeur_sommet +1, p->marque );
        Sommet *q = p->listeDAdjacence;
        while (q!=p)
        {
            q->marque=false;
            //printf( " \n (sommet : %d , marquage : %d ) \n", q->valeur_sommet +1 , q->marque );
            q=q->listeDAdjacence;
        }
    }
}

//___________________________   Voisin suivant   _______________________________

int nextNeighbours(Sommet *Ensemble_Listes_DAdjacences, int previous, 
int current,  int* marquage_sommet)
{

    Sommet *p = (Ensemble_Listes_DAdjacences + current)->listeDAdjacence;
    bool sommets_marques = marquage_total_sommet(Ensemble_Listes_DAdjacences,current,marquage_sommet);

    if(sommets_marques==false){
        //printf("\n On est dans le cas où tous les sommets sont marqués\n ");
        if (previous == current){

            //premier sommet non marqué
            int nb = (Ensemble_Listes_DAdjacences + current)->degre;
            //condition sommet marque et l'arc est marque
            while (nb > 0 && p->marque)
            {
                p = p->listeDAdjacence;
                nb--;
            }
            return (nb > 0) ? (p->valeur_sommet) : -1;
        }

        //printf("\nCas ou current !=previous\n");
        //on se met à previous
        while (p->valeur_sommet != previous){
            p = p->listeDAdjacence;
            //printf("\nwhile :  %d \n",p->valeur_sommet);
        }

        //premier sommet apres previous
        p = p->listeDAdjacence;
        //printf("\nor while %d \n",p->valeur_sommet);

        while (p->valeur_sommet != previous && p->marque){
            p = p->listeDAdjacence;
            //printf("\nwhile : %d \n",p->valeur_sommet);
        }
        //printf("\n %d \n",p->valeur_sommet);
        return (p->valeur_sommet != previous) ? p->valeur_sommet : -1;
    }else{
        //printf(" \n On est dans le cas où il ya des sommets non marqués \n");
        if (previous == current){

            //premier sommet non marqué
            int nb = (Ensemble_Listes_DAdjacences + current)->degre;
            //condition sommet marque et l'arc est marque
            while (nb > 0 && p->marque && marquage_sommet[p->valeur_sommet]==1)
            {
                p = p->listeDAdjacence;
                nb--;
            }
            return (nb > 0) ? (p->valeur_sommet) : -1;
        }
              
        
        //on se met à previous
        while (p->valeur_sommet != previous){
            p = p->listeDAdjacence;
            //printf("\nwhile : %d \n",p->valeur_sommet);
        }

        //premier sommet apres previous
        p = p->listeDAdjacence;
        //printf("\n %d \n",p->valeur_sommet);

        while (p->valeur_sommet != previous && marquage_sommet[p->valeur_sommet]==1){
            p = p->listeDAdjacence;
            //printf("\nwhile : %d \n",p->valeur_sommet);
        }
        
        return (p->valeur_sommet != previous) ? p->valeur_sommet : -1;
    }
    
  
}

//______________________________ CODE _________________________________________

int *deepFirstSearch(Sommet *Ensemble_Listes_DAdjacences, int *ordreDeVisite, int *ordreDeVisite_inverse,
 int begin_value, int premierVoisin, int nbre_sommets, int nbre_arcs, int *tailleCode,int* marquage_sommet)
{
    
  
    //Le sommet actuelle
    Sommet *current = (Ensemble_Listes_DAdjacences + begin_value);
    //Le sommet precedent
    int previous = begin_value;

    //On cherche le premier voisin
    for (int i = 0; i <= premierVoisin; i++)
    {
        current = current->listeDAdjacence;
    }

    //MARQUAGE SOMMET
    marquage_sommet[previous]=1;
    marquage_sommet[current->valeur_sommet]=1;

    //MARQUAGE DE L'ARC
    markEdge(Ensemble_Listes_DAdjacences, previous, current->valeur_sommet,marquage_sommet);

    int j = 0;
    int indice = 0;
    int *k = (int *)malloc(sizeof(int));
    *k = 1;
    int sizeOfstack = nbre_arcs;
    int *stackOfchoosables = (int *)malloc(sizeof(int) *4* sizeOfstack);
    int *codeArray = (int *)malloc(sizeof(int) * nbre_arcs * 3);
    int indexCode = 0;
    int nbre_arcsVisites = 1;


    //On affecte l'ordre
    affecterNumordre(ordreDeVisite, previous, k);
    affecterNumordre(ordreDeVisite, current->valeur_sommet, k);

    affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, previous);
    affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, current->valeur_sommet);

    //Initialise le code avec les deux premiers sommets
    codeArray[indexCode++] = previous;
    codeArray[indexCode++] = current->valeur_sommet;

    //printf("\nVOISIN  À VISITER DU SOMMET %d : %d\n", begin_value , current->valeur_sommet );




// ____________________________________________________INITIALISATION OK________________________________________________


    int y=0;
    //Tant qu'on a pas visité tous les sommets
    while (nbre_arcsVisites < nbre_arcs && y<50)
    {
        y++;
        //On choisit le nouveau voisin
        int nextAvailableNeighbour = nextNeighbours(Ensemble_Listes_DAdjacences, previous, 
        current->valeur_sommet, marquage_sommet);
       
        //printf("\n Le nombre d'arcs visités est : %d \n", nbre_arcsVisites);

       
        if (nextAvailableNeighbour != -1){   

            //printf("\nVOISIN  À VISITER DU SOMMET %d : %d\n", current->valeur_sommet + 1,nextAvailableNeighbour + 1);
         
            if(marquage_sommet[nextAvailableNeighbour]==0){

               // printf("\n ________________________VOISIN NON MARQUE___________________________________");


                //MARQUAGE SOMMET
                marquage_sommet[nextAvailableNeighbour]=1;
  
                //MARQUAGE ARC
                markEdge(Ensemble_Listes_DAdjacences, current->valeur_sommet, nextAvailableNeighbour,marquage_sommet);
                
                //On a visité un arc en plus
                nbre_arcsVisites++;
                 
                //Affecte num ordre
                affecterNumordre(ordreDeVisite, nextAvailableNeighbour, k);
                affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, nextAvailableNeighbour);               
               //historique
                if (j >= sizeOfstack){
                        sizeOfstack += nbre_arcs / 2;
                        stackOfchoosables = (int *)realloc(stackOfchoosables, sizeof(int) * sizeOfstack);
                }

                stackOfchoosables[j] = current->valeur_sommet;
                j++;
                //printf("\n ON AFFICHE j  : %d  et stack %d \n",j, *(stackOfchoosables+j-1)+1);

                  
                codeArray[indexCode++] = nextAvailableNeighbour;          
                previous = current->valeur_sommet;
                current = (Ensemble_Listes_DAdjacences + nextAvailableNeighbour);
            
            }

            else{
                
                //printf("\n_________________________VOISIN MARQUE__________________________________");
                //MARQUAGE ARC
                markEdge(Ensemble_Listes_DAdjacences, current->valeur_sommet, nextAvailableNeighbour,marquage_sommet); 
                
                //On a visité un arc en plus
                nbre_arcsVisites++;
                
                //Affecte num ordre
                affecterNumordre(ordreDeVisite, nextAvailableNeighbour, k);
                affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, nextAvailableNeighbour);               

       

                if (j >= sizeOfstack){
                        sizeOfstack += nbre_arcs / 2;
                        stackOfchoosables = (int *)realloc(stackOfchoosables, sizeof(int) * sizeOfstack);
                }
                stackOfchoosables[j] = current->valeur_sommet;
                j++;
                //printf("\n ON AFFICHE j  : %d  et stack %d \n",j, *(stackOfchoosables+j-1)+1);

                codeArray[indexCode++] = nextAvailableNeighbour;
                    
                Sommet *q=(Ensemble_Listes_DAdjacences+nextAvailableNeighbour)->listeDAdjacence;
                
                while(q->valeur_sommet!=current->valeur_sommet){
                    q=q->listeDAdjacence;
                }
                //printf("\n ******La valeur de q est : %d  \n",q->valeur_sommet +1);
                
                
                
                
                
                
                
                if(q->marque==false){
                    //rintf("\nCAS 1\n");
                    //feedback
                    previous = nextAvailableNeighbour;
                    current = (Ensemble_Listes_DAdjacences + *(stackOfchoosables +j-1));
                    markEdge(Ensemble_Listes_DAdjacences,previous,current->valeur_sommet,marquage_sommet );
                    nbre_arcsVisites++;
                    
                    //codeArray[indexCode++] = previous;
                    codeArray[indexCode++] = current->valeur_sommet;
                    
                    //printf("\n current : %d \n",current->valeur_sommet+1);
                    //printf("\n previous : %d \n", previous+1); 
                }else{
                    //printf("\nCAS 2\n");
                    previous = current->valeur_sommet;
                    current = (Ensemble_Listes_DAdjacences + nextAvailableNeighbour);
                } 
            }
        
        
        }

    }


    
    affichageCode(codeArray, indexCode, ordreDeVisite, nbre_sommets, nbre_arcs, Ensemble_Listes_DAdjacences);
    printf("\nAffichage par ordre\n");
    affichageCode_numordre(codeArray, indexCode, ordreDeVisite, nbre_sommets, nbre_arcs, Ensemble_Listes_DAdjacences);
    printf("\n");
    *tailleCode = indexCode;

/*
    for(int i=0;i<nbre_sommets;i++){
        printf("\n %d \n", ordreDeVisite_inverse[i]);
    }
*/   
    
    


    free(stackOfchoosables);
    free(k);
    //free(Ensemble_Listes_DAdjacences);
    //free(codeArray);
    //free(ordreDeVisite);

    return codeArray;
}


int *deepFirstSearch2(Sommet *G1, Sommet *Ensemble_Listes_DAdjacences,
int *ordreDeVisite1, int *ordreDeVisite1_inverse, int *ordreDeVisite, int *ordreDeVisite_inverse,
 int begin_value, int premierVoisin, int nbre_sommets, int nbre_arcs, int *tailleCode,int* marquage_sommet,
 int * marquage_sommet1)
{
    int temp,temp1;
    int j = 0;
    int *k = (int *)malloc(sizeof(int));
    *k = 1;
    int sizeOfstack = nbre_arcs;
    int *stackOfchoosables = (int *)malloc(sizeof(int) *4* sizeOfstack);
    int *codeArray = (int *)malloc(sizeof(int) * nbre_arcs * 3);
    int indexCode = 0;
    int nbre_arcsVisites = 1;


    //Le sommet actuelle
    Sommet *current = (Ensemble_Listes_DAdjacences + begin_value);
    //Le sommet precedent
    int previous = begin_value;

    //On cherche le premier voisin
    for (int i = 0; i <= premierVoisin; i++)
    {
        current = current->listeDAdjacence;
    }

    affecterNumordre(ordreDeVisite, previous, k);
    affecterNumordre(ordreDeVisite, current->valeur_sommet, k);

    affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, previous);
    affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, current->valeur_sommet);
    
    //temp=ordreDeVisite[current->valeur_sommet]-1;   1
    //temp1= ordreDeVisite[previous]-1;   0
    
    //MARQUAGE DE L'ARC
    markEdge(Ensemble_Listes_DAdjacences, previous, current->valeur_sommet,marquage_sommet);
    markEdge(G1,ordreDeVisite1_inverse[0] -1,
                 ordreDeVisite1_inverse[1] -1,
                 marquage_sommet1);
   

    //MARQUAGE SOMMET
    marquage_sommet[previous]=1;
    marquage_sommet[current->valeur_sommet]=1;
    marquage_sommet1[ordreDeVisite1_inverse[0] -1]=1;
    marquage_sommet1[ordreDeVisite1_inverse[1] -1]=1;
    

    //Initialise le code avec les deux premiers sommets
    codeArray[indexCode++] = previous;
    codeArray[indexCode++] = current->valeur_sommet;


    /*_________________________________OK premier voisin*/ 
    int y=0;
    while(tous_marque(nbre_sommets,marquage_sommet)==false){
        y++;
        int nextAvailableNeighbour= nextNeighbours(Ensemble_Listes_DAdjacences, previous, 
            current->valeur_sommet, marquage_sommet);
    
        if (nextAvailableNeighbour != -1){   
               
                affecterNumordre(ordreDeVisite, nextAvailableNeighbour, k);
                affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, nextAvailableNeighbour);  

                int temp=ordreDeVisite[current->valeur_sommet ]-1;
                int temp1=ordreDeVisite[nextAvailableNeighbour]-1;
               
                markEdge(Ensemble_Listes_DAdjacences, current->valeur_sommet, nextAvailableNeighbour,marquage_sommet);
                markEdge(G1,
                 ordreDeVisite1_inverse[temp]-1,
                 ordreDeVisite1_inverse[temp1]-1,
                 marquage_sommet1);
                                 
                nbre_arcsVisites++;

                marquage_sommet[nextAvailableNeighbour]=1;
                marquage_sommet1[ordreDeVisite1_inverse[temp1]-1]=1;


                if(   (G1 + (ordreDeVisite1_inverse[temp1]-1))->degre != 
                 (Ensemble_Listes_DAdjacences + (ordreDeVisite_inverse[temp1]-1))->degre  ){
                     printf("\n DIFFERENCE DE SOMMET \n");
                     codeArray=NULL;
                     return codeArray;
                 }

               //historique
                if (j >= sizeOfstack){
                        sizeOfstack += nbre_arcs / 2;
                        stackOfchoosables = (int *)realloc(stackOfchoosables, sizeof(int) * sizeOfstack);
                }

                stackOfchoosables[j] = current->valeur_sommet;
                j++;
                //printf("\n ON AFFICHE j  : %d  et stack %d \n",j, *(stackOfchoosables+j-1)+1);
 
                codeArray[indexCode++] = nextAvailableNeighbour;          
                previous = current->valeur_sommet;
                current = (Ensemble_Listes_DAdjacences + nextAvailableNeighbour);
        }   
    }
     
    printf("\n Arcs %d   et   Total      %d\n",nbre_arcsVisites,nbre_arcs);
    while (nbre_arcsVisites < nbre_arcs && y<13)
    {
        y++;
          
        int temp1_neughbour = 
        nextNeighbours(G1, 
        ordreDeVisite1_inverse[ordreDeVisite[ previous ]-1]-1, 
        ordreDeVisite1_inverse[ordreDeVisite[current->valeur_sommet]-1]-1
        , marquage_sommet);

        int nextAvailableNeighbour= ordreDeVisite_inverse [ordreDeVisite1 [temp1_neughbour]-1]-1;
            
        //Affecte num ordre
        affecterNumordre(ordreDeVisite, nextAvailableNeighbour, k);
        affecterNumordre_inverse(ordreDeVisite_inverse,ordreDeVisite, nextAvailableNeighbour);               

        //MARQUAGE ARC
        markEdge(Ensemble_Listes_DAdjacences, current->valeur_sommet, nextAvailableNeighbour,marquage_sommet); 
        markEdge(G1,
        ordreDeVisite1_inverse[ordreDeVisite[current->valeur_sommet]-1]-1,
        ordreDeVisite1_inverse[ordreDeVisite[nextAvailableNeighbour]-1]-1,
        marquage_sommet1);
               
        //On a visité un arc en plus
        nbre_arcsVisites++;
   
        marquage_sommet[nextAvailableNeighbour]=1;
        marquage_sommet1[ordreDeVisite1_inverse[ordreDeVisite[nextAvailableNeighbour]-1]-1]=1;
    
        codeArray[indexCode++] = nextAvailableNeighbour;
        if (j >= sizeOfstack){
            sizeOfstack += nbre_arcs / 2;
            stackOfchoosables = (int *)realloc(stackOfchoosables, sizeof(int) * sizeOfstack);
        }
        stackOfchoosables[j] = current->valeur_sommet;
        j++;    
                
         //printf("\n ON AFFICHE j  : %d  et stack %d \n",j, *(stackOfchoosables+j-1)+1);
       
        Sommet *q=(Ensemble_Listes_DAdjacences+nextAvailableNeighbour)->listeDAdjacence;
                
        while(q->valeur_sommet!=current->valeur_sommet){
            q=q->listeDAdjacence;
        }  
        if(q->marque==false){
            //rintf("\nCAS 1\n");
            //feedback
            previous = nextAvailableNeighbour;
            current = (Ensemble_Listes_DAdjacences + *(stackOfchoosables +j-1));

            temp=ordreDeVisite[previous ]-1;
            temp1=ordreDeVisite[current->valeur_sommet ]-1;
                    
            markEdge(Ensemble_Listes_DAdjacences,previous,current->valeur_sommet,marquage_sommet );
            markEdge(G1,
            ordreDeVisite1_inverse[temp]-1,
            ordreDeVisite1_inverse[temp1]-1,
            marquage_sommet );
                    
            nbre_arcsVisites++;        
            //codeArray[indexCode++] = previous;
            codeArray[indexCode++] = current->valeur_sommet;
                  
            } 
        else{
            //printf("\nCAS 2\n");
                previous = current->valeur_sommet;
                current = (Ensemble_Listes_DAdjacences + nextAvailableNeighbour);
            }
        }
    
        
       
    
    printf("\n Arcs \n%d",nbre_arcsVisites);






    printf("\n");
    affichageCode(codeArray, indexCode, ordreDeVisite, nbre_sommets, nbre_arcs, Ensemble_Listes_DAdjacences);
    printf("\nOrdre :\n");
    affichageCode_numordre(codeArray, indexCode, ordreDeVisite, nbre_sommets, nbre_arcs, Ensemble_Listes_DAdjacences);
    printf("\n");
    *tailleCode = indexCode;


    free(stackOfchoosables);
    free(k);
    //free(Ensemble_Listes_DAdjacences);
    //free(codeArray);
    //free(ordreDeVisite);

    return codeArray;
}

//______________________________________________ ISOMORPHES ________________________________________________________

bool compareStats(Stat *stats_G1, Stat *stats_G2, int taille)
{
    int i = 1;
    int j = taille - 1;

    while (i <= j && stats_G1[i].nbOccDeg == stats_G2[i].nbOccDeg && stats_G1[j].nbOccDeg == stats_G2[j].nbOccDeg)
    {
        j--;
        i++;
    }

    return !(i <= j);
}

bool compareListeAdjacences(Sommet *G1, Sommet *G2, int *OrdreVisite1, int* OrdreVisite2,
 int sommetActuel_G1,int sommetActuel_G2 , int nbre_sommets){
    
    Sommet *p = (G2 + sommetActuel_G2)->listeDAdjacence;
    Sommet *k = (G1 + sommetActuel_G1)->listeDAdjacence;
    int *tab;

    //printf("\n Le sommet actuelle de G1 : %d et celui de G2 est %d \n", sommetActuel_G1+1,sommetActuel_G2+1);
    
    bool temp=false;
    
    if (   (G2+sommetActuel_G2)==NULL || (G1+sommetActuel_G1)==NULL ||
     (G2 + sommetActuel_G2)->degre != (G1 + sommetActuel_G1)->degre )
    {
        printf("\n le sommet n'as pas le même degre : \n ");
        return false;
    }
    
    else
    {     
        tab=(int*)calloc(nbre_sommets, sizeof(int)); 
        while (k != (G1 + sommetActuel_G1) )
        {
            tab[OrdreVisite1[k->valeur_sommet]-1] = 1;
            k = k->listeDAdjacence;
        }
        
        while (p != (G2 + sommetActuel_G2) && tab[OrdreVisite2[p->valeur_sommet]-1] == 1)
        {
            p = p->listeDAdjacence;
        }
        if(p == (G2 + sommetActuel_G2)){
            temp= true;
        }
        //printf("\n Cas 2 : bool vaut %d \n ", temp);  
        
    }
    free(tab);
    return p == (G2 + sommetActuel_G2);
}

bool compareCode(int *CodeArray1, int *CodeArray2, Sommet *G1, Sommet *G2,int *OrdreVisite1, int *OrdreVisite2 ,int tailleCode1, int tailleCode2, int nbre_sommets )
{
    //1 5 3 2 4 C1
    //1 5 3 2 4 C2

    if (tailleCode1 != tailleCode2)
        return false;

    int i = 0;
    int j = tailleCode1 - 1;
    bool ok = true;
    
    int *visiteCode = (int *)calloc( nbre_sommets,sizeof(int) ); // AMÉLIORER 
    
    while (i <= j && OrdreVisite1[CodeArray1[i]] == OrdreVisite2[CodeArray2[i]] && 
    OrdreVisite1[CodeArray1[j]] == OrdreVisite2[CodeArray2[j]] && ok)
    {
        if (visiteCode[OrdreVisite1[CodeArray1[i]]] == 0)
        {
            visiteCode[OrdreVisite1[CodeArray1[i]]] = 1;
            ok = compareListeAdjacences(G1, G2, OrdreVisite1, OrdreVisite2, CodeArray1[i], CodeArray2[i], nbre_sommets);
            i++;
        }else
        {
            i++;
        }
        
        if (visiteCode[OrdreVisite1[CodeArray1[j]]] == 0)
        {
            visiteCode[OrdreVisite1[CodeArray1[j]]] = 1;
            ok = ok && compareListeAdjacences(G1, G2, OrdreVisite1, OrdreVisite2, CodeArray1[j],CodeArray2[j], nbre_sommets);
            j--;
        }else
        {
            j--;
        }
        
    }
    free(visiteCode);
    return !(i <= j);
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

bool isIsomorph(Sommet *G1, Sommet *G2, Stat *stats_G1, Stat *stats_G2, int *CodeArray1, 
int * OrdreVisite1, int * OrdreVisite1_inverse,  int nbre_sommets_G1, int nbre_sommets_G2, int tailleCode1,int nbre_arcs1, 
int nbre_arcs2,int* marquage_sommet,int* marquage_sommet1)
{
    if (nbre_sommets_G1 != nbre_sommets_G2)
    {
        printf("\n NON ISOMORPHES CAR NBRES SOMMETS DIFFERENTS");
        return false;
    }
    else
    {
        if (!compareStats(stats_G1, stats_G2, nbre_sommets_G1))
        {
            printf("\n DIFFERENTES STATS");
            return false;
        }
        else
        {
            //GÉNÉRER LES CODES À COMAPARER AVEC CODEARRAY1
            int begin2 = premierSommetNbOccDEGMin(stats_G1,nbre_sommets_G1); // RECUPÈRE LE SOMMET PAR LEQUEL ON A COMMENCÉ LE PARCOURS dans G1                    
            int nb, taillecode2;
            int *ordreDeVisite2 = (int*)malloc(sizeof(int) * nbre_sommets_G2);
            int *ordreDeVisite2_inverse = (int*)malloc(sizeof(int) * nbre_sommets_G2);
            int *list = sommet_deg_i(G2, (G1+begin2)->degre , nbre_sommets_G2, &nb); // Sommets candidats pour débuter le parcours dans G2
            bool found = false;
            int i=0;
            
            while(i<nb && !found){
                //L'élément génère un certain nombre de codes i.e deg codes
                int j = 0;
                int deg_i= (G2+list[i])->degre;
                //printf(" DEGRE SOMMET %d, %d\t",list[i]+1,deg_i);
                
                while(j < deg_i && !found){
                    //initOrdreVisite(ordreDeVisite,nbre_sommets_G2);
                    free(ordreDeVisite2);
                    free(marquage_sommet);
                    free(marquage_sommet1);
                    ordreDeVisite2 = (int*)calloc(nbre_sommets_G2, sizeof(int));
                    ordreDeVisite2_inverse = (int*)calloc(nbre_sommets_G2, sizeof(int));
                    marquage_sommet=(int*)calloc(nbre_sommets_G2, sizeof(int));
                    marquage_sommet1=(int*)calloc(nbre_sommets_G2, sizeof(int));
                    reinitialiser(G2,nbre_sommets_G2);
                    reinitialiser(G1,nbre_sommets_G1);


                    int *codeArray2=deepFirstSearch2(G1,G2, OrdreVisite1, OrdreVisite1_inverse,
                     ordreDeVisite2, ordreDeVisite2_inverse,
                    list[i], j, nbre_sommets_G2, nbre_arcs2, &taillecode2,marquage_sommet,marquage_sommet1);
                    found = compareCode(CodeArray1, codeArray2, G1, G2, OrdreVisite1, ordreDeVisite2, 
                    tailleCode1, taillecode2, nbre_sommets_G1);
                    j++;
                }
                i++;
            }
    
            return found;
        }
    }
}

int main(int argc, char const *argv[])
{
    srand(getpid()); //getpid() est une fonction qui envoie l'identifiant du processus lancé (le programme) donc le générateur  va générer à chaque fois des suites de nombres différents.
    //time(NULL)

        //-------------------------------------PREMIER GRAPHE-------------------------------------
    printf("\n----------------------------------------PREMIER GRAPHE-------------------------------\n");    
    int somme_deg_G1;
    int nbre_sommets_G1= 4;
    Stat *stats_G1 = initStats(nbre_sommets_G1);
    int begin_value_G1, tailleCode1;
    int *ordreDeVisite1 = (int *)calloc(nbre_sommets_G1,sizeof(int) );
    int *ordreDeVisite1_inverse = (int *)calloc(nbre_sommets_G1,sizeof(int) );
    int * marquage_sommet_G1= (int*)calloc(sizeof(int),nbre_sommets_G1);
    Sommet *G1=generate_Adjacence_Lists(nbre_sommets_G1, stats_G1, ordreDeVisite1, &somme_deg_G1);

    begin_value_G1 = premierSommetNbOccDEGMin(stats_G1, nbre_sommets_G1);
    int * codeArray1= deepFirstSearch(G1, ordreDeVisite1, ordreDeVisite1_inverse, begin_value_G1, rand()%(G1 + begin_value_G1)->degre ,
    nbre_sommets_G1,somme_deg_G1, &tailleCode1,marquage_sommet_G1);
    printf("\n\n-------------------------------------DEUXIÈME GRAPHE------------------------------------\n\n");
    /*
    if(isIsomorph(G1, G1, stats_G1, stats_G1, codeArray1, ordreDeVisite1, nbre_sommets_G1, nbre_sommets_G1, tailleCode1, *somme_deg_G1/2, *somme_deg_G1/2)){
        printf("\nISOMORPHES");
    }else printf("\nNON ISOMORPHES");
      */

        //-------------------------------------DEUXIÈME GRAPHE------------------------------------
    printf("\n\n-------------------------------------DEUXIÈME GRAPHE------------------------------------\n\n");
    int somme_deg_G2 ;
    int tailleCode2;
    int nbre_sommets_G2= 4;
    Stat *stats_G2 = initStats(nbre_sommets_G2);
    int *ordreDeVisite2 = (int *)calloc( nbre_sommets_G2, sizeof(int) );
    int *ordreDeVisite2_inverse = (int *)calloc( nbre_sommets_G2, sizeof(int) );
    int * marquage_sommet_G2= (int*)calloc(sizeof(int),nbre_sommets_G2);
    Sommet *G2=generate_Adjacence_Lists(nbre_sommets_G2, stats_G2, ordreDeVisite2, &somme_deg_G2);
    /*  
    int begin_value_G2= premierSommetNbOccDEGMin(stats_G2, nbre_sommets_G2);
    
    int * codeArray2= deepFirstSearch(G2, ordreDeVisite2,ordreDeVisite2_inverse, begin_value_G2, rand()%(G2 + begin_value_G2)->degre ,
    nbre_sommets_G2, somme_deg_G2, &tailleCode2,marquage_sommet_G2);
*/
    if(isIsomorph (G1,G2,stats_G1,stats_G2,codeArray1, ordreDeVisite1,ordreDeVisite1_inverse ,nbre_sommets_G1,nbre_sommets_G2,
    tailleCode1, somme_deg_G1/2, somme_deg_G2, marquage_sommet_G2 ,marquage_sommet_G1) ){
    printf("\nISOMORPHES");
    }else{ printf("\nNON ISOMORPHES");}

    //compareStats(stats_G1,stats_G2,nbre_sommets_G1);
    //compareListeAdjacences(G1,G2,ordreDeVisite1,ordreDeVisite2,1,2,nbre_sommets_G1);
    //compareCode(codeArray1,codeArray2,G1,G2,ordreDeVisite1,ordreDeVisite2,tailleCode1,tailleCode2,nbre_sommets_G1);
    //reinitialiser(G1,nbre_sommets_G1);
    
    free(G1);
    free(codeArray1);
    free(ordreDeVisite1);
    free(stats_G1); 
    /*free(G2);
    free(ordreDeVisite2);
    free(stats_G2);

    free(codeArray2);*/
    return 0;
}
