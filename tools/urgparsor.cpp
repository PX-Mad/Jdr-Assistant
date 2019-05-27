#include "urgparsor.h"

// TODO: Renommer les méthodes

urgParsor::urgParsor(QWidget *window)
{
    _w=window;
    codexManager = new CodexManager(_w);
}
/** Construire un texte à partir d'un fichier URG Script
 * qGeneratorPath : Path de base du générateur
 * nbGenerate : Nombre d'e li'élément à générer
 * */
QStringList urgParsor::generat(QString qGeneratorPath, unsigned nbGenerate) {
    QStringList returnList;
    QString startPath = qGeneratorPath+"/start.urg";
    // On génère le
    for(unsigned i=0;i<nbGenerate;i++) {
        returnList << analyseLine(readRandomLineOfFile(startPath), qGeneratorPath);
    }
    // Libération de ma QMap QStringList*
    QMapIterator<QString, QStringList*> it(_bufferFile);
    while (it.hasNext()) {
        it.next();
        delete it.value();
    }
    _bufferFile.clear();
    _bufferCodex.clear();
    return returnList;
}

/** Construire un texte à partir d'un fichier URG Script
 * l : Ligne à analyser
 * path : Path de base du générateur
 * */
QString urgParsor::analyseLine(QString l, QString path)
{
    QStringList tmp;
    QString lastRequest="", line=l, ts, r;
    bool isNumber;
    QStringList histo;
    for(int ps=-1, pn=-1, i=0, t, t2; i<line.size();i++)
    {
        if(line[i]==QChar('['))    ps=i;
        else if(line[i]==QChar(']')&&ps!=-1)
        {
            tmp = line.mid(ps+1,i-ps-1).split(",");
            // Si un seul paramètres
            if(tmp.size()==1)
            {
                t=tmp.at(0).toInt(&isNumber);
                if(!isNumber)   // Si requete à faire
                {
                    lastRequest = readRandomLineOfFile(path+"/"+tmp.at(0));
                    lastRequest = analyseLine(lastRequest, path);
                    histo << lastRequest;
                    ts=line.mid(ps,i-ps+1);
                    /*if(lastRequest==NULL||lastRequest.isEmpty())
                        return "Error, empty line: "+line;*/
                    line.replace(line.indexOf(ts), ts.size(), lastRequest.split(";").at(0));
                }
                else            // Si rappel de la précédante requete
                {
                    ts=line.mid(ps,i-ps+1);
                    /*if(lastRequest==NULL||lastRequest.isEmpty())
                        return "Error, empty line: "+lastRequest;
                    if(lastRequest.split(";").size()<=t)
                        return "Index("+QString::number(t)+") error on: "+line;*/
                    line.replace(line.indexOf(ts), ts.size(), lastRequest.split(";").at((lastRequest.size()>t && t>0)?t-1:0));
                }
            }
            else if(tmp.size()==2)
            {
                t=tmp.at(0).toInt(&isNumber);
                if(!isNumber)   // Si requete à faire
                {
                    lastRequest = readRandomLineOfFile(path+"/"+tmp.at(0));
                    lastRequest = analyseLine(lastRequest, path);
                    histo << lastRequest;
                    ts=line.mid(ps,i-ps+1);
                    t=tmp.at(1).toInt(&isNumber)-1;
                    if(t<0) t=0;
                    /*if(lastRequest==NULL||lastRequest.isEmpty())
                        return "Error, empty line: "+lastRequest;
                    if(lastRequest.split(";").size()<=t)
                        return "Index("+QString::number(t)+") error on: "+lastRequest;*/
                    line.replace(line.indexOf(ts), ts.size(), lastRequest.split(";").at(t));
                }
                else            // Si rappel d'une requete d'un index donnée
                {
                    ts=line.mid(ps,i-ps+1);
                    t2=tmp.at(1).toInt(&isNumber);
                    t--;
                    if(t<0 || t>tmp.size()) t=0;
                    line.replace(line.indexOf(ts), ts.size(), histo[t].split(";").at((lastRequest.size()>t2 && t2>0)?t2-1:0));
                }
            }
            else if(tmp.size()==3 && tmp.at(0).contains(".cdx"))
            {
                if(tmp.at(0).contains(".cdx"))  // Appel de cdx
                {
                    ts=line.mid(ps,i-ps+1);
                    t=tmp.at(1).toInt();t2=tmp.at(2).toInt();
                    if(t>t2){t2+=t;t=t2-t;t2-=t;}
                    do r = codexManager->generatWord(getCodex(path+"/"+tmp[0]));
                    while(r.length()>t2||r.length()<t);
                    line.replace(line.indexOf(ts), ts.size(), r);
                }
                /*else if("lien"==tmp.at(0).toLower())  // Lien, TODO: A faire fonctionner
                {
                    QMessageBox::warning(this, "Debug", "<a href=\""+tmp.at(2)+"\">"+tmp.at(1)+"</a>");
                    ts=line.mid(ps,i-ps+1);
                    line.replace(line.indexOf(ts), ts.size(), "<a href=\""+tmp.at(2)+"\">"+tmp.at(1)+"</a>");
                }*/
            }
            else if(tmp.size()>=3 && CPR_QSTRING(tmp.at(0),"info"))
            {
                ts=line.mid(ps,i-ps+1);
                line.replace(line.indexOf(ts), ts.size(), "<span title=\""+tmp.at(2)+"\">"+tmp.at(1)+"</span>");
            }
            i=0;
            ps=-1;
        }

        //Gestion des calcules
        if(line[i]==QChar('{'))    pn=i;
        else if(line[i]==QChar('}')&&pn!=-1)
        {
            ts=line.mid(pn,i-pn+1);
            line.replace(line.indexOf(ts)-1, ts.size()+2, QString::number(Des::lancerDes(ts.remove('{').remove('}'))));
            pn=-1;
            i=0;
        }
    }
    if(line.contains("[")&&line.contains("]")){
        return analyseLine(line, path);
    }
    return line;
}

codex urgParsor::getCodex(QString path)
{
    if(_bufferCodex.contains(path)==false) {
        _bufferCodex[path]=codexManager->buildData(path);
    }
    return _bufferCodex[path];
}

// Récupère une ligne au hasard dans le fichier désigné par "path" en considérant les #$
QString urgParsor::readRandomLineOfFile(QString path)
{
    if(_bufferFile.contains(path)==false) {
        //Récupération de la ligne du fichier
        QFile file(path);
        if(!file.open(QFile::ReadOnly | QIODevice::Text))   return "";
        QStringList *fileBuffer = new QStringList;
        QString qsLine;
        int i, j;
        while(!file.atEnd())
        {
            qsLine = file.readLine();
            if(qsLine.length()>2&&qsLine[0]==QChar('#')&&qsLine[1]==QChar('$'))
            {
                // On considère la ligne suivante comme plusieurs
                qsLine=qsLine.remove("#$");
                if(!file.atEnd())
                {
                    j=qsLine.toInt();
                    qsLine = file.readLine();
                    for(i=0;i<j;i++) {
                        fileBuffer->append(qsLine.trimmed());
                    }
                }
            } else {
                // On ne stock qu'une ligne
                fileBuffer->append(qsLine);
            }
        }
        _bufferFile[path]=fileBuffer;
    }
    return _bufferFile[path]->at(rand()%_bufferFile[path]->size());
}
