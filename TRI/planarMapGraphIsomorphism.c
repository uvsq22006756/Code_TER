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

    (liste->nbre_voisins_ajoutes)+=1;
}

void ajoutSommetByDeg(SommetByDeg *liste, int val, int pos){
    SommetByDeg *s = (SommetByDeg *)malloc(sizeof(SommetByDeg));
    s->suiv = NULL;
    s->deg = val;
    s->indice = pos;

    if (liste->suiv != NULL)
    {
        SommetByDeg *p = liste;
        SommetByDeg *q;
        //CHERCHER LA BONNE POSITION POUR INSÉRER
        while (p->suiv && (val > p->suiv->deg))
        {
            p = p->suiv;
        }
        // SI ON A PAS TOUT PARCOURS (INSERTION AU MILIEU)
        if(p->suiv){
            s->suiv = p->suiv;
            p->suiv = s;
        }else //INSERTION EN QUEUE
        {
            p->suiv=s;
        }
        
    }else// PREMIERE INSERTION
    {
        liste->suiv=s;
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

int *initialiser_Ensemble_Liste_DAjacences(Sommet *Ensemble_Listes_DAdjacences, int* Liste_Voisins_Libres, int nbre_sommets)
{

    int k;
    int min_des_degre_choisi = nbre_sommets;
    int *tampon = (int *)malloc(sizeof(int) * nbre_sommets);
    for (int i = 0; i < nbre_sommets; i++)
    {

        (Ensemble_Listes_DAdjacences + i)->valeur_sommet = i;
        (Ensemble_Listes_DAdjacences + i)->listeDAdjacence = NULL;
        (Ensemble_Listes_DAdjacences + i)->marque = false;
        (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes = 0;
        k = 1 + rand() % (nbre_sommets - 1);
        (Ensemble_Listes_DAdjacences + i)->degre = k;
        *(Liste_Voisins_Libres +i) = i;
        *(tampon + i) = i;
    }

    return tampon;
}

void generate_Adjacence_Lists(int nbre_sommets, SommetByDeg *Ensemble_Listes_DAdjacences_DEG , Stat *statsDegres, int *somme_deg)
{

    Sommet *Ensemble_Listes_DAdjacences = (Sommet *)malloc(sizeof(Sommet) * nbre_sommets);
    int *Liste_Voisins_Libres = (int *)malloc(sizeof(int) * nbre_sommets);
    int valeur_Du_Nouv_Voisin;
    int domaineDeChoix, x, temp, temp1;

    int *tampon = initialiser_Ensemble_Liste_DAjacences(Ensemble_Listes_DAdjacences, Liste_Voisins_Libres ,nbre_sommets);

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
                //(Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes = (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes +1;

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
                domaineDeChoix--;

                // ****************Si (i,j) £ E alors (j,i) £ E  *********************
                ajout(Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin, i);
                //(Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin)->nbre_voisins_ajoutes = (Ensemble_Listes_DAdjacences + valeur_Du_Nouv_Voisin)->nbre_voisins_ajoutes +1; 
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
        (Ensemble_Listes_DAdjacences_DEG + i)->deg = (Ensemble_Listes_DAdjacences + i)->degre = (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes;
        (Ensemble_Listes_DAdjacences_DEG + i)->indice = i;
        (Ensemble_Listes_DAdjacences_DEG + i)->suiv = NULL;
        int pos = (Ensemble_Listes_DAdjacences + i)->nbre_voisins_ajoutes;
        while (q != (Ensemble_Listes_DAdjacences + i))
        {
            ajoutSommetByDeg(Ensemble_Listes_DAdjacences_DEG + i, (Ensemble_Listes_DAdjacences + q->valeur_sommet)->nbre_voisins_ajoutes, pos--);
            printf("%d\t",q->valeur_sommet+1);
            //fprintf(F, "%d ", q->valeur_sommet + 1);
            q = q->listeDAdjacence;
            *(somme_deg) += 1;
            k++;
        }

        
        (statsDegres[k].nbOccDeg) += 1;
        if ((statsDegres + k)->somet == -1)
        {
            (statsDegres + k)->somet = i;
        }
        
        //fprintf(F, "\n");
    }

    free(Liste_Voisins_Libres);
    free(Ensemble_Listes_DAdjacences);
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

void fusion(int *a, int n, int m) {
    int i, j, k;
    int *x = malloc(n * sizeof (int));
    for (i = 0, j = m, k = 0; k < n; k++) {
        
        x[k] = j == n      ? a[i++]
             : i == m      ? a[j++]
             : a[j] < a[i] ? a[j++]
             :               a[i++];
        
        
    }
    for (i = 0; i < n; i++) {
        a[i] = x[i];
    }
    free(x);
}
 
void tri_fusion(int *liste, int taille){
    if (taille < 2) return;
    int milieu = taille / 2;
    tri_fusion(liste, milieu);
    tri_fusion((liste +milieu), taille - milieu);
    fusion(liste, taille, milieu);
}

void fusionDeg(SommetByDeg *a, int n, int m) {
    int i, j, k;
    SommetByDeg *x = malloc(n * sizeof (SommetByDeg));
    for (i = 0, j = m, k = 0; k < n; k++) {

        int li = i;
        int lj = j;
        x[k].deg = j == n          ? a[i++].deg
             : i == m              ? a[j++].deg
             : a[j].deg < a[i].deg ? a[j++].deg
             :                       a[i++].deg;
        

        x[k].indice = lj == n        ? a[li].indice
             : li == m               ? a[lj].indice
             : a[lj].deg < a[li].deg ? a[lj].indice
             :                         a[li].indice;

    }
    for (i = 0; i < n; i++) {
        a[i].deg    = x[i].deg;
        a[i].indice = x[i].indice;
    }
    
    free(x);
}
 
void tri_fusionDeg(SommetByDeg *liste, int taille){
    if (taille < 2) return;
    int milieu = taille / 2;
    tri_fusionDeg(liste, milieu);
    tri_fusionDeg(liste + milieu, taille - milieu);
    fusionDeg(liste, taille, milieu);
}

void fusionDeg2(SommetByDeg *a, SommetByDeg *b, int n, int m){
    int i, j, k, i1,j1;
    SommetByDeg *x =(SommetByDeg *) malloc(n * sizeof (SommetByDeg));
    SommetByDeg *y =(SommetByDeg *)malloc(n * sizeof (SommetByDeg));

    for (i = 0,i1=0, j = m, j1=m, k = 0; k < n; k++) {

        int li = i;
        int lj = j;
        x[k].deg = j == n          ? a[i++].deg
             : i == m              ? a[j++].deg
             : a[j].deg < a[i].deg ? a[j++].deg
             :                       a[i++].deg;

    
        x[k].indice = lj == n        ? a[li].indice
             : li == m               ? a[lj].indice
             : a[lj].deg < a[li].deg ? a[lj].indice
             :                         a[li].indice;


        int li1 = i1;
        int lj1 = j1;
        y[k].deg = j1 == n          ? b[i1++].deg
             : i1 == m              ? b[j1++].deg
             : b[j1].deg < b[i1].deg ? b[j1++].deg
             :                       b[i1++].deg;
        
        y[k].indice = lj1 == n        ? a[li1].indice
             : li1 == m               ? a[lj1].indice
             : b[lj1].deg < b[li1].deg ? a[lj1].indice
             :                         a[li1].indice;
    }

    for (i = 0; i < n; i++) {
        a[i].deg    = x[i].deg;
        a[i].indice = x[i].indice;

        b[i].deg    = y[i].deg;
        b[i].indice = y[i].indice;
    }
    
    free(x);
    free(y);
}

void tri_fusionDeg2(SommetByDeg *liste, SommetByDeg *liste2, int taille){
    if (taille < 2) return;
    int milieu = taille / 2;
    tri_fusionDeg2(liste,liste2, milieu);
    tri_fusionDeg2(liste + milieu,liste2 + milieu, taille - milieu);
    fusionDeg2(liste, liste2, taille, milieu);
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
    free(stats_G1);
    free(stats_G2);
    return !(i <= j);
}

bool compareColumn(SommetByDeg *R1, SommetByDeg *R2)
{
    SommetByDeg *p = R1;
    SommetByDeg *q = R2;
    
    if(p==NULL || q==NULL) return false; 
    
    while (p && q && (p->deg==q->deg) )
    {
        p = p->suiv;
        q = q->suiv; 
    }
    return (!p && !q);
     
}

bool searchColumn(SommetByDeg *RepresentG1, SommetByDeg *column, int firstDeg, int nbre_sommets ){

    SommetByDeg *p = RepresentG1;

    while ((p->deg != firstDeg) && (p < RepresentG1 + nbre_sommets))
    {
        p++;
    }
    if(!(p < RepresentG1 + nbre_sommets)) return false;
    
    bool stop = false;

    while (!stop && (p < RepresentG1 + nbre_sommets) && p->deg == firstDeg )
    {
        stop = compareColumn((RepresentG1 + p->indice)->suiv , column->suiv);
        /*
        if(stop){ 
            printf("\n'%d'",p->indice);
            colonnes_marquesRG1[p->indice]=1;}
        */
        p++;
    }
    return stop;
    
}

bool Isomorph(SommetByDeg *RG1, SommetByDeg *RG2 , Stat *stats_G1, Stat *stats_G2, int nbre_sommets_G1, int nbre_sommets_G2,int nbre_arcs1, int nbre_arcs2){
    if (nbre_sommets_G1 != nbre_sommets_G2)
    {
        printf("\n NON ISOMORPHES CAR NBRES SOMMETS DIFFERENTS");
        return false;
    }
    else
    {
        if(nbre_arcs1!=nbre_arcs2){
            printf("\n NON ISOMORPHES CAR NBRES ARCS DIFFERENTS");
            return false;
        } 
        else if (!compareStats(stats_G1, stats_G2, nbre_sommets_G1))
        {
            printf("\n DIFFERENTES STATS");
            return false;
        }
        else{

            bool isIsomorph = true;
            SommetByDeg *p = RG2;

            tri_fusionDeg(RG1, nbre_sommets_G1);
            tri_fusionDeg(RG2, nbre_sommets_G2);
            
            ///tri_fusionDeg2(RG1, RG2, nbre_sommets_G1);
            printf("\n-----------TRI DES SOMMETS---------\n");
            for (int i = 0; i < nbre_sommets_G1; i++)
            {
                printf("(%d,%d) -- (%d,%d)\t",RG1[i].deg, RG1[i].indice, RG2[i].deg, RG2[i].indice);
            }
            
            printf("\n-----------REPRESENATION EN DEGRE GRAPHE 2---------\n");
            for (int i = 0; i < nbre_sommets_G1; i++)
            {
                
                SommetByDeg *q = (RG2 + i);

                //printf("\n%d===>", i+1 );
                printf("\n(%d, %d)===>", q->deg, q->indice);

                q = (RG2 +(RG2 + i)->indice)->suiv;
                //fprintf(F, "%d\t", i + 1);
                while (q)
                {
                    printf("(%d, %d)\t",q->deg, q->indice);
                    q = q->suiv;
                }
                
            }
            
            printf("\n-----------REPRESENATION EN DEGRE G1---------\n");
            for (int i = 0; i < nbre_sommets_G1; i++)
            {
                
                SommetByDeg *q = (RG1 + i);

                //printf("\n%d===>", i+1 );
                printf("\n(%d, %d)===>", q->deg, q->indice);
                
                q = (RG1 +(RG1 + i)->indice)->suiv;
                //fprintf(F, "%d\t", i + 1);
                while (q)
                {
                    printf("(%d, %d)\t",q->deg, q->indice);
                    q = q->suiv;
                }
                
            }
            //int *colonnes_marquesRG1 = (int*)calloc(nbre_sommets_G1, sizeof(int));
            while ((p < RG2 + nbre_sommets_G2) && isIsomorph)
            {
                isIsomorph = searchColumn(RG1, (RG2 + p->indice), p->deg, nbre_sommets_G2);
                p++;
            }
            if (isIsomorph)
            {
                printf("\nRG2 DANS RG1\n");
            }

            //RG1 DANS RG2
            bool isIsomorph2 = true;
            p = RG1;

            while ((p < RG1 + nbre_sommets_G1) && isIsomorph)
            {
                isIsomorph2 = searchColumn(RG2, (RG1 + p->indice), p->deg, nbre_sommets_G1);
                p++;
            }
            if (isIsomorph2)
            {
                printf("\nRG1 DANS RG2\n");
            }
            
            
            free(RG1);
            free(RG2);
            /*
            int i = 0;
            while (i < nbre_sommets_G1 && colonnes_marquesRG1[i])
            {
               i++;
            }
            if (isIsomorph)
            {
                printf("\nRG2 DANS RG1\n");
            }
            */
            
            //return isIsomorph && (i == nbre_sommets_G1);
            return isIsomorph && isIsomorph2;
        }
        return false;
    }

}

