#include "tache.h"
#include <string.h>

Tache creerTache(int id, const char* nom, int duree) {
    Tache tache;
    tache.id = id;
    strncpy(tache.nom, nom, 50);
    tache.duree = duree;
    tache.dateDebut = 0;
    tache.dateFin = 0;
    tache.marge = 0;
    return tache;
}

void setDateDebut(Tache* tache, int date) {
    tache->dateDebut = date;
    tache->dateFin = date + tache->duree;
}

void setDateFin(Tache* tache, int date) {
    tache->dateFin = date;
}

int getDateDebut(Tache* tache) {
    return tache->dateDebut;
}

int getDateFin(Tache* tache) {
    return tache->dateFin;
}

int getDuree(Tache* tache) {
    return tache->duree;
}

int getId(Tache* tache) {
    return tache->id;
}

const char* getNom(Tache* tache) {
    return tache->nom;
}
