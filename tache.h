#ifndef PERTROC_TACHE_H
#define PERTROC_TACHE_H

typedef struct {
    int id;
    char nom[50];
    int duree;
    int dateDebut;
    int dateFin;
    int marge;
} Tache;

Tache creerTache(int id, const char *nom, int duree);

void setDateDebut(Tache* tache, int date);
void setDateFin(Tache* tache, int date);
int getDateDebut(Tache* tache);
int getDateFin(Tache* tache);
int getDuree(Tache* tache);
int getId(Tache* tache);
const char* getNom(Tache* tache);

#endif //PERTROC_TACHE_H
