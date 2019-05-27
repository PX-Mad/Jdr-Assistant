#ifndef DATACODEX_H
#define DATACODEX_H

#include <QVariant>

class DataCodex
{
    public:
        //! Variables à stocker
        int nbValus;
        char *values;
        int ***data;

        //! Méthodes
        static void initCodexSystem ();


    private:
        friend QDataStream & operator << (QDataStream &, const DataCodex &);
        friend QDataStream & operator >> (QDataStream &, DataCodex &);
};



Q_DECLARE_METATYPE(DataCodex);
QDataStream & operator << (QDataStream & out, const DataCodex & Valeur);
QDataStream & operator >> (QDataStream & in, DataCodex & Valeur);

#endif // DATACODEX_H
