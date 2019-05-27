#include "fenetre.h"


// Création de la fenetre et des onglet à affiché en fonction des paramètres sauvegardé
fenetre::fenetre()
{
    srand(time(NULL));

    this->setWindowTitle("JdR Assistant");
    this->resize(850,475);
    this->setMinimumSize(400, 250);

    this->setCentralWidget(&tab_);

    //Géston de la configuration
    CONFIG.load(QCoreApplication::applicationDirPath()+"/res/urg.conf");

    //Traitemetn du urgScripte
    _urgParsor = new urgParsor(this);

    //! ---------------------------------------------------------- ONGLETS
    tab_.addTab(new assistant(), tr("Assistant"));
    tab_.addTab(new mot(), tr("Nom"));
    tab_.addTab(new item(), tr("Objet"));
    tab_.addTab(new divers(), tr("Divers"));
    tab_.addTab(new parametre(), tr("Parametre"));
}
