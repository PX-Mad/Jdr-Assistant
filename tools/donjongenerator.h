#ifndef DONJONGENERATOR_H
#define DONJONGENERATOR_H

#include <QList>

struct salle
{
    unsigned h, l;  // position (hauteur, largeur) de la salle
    salle() {h=0;l=0;}
    salle(unsigned hauteur, unsigned largeur) {h=hauteur;l=largeur;}
};

class DonjonGenerator
{
public:
    DonjonGenerator(unsigned nbSalle, unsigned hauteur=100, unsigned largeur=100);
    ~DonjonGenerator();

private:

    // METHODES
    void ajouterSalle();

    // ATTRIBUTS
    unsigned hauteur, largeur, nbSalle;

    //! Si une case est à true alors il y aura une salle dans ces lieux.
    salle **ppbSalle;
    QList<salle *> salleCree;

};

#endif // DONJONGENERATOR_H
