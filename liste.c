#include "liste.h"

#include <stdio.h>
#include <stdlib.h>

static int blocs = 0;

static void *suivi_malloc(size_t taille)
{
    void *adresse = malloc(taille);
    if (adresse != NULL) blocs++;
    return adresse;
}

static void suivi_free(void *adresse)
{
    if (adresse != NULL) {
        blocs--;
        free(adresse);
    }
}

int liste_blocs_en_circulation(void)
{
    return blocs;
}

Maillon *liste_inserer(Maillon *tete, int valeur)
{
    Maillon *nouveau = suivi_malloc(sizeof *nouveau);

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

bool liste_maximum(const Maillon *tete, int *resultat)
{
    if (tete == NULL) return false;

    int maximum = tete->valeur;
    const Maillon *courant = tete->suivant;

    while (courant != NULL) {
        if (courant->valeur > maximum) maximum = courant->valeur;
        courant = courant->suivant;
    }

    *resultat = maximum;
    return true;
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
        suivi_free(tete);
        tete = suivant;
    }
}
