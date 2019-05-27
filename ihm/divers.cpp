#include "fenetre.h"

// Construit l'onglet de génération "Non-trié"
divers::divers()
    : _urgParsor(this)
{
    //IHM
    QVBoxLayout* mainLayout3 = new QVBoxLayout();
    this->setLayout(mainLayout3);

    QGroupBox* mainOng3 = new QGroupBox("Générer quelque chose");
    mainLayout3->addWidget(mainOng3);
    QVBoxLayout* vb1 = new QVBoxLayout();
    mainOng3->setLayout(vb1);
    QHBoxLayout* hb1 = new QHBoxLayout();
    vb1->addLayout(hb1);

    initComboDossier(&cbSelectDivers, QCoreApplication::applicationDirPath()+"/res/divers", "lDivers");
    hb1->addWidget(&cbSelectDivers);
    nombreDivers.setMinimum(0);
    nombreDivers.setMaximum(99999);
    nombreDivers.setValue(CONFIG.getParam("nbDivers","10").toInt());
    hb1->addWidget(&nombreDivers);
    bGeneratDivers.setText("Générer");
    hb1->addWidget(&bGeneratDivers);

    resultDivers.setReadOnly(true);
    resultDivers.setStyleSheet("font-size: 16px;");
    vb1->addWidget(&resultDivers);

    QObject::connect(&bGeneratDivers, SIGNAL(clicked()), this, SLOT(genererDivers()));
}

void divers::genererDivers()
{

    CONFIG.setParam("nbDivers",QString::number(nombreDivers.value()))
    ->setParam("lDivers", cbSelectDivers.currentText())->save();
    QString r ="";
    QStringList qslGeneratedElements = _urgParsor.generat(QCoreApplication::applicationDirPath()+
        "/res/divers/"+cbSelectDivers.currentText(), nombreDivers.value());
    for(int i=0,n=qslGeneratedElements.size();i<n;i++)
        r += qslGeneratedElements.at(i)+"<br>";
    resultDivers.setHtml(r);
}

// Initialiser un comboBox avec les dossiers du path
void divers::initComboDossier(QComboBox* c, QString path, QString initValue)
{
    //Récupérer toutes les valeurs
    QDir l_path(path);
    l_path.setFilter(QDir::AllDirs);
    foreach(QString s, l_path.entryList())
        if(s!="."&&s!="..") c->addItem(s);

    //Initialiser la valeur via la classe Config
    if(initValue=="")    return;
    int i = c->findText(CONFIG.getParam(initValue), Qt::MatchExactly | Qt::MatchCaseSensitive);
    if(i!=-1)  c->setCurrentIndex(i);
}
