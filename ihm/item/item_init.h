#ifndef ITEM_INIT_H
#define ITEM_INIT_H

#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>

class item_init : public QWidget
{
    public:
        item_init(QWidget *parent, QString nom, int valeur);
        item_init(QString nom, int valeur);

    public slots:
        void set_init(int value);
        int get_init();

    signals:
        void value_change(int value);

    protected:
        void init_graphic(QString nom="", int valeur=0);

        QLabel *m_nom;
        QSpinBox *m_valeur;

};

#endif // ITEM_INIT_H
