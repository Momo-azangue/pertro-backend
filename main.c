#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projet.h"

/**
void afficherProjet(Projet *projet) {
    printf("Taches du projet:\n");
    for (int i = 0; i < projet->nbTaches; i++) {
        printf("ID: %d, Nom: %s, Duree: %d, Date debut: %d, Date fin: %d, Marge: %d\n",
               projet->taches[i].id, projet->taches[i].nom, projet->taches[i].duree,
               projet->taches[i].dateDebut, projet->taches[i].dateFin, projet->taches[i].marge);
    }
}

int main() {
    Projet projet = creerProjet();
    int nbTaches, nbRelations;

    printf("Entrez le nombre de taches: ");
    scanf("%d", &nbTaches);
    getchar();  // Clear the newline character from the buffer

    for (int i = 0; i < nbTaches; i++) {
        int id, duree;
        char nom[50];

        printf("Entrez l'ID de la tache %d: ", i + 1);
        scanf("%d", &id);
        getchar();  // Clear the newline character from the buffer

        printf("Entrez le nom de la tache %d: ", i + 1);
        fgets(nom, sizeof(nom), stdin);
        // Remove newline character from fgets
        nom[strcspn(nom, "\n")] = 0;

        printf("Entrez la duree de la tache %d: ", i + 1);
        scanf("%d", &duree);
        getchar();  // Clear the newline character from the buffer

        Tache tache = creerTache(id, nom, duree);
        ajouterTache(&projet, tache);
    }

    printf("Entrez le nombre de relations de precedence: ");
    scanf("%d", &nbRelations);
    getchar();  // Clear the newline character from the buffer

    for (int i = 0; i < nbRelations; i++) {
        int id, predecesseur;

        printf("Entrez l'ID de la tache dependante pour la relation %d: ", i + 1);
        scanf("%d", &id);
        getchar();  // Clear the newline character from the buffer

        printf("Entrez l'ID de la tache precedence pour la relation %d: ", i + 1);
        scanf("%d", &predecesseur);
        getchar();  // Clear the newline character from the buffer

        ajouterRelation(&projet, id, predecesseur);
    }

    if (aDesCycles(&projet)) {
        printf("Erreur : Le projet contient des cycles de dependance.\n");
        return 1;
    }

    calculerDates(&projet);

    printf("Duree totale du projet: %d\n", getDureeTotale(&projet));

    Tache cheminCritique[100];
    int nbCheminCritique;
    getCheminCritique(&projet, cheminCritique, &nbCheminCritique);

    printf("Chemin critique:\n");
    for (int i = 0; i < nbCheminCritique; i++) {
        printf("Tache %d: %s\n", cheminCritique[i].id, cheminCritique[i].nom);
    }

    afficherProjet(&projet);

    return 0;
}
**/



int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Projet projet = creerProjet();
    lireEntrees(&projet, argv[1]);

    if (aDesCycles(&projet)) {
        fprintf(stderr, "Erreur : Le projet contient des cycles de dépendance.\n");
        exit(EXIT_FAILURE);
    }

    calculerDates(&projet);
    ecrireResultats(&projet, argv[2]);

    return 0;
}