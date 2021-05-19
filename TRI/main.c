#include "planarMapGraphIsomorphism.c"


int main(int argc, char const *argv[])
{
    srand(getpid()); //getpid() est une fonction qui envoie l'identifiant du processus lancé (le programme) donc le générateur  va générer à chaque fois des suites de nombres différents.
    //time(NULL)

    printf("\n----------------------------------------PREMIER GRAPHE-------------------------------\n");    
    int somme_deg_G1 ;
    int nbre_sommets_G1 = 5;
    Stat *stats_G1 = initStats(nbre_sommets_G1);
    int begin_value_G1, tailleCode1;
    SommetByDeg *Ensemble_Listes_DAdjacences_DEG1 = (SommetByDeg*)malloc(sizeof(SommetByDeg)*nbre_sommets_G1);
    generate_Adjacence_Lists(nbre_sommets_G1,Ensemble_Listes_DAdjacences_DEG1, stats_G1, &somme_deg_G1);

    printf("\n-----------------------------------------2ème GRAPHE------------------------------------\n");

    int somme_deg_G2;
    int nbre_sommets_G2 = 5;
    Stat *stats_G2 = initStats(nbre_sommets_G2);
    int begin_value_G2, tailleCode2;
    SommetByDeg *Ensemble_Listes_DAdjacences_DEG2 = (SommetByDeg*)malloc(sizeof(SommetByDeg)*nbre_sommets_G2);
    generate_Adjacence_Lists(nbre_sommets_G2,Ensemble_Listes_DAdjacences_DEG2, stats_G2, &somme_deg_G2);

    if(Isomorph(Ensemble_Listes_DAdjacences_DEG1, Ensemble_Listes_DAdjacences_DEG2,stats_G1, stats_G2,nbre_sommets_G1, nbre_sommets_G2,somme_deg_G1, somme_deg_G2)){
        printf("\n--------ISOMORPHES--------\n");
    }else printf("\n--------NON ISOMORPHES--------\n");
    
    
    /*
    tri_fusionDeg(Ensemble_Listes_DAdjacences_DEG1, nbre_sommets_G1);
    tri_fusionDeg(Ensemble_Listes_DAdjacences_DEG2, nbre_sommets_G2);
    if (compareStats(stats_G1,stats_G2,nbre_sommets_G1))
    {
        tri_fusionDeg2(Ensemble_Listes_DAdjacences_DEG2, Ensemble_Listes_DAdjacences_DEG2, nbre_sommets_G2);
        //tri_fusionDeg(Ensemble_Listes_DAdjacences_DEG1, nbre_sommets_G1);

    }
    
    if(Isomorph(Ensemble_Listes_DAdjacences_DEG1, Ensemble_Listes_DAdjacences_DEG2,stats_G1, stats_G2,nbre_sommets_G1, nbre_sommets_G2,somme_deg_G1, somme_deg_G2)){
        printf("\n--------ISOMORPHES--------\n");
    }else printf("\n--------NON ISOMORPHES--------\n");
    
    

    
    printf("\nREPRÉSENTATION DEG\n");
    for (int i = 0; i < nbre_sommets_G1; i++)
    {
        printf("%d\t:",Ensemble_Listes_DAdjacences_DEG1[i].deg);
        SommetByDeg * p = Ensemble_Listes_DAdjacences_DEG1[i].suiv;
        while (p)
        {
            printf("%d\t", p->deg);
            p = p->suiv;
        }
        printf("\n");
        
    }

    printf("\n\nREPRÉSENTATION DEG TRIÉS\n");
    tri_fusionDeg(Ensemble_Listes_DAdjacences_DEG1, nbre_sommets_G1);
    
    for (int i = 0; i < nbre_sommets_G1; i++)
    {
        printf("(%d,%d)\t",Ensemble_Listes_DAdjacences_DEG1[i].deg, Ensemble_Listes_DAdjacences_DEG1[i].indice);
    }

    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(Isomorph(Ensemble_Listes_DAdjacences_DEG1, Ensemble_Listes_DAdjacences_DEG1,stats_G1, stats_G1,nbre_sommets_G1, nbre_sommets_G1,*somme_deg_G1, *somme_deg_G1)){
        printf("\n--------ISOMORPHES--------\n");
    }else printf("\n--------NON ISOMORPHES--------\n");
    */
    
    return 0;
}
