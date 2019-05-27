#ifndef PERSO_INFO_H
#define PERSO_INFO_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>

#include "monster_cell.h"
#include "player_cell.h"
#include "../../tools/macro.h"

// Singleton servant à gérer tout les monster_cell instencié
#define PERSO_INFO perso_info::getInstance()
class perso_info : public QDockWidget
{
    Q_OBJECT

    public:
        static perso_info* getInstance()
        {
            static perso_info _pi;
            return &_pi;
        }

        void setMonster(monster_cell *pMonster);
        void setPlayer(player_cell *pPlayer);

    protected:
        void hideEvent(QHideEvent *event) override;
        void showEvent(QShowEvent *event) override;

    signals:
        void isHide();
        void isShow();

    private:
        perso_info();

        monster_cell *_previousMonster, *_monsterCell;
        player_cell *_previousPlayer, *_playerCell;
};
#endif // PERSO_INFO_H
