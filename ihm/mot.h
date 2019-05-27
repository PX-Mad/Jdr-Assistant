#ifndef MOT_H
#define MOT_H

#include <ctime>

#include "tools/config.h"
#include "tools/urgparsor.h"
#include "tools/codexmanager.h"

class mot : public QWidget
{
    Q_OBJECT

    public:
        mot();

    protected:
        QComboBox race, longeur;
        QPushButton creer, suppr, import, rename;
        QTextEdit results;
        QSpinBox nombre;

        CodexManager codexManager;

        void initCombo(QComboBox *c, QString path, QString typeFile, QString initValue="");

    public slots:

        void genererMots();
        void genererCodex();
        void supprCodex();
        void importCodex();
        void renameCodex();
};

#endif // MOT_H
