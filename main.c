#include "liste.h"

#include <stdio.h>

int main(void)
{
    Maillon *liste = NULL;

    for (int i = 1; i <= 5; i++) {
        liste = liste_inserer(liste, i * 10);
    }

    printf("blocs apres creation : %d\n", liste_blocs_en_circulation());
    printf("liste       : ");
    liste_afficher(liste);
    printf("longueur    : %d\n", liste_longueur(liste));
    printf("contient 30 : %s\n", liste_contient(liste, 30) ? "oui" : "non");

    int maximum = 0;
    if (liste_maximum(liste, &maximum)) {
        printf("maximum     : %d\n", maximum);
    }

    int resultat_vide = 12345;
    if (!liste_maximum(NULL, &resultat_vide)) {
        printf("liste vide  : aucun maximum (resultat=%d)\n", resultat_vide);
    }

    liste_liberer(liste);
    puts("liberee");
    printf("blocs apres liberation : %d\n", liste_blocs_en_circulation());
    return 0;
}
