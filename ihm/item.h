#ifndef ITEM_H
#define ITEM_H

#include <ctime>

#include "tools/config.h"
#include "tools/urgparsor.h"
#include "tools/codexmanager.h"

class item : public QWidget
{
    Q_OBJECT

    public:
        item();

    protected:

        QComboBox cbSelectItem, cbSet;
        QPushButton bGeneratItem;
        QTextEdit resultItem;
        QSpinBox nombreItem;

        urgParsor _urgParsor;

        void initComboDossier(QComboBox* c, QString path, QString initValue="");

    public slots:

        void genererItem();
        void comboChange(int i);

};
#endif // ITEM_H
