#include "player_cell.h"
#include "perso_info.h"

#define FIXED_HEIGHT    55

// Construit l'onglet de génération "Non-trié"
player_cell::player_cell()
{
    QVBoxLayout *layoutV;
    QRegExpValidator *validatorDi = new QRegExpValidator(QRegExp(VALID_TEXT),nullptr);

    _leName.setPlaceholderText("Joueur");
    _leName.setToolTip(TT_PLAYER_NAME);
    _leName.setValidator(validatorDi);
    _init.setRange(-999, 999);
    _init.setToolTip(TT_PLAYER_INIT);

    layoutV = new QVBoxLayout;
    layoutV->setSpacing(3);
    layoutV->addWidget(&_leName);
    layoutV->addWidget(&_init);

    player_cell_manager *pcm=PLAYER_MANAGER;
    pcm->addItem(this);

    //! SLOTS
    QObject::connect(&_leName, SIGNAL(editingFinished()), pcm, SLOT(savePlayers()));
    QObject::connect(&_init, SIGNAL(editingFinished()), pcm, SLOT(savePlayers()));

    this->setLayout(layoutV);
    this->setFixedHeight(FIXED_HEIGHT);
}

void player_cell::reset()
{
    _leName.clear();
    _init.setValue(0);
    _qsFreeNote.clear();
}

void player_cell::focusInEvent(QFocusEvent *event) {
    Q_UNUSED(event);
    focused();
}

void player_cell::focused() {
    PERSO_INFO->setPlayer(this);
}

//! MANAGER

// Sauvegarde les modification apporté au PJ dans le fichier "res/assistant/players.txt"
void player_cell_manager::savePlayers()
{
    QString r = "";
    foreach(player_cell* player, _items)
        r+= player->get_nom()+"\n"+QString::number(player->get_init())+"\n";
    std::ofstream flux("res/assistant/players.txt");
    if(!flux.is_open())return;
    flux << r.toStdString()+"\n";
}

// Charge les PJ à partir de "res/assistant/players.txt"
void player_cell_manager::loadPlayers()
{
    std::ifstream flux("res/assistant/players.txt");
    if(!flux.is_open())return;
    std::string line;
    for(int i=0;getline(flux,line)&&i<14;i++)
    {
        if(i%2==0)  _items[i/2]->set_nom(QString::fromStdString(line));
        else        _items[i/2]->set_init(QString::fromStdString(line).toInt());
    }
}
