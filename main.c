#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char nom[50];
    int duree;
    int dateDebut;
    int dateFin;
    int marge;
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
    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].dateDebut = 0;
        projet->taches[i].dateFin = projet->taches[i].duree;
    }
    for (int i = 0; i < projet->nbRelations; i++) {
        int id = projet->relations[i].id;
        int predecesseur = projet->relations[i].predecesseur;
        for (int j = 0; j < projet->nbTaches; j++) {
            if (projet->taches[j].id == id) {
                for (int k = 0; k < projet->nbTaches; k++) {
                    if (projet->taches[k].id == predecesseur) {
                        if (projet->taches[k].dateFin > projet->taches[j].dateDebut) {
                            projet->taches[j].dateDebut = projet->taches[k].dateFin;
                            projet->taches[j].dateFin = projet->taches[j].dateDebut + projet->taches[j].duree;
                        }
                    }
                }
            }
        }
    }
    int dureeTotale = 0;
    for (int i = 0; i < projet->nbTaches; i++) {
        if (projet->taches[i].dateFin > dureeTotale) {
            dureeTotale = projet->taches[i].dateFin;
        }
    }
    for (int i = 0; i < projet->nbTaches; i++) {
        projet->taches[i].marge = dureeTotale - projet->taches[i].dateFin;
    }
    printf("Durée totale du projet: %d\n", dureeTotale);
}

void afficherResultat(Projet *projet) {
    printf("Chemin critique:\n");
    for (int i = 0; i < projet->nbTaches; i++) {
        if (projet->taches[i].marge == 0) {
            printf("Tâche %d: %s\n", projet->taches[i].id, projet->taches[i].nom);
        }
    }
    printf("Tâches du projet:\n");
    for (int i = 0; i < projet->nbTaches; i++) {
        printf("ID: %d, Nom: %s, Durée: %d, Date début: %d, Date fin: %d, Marge: %d\n",
               projet->taches[i].id, projet->taches[i].nom, projet->taches[i].duree,
               projet->taches[i].dateDebut, projet->taches[i].dateFin, projet->taches[i].marge);
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
        fprintf(output, "%d %s %d %d %d %d %d\n",
                projet.taches[i].id, projet.taches[i].nom, projet.taches[i].duree,
                projet.taches[i].dateDebut, projet.taches[i].dateFin, projet.taches[i].marge,
                projet.taches[i].marge == 0 ? 1 : 0);
    }
    fclose(output);
    return 0;
}
