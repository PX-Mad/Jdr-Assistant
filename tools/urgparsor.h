#ifndef URGPARSOR_H
#define URGPARSOR_H

#include <QtWidgets>
#include "des.h"
#include "macro.h"
#include "codexmanager.h"

// Parseur maison pour le URG Script
// URG = Ultimat RP Generator
// Psedo-langage maison de génération de texte

class urgParsor
{
public:
    urgParsor(QWidget *window=Q_NULLPTR);

    QStringList generat(QString path, unsigned nbGenerate);
    QString analyseLine(QString l, QString path);
    QString readRandomLineOfFile(QString path);
    codex getCodex(QString path);

private:
    QWidget* _w;
    CodexManager *codexManager;
    QMap<QString, QStringList *> _bufferFile;
    QMap<QString, codex> _bufferCodex;
};

#endif // URGPARSOR_H
