#include "planarMapGraphIsomorphism.c"


int main(int argc, char const *argv[])
{
    srand(getpid()); //getpid() est une fonction qui envoie l'identifiant du processus lancé (le programme) donc le générateur  va générer à chaque fois des suites de nombres différents.
    //time(NULL)

        //-------------------------------------PREMIER GRAPHE-------------------------------------
    printf("\n----------------------------------------PREMIER GRAPHE-------------------------------\n");    
    int somme_deg_G1;
    int nbre_sommets_G1= 10000;
    Stat *stats_G1 = initStats(nbre_sommets_G1);
    int begin_value_G1, tailleCode1;
    int *ordreDeVisite1 = (int *)malloc(sizeof(int) * nbre_sommets_G1);
    Sommet *G1=generate_Adjacence_Lists(nbre_sommets_G1, stats_G1, ordreDeVisite1, &somme_deg_G1);

    begin_value_G1 = premierSommetNbOccDEGMin(stats_G1, nbre_sommets_G1);
    int * codeArray1= deepFirstSearch(G1, ordreDeVisite1, begin_value_G1, rand()%(G1 + begin_value_G1)->degre ,nbre_sommets_G1,somme_deg_G1/2, &tailleCode1);
    /*
    printf("\n\n-------------------------------------DEUXIÈME GRAPHE------------------------------------\n\n");
    if(isIsomorph(G1, G1, stats_G1, stats_G1, codeArray1, ordreDeVisite1, nbre_sommets_G1, nbre_sommets_G1, tailleCode1, *somme_deg_G1/2, *somme_deg_G1/2)){
        printf("\nISOMORPHES");
    }else printf("\nNON ISOMORPHES");
    */

        //-------------------------------------DEUXIÈME GRAPHE------------------------------------
    printf("\n\n-------------------------------------DEUXIÈME GRAPHE------------------------------------\n\n");
    int somme_deg_G2 ;
    int nbre_sommets_G2= 10000;
    Stat *stats_G2 = initStats(nbre_sommets_G2);
    int *ordreDeVisite2 = (int *)malloc(sizeof(int) * nbre_sommets_G2);
    Sommet *G2=generate_Adjacence_Lists(nbre_sommets_G2, stats_G2, ordreDeVisite2, &somme_deg_G2);
    
    //int begin_value_G2= premierSommetNbOccDEGMin(stats_G2, nbre_sommets_G2);
    //int tailleCode2;
    //int * codeArray2= deepFirstSearch(G2, ordreDeVisite2, begin_value_G2, rand()%(G2 + begin_value_G2)->degre ,nbre_sommets_G2,*somme_deg_G2/2, &tailleCode2);

    if(isIsomorph(G1,G2,stats_G1,stats_G2,codeArray1, ordreDeVisite1 ,nbre_sommets_G1,nbre_sommets_G2,tailleCode1, somme_deg_G1/2, somme_deg_G2/2)){
        printf("\nISOMORPHES");
    }else printf("\nNON ISOMORPHES");
    
    free(G1);
    free(codeArray1);
    free(ordreDeVisite1);
    free(stats_G1); 
    free(G2);
    free(ordreDeVisite2);
    free(stats_G2);

    return 0;
}
