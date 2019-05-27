#ifndef CONFIG_H
#define CONFIG_H

#include <QtWidgets>

#define CONFIG Config::getInstance()

// Système de configuration de projet générique et réutilisable
// Chaque objet correspond à un fichier
class Config
{
public:

    static Config& getInstance();

    Config* setParam(QString paramName, QString value);
    QString getParam(QString paramName,QString defaultReturn="");

    Config* save(QChar separator=QChar(';'));
    Config* load(QString filepath, QChar separator=QChar(';'));

    class param{public:QString name,value;param(QString n, QString v){name=n;value=v;};};

private:

    Config() {};

    std::vector<param*> param_;
    QString filePath;
};

#endif // CONFIG_H

