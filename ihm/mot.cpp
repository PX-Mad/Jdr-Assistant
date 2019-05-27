#include "mot.h"

// Corpus = liste de mot à analyser
// Codex = liste de mot analysé et utilisable pour généré des mots nouveaux

// Création de l'interface de génération de mot ("Nom")
mot::mot()
    : codexManager(this)
{
    QVBoxLayout* mainLayout1 = new QVBoxLayout();
    this->setLayout(mainLayout1);

    QGroupBox* mainOng = new QGroupBox("Générer un nom");
    mainLayout1->addWidget(mainOng);
    QVBoxLayout* mv1 = new QVBoxLayout();
    mainOng->setLayout(mv1);
    QHBoxLayout* mh1 = new QHBoxLayout();
    mv1->addLayout(mh1);
    longeur.addItem(tr("Indifférent"));   // 3 - 15
    longeur.addItem(tr("Court"));         // 3 - 5
    longeur.addItem(tr("Moyen"));         // 6 - 10
    longeur.addItem(tr("Long"));          // 11 - 15
    longeur.setCurrentIndex(CONFIG.getParam("longMot","0").toInt());
    QPushButton *bGenerat = new QPushButton(tr("Générer"));
    nombre.setValue(CONFIG.getParam("nbMot","10").toInt());
    nombre.setMinimum(1);
    nombre.setMaximum(9999);
    results.setReadOnly(true);
    results.setStyleSheet("font-size: 16px;");
    mh1->addWidget(&race);
    mh1->addWidget(&longeur);
    mh1->addWidget(&nombre);
    mh1->addWidget(bGenerat);
    mv1->addWidget(&results);

    QGroupBox* mainOng2 = new QGroupBox(tr("Gestion des codexs"));
    mainLayout1->addWidget(mainOng2);
    QHBoxLayout* mh2 = new QHBoxLayout();
    mainOng2->setLayout(mh2);
    creer.setText(tr("Créer"));
    suppr.setText(tr("Supprimer"));
    import.setText(tr("Importer"));
    rename.setText(tr("Renommer"));
    mh2->addWidget(&creer);
    mh2->addWidget(&suppr);
    mh2->addWidget(&import);
    mh2->addWidget(&rename);

    initCombo(&race, "/res/codex/", ".cdx", CONFIG.getParam("lMot"));

    // Création des connexions
    QObject::connect(&creer, SIGNAL(clicked()), this, SLOT(genererCodex()));
    QObject::connect(bGenerat, SIGNAL(clicked()), this, SLOT(genererMots()));
    QObject::connect(&suppr, SIGNAL(clicked()), this, SLOT(supprCodex()));
    QObject::connect(&import, SIGNAL(clicked()), this, SLOT(importCodex()));
    QObject::connect(&rename, SIGNAL(clicked()), this, SLOT(renameCodex()));
}

//! SLOTS
// Générer un enssemble de mots à partir des contraintes choisies.
void mot::genererMots()
{
    //Configuration
    CONFIG.setParam("lMot",race.currentText())->save();
    CONFIG.setParam("longMot",QString::number(longeur.currentIndex()))->save();
    CONFIG.setParam("nbMot",nombre.text())->save();

    // Généré le dictionnaire de mot
    codex c = codexManager.buildData(QCoreApplication::applicationDirPath()+"/res/codex/"+race.currentText()+".cdx");

    // Récupérer les contraintes
    int miw=3, maw=15, nbw=nombre.value();
    if(longeur.currentIndex()==1)  maw=5;
    if(longeur.currentIndex()==2)  {miw=6;maw=10;}
    if(longeur.currentIndex()==3)  miw=11;

    // Récupérer l'enssemble des mots
    QString *s = new QString[nbw], tmp;
    for(int i=0;i<nbw;i++)
    {
        do tmp = codexManager.generatWord(c);
        while(tmp.length()>maw||tmp.length()<miw);
        s[i] = tmp;
    }

    // Ecrire les mot dans le QTextEdit
    results.setText("");
    for(int i=0;i<nbw;i++) results.append(s[i]);
}

// Lancer la génération d'un codex à partir d'un corpus
void mot::genererCodex()
{
    QString c = QFileDialog::getOpenFileName(this, tr("Choisir le corpus"), QString(), "Textes (*.txt)");
    if(c.isEmpty()) return;
    codexManager.creatCodex(c);
    if(QMessageBox::question(this, tr("Codex créé avec succès"), tr("Codex créé avec succès. L'ajouter à votre liste de codex personnel?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    //On créer le répèrtoire si il n'existe pas.
    QDir d1(QCoreApplication::applicationDirPath());
    d1.mkdir("res");
    d1.mkdir("res/codex");

    //On copie le fichier codex
    c=c.replace(".txt", ".cdx");
    QFile file(c);
    QFileInfo fi(file);
    file.copy(QCoreApplication::applicationDirPath()+"/res/codex/"+fi.fileName());

    this->initCombo(&race, "/res/codex/", ".cdx");
}

//Permet de supprimer un codex dans ceux existant... merci pour ce commentaire
void mot::supprCodex()
{
    bool ok;
    QStringList items;
    QStringList files = QDir(QCoreApplication::applicationDirPath()+"/res/codex/").entryList();
    foreach(QString file, files)  if(file!="."&&file!="..") items << file.replace(".cdx", "");
    QString item = QInputDialog::getItem(this, tr("Suppression"),  tr("Choisir le codex à supprimer"), items, 0, false, &ok);
    if(!ok) return;
    if(QFile::remove(QCoreApplication::applicationDirPath()+"/res/codex/"+item+".cdx"))
         QMessageBox::information(this, tr("Réussite"), tr("Fichier supprimé avec succès."));
    else QMessageBox::information(this, tr("Erreur"), tr("Problème lors de la suppression du fichier."));
    this->initCombo(&race, "/res/codex/", ".cdx");
}

// Importe un codex existant dans la liste des codexs utilisable
void mot::importCodex()
{
    QString c = QFileDialog::getOpenFileName(this, tr("Choisir le codex à importer"), QString(), "Codex (*.cdx)");
    if(c.isEmpty()) return;
    QFile file(c);
    QFileInfo fi(file);
    if(QFile::exists(QCoreApplication::applicationDirPath()+"/res/codex/"+fi.fileName()))
    {
        if(QMessageBox::question(this, tr("Remplacer?"), tr("Codex déjà existant, ecraser celui-ci?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) return;
        else    QFile::remove(QCoreApplication::applicationDirPath()+"/res/codex/"+fi.fileName());
    }
    file.copy(QCoreApplication::applicationDirPath()+"/res/codex/"+fi.fileName());
}

// Changer le nom d'un codex chargé en mémoire
void mot::renameCodex()
{
    bool ok;
    QInputDialog qDialog ;
    QStringList items;
    QStringList files = QDir(QCoreApplication::applicationDirPath()+"/res/codex/").entryList();
    foreach(QString file, files)  if(file!="."&&file!="..") items << file.replace(".cdx", "");

    QString oldName = QInputDialog::getItem(this, tr("Renommer"),  tr("Choisir le codex à renomer"), items, 0, false, &ok);
    if(!ok) return;

    QString newName = QInputDialog::getText(this, tr("Nouveau nom"), tr("Renommer le codex \"")+oldName+tr("\" par ?"), QLineEdit::Normal, QString(), &ok);
    if (!ok||newName.isEmpty())   return;

    QFile file(QCoreApplication::applicationDirPath()+"/res/codex/"+oldName+".cdx");
    file.rename(QCoreApplication::applicationDirPath()+"/res/codex/"+newName+".cdx");
    this->initCombo(&race, "/res/codex/", ".cdx");
}

void mot::initCombo(QComboBox *c, QString path, QString typeFile, QString initValue)
{
    c->clear();
    int i=-1,j=0;
    QStringList files = QDir(QCoreApplication::applicationDirPath()+path).entryList();
    QString s;
    foreach(QString file, files)
        if(file.contains(typeFile))
        {
            s = file.replace(typeFile, "");
            c->addItem(s);
            if(s==initValue)i=j;
            j++;
        }
    if(initValue==""||i==-1)    return;
    c->setCurrentIndex(i);
}
