#include "des.h"

//TODO: A REFONDRE DE ZERO, mes yeux brulent !

Des::Des(){}

//Lance les dès et retourne le résultat
int Des::lancerDes(QString textSaisie, MJ mj)
{
    QString tmp, signe, sp="+";
    int resultDes=0, j=0, l, tmpT, nbDes, nbFaceDes, tmpDes, longueurChaine=textSaisie.length(), tmpMoins, meilleur=0, multi;
    bool premierSigne=true;

    for(int i=0;i<=longueurChaine;i++)
    {
        tmp="";
        if(textSaisie[i]=='+' || textSaisie[i]=='-' || i==longueurChaine)
        {
            meilleur=0;
            multi=0;
            if(isMulti(j, i, textSaisie))                //Si il faut multiplier
            {
                for(tmp="";textSaisie[j]!='*' && textSaisie[j]!='x';j++)
                    tmp+=textSaisie[j];
                if(tmp=="")  tmp='0';
                multi=tmp.toInt();
            }
            if(meilleur!=0)    j++;
            tmp=textSaisie[j];
            if(j!=0)    j++;
            if(isMeilleur(j, i, textSaisie))        //Si il y a X meilleurs
            {
                for(tmp="";textSaisie[j]!='m' && textSaisie[j]!='M';j++)
                    tmp+=textSaisie[j];
                if(tmp=="")  tmp='0';
                meilleur=tmp.toInt();
            }
            if(meilleur!=0)    j++;
            signe=signePrecedant(i, textSaisie);
            if(!isDes(j, i, textSaisie))            //Si ce n'est pas un dès
            {
                for(tmp="";j<i;j++)
                    tmp+=textSaisie[j];
                if(tmp=="")  tmp='0';
                if(multi==0)    multi++;
                for(int m=0;m<multi;m++)
                {
                    (tmp.toInt()>=0)    ?   tmpMoins=tmp.toInt()    :   tmpMoins= -(tmp.toInt());
                    (signe==sp)         ?   resultDes+=tmpMoins     :   resultDes-=tmpMoins;
                    if(premierSigne==true)  premierSigne=false;
                }
            }
            else                                        //Si c'est un dès
            {
                for(tmp="";textSaisie[j]!='d' && textSaisie[j]!='D';j++)    //NbDes
                    tmp+=textSaisie[j];
                nbDes=tmp.toInt();
                if(nbDes<0) nbDes= -nbDes;
                tmp="";
                if(nbDes==0)    nbDes=1;
                j++;
                for(j=j;j<i;j++)    //NbFace
                   tmp+=textSaisie[j];
                nbFaceDes=tmp.toInt();
                if(premierSigne==false)     premierSigne = false;

                //Secure lancé de dès
                if(nbFaceDes>10000 || nbDes>10000)  return 0;
                if(meilleur>nbDes)                  return 0;
                if(multi>10000)                     return 0;

                //LANCEMENT
                if(meilleur==0) meilleur=nbDes;
                if(multi==0)    multi++;
                for(int m=0;m<multi;m++)
                {
                    tmpDes=0;
                    //déclarer un tableau dinamique te taille NbDes
                    int* T = new int [nbDes];
                    //Lancer les des et stocker les résultats dans le tableaux
                    for(int k=0;k<nbDes;k++)
                        T[k]=des(nbFaceDes,mj);

                    //Trie du tableau
                    l=1;
                    if(nbDes>=2)
                        while(l<nbDes)
                        {
                            if(T[l]>T[l-1])
                            {
                                tmpT = T[l];
                                T[l] = T[l-1];
                                T[l-1] = tmpT;
                                l=1;
                            }
                            else l++;
                        }
                    //Valider les meilleurs valeurs.
                    for(int k=0;k<meilleur;k++) tmpDes+=T[k];

                    (signe==sp)   ?   resultDes+=tmpDes   :   resultDes-=tmpDes;
                    delete T;
                }
            }
        }
    }
    return resultDes;
}

//lance les dès en détaillent toutes les étapes
QString Des::lancerDesComplex(QString textSaisie, MJ mj)
{
    if(textSaisie==NULL)
        return "";

    QString tmp, textAffiche="", signe, sp="+";
    int resultDes=0, j=0, l, tmpT, nbDes, nbFaceDes, tmpDes, longueurChaine=textSaisie.length(), tmpMoins, meilleur=0, multi;
    bool premierSigne=true;

    for(int i=0;i<=longueurChaine;i++)
    {
        tmp="";
        if(textSaisie[i]=='+' || textSaisie[i]=='-' || i==longueurChaine)
        {
            meilleur=0;
            multi=1;
            if(isMulti(j, i, textSaisie))                //Si il faut multiplier
            {
                for(tmp="";textSaisie[j]!='*' && textSaisie[j]!='x';j++)
                    tmp+=textSaisie[j];
                multi=tmp.toInt();
            }
            tmp=textSaisie[j];
            if(j!=0)    j++;
            if(isMeilleur(j, i, textSaisie))        //Si il y a X meilleurs
            {
                for(tmp="";textSaisie[j]!='m' && textSaisie[j]!='M';j++)
                    tmp+=textSaisie[j];
                if(tmp=="")  tmp='0';
                meilleur=tmp.toInt();
            }
            if(meilleur!=0)    j++;
            signe=signePrecedant(i, textSaisie);
            if(!isDes(j, i, textSaisie))            //Si ce n'est pas un dès
            {
                for(tmp="";j<i;j++)
                    tmp+=textSaisie[j];
                if(tmp=="")  tmp='0';
                for(int m=0;m<multi;m++)
                {
                    (tmp.toInt()>=0)    ?   tmpMoins=tmp.toInt()    :   tmpMoins= -(tmp.toInt());
                    (signe==sp)         ?   resultDes+=tmpMoins     :   resultDes-=tmpMoins;
                    if(premierSigne==false) textAffiche+=signe ;
                    else    premierSigne=false;
                    textAffiche+= " " + QString::number(tmpMoins) + " ";
                }
            }
            else                                        //Si c'est un dès
            {
                for(tmp="";textSaisie[j]!='d' && textSaisie[j]!='D';j++)    //NbDes
                    tmp+=textSaisie[j];
                nbDes=tmp.toInt();
                if(nbDes<0) nbDes= -nbDes;
                tmp="";
                if(nbDes==0)    nbDes=1;
                j++;
                for(j=j;j<i;j++)    //NbFace
                   tmp+=textSaisie[j];
                nbFaceDes=tmp.toInt();
                if(premierSigne==false)     textAffiche += signe;
                else                        premierSigne = false;

                //Secure lancé de dès
                if(nbFaceDes>10000 || nbDes>10000)
                    return "Impossible de lancer plus de 10000 dès ou 10000 faces !";
                if(meilleur>nbDes)
                    return "Entrez un nombre de dès plus grand que les meilleurs dès à choisir !";
                if(multi>10000)
                    return "Impossible de lancer plus de 10000 fois le même dès !";

                //LANCEMENT
                if(meilleur==0) meilleur=nbDes;
                for(int m=0;m<multi;m++)
                {
                    tmpDes=0;
                    textAffiche+= " (" + QString::number(nbDes) + "d" + QString::number(nbFaceDes) + ") ";
                    int* T = new int [nbDes];
                    //Lancer les des et stocker les résultats dans le tableaux
                    for(int k=0;k<nbDes;k++)
                        T[k]=des(nbFaceDes, mj);
                    //Trie du tableau
                    l=1;
                    if(nbDes>=2)
                        while(l<nbDes)
                        {
                            if(T[l]>T[l-1])
                            {
                                tmpT = T[l];
                                T[l] = T[l-1];
                                T[l-1] = tmpT;
                                l=1;
                            }
                            else l++;
                        }
                    //Garder les meilleurs valeurs.
                    for(int k=0;k<meilleur;k++)
                    {
                        tmpDes+=T[k];
                        if(k!=0)    textAffiche += signe;
                        textAffiche +=  " " + QString::number(T[k]) + " ";
                    }

                    (signe==sp)   ?   resultDes+=tmpDes   :   resultDes-=tmpDes;
                    delete T;
                }
            }

        }
    }
    textAffiche+= "= " + QString::number(resultDes);
    return textAffiche;
}

//! Vérifier si il n'y a pas d'erreur de grammaire lors de la saisie

bool Des::isMeilleur(int debut, int fin, QString textSaisie)
{
    if(debut==fin)  return false;
    for(int i=debut;i<fin;i++)
        if(textSaisie[i]=='m' || textSaisie[i]=='M')
            return true;
    return false;
}

bool Des::isDes(int debut, int fin, QString textSaisie)
{
    if(debut==fin)  return false;
    for(int i=debut;i<fin;i++)
        if(textSaisie[i]=='d' || textSaisie[i]=='D')
            return true;
    return false;
}

bool Des::isMulti(int debut, int fin, QString textSaisie)
{
    if(debut==fin)  return false;
    for(int i=debut;i<fin;i++)
        if(textSaisie[i]=='*' || textSaisie[i]=='x')
            return true;
    return false;
}

QString Des::signePrecedant(int n, QString textSaisie)
{
    for(int i=n-1;i>=0;i--)
       if(textSaisie[i]=='+' || textSaisie[i]=='-')
           return QString(textSaisie[i]);
    return QString("+");
}


// Comportement du lanceur de dès potentiellement truquer
int Des::des(int d, MJ m)
{
    int t = (rand()%(d))+1;
    if(m==MJ::N) return t;
    if(m==MJ::TM)
    {
        if(t>d)   return t;
        int t2=(rand()%(d))+1;
        if(t>t2)    return t;
        else        return t2;
    }
    if(m==MJ::M)
    {
        if(t>(d*0.8))    return t;
        int t2=(rand()%(d))+1;
        if(t>t2)    return t;
        else        return t2;
    }
    if(m==MJ::G)
    {
        if(t<d)    return t;
        int t2=(rand()%(d))+1;
        if(t<t2)    return t;
        else        return t2;
    }
    if(m==MJ::TG)
    {
        if(t<(d*0.8))    return t;
        int t2=(rand()%(d))+1;
        if(t<t2)    return t;
        else        return t2;
    }
    return t;
}
