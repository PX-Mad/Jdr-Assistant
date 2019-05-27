#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QMessageBox>
#include <QSplitter>

#include <ctime>

#include "item/init_area.h"
#include "item/perso_info.h"
#include "item/player_cell.h"
#include "item/monster_cell.h"
#include "../tools/config.h"
#include "../tools/urgparsor.h"

class assistant : public QMainWindow
{
    Q_OBJECT

    public:
        assistant();

    protected:

        // Structure tampon pour le tri d'init, TODO: Faire une tri plus propre


        QTextEdit afficheInit;
        QPushButton pBoutonLanceDes;
        QLineEdit SaisieNomP[7], saisieLanceDes, resultDes;
        QComboBox comboSauv, selectEnnemi;
        QSpinBox SaisieInitP[7];

        init sauvInit[19];
        int nbSauvInit;

        void initComboSave();
        void loadPlayers();
        void loadEnnemi();
        void afficherInit();

    public slots:
        void reset();
        void genererInit();
        void resetJoueur();
        bool estVivant(QString nomAChercher);
        void savePage();
        void loadPage();
        void removePage();
        void savePlayers();
        void throwDes();
        void ajouterEnnemi();
};

#endif // ASSISTANT_H
