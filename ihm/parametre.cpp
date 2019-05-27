#include "parametre.h"

parametre::parametre()
{
    QVBoxLayout *mainLayout1 = new QVBoxLayout();
    QHBoxLayout *HL1 = new QHBoxLayout();
    mainLayout1->setAlignment(Qt::AlignTop);
    mainLayout1->addLayout(HL1);
    this->setLayout(mainLayout1);

    // ASSISTANT
    QGroupBox* mainOng = new QGroupBox("Assistant");
    mainOng->setMaximumHeight(200);
    HL1->addWidget(mainOng);
    //Base vertical
    QVBoxLayout* mv1 = new QVBoxLayout();
    mv1->setAlignment(Qt::AlignTop);
    mainOng->setLayout(mv1);
    typeMJ.addItem("Très gentil MJ");
    typeMJ.addItem("Gentil MJ");
    typeMJ.addItem("MJ Normal");
    typeMJ.addItem("Méchant MJ");
    typeMJ.addItem("Très méchant MJ");
    mv1->addWidget(&typeMJ);
    //Dès d'initaitive
    QHBoxLayout *hl2 = new QHBoxLayout();
    hl2->setAlignment(Qt::AlignLeft);
    mv1->addLayout(hl2);
    QLabel *lblInit = new QLabel("Dès d'initiative : ");
    hl2->addWidget(lblInit);
    desInit.setRange(1, 10000);
    hl2->addWidget(&desInit);

    paramNum.setText("Numérotation automatique");
    mv1->addWidget(&paramNum);
    paramStat.setText("Statistique automatique");
    mv1->addWidget(&paramStat);


    // Style
    QGroupBox* mainOng2 = new QGroupBox("Apparence");
    mainOng2->setMaximumHeight(200);
    HL1->addWidget(mainOng2);
    QVBoxLayout* mv2 = new QVBoxLayout();
    mv2->setAlignment(Qt::AlignTop);
    mainOng2->setLayout(mv2);
    listStyle.addItems(QStyleFactory::keys());
    mv2->addWidget(&listStyle);

    paramInit();

    linkCheckNum();
    QApplication::setStyle(QStyleFactory::create(listStyle.currentText()));

    QObject::connect(&typeMJ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &parametre::paramSave);
    QObject::connect(&listStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &parametre::paramSave);
    QObject::connect(&listStyle, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &parametre::change_style);
    QObject::connect(&paramNum, &QCheckBox::stateChanged, this, &parametre::paramSave);
    QObject::connect(&paramStat, &QCheckBox::stateChanged, this, &parametre::paramSave);
    QObject::connect(&desInit, QOverload<int>::of(&QSpinBox::valueChanged), this, &parametre::paramSave);
    QObject::connect(&paramNum, &QCheckBox::stateChanged, this, &parametre::linkCheckNum);
    QObject::connect(&paramStat, &QCheckBox::stateChanged, this, &parametre::linkCheckNum);
}

// Sauvegarde les paramètres à partir du système de configuration
void parametre::paramSave(int i)
{
    Q_UNUSED(i);

    // ASSISTANT
    CONFIG.setParam("typeMJ", QString::number(typeMJ.currentIndex()));
    CONFIG.setParam("current_style", listStyle.currentText());
    CONFIG.setParam("numAuto", QString::number(paramNum.isChecked()));
    CONFIG.setParam("statAuto", QString::number(paramStat.isChecked()));
    CONFIG.setParam("desInit", QString::number(desInit.value()))->save();
}

// Charge les paramètre à partir du système de configuration
void parametre::paramInit()
{
    typeMJ.setCurrentIndex(CONFIG.getParam("typeMJ","2").toInt());
    listStyle.setCurrentText(CONFIG.getParam("current_style", QStyleFactory::keys()[0]));
    paramNum.setChecked(CONFIG.getParam("numAuto","1").toInt()==1);
    paramStat.setChecked(CONFIG.getParam("statAuto","1").toInt()==1);
    desInit.setValue(CONFIG.getParam("desInit","20").toInt());
}

// Gère le lien entre la numérotation automatique et la complétion des autre stats (vie/init)
void parametre::linkCheckNum(int i)
{
    Q_UNUSED(i);
    if(!paramNum.isChecked())
    {
        paramStat.setChecked(false);
        paramStat.setEnabled(false);
    }
    else
        paramStat.setEnabled(true);

    this->paramSave(0);
}

void parametre::change_style(QString style)
{
    QApplication::setStyle(QStyleFactory::create(style));
}
