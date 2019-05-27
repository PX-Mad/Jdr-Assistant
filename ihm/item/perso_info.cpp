#include "perso_info.h"

perso_info::perso_info()
{
    QVBoxLayout *pMainLayout=new QVBoxLayout();

    //! Gestion des monstres
    _previousMonster=nullptr;
    _monsterCell = new monster_cell(false);
    pMainLayout->addWidget(_monsterCell);

    //! Gestion des joueurs
    _previousPlayer=nullptr;
    _playerCell = new player_cell();
    pMainLayout->addWidget(_playerCell);

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->setVisible(false);
    this->setLayout(pMainLayout);
    ADD_LAYOUT(this->setWidget, pMainLayout);
}

void perso_info::setMonster(monster_cell *pMonster)
{
    if(pMonster==_previousMonster) {return;}

    // On déconnect l'entrée précédante et conencte la nouvelle
    if(_previousMonster!=nullptr) {
        _monsterCell->disconnect(_previousMonster);
    }
    if(_previousPlayer!=nullptr) {
        _playerCell->disconnect(_previousPlayer);
        _previousPlayer=nullptr;
    }
    _previousMonster=pMonster;
    _monsterCell->connect(_previousMonster);

    // On s'occupe de la visibilité des widgets
    _monsterCell->setVisible(true);
    _playerCell->setVisible(false);
    this->setVisible(true);
}

void perso_info::setPlayer(player_cell *pPlayer)
{
    if(pPlayer==_previousPlayer) {return;}

    // On déconnect l'entrée précédante et conencte la nouvelle
    if(_previousMonster!=nullptr) {
        _monsterCell->disconnect(_previousMonster);
        _previousMonster=nullptr;
    }
    if(_previousPlayer!=nullptr) {
        _playerCell->disconnect(_previousPlayer);
    }
    _previousPlayer=pPlayer;
    //_playerCell->connect(_previousPlayer);

    // On s'occupe de la visibilité des widgets
    _playerCell->setVisible(false);
    _monsterCell->setVisible(true);
    this->setVisible(true);
}

void perso_info::hideEvent(QHideEvent *event) {
    Q_UNUSED(event);
    emit isHide();
    DEBUG_MSG("HIDE");
}

void perso_info::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    emit isShow();
    DEBUG_MSG("SHOW");
}
