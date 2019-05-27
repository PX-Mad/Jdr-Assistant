#include "datacodex.h"

DataCodex::DataCodex()
{

}


QDataStream & operator << (QDataStream & out, const DataCodex & Valeur)
{
    out << Valeur.nbValus
        << Valeur.values
        << Valeur.data;

    return out;
}
QDataStream & operator >> (QDataStream & in, DataCodex & Valeur)
{
    in >> Valeur.data;
    in >> Valeur.values;
    in >> Valeur.data;

    return in;
}

void Contact::initContactSystem ()
{
    qRegisterMetaTypeStreamOperators<DataCodex>("DataCodex");
    qMetaTypeId<DataCodex>();
}
