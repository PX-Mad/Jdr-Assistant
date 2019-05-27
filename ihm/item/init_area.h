#ifndef INIT_AREA_H
#define INIT_AREA_H

#include "item_init.h"

typedef struct{
    QString nom;
    int initiative;
}init;

class init_area : public QWidget
{
    public:
        init_area(QWidget *parent);

    public slots:
        void value_change(int value);



};

#endif // INIT_AREA_H
