#include "item_init.h"

item_init::item_init(QWidget *parent, QString nom, int valeur)
{
    this->setParent(parent);
    this->init_graphic(nom, valeur);
}

item_init::item_init(QString nom, int valeur)
{
    this->init_graphic(nom, valeur);
}

void item_init::init_graphic(QString nom, int valeur)
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    // Nom
    m_nom = new QLabel(nom);
    mainLayout->addWidget(m_nom);
    // Spacer
    mainLayout->addWidget(new QLabel(""));
    // Valeur
    m_valeur = new QSpinBox();
    m_valeur->setValue(valeur);
    m_valeur->setRange(-999999, 999999);
    mainLayout->addWidget(m_valeur, Qt::AlignRight);

    // Connection signeau slote
    QObject::connect(m_valeur, SIGNAL(valueChanged(int)),
        this, SIGNAL(value_change(int)));
}

void item_init::set_init(int value)
{
    m_valeur->setValue(value);
}

int item_init::get_init()
{
    return m_valeur->value();
}
