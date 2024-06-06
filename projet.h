#ifndef PERTROC_PROJET_H
#define PERTROC_PROJET_H

#include <stdio.h>
#include <stdlib.h>
#include "tache.h"

typedef struct {
    int id;
    int predecesseur;
} Relation;

typedef struct {
    Tache taches[100];
    int nbTaches;
    Relation relations[100];
    int nbRelations;
} Projet;

Projet creerProjet();
void ajouterTache(Projet *projet, Tache tache);
void ajouterRelation(Projet *projet, int id, int predecesseur);
void calculerDates(Projet *projet);
int getDureeTotale(Projet *projet);
void getCheminCritique(Projet *projet, Tache *cheminCritique, int *nbCheminCritique);
int detecterCycle(Projet *projet, int id, int *visite, int *pileRec);
int aDesCycles(Projet *projet);
void lireEntrees(Projet *projet, const char *filename);
void ecrireResultats(Projet *projet, const char *filename);

#endif //PERTROC_PROJET_H
