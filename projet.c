#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "projet.h"

// Fonctions pour lire les entrées depuis un fichier
void lireEntrees(Projet *projet, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", &projet->nbTaches);
    for (int i = 0; i < projet->nbTaches; i++) {
        int id, duree;
        char nom[50];
        fscanf(file, "%d %s %d", &id, nom, &duree);
        Tache tache = creerTache(id, nom, duree);
        ajouterTache(projet, tache);
    }

    fscanf(file, "%d", &projet->nbRelations);
    for (int i = 0; i < projet->nbRelations; i++) {
        int id, predecesseur;
        fscanf(file, "%d %d", &id, &predecesseur);
        ajouterRelation(projet, id, predecesseur);
    }

    fclose(file);
}

// Fonction pour écrire les résultats dans un fichier
void ecrireResultats(Projet *projet, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Durée totale du projet: %d\n", getDureeTotale(projet));

    Tache cheminCritique[100];
    int nbCheminCritique;
    getCheminCritique(projet, cheminCritique, &nbCheminCritique);

    fprintf(file, "Chemin critique:\n");
    for (int i = 0; i < nbCheminCritique; i++) {
        fprintf(file, "Tâche %d: %s\n", cheminCritique[i].id, cheminCritique[i].nom);
    }

    fprintf(file, "Tâches du projet:\n");
    for (int i = 0; i < projet->nbTaches; i++) {
        fprintf(file, "ID: %d, Nom: %s, Durée: %d, Date début: %d, Date fin: %d, Marge: %d\n",
                projet->taches[i].id, projet->taches[i].nom, projet->taches[i].duree,
                projet->taches[i].dateDebut, projet->taches[i].dateFin, projet->taches[i].marge);
    }

    fclose(file);
}

Projet creerProjet() {
    Projet projet;
    projet.nbTaches = 0;
    projet.nbRelations = 0;
    return projet;
}

void ajouterTache(Projet *projet, Tache tache) {
    projet->taches[projet->nbTaches++] = tache;
}

void ajouterRelation(Projet *projet, int id, int predecesseur) {
    Relation relation;
    relation.id = id;
    relation.predecesseur = predecesseur;
    projet->relations[projet->nbRelations++] = relation;
}

void calculerDates(Projet *projet) {
    // Initialisation des dates au plus tôt
    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].dateDebut = 0;
        projet->taches[i].dateFin = projet->taches[i].duree;
    }

    // Calcul des dates au plus tôt (ES - Early Start)
    for (int i = 0; i < projet->nbRelations; i++) {
        int id = projet->relations[i].id;
        int predecesseur = projet->relations[i].predecesseur;

        for (int j = 0; j < projet->nbTaches; j++) {
            if (projet->taches[j].id == predecesseur) {
                for (int k = 0; k < projet->nbTaches; k++) {
                    if (projet->taches[k].id == id) {
                        int es = projet->taches[j].dateFin;
                        if (es > projet->taches[k].dateDebut) {
                            projet->taches[k].dateDebut = es;
                            projet->taches[k].dateFin = es + projet->taches[k].duree;
                        }
                    }
                }
            }
        }
    }

    // Calcul des dates au plus tard (LF - Late Finish)
    int dureeTotale = getDureeTotale(projet);
    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].dateFin = dureeTotale;
    }

    for (int i = projet->nbRelations - 1; i >= 0; i--) {
        int id = projet->relations[i].id;
        int predecesseur = projet->relations[i].predecesseur;

        for (int j = 0; j < projet->nbTaches; j++) {
            if (projet->taches[j].id == id) {
                for (int k = 0; k < projet->nbTaches; k++) {
                    if (projet->taches[k].id == predecesseur) {
                        int lf = projet->taches[j].dateDebut;
                        if (lf < projet->taches[k].dateFin) {
                            projet->taches[k].dateFin = lf;
                        }
                    }
                }
                projet->taches[j].dateFin -= projet->taches[j].duree;
            }
        }
    }

    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].marge = projet->taches[i].dateFin - (projet->taches[i].dateDebut + projet->taches[i].duree);
    }
}

int getDureeTotale(Projet *projet) {
    int dureeTotale = 0;
    for (int i = 0; i < projet->nbTaches; i++) {
        if (projet->taches[i].dateFin > dureeTotale) {
            dureeTotale = projet->taches[i].dateFin;
        }
    }
    return dureeTotale;
}

void getCheminCritique(Projet *projet, Tache *cheminCritique, int *nbCheminCritique) {
    *nbCheminCritique = 0;
    for (int i = 0; i < projet->nbTaches; i++) {
        if (projet->taches[i].marge == 0) {
            cheminCritique[(*nbCheminCritique)++] = projet->taches[i];
        }
    }
}

int detecterCycle(Projet *projet, int id, int *visite, int *pileRec) {
    if (!visite[id]) {
        visite[id] = 1;
        pileRec[id] = 1;

        for (int i = 0; i < projet->nbRelations; i++) {
            if (projet->relations[i].id == id) {
                int successeur = projet->relations[i].predecesseur;
                if (!visite[successeur] && detecterCycle(projet, successeur, visite, pileRec)) {
                    return 1;
                } else if (pileRec[successeur]) {
                    return 1;
                }
            }
        }
    }
    pileRec[id] = 0;
    return 0;
}

int aDesCycles(Projet *projet) {
    int visite[100] = {0};
    int pileRec[100] = {0};
    for (int i = 0; i < projet->nbTaches; i++) {
        if (detecterCycle(projet, projet->taches[i].id, visite, pileRec)) {
            return 1;
        }
    }
    return 0;
}
