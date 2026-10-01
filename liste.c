#include "liste.h"

#include <stdio.h>
#include <stdlib.h>

Maillon *liste_inserer(Maillon *tete, int valeur)
{
    Maillon *nouveau = malloc(sizeof *nouveau);

    if (nouveau == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    nouveau->valeur = valeur;
    nouveau->suivant = tete;
    return nouveau;
}

int liste_longueur(const Maillon *tete)
{
    int longueur = 0;

    while (tete != NULL) {
        longueur++;
        tete = tete->suivant;
    }

    return longueur;
}

bool liste_contient(const Maillon *tete, int valeur)
{
    while (tete != NULL) {
        if (tete->valeur == valeur) return true;
        tete = tete->suivant;
    }

    return false;
}

void liste_afficher(const Maillon *tete)
{
    while (tete != NULL) {
        printf("%d -> ", tete->valeur);
        tete = tete->suivant;
    }
    puts("NULL");
}

void liste_liberer(Maillon *tete)
{
    while (tete != NULL) {
        Maillon *suivant = tete->suivant;
        free(tete);
        tete = suivant;
    }
}
