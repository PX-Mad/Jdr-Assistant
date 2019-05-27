#ifndef CODEXMANAGER_H
#define CODEXMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <QtWidgets>
#include <QWidget>

#define qTr QObject::tr

// Représentation en mémoire d'un codex
struct codex
{
    QString dico;
    int*** stat;
};

// Gère les codèx, génère des mots à partir de ceux-ci et analiser les corpus
class CodexManager
{
public:
    CodexManager(QWidget *window=NULL);
    void creatCodex(QString c);
    codex buildData(QString c);
    QString generatWord(codex c);

    QString toShortSerial(QString s);
    QString fromShortSerial(QString s);

private:
    QWidget* _w;

    int positionOn(QString l, QChar c);
    int drawLettre(codex c, int m1, int m2);
};

#endif // CODEXMANAGER_H
