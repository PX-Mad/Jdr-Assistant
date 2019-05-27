#include "monster_cell.h"
#include "perso_info.h"

#define MIN_HEIGHT  20
#define MOVE_FACTOR 0.8

// Construit l'onglet de génération "Non-trié"
monster_cell::monster_cell(bool bConnected)
{
    QVBoxLayout *layoutV;
    QHBoxLayout *layoutH1, *layoutH2, *layoutH3;
    QRegExpValidator *validatorDi = new QRegExpValidator(QRegExp(VALID_TEXT),nullptr);
    QRegExpValidator *validatorSe = new QRegExpValidator(QRegExp(VALID_DICE),nullptr);

    layoutV = new QVBoxLayout;
    layoutH1 = new QHBoxLayout;
    layoutH2 = new QHBoxLayout;
    layoutH3 = new QHBoxLayout;

    layoutV->setSpacing(3);

    // Première ligne
    _leName.setParent(this);
    _leName.setValidator(validatorDi);
    _leName.setToolTip(TT_PNJ_NAME);
    _leName.setMinimumHeight(MIN_HEIGHT);
    _init.setParent(this);
    _init.setRange(-999, 999);
    _init.setToolTip(TT_PNJ_INIT);
    _init.setMinimumHeight(MIN_HEIGHT);

    layoutH1->addWidget(&_leName);
    layoutH1->addWidget(&_init);
    layoutV->addLayout(layoutH1);

    // Seconde ligne
    _leMoins.setParent(this);
    _leMoins.setValidator(validatorSe);
    _leMoins.setPlaceholderText("-");
    _leMoins.setAlignment(Qt::AlignHCenter);
    _leMoins.setToolTip(TT_PNJ_LIFE_SUB);
    _leMoins.setMinimumHeight(MIN_HEIGHT);
    _lePlus.setParent(this);
    _lePlus.setValidator(validatorSe);
    _lePlus.setPlaceholderText("+");
    _lePlus.setAlignment(Qt::AlignHCenter);
    _lePlus.setToolTip(TT_PNJ_LIFE_ADD);
    _lePlus.setMinimumHeight(MIN_HEIGHT);
    _vie.setParent(this);
    _vie.setText("0");
    _vie.setToolTip(TT_PNJ_LIFE_COUNT);
    _vie.setMinimumHeight(MIN_HEIGHT);

    layoutH2->addWidget(&_leMoins);
    layoutH2->addWidget(&_vie);
    layoutH2->addWidget(&_lePlus);
    layoutV->addLayout(layoutH2);

    // Troisième ligne
    _leDot.setParent(this);
    _leDot.setValidator(validatorSe);
    _leDot.setPlaceholderText("DoT");
    _leDot.setAlignment(Qt::AlignHCenter);
    _leDot.setToolTip(TT_DOT);
    _leDot.setMinimumHeight(MIN_HEIGHT);
    _leHot.setParent(this);
    _leHot.setValidator(validatorSe);
    _leHot.setPlaceholderText("HoT");
    _leHot.setAlignment(Qt::AlignHCenter);
    _leHot.setToolTip(TT_HOT);
    _leHot.setMinimumHeight(MIN_HEIGHT);

    layoutH3->addWidget(&_leDot);
    layoutH3->addWidget(&_leHot);
    layoutV->addLayout(layoutH3);

    // Quatrième ligne
    _teFreeNote.setParent(this);
    _teFreeNote.setToolTip(TT_FREE_NOTE);
    _teFreeNote.setMinimumHeight(MIN_HEIGHT);
    layoutV->addWidget(&_teFreeNote);

    _iVie=0;
    _nLine=2;
    _bConnected=bConnected;

    //! On fait en sorte que cette cellule soit connecté aux autres.
    if(_bConnected==true) {
        monster_cell_manager *mcm=MONSTER_MANAGER;
        mcm->addItem(this);

        //! SLOTES
        QObject::connect(&_leMoins, SIGNAL(clicked()), mcm, SLOT(calculerVie()));
        QObject::connect(&_vie, SIGNAL(clicked()), mcm, SLOT(calculerVie()));
        QObject::connect(&_lePlus, SIGNAL(clicked()), mcm, SLOT(calculerVie()));
        QObject::connect(&_leName, SIGNAL(editingFinished()), mcm, SLOT(numeroterPNJ()));

        //! Connection des slots
        #define CONNECT_FOCUS(__item) \
            QObject::connect(__item, SIGNAL(clicked()), this, SLOT(focused()));
        CONNECT_FOCUS(&_leName);
        CONNECT_FOCUS(&_leMoins);
        CONNECT_FOCUS(&_lePlus);
        CONNECT_FOCUS(&_leHot);
        CONNECT_FOCUS(&_leDot);
        CONNECT_FOCUS(&_teFreeNote);
        CONNECT_FOCUS(&_vie);
        CONNECT_FOCUS(&_init);


        this->setFocusPolicy(Qt::StrongFocus);
    } else {
        QObject::connect(&_vie, SIGNAL(clicked()), this, SLOT(calculerVie()));
    }

    //! Émission des signaux de changements
    QObject::connect(&_leName, SIGNAL(textChanged(String)), this, SIGNAL(nom_change(String)));
    QObject::connect(&_teFreeNote, SIGNAL(textChanged(String)), this, SIGNAL(free_note_change(String)));
    QObject::connect(&_leMoins, SIGNAL(textChanged(String)), this, SIGNAL(vie_moins_change(String)));
    QObject::connect(&_lePlus, SIGNAL(textChanged(String)), this, SIGNAL(vie_plus_change(String)));
    QObject::connect(&_leDot, SIGNAL(textChanged(String)), this, SIGNAL(dot_change(String)));
    QObject::connect(&_leHot, SIGNAL(textChanged(String)), this, SIGNAL(hot_change(String)));
    QObject::connect(&_init, SIGNAL(valueChanged(String)), this, SIGNAL(init_change(int)));
    QObject::connect(&_vie, SIGNAL(clicked()), this, SIGNAL(vie_change()));

    this->setLayout(layoutV);
    resize(this->size());
}

void monster_cell::connect(monster_cell *pMonster)
{
    // créer une signal custom pour _teFreeNote, setHtml...
    //! Connection de this vers l'autre
    QObject::connect(&_leName, SIGNAL(textChanged(QString)), pMonster, SLOT(set_nom(QString)));
    QObject::connect(&_leMoins, SIGNAL(textChanged(QString)), pMonster, SLOT(set_vie_moins(QString)));
    QObject::connect(&_lePlus, SIGNAL(textChanged(QString)), pMonster, SLOT(set_vie_plus(QString)));
    QObject::connect(&_leHot, SIGNAL(textChanged(QString)), pMonster, SLOT(set_dot(QString)));
    QObject::connect(&_leDot, SIGNAL(textChanged(QString)), pMonster, SLOT(set_hot(QString)));
    QObject::connect(&_teFreeNote, SIGNAL(setHtml(QString)), pMonster, SLOT(set_free_note(QString)));
    QObject::connect(&_init, SIGNAL(valueChanged(int)), pMonster, SLOT(set_inti(int)));
    QObject::connect(&_vie, SIGNAL(clicked()), pMonster, SLOT(calculerVie()));

    //! Connection de l'autre vers this
    QObject::connect(pMonster, SIGNAL(nom_change(QString)), &_leName, SLOT(setText(QString)));
    QObject::connect(pMonster, SIGNAL(vie_moins_change(QString)), &_leMoins, SLOT(setText(QString)));
    QObject::connect(pMonster, SIGNAL(vie_plus_change(QString)), &_lePlus, SLOT(setText(QString)));
    QObject::connect(pMonster, SIGNAL(hot_change(QString)), &_leHot, SLOT(setText(QString)));
    QObject::connect(pMonster, SIGNAL(dot_change(QString)), &_leDot, SLOT(setText(QString)));
    QObject::connect(pMonster, SIGNAL(free_note_change(QString)), &_teFreeNote, SLOT(setHtml(QString)));
    QObject::connect(pMonster, SIGNAL(init_change(int)), &_init, SLOT(set_inti(int)));
    QObject::connect(pMonster, SIGNAL(vie_change()), &_vie, SLOT(calculerVie()));
}

void monster_cell::disconnect(monster_cell *pMonster)
{
    //! Déconnection de this vers l'autre
    QObject::disconnect(&_leName, SIGNAL(textChanged(QString)), pMonster, SLOT(set_nom(QString)));
    QObject::disconnect(&_leMoins, SIGNAL(textChanged(QString)), pMonster, SLOT(set_vie_moins(QString)));
    QObject::disconnect(&_lePlus, SIGNAL(textChanged(QString)), pMonster, SLOT(set_vie_plus(QString)));
    QObject::disconnect(&_leHot, SIGNAL(textChanged(QString)), pMonster, SLOT(set_dot(QString)));
    QObject::disconnect(&_leDot, SIGNAL(textChanged(QString)), pMonster, SLOT(set_hot(QString)));
    QObject::disconnect(&_teFreeNote, SIGNAL(setHtml(QString)), pMonster, SLOT(set_free_note(QString)));
    QObject::disconnect(&_init, SIGNAL(valueChanged(int)), pMonster, SLOT(set_inti(int)));
    QObject::disconnect(&_vie, SIGNAL(clicked()), pMonster, SLOT(calculerVie()));

    //! Déconnection de l'autre vers this
    QObject::disconnect(pMonster, SIGNAL(nom_change(QString)), &_leName, SLOT(setText(QString)));
    QObject::disconnect(pMonster, SIGNAL(vie_moins_change(QString)), &_leMoins, SLOT(setText(QString)));
    QObject::disconnect(pMonster, SIGNAL(vie_plus_change(QString)), &_lePlus, SLOT(setText(QString)));
    QObject::disconnect(pMonster, SIGNAL(hot_change(QString)), &_leHot, SLOT(setText(QString)));
    QObject::disconnect(pMonster, SIGNAL(dot_change(QString)), &_leDot, SLOT(setText(QString)));
    QObject::disconnect(pMonster, SIGNAL(free_note_change(QString)), &_teFreeNote, SLOT(setHtml(QString)));
    QObject::disconnect(pMonster, SIGNAL(init_change(int)), &_init, SLOT(set_inti(int)));
    QObject::disconnect(pMonster, SIGNAL(vie_change()), &_vie, SLOT(calculerVie()));
}

void monster_cell::focusInEvent(QFocusEvent *event) {
    Q_UNUSED(event);
    focused();
}

void monster_cell::focused() {
    if(_bConnected==true) {
        PERSO_INFO->setMonster(this);
    }
}

//Calucul la vie de tous les PNJ
void monster_cell::calculerVie()
{
    set_vie(_iVie + Des::lancerDes(_leMoins.text()) - Des::lancerDes(_lePlus.text()));
    _leMoins.clear();
    _lePlus.clear();
}

void monster_cell::reset()
{
    _iVie=0;
    _vie.setText("0");
    _leName.clear();
    _init.setValue(0);
    _leMoins.clear();
    _lePlus.clear();
}

void monster_cell::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    resize(event->size());
}

#define SET_VISIBLE(__life, __dot_hot, __free_note) \
    _leMoins.setVisible(__life); \
    _lePlus.setVisible(__life); \
    _vie.setVisible(__life); \
    _leDot.setVisible(__dot_hot); \
    _leHot.setVisible(__dot_hot); \
    _teFreeNote.setVisible(__free_note)

void monster_cell::resize(QSize size)
{
    int nline = (int)(size.height()*MOVE_FACTOR)/MIN_HEIGHT;
    if(nline==0) nline=1;
    if(nline!=_nLine)
    {
        _nLine=nline;
        switch(nline)
        {
            case 1:
                SET_VISIBLE(false, false, false);
                break;
            case 2:
                SET_VISIBLE(true, false, false);
                break;
            case 3:
                SET_VISIBLE(true, true, false);
                break;
            default:
                SET_VISIBLE(true, true, true);
        }
    }
}
#undef SET_VISIBLE

//! MANAGER
void monster_cell_manager::calculerVie()
{
    foreach(monster_cell* item, _items)
        item->calculerVie();
}

void monster_cell_manager::numeroterPNJ()
{
    if(CONFIG.getParam("numAuto").toInt()!=1)  return;
    QString tmp;
    bool num=false, ok, statAuto=(CONFIG.getParam("statAuto").toInt()==1);
    unsigned int ce=2;

    // Supprimer la numérotation
    foreach(monster_cell* item, _items)
        if(item->get_nom()!=nullptr)
            while(true)
            {
                tmp=item->get_nom().right(1);
                tmp.toInt(&ok, 10);
                if(ok==true || tmp==" ")
                {
                    tmp=item->get_nom();
                    tmp.chop(1);
                    item->set_nom(tmp);
                }
                else break;
            }

    // Numérote les PNJ
    foreach(monster_cell* item, _items)
    {
        if(item->get_nom()!=nullptr)
            foreach(monster_cell* item2, _items)
                if(item!=item2 && item->get_nom()==item2->get_nom())
                {
                    item2->set_nom(item2->get_nom()+" "+QString::number(ce));
                    if(statAuto)
                    {
                        item2->set_vie_moins(item->get_vie_moins());
                        item2->set_vie_plus(item->get_vie_plus());
                        item2->set_init(item->get_init());
                    }
                    ce++;
                    num=true;
                }
        if(num==true)  item->set_nom(item->get_nom()+" 1");
        num=false;
        ce=2;
    }
}
