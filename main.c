#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000

typedef struct {
    int id;
    char nom[50];
    int duree;
    int dateDebut;  // EST (Earliest Start Time)
    int dateFin;    // LFT (Latest Finish Time)
    int marge;
    int critique;   // 1 if the task is critical, otherwise 0
} Tache;

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

Projet creerProjet() {
    Projet projet;
    projet.nbTaches = 0;
    projet.nbRelations = 0;
    return projet;
}

void ajouterTache(Projet *projet, Tache tache) {
    projet->taches[projet->nbTaches++] = tache;
}

void ajouterRelation(Projet *projet, Relation relation) {
    projet->relations[projet->nbRelations++] = relation;
}

void lireInput(Projet *projet, FILE *input) {
    int nbTaches, nbRelations;
    fscanf(input, "%d", &nbTaches);
    for (int i = 0; i < nbTaches; i++) {
        Tache tache;
        fscanf(input, "%d %s %d", &tache.id, tache.nom, &tache.duree);
        ajouterTache(projet, tache);
    }
    fscanf(input, "%d", &nbRelations);
    for (int i = 0; i < nbRelations; i++) {
        Relation relation;
        fscanf(input, "%d %d", &relation.id, &relation.predecesseur);
        ajouterRelation(projet, relation);
    }
}

void calculerDates(Projet *projet) {
    // Initialisation des dates au plus tôt (EST) et au plus tard (LFT)
    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].dateDebut = 0;
        projet->taches[i].dateFin = INT_MAX;  // Initialiser avec un grand nombre
    }

    // Calcul des dates au plus tôt (EST)
    for (int i = 0; i < projet->nbRelations; i++) {
        int id = projet->relations[i].id;
        int predecesseur = projet->relations[i].predecesseur;
        for (int j = 0; j < projet->nbTaches; j++) {
            if (projet->taches[j].id == id) {
                for (int k = 0; k < projet->nbTaches; k++) {
                    if (projet->taches[k].id == predecesseur) {
                        int est = projet->taches[k].dateDebut + projet->taches[k].duree;
                        if (est > projet->taches[j].dateDebut) {
                            projet->taches[j].dateDebut = est;
                        }
                    }
                }
            }
        }
    }

    // Calcul des dates au plus tard (LFT)
    int dureeTotale = 0;
    for (int i = 0; i < projet->nbTaches; i++) {
        int fin = projet->taches[i].dateDebut + projet->taches[i].duree;
        if (fin > dureeTotale) {
            dureeTotale = fin;
        }
    }
    for (int i = 0; i < projet->nbTaches; i++) {
        if (projet->taches[i].dateFin == INT_MAX) {
            projet->taches[i].dateFin = dureeTotale;
        }
    }

    for (int i = projet->nbRelations - 1; i >= 0; i--) {
        int id = projet->relations[i].id;
        int successeur = projet->relations[i].predecesseur;
        for (int j = 0; j < projet->nbTaches; j++) {
            if (projet->taches[j].id == id) {
                for (int k = 0; k < projet->nbTaches; k++) {
                    if (projet->taches[k].id == successeur) {
                        int lft = projet->taches[j].dateFin - projet->taches[j].duree;
                        if (lft < projet->taches[k].dateFin) {
                            projet->taches[k].dateFin = lft;
                        }
                    }
                }
            }
        }
    }

    // Calcul des marges et identification des tâches critiques
    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].marge = (projet->taches[i].dateFin - (projet->taches[i].dateDebut + projet->taches[i].duree)) ;
        projet->taches[i].critique = (projet->taches[i].marge == 0) ? 1 : 0;
    }

    printf("Durée totale du projet: %d\n", dureeTotale);
}

void afficherResultat(Projet *projet) {
    printf("Chemin critique:\n");
    for (int i = 0; i < projet->nbTaches; i++) {
        if (projet->taches[i].critique) {
            printf("Tâche %d: %s\n", projet->taches[i].id, projet->taches[i].nom);
        }
    }
    printf("Tâches du projet:\n");
    for (int i = 0; i < projet->nbTaches; i++) {
        printf("ID: %d, Nom: %s, Durée: %d, Date début au plus tôt: %d, Date fin au plus tard: %d, Marge: %d, Critique: %s\n",
               projet->taches[i].id, projet->taches[i].nom, projet->taches[i].duree,
               projet->taches[i].dateDebut, projet->taches[i].dateFin,
               projet->taches[i].marge, projet->taches[i].critique ? "Oui" : "Non");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }

    Projet projet = creerProjet();
    lireInput(&projet, input);
    fclose(input);

    calculerDates(&projet);

    FILE *output = fopen(argv[2], "w");
    if (output == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return 1;
    }
    for (int i = 0; i < projet.nbTaches; i++) {
        fprintf(output, "%d %s %d %d %d %d %d %d\n",
                projet.taches[i].id, projet.taches[i].nom, projet.taches[i].duree,
                projet.taches[i].dateDebut, projet.taches[i].dateFin - projet.taches[i].duree,
                projet.taches[i].dateDebut + projet.taches[i].duree, projet.taches[i].dateFin,
                projet.taches[i].marge);
    }
    fclose(output);
    return 0;
}
