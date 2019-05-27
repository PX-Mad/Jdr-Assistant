#ifndef PLAYER_H
#define PLAYER_H

#include "perso.h"

class player : public perso
{
    public:
        player();

        bool alive() override {return true;};

        //! Serialisation
        QString to_string(QString separator="<.>") {return name_+separator+description_+separator+QString::number(initiative_)+separator;}
        void from_string(QString str, QString separator="<.>") {
            QStringList strl=str.split();
            if(strl.size()!=3)  return;
            name_=strl[0];
            description_=strl[1];
            initiative_=strl[2].toInt();
        }
};

#endif // PLAYER_H
