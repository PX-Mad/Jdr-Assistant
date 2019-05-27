#ifndef PARAMETRE_H
#define PARAMETRE_H

#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../tools/config.h"

class parametre : public QWidget
{
    Q_OBJECT

public:
    parametre();

protected:

    QCheckBox checkParam, paramNum, paramStat;
    QComboBox typeMJ, listStyle;
    QSpinBox desInit;

protected slots:

    void paramSave(int i = 0);
    void paramInit();
    void linkCheckNum(int i = 0);
    void change_style(QString style);
};

#endif // PARAMETRE_H
