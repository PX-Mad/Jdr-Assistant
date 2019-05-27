#include "codexmanager.h"

CodexManager::CodexManager(QWidget* window)
{
    _w = window;
}
// Retourne un mot généré à partir du codex c
QString CodexManager::generatWord(codex c)
{
    QString ret="";
    int m1=0, m2=0, m3;
    while(true)
    {
        m3 = drawLettre(c, m1, m2);
        if(m3==2)   break;
        ret+=c.dico[m3-3];
        m1=m2;m2=m3;
    }
    return ret;
}

// Pioche la valeur d'une lettre dans la matrice à partir du dico
int CodexManager::drawLettre(codex c, int m1, int m2)
{
    if(m1!=0)m1--;
    if(m2!=0)m2--;
    int t=0;
    // Nombre total
    for(int j=c.dico.length()+2, k=0;k<j;k++)t+=c.stat[m1][m2][k];
    int rng=(rand()%t)+1, i=0;
    // Choix
    for(;rng>0;i++) rng-=c.stat[m1][m2][i];
    return i;
}

// Retourne une structure codex tiré à partir du codex c
codex CodexManager::buildData(QString c)
{
    codex ret;
    QFile file(c);
    if(!file.open(QFile::ReadOnly | QIODevice::Text))
    {
        if(_w!=NULL)QMessageBox::warning(_w, qTr("Erreur, fichier introuvable"), qTr("Le fichier \"")+c+qTr("\" n'existe pas, veuillez verifier l'emplacement !"));
        return ret;
    }
    QTextStream flux(&file);

    //! Récupération du dictionnaire et de la taille de ce dernier
    ret.dico=flux.readLine();
    int r = ret.dico.length()+2;

    //! Récupération du "tableau" de probabilité
    QString bProba=flux.readLine();
    QStringList tmp = fromShortSerial(bProba).split(' ');
    file.close();

    //! Création du tableau de récupération des données
    int ***t = new int** [r];
    for(int i=0,j;i<r;i++)
    {
        t[i] = new int* [r];
        for(j=0;j<r;j++)
        {
            t[i][j] = new int[r];
            for(int k=0;k<r;k++) t[i][j][k] = 0;
        }
    }

    //! Convertion du "tableau" en matrice
    for(int i=0, bi=0, bj=0, bk=0, s=tmp.size()-1;i<s;i++)
    {
        t[bi][bj][bk] = tmp.at(i).toInt();
        if(++bk>=r){bj++;bk=0;}
        if(bj>=r){bi++;bj=0;}
    }
    ret.stat=t;
    return ret;
}

// Créer un codex à partir du corpus c (Path complet)
void CodexManager::creatCodex(QString c)
{
    //! Ouvertur du fichier
    QFile file(c);
    if(!file.open(QFile::ReadOnly | QIODevice::Text))
    {
        if(_w!=NULL)QMessageBox::warning(_w, qTr("Erreur, fichier introuvable"), qTr("Le fichier \"")+c+qTr("\" n'existe pas, veuillez vérifier l'emplacement !"));
        return;
    }
    QTextStream flux(&file);

    //! Compter les différant types de charactères
    int n=1;
    QString typeChar;
    for(QChar ch;!flux.atEnd();)
    {
        flux >> ch;
        if(!typeChar.contains(ch))  typeChar+=ch;
    }

    //On supprime le retour à la ligne
    typeChar.remove("\n");
    typeChar.remove("\r");
    typeChar.remove("\r\n");
    int r=typeChar.length()+2;  //L'enssemble des char+Start et END

    //! Création du tableau de récupération des données
    int ***t = new int** [r];
    for(int i=0,j;i<r;i++)
    {
        t[i] = new int* [r];
        for(j=0;j<r;j++)
        {
            t[i][j] = new int[r];
            for(int k=0;k<r;k++) t[i][j][k] = 0;
        }
    }

    //! Récupération des données à proprement parlé
    file.close();file.open(QFile::ReadOnly | QIODevice::Text);flux.reset();    //reset du pointeur de lecture
    QString line=flux.readLine();

    for(int m1, m2, m3;!flux.atEnd();line=flux.readLine())
    {
        n++;
        m1 = 0;   // Position de START dans tout les cas.
        m2 = 0;   // Position de START dans tout les cas.
        for(int i=0;i<line.length();i++)
        {
            m3 = positionOn(typeChar, line[i])+2;
            t[m1][m2][m3]+=1;
            m1=m2;m2=m3;
        }
        t[m1][m2][1]+=1;    // définition de END
    }


    // Convertion des données en texte
    QString toWrite = typeChar+'\n',tr="";
    //Tableau de données
    for(int i=0;i<r;i++)
        for(int j=0;j<r;j++)
            for(int k=0;k<r;k++)
                tr += QString::number(t[i][j][k])+" ";
    toWrite+=toShortSerial(tr)+"\nBuild whith "+QString::number(n)+" words";

    // Ecriture des données dans le fichier texte
    QFile fileOut(c.replace(".txt", ".cdx"));
    fileOut.open(QFile::WriteOnly | QIODevice::Text);
    fileOut.write(toWrite.toLatin1(),toWrite.size());
    fileOut.close();
}

// TODO: Passer par du standard, mieux géré les erreurs
// Récupère la position de la lettre c dans la chaine l
int CodexManager::positionOn(QString l, QChar c)
{
    if(&c==new QChar('\n')) return -2;
    for(int i=0;i<l.length();i++)   if(l[i]==c)   return i;
    return -1;
}

// TODO: l'integrer directement dans l'ecriture sans passé par cette trensition de merde
/**
 * @brief toShortSerial Convertie un tabeau sérialiser pour le rendre plus court afin d'être ecrit dans un fichier
 * @param s Chaine à racourssir
 * @return
 */
QString CodexManager::toShortSerial(QString s)
{
    QString r = "";int i=0,j;
    QStringList l = s.split(' ');
    foreach(QString t, l)
    {
        j=t.toInt();
        if(j==0) i++;
        else
        {
            r+=QString::number(i)+" "+QString::number(j)+" ";
            i=0;
        }
    }
    return r;
}

// TODO: l'integrer directement dans la lecture sans passé par cette trensition de merde
/**
 * @brief fromShortSerial Couverti une chaine racoursit en chaine longue pour récupérer les données.
 * @param s
 * @return
 */
QString CodexManager::fromShortSerial(QString s)
{
    QString r = "";
    QStringList l = s.split(' ');
    for(int i=0,k,j;i<l.size();i++)
    {
        for(j=0,k=l[i].toInt();j<k;j++) r+="0 ";
        if((++i)>=l.size())             break;
        r+=l[i]+" ";
    }
    return r;
}
