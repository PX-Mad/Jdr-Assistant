#include "assistant.h"

// TODO: Numérotation que chaque éléments des PNJ
// TODO: Ajouter le Logo de URG dans les paramètres
// TODO: Bouton Entrée sur chaque élement URG

// Construit l'onglet du gestionnaire de partie
assistant::assistant()
{
    Q_INIT_RESOURCE(res);

    QHBoxLayout *layoutPersonnage, *layoutSauv, *layoutLanceDes, *layoutEnnemi;
    QVBoxLayout *layoutPrincipal, *layoutAfficheInit, *layoutBas;
    QGridLayout *layoutGrid, *layoutGrid2;
    QSplitter *separator = new QSplitter(Qt::Horizontal);

    QRegExpValidator *validatorSe = new QRegExpValidator(QRegExp(VALID_DICE),nullptr);

    layoutPrincipal = new QVBoxLayout;
    layoutPersonnage = new QHBoxLayout;
    layoutBas = new QVBoxLayout;
    layoutLanceDes = new QHBoxLayout;
    layoutAfficheInit = new QVBoxLayout;
    layoutGrid = new QGridLayout;
    layoutGrid2 = new QGridLayout;
    layoutSauv = new QHBoxLayout;
    layoutEnnemi = new QHBoxLayout;

    layoutGrid->setSpacing(0);

    layoutPrincipal->addLayout(layoutPersonnage);
    H_SEPARATOR(layoutBas, Qt::AlignTop);
    layoutBas->addLayout(layoutGrid);
    H_SEPARATOR(layoutBas, Qt::AlignBottom);
    layoutBas->addLayout(layoutGrid2, Qt::AlignBottom);
    ADD_LAYOUT(separator->addWidget, layoutAfficheInit)
    ADD_LAYOUT(separator->addWidget, layoutBas)
    layoutPrincipal->addWidget(separator);

    V_SEPARATOR(layoutGrid2, 0, 1, 2, 1);
    V_SEPARATOR(layoutGrid2, 0, 3, 2, 1);
    layoutGrid2->addLayout(layoutSauv, 1, 4);   // Colonne gestion page

    QPushButton *pBoutonAddEnnemi= new QPushButton(QIcon("./res/img/add.png"), "", this);
    pBoutonAddEnnemi->setToolTip(TT_ADD_ENNEMI);

    QPushButton *pBoutonReset = new QPushButton(QIcon("./res/img/pnj.png"), "", this);
    pBoutonReset->setToolTip(TT_RESET_PNJ);
    QPushButton *pBoutonResetJoueur = new QPushButton(QIcon("./res/img/pj.png"), "", this);
    pBoutonResetJoueur->setToolTip(TT_RESET_PLAYERS);

    QPushButton *pBoutonInit = new QPushButton("Lancer tour", this);
    pBoutonInit->setToolTip(TT_ROLL_INIT);
    QPushButton *pBoutonSauv = new QPushButton(QIcon("./res/img/save.png"), "", this);
    pBoutonSauv->setToolTip(TT_SAVE_PNJ);
    QPushButton *pBoutonSupp = new QPushButton(QIcon("./res/img/trash.png"), "", this);
    pBoutonSupp->setToolTip(TT_DELETE_PNJ);
    QPushButton *pBoutonImport = new QPushButton(QIcon("./res/img/load.png"), "", this);
    pBoutonImport->setToolTip(TT_LOAD_PNJ);

    afficheInit.setReadOnly(true);
    afficheInit.setMinimumWidth(150);

    this->initComboSave();

    layoutAfficheInit->addWidget(&afficheInit);
    layoutAfficheInit->addWidget(pBoutonInit);

    // Layout reset
    layoutGrid2->addWidget(pBoutonReset, 0, 2);
    layoutGrid2->addWidget(pBoutonResetJoueur, 1, 2);
    layoutEnnemi->addWidget(&selectEnnemi, 5);
    layoutEnnemi->addWidget(pBoutonAddEnnemi, 1);
    layoutSauv->addWidget(pBoutonImport);
    layoutSauv->addWidget(pBoutonSauv);
    layoutSauv->addWidget(pBoutonSupp);
    layoutGrid2->addLayout(layoutEnnemi, 0, 0);
    layoutGrid2->addWidget(&comboSauv, 0, 4);

    //---------------------------------------------------Création dynamique de la saisie d'inititative et de vie

    layoutPersonnage->setSpacing(0);
    for(int i=0;i<=6;i++)
    {
        layoutPersonnage->addWidget(new player_cell());
    }
    PLAYER_MANAGER->loadPlayers();

    for(int i=0;i<12;i++)
    {
        layoutGrid->addWidget(new monster_cell(), i/3, i%3);
        layoutGrid->setRowStretch(i/3, 1);
        layoutGrid->setColumnStretch(i%3, 1);
    }

    //---------------------------------------------------Lanceur de dès

    saisieLanceDes.setPlaceholderText("Saisie Dès");
    saisieLanceDes.setValidator(validatorSe);
    saisieLanceDes.setToolTip(TT_DES_SAISIE);
    resultDes.setPlaceholderText("Résultat");
    resultDes.setReadOnly(true);
    resultDes.setToolTip(TT_DES_LANCER);
    pBoutonLanceDes.setIcon(QIcon("./res/img/dice.png"));
    resultDes.setToolTip(TT_DES_RESULT);


    layoutGrid2->addLayout(layoutLanceDes, 1, 0);
    layoutLanceDes->addWidget(&saisieLanceDes, 5);
    layoutLanceDes->addWidget(&pBoutonLanceDes, 1);
    layoutGrid2->addWidget(&resultDes, 2, 0, 1, 5);

    // Stretch de la partie du bas
    layoutGrid2->setColumnStretch(0, 5);
    layoutGrid2->setColumnStretch(2, 1);
    layoutGrid2->setColumnStretch(4, 5);

    //---------------------------------------------------Panneau latéral
    addDockWidget(Qt::RightDockWidgetArea, PERSO_INFO);

    //---------------------------------------------------Connection des slots

    QObject::connect(pBoutonReset, SIGNAL(clicked()), this, SLOT(reset()));
    QObject::connect(pBoutonResetJoueur, SIGNAL(clicked()), this, SLOT(resetJoueur()));
    QObject::connect(pBoutonAddEnnemi, SIGNAL(clicked()), this, SLOT(ajouterEnnemi()));
    QObject::connect(pBoutonInit, SIGNAL(clicked()), this, SLOT(genererInit()));
    QObject::connect(pBoutonSauv, SIGNAL(clicked()), this, SLOT(savePage()));
    QObject::connect(pBoutonImport, SIGNAL(clicked()), this, SLOT(loadPage()));
    QObject::connect(pBoutonSupp, SIGNAL(clicked()), this, SLOT(removePage()));
    QObject::connect(&pBoutonLanceDes, SIGNAL(clicked()), this, SLOT(throwDes()));
    QObject::connect(&saisieLanceDes, SIGNAL(returnPressed()), this, SLOT(throwDes()));

    loadEnnemi();
    nbSauvInit=0;

    ADD_LAYOUT(this->setCentralWidget, layoutPrincipal)

}

//Charge un des PNJ de la liste d'monstres stocké dans "res/assistant/data.txt" (nom;init;DV)
void assistant::ajouterEnnemi()
{
    QString e = selectEnnemi.currentText(), v, i;
    QList<monster_cell*> *monsters=MONSTER_MANAGER->getItems();
    QFile f("res/assistant/data.txt");
    if(!f.open(QFile::ReadOnly | QIODevice::Text))   return;
    QStringList sl;
    for(QString s;!f.atEnd();)
    {
        s = f.readLine();
        sl = s.split(";");
        if(sl[0]==e)
        {
            e=sl[0];
            v=sl[1];
            i=sl[2];
            break;
        }
    }

    foreach(monster_cell* monster, (*monsters))
        if(monster->get_nom().isEmpty())
        {
            monster->set_nom(e);
            monster->set_vie_plus(v);
            monster->set_init(i.toInt());
            break;
        }

    MONSTER_MANAGER->numeroterPNJ();
}

// Initialise la combobox de selection de PNJ
void assistant::loadEnnemi()
{
    QFile f("res/assistant/data.txt");
    if(!f.open(QFile::ReadOnly | QIODevice::Text))   return;
    QStringList sl;
    for(QString s;!f.atEnd();)
    {
        s = f.readLine();
        sl = s.split(";");
        selectEnnemi.addItem(sl[0]);
    }
}

// Supprime toute la page de PNJ
void assistant::reset()
{
    QList<monster_cell*> *monsters=MONSTER_MANAGER->getItems();
    foreach(monster_cell* monster, (*monsters))
        monster->reset();
}

// Lance un tour d'initiative
void assistant::genererInit()
{
    init initiative[19], temp;
    QList<monster_cell*> *monsters=MONSTER_MANAGER->getItems();
    int i=0, j=1, k, sortie=0, desInit=CONFIG.getParam("desInit","20").toInt();

    for(k=0;k<7;k++)
        if(SaisieNomP[k].text()!=nullptr)
        {
            initiative[i].initiative = SaisieInitP[k].value()+(rand()%desInit)+1;
            initiative[i].nom = SaisieNomP[k].text();
            i++;
        }

    foreach(monster_cell* monster, (*monsters))
        if(monster->get_nom().isEmpty() && monster->isAlive())
        {
            initiative[i].initiative = monster->get_init()+(rand()%desInit)+1;
            initiative[i].nom = monster->get_nom();
            i++;
        }
    nbSauvInit=i;
    // Tri du tableau... TODO: Changer ce tri à bulle par un truc plus propre
    if(i>1)
        while(sortie==0)
        {
            if(j>=i) sortie=1;
            else if(initiative[j].initiative>initiative[j-1].initiative)
            {
                temp.initiative = initiative[j].initiative;
                initiative[j].initiative = initiative[j-1].initiative;
                initiative[j-1].initiative = temp.initiative;
                temp.nom = initiative[j].nom;
                initiative[j].nom = initiative[j-1].nom;
                initiative[j-1].nom = temp.nom;
                j=1;
            }
            else j++;
        }

    for(int l=0;l<i;l++)
    {
        sauvInit[l].initiative=initiative[l].initiative;
        sauvInit[l].nom=initiative[l].nom;
    }

    // Affichage du tableau
    afficherInit();
}

//Supprime tout les joueurs en mémoire après une demande de validation de l'action
void assistant::resetJoueur()
{
    if(QMessageBox::question(this, "Supprimer ?", "Supprimer tout les joueurs?", QMessageBox::Yes | QMessageBox::No)==QMessageBox::No)
        return;
    for(int i=0;i<7;i++)
    {
        SaisieNomP[i].setText("");
        SaisieInitP[i].setValue(0);
    }
    std::ofstream flux("res/assistant/players.txt");
    if(!flux.is_open())return;
    flux << "\n0\n\n0\n\n0\n\n0\n\n0\n\n0\n\n0\n\n";
}

// Affiche le tableau trié en supprimant les PNJ morts
void assistant::afficherInit()
{
    QString texte="";
    for(int j=0;j<nbSauvInit;j++)
        if(estVivant(sauvInit[j].nom))
            texte+=sauvInit[j].nom+": "+QString::number(sauvInit[j].initiative)+"\n";
    afficheInit.setText(texte);
}

//Test si un PNJ est vivant
bool assistant::estVivant(QString nomAChercher)
{
    QList<monster_cell*> *monsters=MONSTER_MANAGER->getItems();
    if(nomAChercher.isEmpty())  return false;
    for(int i=0;i<7;i++)
        if(SaisieNomP[i].text()!=nullptr && SaisieNomP[i].text()==nomAChercher)
            return true;
    foreach(monster_cell* monster, (*monsters))
        if(monster->get_nom()==nomAChercher && monster->isAlive())
            return true;
    return false;
}

//Slot: Sauvegarde une page en csv, une ligne = un page, "res/assistant/pages.txt"
void assistant::savePage()
{
    bool ok;
    // Récupération du nom de sauvegarde
    QString r = QInputDialog::getText(this, "Nom de la sauvegarde", "Quel nom donner à votre sauvegarde?", QLineEdit::Normal, QString(), &ok);
    if(!ok || r.isEmpty()) {return;}
    QList<monster_cell*> *monsters=MONSTER_MANAGER->getItems();
    // Serialization de la page
    foreach(monster_cell* monster, (*monsters)) {
        monster->calculerVie();
        r+= "\t!\t"+monster->get_nom()+"\t?\t"+QString::number(monster->get_init())+"\t?\t"+QString(monster->get_vie());
    }
    // Sauvegarder
    std::ofstream flux("res/assistant/pages.txt", std::ios::app);
    if (!flux.is_open()){QMessageBox::information(this, "Erreur", "Impossible d'accéder au fichier de sauvegarde.");return;}
    flux << r.toStdString()+"\n";
    // Clear le terrain
    foreach(monster_cell* monster, (*monsters))
        monster->reset();
    flux.close();
    initComboSave();
}

//Slot: Charger une page (ligne du csv: "res/assistant/pages.txt")
void assistant::loadPage()
{
    QFile f("res/assistant/pages.txt");
    if(!f.open(QFile::ReadOnly | QIODevice::Text)){QMessageBox::information(this, "Erreur", "Impossible d'accéder au fichier de sauvegarde.");return;}
    QList<monster_cell*> *monsters=MONSTER_MANAGER->getItems();
    QString p = comboSauv.currentText(),l;
    QStringList sl;
    int i;
    while(!f.atEnd())
    {
        i=0;
        l = f.readLine();
        sl = l.split("\t!\t");
        if(sl.at(0)==p)
            foreach(monster_cell* monster, (*monsters))
            {
                QString s = sl.at(i+1);
                s = s.replace("\t","\t \t");
                QStringList l2=s.split(QString("\t?\t"));
                monster->set_nom(l2.at(0));
                monster->set_init(l2.at(1).toInt());
                monster->set_vie(l2.at(2).toInt());
                i++;
            }
    }
}

//Slot: Supprimer une entré du csv "res/assistant/pages.txt"
void assistant::removePage()
{
    std::string line,r="";
    std::ifstream f("res/assistant/pages.txt");
    if(!f.is_open()){QMessageBox::information(this, "Erreur", "Impossible d'accéder au fichier de sauvegarde.");return;}
    for(int i=0, p=comboSauv.currentIndex();getline(f,line);i++)   if(i!=p)r+=line+"\n";
    f.close();
    std::ofstream f2("res/assistant/pages.txt");
    if(!f2.is_open()){QMessageBox::information(this, "Erreur", "Impossible d'accéder au fichier de sauvegarde.");return;}
    f2 << r;
    f2.close();
    initComboSave();
}

// Sauvegarde les modification apporté au PJ dans le fichier "res/assistant/players.txt"
void assistant::savePlayers()
{
    QString r = "";
    for(int i=0;i<7;i++)
        r+= SaisieNomP[i].text()+"\n"+SaisieInitP[i].text()+"\n";
    std::ofstream flux("res/assistant/players.txt");
    if(!flux.is_open())return;
    flux << r.toStdString()+"\n";
}

// Charge les PJ à partir de "res/assistant/players.txt"
void assistant::loadPlayers()
{
    std::ifstream flux("res/assistant/players.txt");
    if(!flux.is_open())return;
    std::string line;
    for(int i=0;getline(flux,line)&&i<14;i++)
    {
        if(i%2==0)  SaisieNomP[i/2].setText(QString::fromStdString(line));
        else        SaisieInitP[i/2].setValue(QString::fromStdString(line).toInt());
    }
}

// Charge la liste des pages de PNJ en dans la combobox
void assistant::initComboSave()
{
    comboSauv.clear();
    QFile f("res/assistant/pages.txt");
    if(!f.open(QFile::ReadOnly | QIODevice::Text))   return;
    QString s;
    while(!f.atEnd())
    {
        s = f.readLine();
        s = s.split("\t!\t").at(0);
        if(s!="")
            comboSauv.addItem(s.trimmed());
    }
}

// Lance udes dès en passant par le parser
void assistant::throwDes()
{
    MJ mj[5]={MJ::TG,MJ::G,MJ::N,MJ::M,MJ::TM};
    resultDes.setText(Des::lancerDesComplex(saisieLanceDes.text(),mj[CONFIG.getParam("typeMJ").toInt()]));
}
