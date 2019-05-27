#include "config.h"

Config& Config::getInstance()
{
    static Config config_;
    return config_;
}

/**
 * @brief setParam Ajouter/Modifier un paramètre en mémoire <br>/!\ Terminer les modifications par un save
 * @param paramName Nom/Etiquette du paramètre à modifier
 * @param value Valeur à placer dans le paramètre
 * @return this pour call succesif
 */
Config* Config::setParam(QString paramName, QString value)
{
    for(param *p : param_)
        if(p->name==paramName)
        {
            p->value=value;
            return this;
        }
    param_.push_back(new param(paramName,value));
    return this;
}

/**
 * @brief setParam Lit un paramètre en mémoire
 * @param paramName Nom/Etiquette du paramètre à lire
 * @param defaultReturn valeur par défaut si le paramètre n'existe pas
 * @return Valeur du paramètre
 */
QString Config::getParam(QString paramName, QString defaultReturn)
{
    for(param *p : param_)
        if(p->name==paramName)
            return p->value;
    return defaultReturn;
}

/**
 * @brief setParam Sauvegarde les paramètres en mémoire dans le fichier texte
 * @param separator séparateur à utiliser entre l'étiquette et la valeur, par défaut ";"
 * @return this pour call succesif
 */
Config* Config::save(QChar separator)
{
    QFile f(filePath);
    if(!f.open(QFile::WriteOnly | QIODevice::Text))
        return this;
    QTextStream out(&f);
    out.setCodec("UTF-8");
    for(param *p : param_)
        out << p->name << separator << p->value << "\n";
    return this;
}

/**
 * @brief setParam Charge les paramètres à partir du fichier texte
 * @param separator séparateur à utiliser entre l'étiquette et la valeur, par défaut ";"
 * @return this pour call succesif
 */
Config* Config::load(QString filepath, QChar separator)
{
    filePath=filepath;
    QFile f(filePath);
    if(!f.open(QFile::ReadOnly | QIODevice::Text))
        return this;
    QStringList sl;
    for(param_.clear();!f.atEnd();)
    {
        sl = ((QString)f.readLine()).split(separator);
        if(sl.size()==2)
            param_.push_back(new param(sl[0],sl[1].remove("\n").remove("\r")));
    }
    return this;
}
