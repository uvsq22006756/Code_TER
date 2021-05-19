#include "planarMapGraphIsomorphism.h"

/**********************************************************************************************
    pour tout graphe planaire connexe  G=(V,E) ayant au moins deux arêtes, on a l’inégalité |E|≤ 3|V| −6 .

    1ère étape: 
        Générer une carte
            ===> Prendre une nombre de sommets de manière aléatoire
            ===> Prendre une nombre d'arcs de manière  aléatoire telque: |E| ≤ 3|V| − 6 
            ===> Liste d' adjacence pour chaque sommet:
                    * FORMAT: sommet degre voisin priorite voisin priorite
                    * Si (i,j) £ E alors (j,i) £ E 
                    * d(x)>=1 , x £ V

    2ème étape:
        Vérifier la planarité
            ===> 4-Coloriable
            ===> non Homéomorphe à K3,3 ni à K5


    3ème étape:
        Algo de Parcours


***************************************************************************************************/

/*
int caracteristics(){
    
    int  nbre_sommets =0;
    bool test;
    
    do{
            nbre_sommets = rand() %100; //1000000
    }while (nbre_sommets < 3);
    

    return nbre_sommets;

}
*/


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
        //printf("\n%d===>", i+1 );
        //fprintf(F, "%d\t", i + 1);

        while (q != (Ensemble_Listes_DAdjacences + i))
        {
            //printf("%d\t",q->valeur_sommet+1);
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

//*************************************ALGO DE PARCOURS*************************************

int nextNeighbours(Sommet *Ensemble_Listes_DAdjacences, int previous, int current)
{
    Sommet *p = (Ensemble_Listes_DAdjacences + current)->listeDAdjacence;
    if (previous == current)
    {
        int nb = (Ensemble_Listes_DAdjacences + current)->degre;
        while (nb > 0 && p->marque)
        {
            p = p->listeDAdjacence;
            nb--;
        }
        return (nb > 0) ? (p->valeur_sommet) : -1;
    }

    while (p->valeur_sommet != previous)
    {
        p = p->listeDAdjacence;
    }

    p = p->listeDAdjacence;

    while (p->valeur_sommet != previous && p->marque)
    {
        p = p->listeDAdjacence;
    }

    return (p->valeur_sommet != previous) ? p->valeur_sommet : -1;
}

void markEdge(Sommet *Ensemble_Listes_DAdjacences, int current, int nextAvailableNeighbour)
{

    (Ensemble_Listes_DAdjacences + current)->marque = true;
    (Ensemble_Listes_DAdjacences + nextAvailableNeighbour)->marque = true;
    Sommet *p = (Ensemble_Listes_DAdjacences + current)->listeDAdjacence;
    while (p != (Ensemble_Listes_DAdjacences + current) && p->valeur_sommet != nextAvailableNeighbour)
    {
        p = p->listeDAdjacence;
    }
    p->marque = true;
    //MARQUAGE SYMÉTRIQUE
    p = (Ensemble_Listes_DAdjacences + nextAvailableNeighbour)->listeDAdjacence;
    while (p != (Ensemble_Listes_DAdjacences + nextAvailableNeighbour) && p->valeur_sommet != current)
    {
        p = p->listeDAdjacence;
    }
    p->marque = true;
}

void affecterNumordre(int *ordreDeVisite, Sommet *Ensemble_Listes_DAdjacences, int sommet, int *numActuel)
{
    /*
    Sommet *p = (Ensemble_Listes_DAdjacences + sommet);
    int degre = p->degre;
    for (int i = 0; i <= degre; i++)
    {
        int val = p->valeur_sommet;
        if (ordreDeVisite[val] == -1)
        {
            ordreDeVisite[val] = *numActuel;
            *(numActuel) += 1;
        }
        p = p->listeDAdjacence;
    }
    */

    if (ordreDeVisite[sommet] == 0)
    {
        ordreDeVisite[sommet] = *numActuel;
        *(numActuel) += 1;
    }
}

int indice(int *ordreDeVisite, int val, int nbre_sommet)
{
    int i = 0;
    while (i < nbre_sommet && ordreDeVisite[i] != val)
    {
        i++;
    }
    return i;
}

void affichageCode(int *codeArray, int indexCode, int *ordreDeVisite, int nbre_sommet, int nbre_arcs, Sommet *Ensemble_Listes_DAdjacences)
{
    //int *finalCode=(int*)malloc(sizeof(int) *(indexCode + nbre_arcs +2));
    int k = 0;
    FILE *F = fopen("Code.data", "w");
    if (F == NULL)
        exit(4);

    FILE *F2 = fopen("CodeSansParanthese.data", "w");
    if (F2 == NULL)
        exit(5);

    //printf(" CODE: ");

    for (int i = 0; i < indexCode; i++)
    {
        fprintf(F, "%d ", ordreDeVisite[codeArray[i]]);
        //printf("%d",ordreDeVisite[codeArray[i]]);
        //finalCode[k++]=ordreDeVisite[codeArray[i]];
        //**NOPE int ind= indice(ordreDeVisite,codeArray[i],nbre_sommet);

        fprintf(F2, "%d ", ordreDeVisite[codeArray[i]]);
        Sommet *p = (Ensemble_Listes_DAdjacences + codeArray[i])->listeDAdjacence;
        fprintf(F, "(");
        //printf("(");
        (Ensemble_Listes_DAdjacences + codeArray[i])->marque = false;
        while (p != (Ensemble_Listes_DAdjacences + codeArray[i]))
        {
            fprintf(F, "%d ", ordreDeVisite[p->valeur_sommet]);
            //printf("%d",ordreDeVisite[p->valeur_sommet]);
            //finalCode[k++]=ordreDeVisite[p->valeur_sommet];
            fprintf(F2, "%d ", ordreDeVisite[p->valeur_sommet]);
            
            p->marque=false;
            
            p = p->listeDAdjacence;
        }
        //printf(")\t");
        fprintf(F, ")\t");
    }
}

int *deepFirstSearch(Sommet *Ensemble_Listes_DAdjacences, int *ordreDeVisite, int begin_value, int premierVoisin, int nbre_sommets, int nbre_arcs, int *tailleCode)
{
    //OPTIMISATION: ORDRE VISITE À INITIALISER DANS LA GÉNÉRATION DU GRAPHE
    //CHOISIR ALÉATOIREMENT LE PREMIER VOISIN À VISITER DU SOMMET BEGIN_VALUE
    Sommet *current = (Ensemble_Listes_DAdjacences + begin_value);
    int previous = begin_value;

    //int val_aleatoire = rand() % (current->degre);

    for (int i = 0; i <= premierVoisin; i++)
    {
        current = current->listeDAdjacence;
    }

    //printf("\nPREMIER VOISIN À VISITER DU SOMMET %d : %d\n", begin_value + 1, current->valeur_sommet + 1);
    //MARQUAGE DE L'ARC
    current->marque = true;
    markEdge(Ensemble_Listes_DAdjacences, previous, current->valeur_sommet);
    int j = 0;
    int indice = 0;
    int *k = (int *)malloc(sizeof(int));
    *k = 1;
    int sizeOfstack = nbre_arcs;
    int *stackOfchoosables = (int *)malloc(sizeof(int) * sizeOfstack);

    int *codeArray = (int *)malloc(sizeof(int) * nbre_arcs * 2);
    int indexCode = 0;

    int nbre_arcsVisites = 1;

    /*
    int *ordreDeVisite = (int *)malloc(sizeof(int) * nbre_sommets);
    for (int i = 0; i < nbre_sommets; i++)
    {
        *(ordreDeVisite + i) = -1;
    }
    */

    affecterNumordre(ordreDeVisite, Ensemble_Listes_DAdjacences, previous, k);
    affecterNumordre(ordreDeVisite, Ensemble_Listes_DAdjacences, current->valeur_sommet, k);

    codeArray[indexCode++] = previous;
    codeArray[indexCode++] = current->valeur_sommet;

    while (nbre_arcsVisites != nbre_arcs)
    {
        //affecterNumordre(ordreDeVisite,Ensemble_Listes_DAdjacences,current->valeur_sommet,k);
        int nextAvailableNeighbour = nextNeighbours(Ensemble_Listes_DAdjacences, previous, current->valeur_sommet);
        if (nextAvailableNeighbour != -1)
        {
            markEdge(Ensemble_Listes_DAdjacences, current->valeur_sommet, nextAvailableNeighbour);
            affecterNumordre(ordreDeVisite, Ensemble_Listes_DAdjacences, nextAvailableNeighbour, k);
            nbre_arcsVisites++;
            // GERER TAILLE PILE
            // Étant donné que la pile n'est pas déchargée au cours de l'éxécution, il se peut qu'il y ait beaucoup de feedbacks
            // À tel point que la pile soit remplie. Du coup je réalloue encore de l'espace (j'ai juste choisi d'ajouter le nbre d'arc/2 j'aurai bien pu ajouter le nbre de sommets).
            // NB: C'est stackOfchoosables qui est la pile. Mais e réalité c'est un tableau et l'ajout se fait en queue.
            if (j >= sizeOfstack)
            {
                sizeOfstack += nbre_arcs / 2;
                stackOfchoosables = (int *)realloc(stackOfchoosables, sizeof(int) * sizeOfstack);
            }

            //On ajoute l'élément actuel (current) à la pile pour pouvoir, éventuellement l'utiliser  quand il y aura le feedback
            stackOfchoosables[j++] = current->valeur_sommet;
            // le nextAvailableNeighbour est ajouté au code (codeArray n'est pas le code définitif  il contient la suite des sommets telsqu'on les rencontre lors du parcours )
            codeArray[indexCode++] = nextAvailableNeighbour;
            // previous prend current et current prend le sommet correspondant à nextAvailableNeighbour
            previous = current->valeur_sommet;
            current = (Ensemble_Listes_DAdjacences + nextAvailableNeighbour);
        }
        else
        {
            //ICI on est dans le cas où on doit faire un feedback
            // Du coup on prend le sommet de la liste (ON DÉPILE)
            current = (Ensemble_Listes_DAdjacences + stackOfchoosables[--j]);
            previous = stackOfchoosables[j];
            //JUSTE MESURE DE PRÉCAUTION CAR SI ON ARRIVE À VIDER LA PILE ça veut dire qu'on pourra plus faire de feedbacks donc la fin de l'algo.
        }
    }

    affichageCode(codeArray, indexCode, ordreDeVisite, nbre_sommets, nbre_arcs, Ensemble_Listes_DAdjacences);
    *tailleCode = indexCode;
    

    /*

    printf("\n");
    printf("CODE:\t");
    for (int i = 0; i < indexCode; i++)
    {
        printf("%d\t", codeArray[i] + 1);
    }
    
    printf("SOMMETS DU GRAPHE: ");
    for (int i = 0; i < nbre_sommets; i++)
    {
        printf("%d\t",i+1);
    }
    printf("\n");
    printf("ORDRE  DE VISITE: ");
    for (int i = 0; i < nbre_sommets; i++)
    {
        printf("%d\t",ordreDeVisite[i]);
    }
    
    printf("\n");
    printf("CODE:\t");
    for (int i = 0; i < indexCode; i++)
    {
        printf("%d\t",codeArray[i]+1);
    }
    */

    free(stackOfchoosables);
    free(k);
    //free(Ensemble_Listes_DAdjacences);
    //free(codeArray);
    //free(ordreDeVisite);

    return codeArray;
}

bool isIsomorph(Sommet *G1, Sommet *G2, Stat *stats_G1, Stat *stats_G2, int *CodeArray1, int * OrdreVisite1, int nbre_sommets_G1, int nbre_sommets_G2, int tailleCode1,int nbre_arcs1, int nbre_arcs2)
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
                    ordreDeVisite2 = (int*)calloc(nbre_sommets_G2, sizeof(int));
                    int *codeArray2=deepFirstSearch(G2, ordreDeVisite2,list[i], j, nbre_sommets_G2, nbre_arcs2, &taillecode2);
                    found = compareCode(CodeArray1, codeArray2, G1, G2, OrdreVisite1, ordreDeVisite2, tailleCode1, taillecode2, nbre_sommets_G1);
                    j++;
                }
                i++;
            }
    
            return found;
        }
    }
}

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
    
    while (i <= j && OrdreVisite1[CodeArray1[i]] == OrdreVisite2[CodeArray2[i]] && OrdreVisite1[CodeArray1[j]] == OrdreVisite2[CodeArray2[j]] && ok)
    {
        if (visiteCode[OrdreVisite1[CodeArray1[i]]] == 0)
        {
            visiteCode[OrdreVisite1[CodeArray1[i]]] = 1;
            ok = compareListeAdjacences(G1, G2, OrdreVisite1, OrdreVisite2, CodeArray1[i],CodeArray2[i++], nbre_sommets);
        }else
        {
            i++;
        }
        
        if (visiteCode[OrdreVisite1[CodeArray1[j]]] == 0)
        {
            visiteCode[OrdreVisite1[CodeArray1[j]]] = 1;
            ok = ok && compareListeAdjacences(G1, G2, OrdreVisite1, OrdreVisite2, CodeArray1[j],CodeArray2[j--], nbre_sommets);
        }else
        {
            j--;
        }
        
    }
    free(visiteCode);
    return !(i <= j);
}

/*
bool compareListeAdjacence(Sommet *G1, Sommet *G2, int *OrdreVisite1, int* OrdreVisite2 , int sommetActuel)
{
    //printf("\t%d", sommetActuel+1);

    if ((G2 + sommetActuel)->degre != (G1 + sommetActuel)->degre)
    {
        return false;
    }
    else
    {
        Sommet *p = (G2 + sommetActuel)->listeDAdjacence;
        Sommet *k = (G1 + sommetActuel)->listeDAdjacence;
        Sommet *q;

        while (OrdreVisite2[p->valeur_sommet] != OrdreVisite1[k->valeur_sommet] && p != (G2 + sommetActuel))
        {
            p = p->listeDAdjacence;
        }

        
        //p pointe sur l'élément tel que p=k

        if (OrdreVisite2[p->valeur_sommet] == OrdreVisite1[k->valeur_sommet])
        {
            q = p;
            //on parcourt
            k = k->listeDAdjacence;
            p = p->listeDAdjacence;
        
            //tant qu'on revient pas au debut et qu'on a l'egalite
            if (p == (G2 + sommetActuel))
            {
                p = p->listeDAdjacence;
            }
            if (k == (G1 + sommetActuel))
            {
                k = k->listeDAdjacence;
            }

            while (OrdreVisite1[k->valeur_sommet] == OrdreVisite2[p->valeur_sommet] && p != q)
            {
                p = p->listeDAdjacence;
                k = k->listeDAdjacence;
                if (p == (G2 + sommetActuel))
                {
                    p = p->listeDAdjacence;
                }
                if (k == (G1 + sommetActuel))
                {
                    k = k->listeDAdjacence;
                }
            }
            return (p == q);
        }
        else
        {
            printf("\nLISTES D'ADJACENCES NON EQUIVALENTE À ROTAION PRÈS");
            return false;
        }
    }
}
*/
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

bool compareListeAdjacences(Sommet *G1, Sommet *G2, int *OrdreVisite1, int* OrdreVisite2 , int sommetActuel_G1,  int sommetActuel_G2, int nbre_sommets){

    if ((G2+sommetActuel_G2)==NULL || (G1+sommetActuel_G1)==NULL ||(G2 + sommetActuel_G2)->degre != (G1 + sommetActuel_G1)->degre)
    {
        return false;
    }else
    {
        int *tab=(int*)calloc(nbre_sommets, sizeof(int));
        Sommet *p = (G2 + sommetActuel_G2)->listeDAdjacence;
        Sommet *k = (G1 + sommetActuel_G1)->listeDAdjacence;
        Sommet *q;
        while (k != (G1 + sommetActuel_G1))
        {
            tab[OrdreVisite1[k->valeur_sommet]-1] = 1;
            k = k->listeDAdjacence;
        }
        
        while (p != (G2 + sommetActuel_G2) && tab[OrdreVisite2[p->valeur_sommet]-1] == 1)
        {
            p = p->listeDAdjacence;
        }
        free(tab);
        return p == (G2 + sommetActuel_G2);
    }
        
}