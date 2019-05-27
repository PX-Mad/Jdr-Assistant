#ifndef MONSTER_CELL_H
#define MONSTER_CELL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

#include "../../tools/config.h"
#include "../../tools/macro.h"
#include "../../tools/des.h"

class monster_cell_manager;

class monster_cell : public QWidget, public MFocusable
{
    Q_OBJECT

    public:
        monster_cell(bool bConnected=true);

        void reset();
        bool isAlive(){return _iVie>0;}
        void resize(QSize size);

        void resizeEvent(QResizeEvent* event) override;
        void focusInEvent(QFocusEvent *event) override;

        void connect(monster_cell *pMonster);
        void disconnect(monster_cell *pMonster);

        // ACCESSEURS
        // GET
        QString get_nom()               {return _leName.text();}
        int get_init()                  {return _init.value();}
        QString get_free_note()         {return _teFreeNote.toHtml();}
        QString get_vie_moins()         {return _leMoins.text();}
        QString get_vie_plus()          {return _lePlus.text();}
        QString get_dot()               {return _leDot.text();}
        QString get_hot()               {return _leHot.text();}
        int get_vie()                   {return _iVie;}

    public slots:
        void calculerVie();

        // SET
        void set_nom(QString n)         {_leName.setText(n);}
        void set_init(int n)            {_init.setValue(n);}
        void set_free_note(QString n)   {_teFreeNote.setHtml(n);}
        void set_vie_moins(QString n)   {_leMoins.setText(n);}
        void set_vie_plus(QString n)    {_lePlus.setText(n);}
        void set_dot(QString n)         {_leDot.setText(n);}
        void set_hot(QString n)         {_leHot.setText(n);}
        void set_vie(int n){
            _vie.setText(((_iVie=n)< 0)?"Mort ("+QString::number(_iVie)+")":QString::number(_iVie));
        }

    protected slots:
        void focused() override;

    signals:
        void selected(monster_cell *pMonster);
        void nom_change(QString n);
        void init_change(int n);
        void free_note_change(QString n);
        void vie_moins_change(QString n);
        void vie_plus_change(QString n);
        void dot_change(QString n);
        void hot_change(QString n);
        void vie_change();

    protected:

        MLineEdit _leName, _leMoins, _lePlus, _leHot, _leDot;
        MTextEdit _teFreeNote;
        MPushButton _vie;
        MSpinBox _init;

        int _iVie, _nLine;
        bool _bConnected;
};

// Singleton servant à gérer tout les monster_cell instencié
#define MONSTER_MANAGER monster_cell_manager::getInstance()
class monster_cell_manager : public QObject
{
    Q_OBJECT

    public:
        static monster_cell_manager* getInstance()
        {
            static monster_cell_manager _mcm;
            return &_mcm;
        }

        void addItem(monster_cell *item){_items << item;}
        QList<monster_cell*>* getItems(){return &_items;}

    public slots:
        void calculerVie();
        void numeroterPNJ();

    private:
        monster_cell_manager() {}
        QList<monster_cell*> _items;
};
#endif // MONSTER_CELL_H
