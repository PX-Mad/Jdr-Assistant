#ifndef PNJ_H
#define PNJ_H

#include "perso.h"

class PNJ : public perso
{
    public:
        PNJ();

        int get_pv() {return pv_;};
        void set_pv(int pv) {pv_ = pv;};
        bool alive() override {return pv_ > 0;};

        //! Serialisation
        QString to_string(QString separator="<.>") {return name_+separator+description_+separator+QString::number(initiative_)+separator+QString::number(pv_);}
        void from_string(QString str, QString separator="<.>") {
            QStringList strl=str.split();
            if(strl.size()!=4)  return 4;
            name_=strl[0];
            description_=strl[1];
            initiative_=strl[2].toInt();
            pv_=strl[3].toInt();
        }

    protected:
        int pv_;
};

#endif // PNJ_H
