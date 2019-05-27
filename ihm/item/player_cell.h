#ifndef PLAYER_CELL_H
#define PLAYER_CELL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

#include <iostream>
#include <fstream>

#include "../../tools/config.h"
#include "../../tools/macro.h"
#include "../../tools/des.h"

class player_cell_manager;

class player_cell : public QWidget, public MFocusable
{
    Q_OBJECT

    public:
        player_cell();

        void calculerVie();
        void reset();

        void focusInEvent(QFocusEvent *event) override;

        // ACCESSEURS
        // GET
        QString get_nom()               {return _leName.text();}
        int get_init()                  {return _init.value();}
        QString get_free_note()         {return _qsFreeNote;}

    public slots:
        // SET
        void set_nom(QString n)         {_leName.setText(n);}
        void set_init(int n)            {_init.setValue(n);}
        void set_free_note(QString n)   {_qsFreeNote=n;}

    protected slots:
        void focused() override;

    protected:

        MLineEdit _leName;
        MSpinBox _init;

        QString _qsFreeNote;
};

// Singleton servant à gérer tout les monster_cell instencié
#define PLAYER_MANAGER player_cell_manager::getInstance()
class player_cell_manager : public QObject
{
    Q_OBJECT

    public:
        static player_cell_manager* getInstance()
        {
            static player_cell_manager _mcm;
            return &_mcm;
        }

        void addItem(player_cell *item){_items << item;}
        QList<player_cell*>* getItems(){return &_items;}

    public slots:
        void loadPlayers();
        void savePlayers();

    private:
        player_cell_manager() {}
        QList<player_cell*> _items;
};

#endif // PLAYER_CELL_H
