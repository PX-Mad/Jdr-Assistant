#ifndef DES_H
#define DES_H

#include <QtWidgets>
#include <time.h>

// Enumération de la violance des lancé de dès.
// Très Gentil, Gentil, Neutre, Méchant, Très méchant
enum MJ {TG,G,N,M,TM};

// Lanceur de dès
class Des
{
public:
    Des();

    int static lancerDes(QString textSaisie, MJ mj = N);
    QString static lancerDesComplex(QString textSaisie, MJ mj = N);
    int static des(int d, MJ m = N);

    bool static isMeilleur(int debut, int fin, QString textSaisie);
    bool static isDes(int debut, int fin, QString textSaisie);
    bool static isMulti(int debut, int fin, QString textSaisie);
    QString static signePrecedant(int n, QString textSaisie);

};

#endif // DES_H
