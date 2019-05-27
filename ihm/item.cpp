#include "item.h"

// Construit l'interface de génération d'objet En fonction des JDRs
// TODO: Ajouter des objet pour des JDRs
item::item()
    : _urgParsor(this)
{
    //IHM
    QVBoxLayout *mainLayout3 = new QVBoxLayout(), *vb1 = new QVBoxLayout();
    QGroupBox* mainOng3 = new QGroupBox("Générer quelque chose");
    QHBoxLayout* hb1 = new QHBoxLayout();
    this->setLayout(mainLayout3);
    mainLayout3->addWidget(mainOng3);
    mainOng3->setLayout(vb1);
    vb1->addLayout(hb1);

    initComboDossier(&cbSet, QCoreApplication::applicationDirPath()+"/res/objet/","ObjSet");
    hb1->addWidget(&cbSet);
    if(cbSet.count()>0)
        initComboDossier(&cbSelectItem, QCoreApplication::applicationDirPath()+"/res/objet/"+cbSet.currentText(), "lItem");
    hb1->addWidget(&cbSelectItem);
    nombreItem.setMinimum(0);
    nombreItem.setMaximum(99999);
    nombreItem.setValue(CONFIG.getParam("nbItem","10").toInt());
    hb1->addWidget(&nombreItem);
    bGeneratItem.setText("Générer");
    hb1->addWidget(&bGeneratItem);

    resultItem.setReadOnly(true);
    resultItem.setStyleSheet("font-size: 16px;");
    vb1->addWidget(&resultItem);

    QObject::connect(&bGeneratItem, SIGNAL(clicked()), this, SLOT(genererItem()));
    QObject::connect(&cbSet, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange(int)));
}

// Fait le lien entre la selection des JDR et des items
void item::comboChange(int i)
{
    Q_UNUSED(i);
    while(cbSelectItem.count()>0)    cbSelectItem.removeItem(0);
    initComboDossier(&cbSelectItem, QCoreApplication::applicationDirPath()+"/res/objet/"+cbSet.currentText());
    CONFIG.setParam("ObjSet",cbSet.currentText());
    CONFIG.setParam("lItem",cbSelectItem.currentText());
    CONFIG.setParam("nbItem",nombreItem.text())->save();
}

//Slot: Générer une liste d'objets
void item::genererItem()
{
    CONFIG.setParam("ObjSet",cbSet.currentText())
    ->setParam("lItem",cbSelectItem.currentText())
    ->setParam("nbItem",nombreItem.text())->save();
    QString r ="";
    QStringList qslGeneratedElements = _urgParsor.generat(QCoreApplication::applicationDirPath()+
        "/res/objet/"+cbSet.currentText()+"/"+cbSelectItem.currentText(), nombreItem.value());
    for(int i=0,n=qslGeneratedElements.size();i<n;i++)
        r += qslGeneratedElements.at(i)+"<br>";
    resultItem.setHtml(r);
    CONFIG.setParam("lItem",cbSelectItem.currentText())->save();
}

void item::initComboDossier(QComboBox* c, QString path, QString initValue)
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
