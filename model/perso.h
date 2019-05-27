#ifndef PERSO_H
#define PERSO_H

#include <QVariant>

class perso
{
    public:

        //! To sort on std containers
        bool operator <(perso& p){ return initiative_ < p.get_initiative();}
        bool operator >(perso& p){ return initiative_ > p.get_initiative();}

        //! Accesseurs
        QString get_name() {return name_;}
        void set_name(QString name) {name_ = name;}
        int get_initiative() {return initiative_;}
        void set_initiative(int initiative) {initiative_ = initiative;}
        QString get_desc() {return description_;}
        void set_desc(QString desc) {description_ = desc;}

        //! Serialisation
        virtual QString to_string(QString separator) =0;
        virtual void from_string(QString str) =0;

        virtual bool alive();

    protected:
        QString name_;
        QString description_;
        int initiative_;
};

#endif // PERSO_H
