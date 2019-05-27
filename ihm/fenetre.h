#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>

#include "assistant.h"
#include "parametre.h"
#include "mot.h"
#include "item.h"
#include "divers.h"

class fenetre : public QMainWindow
{
    Q_OBJECT

    public:
        fenetre();

    protected:

        //Traitement du urgScript
        urgParsor *_urgParsor;

        QTabWidget tab_;
};

#endif // FENETRE_H
