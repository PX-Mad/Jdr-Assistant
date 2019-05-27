#ifndef DIVERS_H
#define DIVERS_H

#include "tools/config.h"
#include "tools/urgparsor.h"
#include "tools/codexmanager.h"

class divers : public QWidget
{
    Q_OBJECT

    public:
        divers();

    protected:

        QComboBox cbSelectDivers;
        QPushButton bGeneratDivers;
        QTextEdit resultDivers;
        QSpinBox nombreDivers;

        urgParsor _urgParsor;

        void initComboDossier(QComboBox* c, QString path, QString initValue="");

    public slots:

        void genererDivers();
};
#endif // DIVERS_H
