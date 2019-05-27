#ifndef MACRO_H
#define MACRO_H

#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>

// Comparer un QString et un char*.
#define CPR_QSTRING(__qstring, __const_char)\
    (__qstring == QLatin1String(__const_char))

// Afficher une boite de dialog servant de débug.
#define DEBUG_MSG(__msg)\
    QMessageBox::information(NULL, "Debug", __msg, QMessageBox::Ok);

// Separateurs
#define V_SEPARATOR(__layout, ...) { \
    QFrame* line = new QFrame(); \
    line->setFrameShape(QFrame::VLine); \
    line->setFrameShadow(QFrame::Raised); \
    __layout->addWidget(line, __VA_ARGS__); \
}
#define H_SEPARATOR(__layout, ...) { \
    QFrame* line = new QFrame(); \
    line->setFrameShape(QFrame::HLine); \
    line->setFrameShadow(QFrame::Raised); \
    __layout->addWidget(line, __VA_ARGS__); \
}

// Ajouter un layout comme un widget
#define ADD_LAYOUT(__func, __layout) { \
    QWidget *w = new QWidget(); \
    w->setLayout(__layout); \
    __func(w); \
}

// Getteur / Setteur
#define GET(__type, __varname) \
    __type get##__varname() {return __varname;}

#define SET(__type, __varname) \
    void set##__varname(__type _var_) {__varname=_var_;}

#define GET_SET(__type, __varname)\
    GET(__type, __varname)\
    SET(__type, __varname)

// TOOLTIPS
// ASSISTANT
#define TT_ADD_ENNEMI       "Ajouter un PNJ"
#define TT_RESET_PNJ        "Supprimer tout les PNJ"
#define TT_RESET_LIFE       "Supprimer la vie tout les PNJ"
#define TT_RESET_PLAYERS    "Supprimer tout les joueur"
#define TT_ROLL_INIT        "Lancer un nouveau tour d'initiative"
#define TT_SAVE_PNJ         "Sauvegarder une page de PNJ"
#define TT_DELETE_PNJ       "Supprimer une page de PNJ"
#define TT_LOAD_PNJ         "Importer une page de PNJ"
#define TT_PLAYER_NAME      "Nom de votre victime"
#define TT_PLAYER_INIT      "Initiative de votre victime"
#define TT_PNJ_NAME         "Nom du PNJ qui va s'occuper de vos joueurs"
#define TT_PNJ_INIT         "Initiative du PNJ"
#define TT_PNJ_LIFE_SUB     "Soustraire des points de vie au PNJ.\nPour valider la modification, appuierez sur le compteur de PV."
#define TT_PNJ_LIFE_ADD     "Ajouter des points de vie au PNJ.\nPour valider la modification, appuierez sur le compteur de PV."
#define TT_PNJ_LIFE_COUNT   "Compteur de point de vie de votre créature"
#define TT_DES_LANCER       "Lancer les dès"
#define TT_DES_RESULT       "Résultats des dès"
#define TT_DES_SAISIE       "Lancer des dès. Par exemple:\nd6+10: lance un dès à six faces et y ajouter 10\n2m4d8: lancer 4 dès à 8 face et garde les 2 meilleurs\n"
#define TT_DOT              "Dégat sur le temps : \nces dégats sont infligé à chaque nouveau tour."
#define TT_HOT              "Soin sur le temps : \nces soins sont infligé à chaque nouveau tour."
#define TT_FREE_NOTE        "Zone de saisie libre pour vos créatures."

// REGEXP
#define VALID_DICE "[0-9dDmM+-]{1,100}"
#define VALID_TEXT "[^#^;]{1,25}"

class MFocusable{public:virtual void focused()=0;};

//! Macro générant des classes propageant le signal de focus vers leurs parents.
//! le paramètre doit être un objet héritant de QWidget sans le Q initial dans son nom.
//! L'obet résultant est nommé comme l'objet QT sinon que le T est remplacé par un M.
#define FOCUS_WIDGET(__qwidget) \
class M##__qwidget : public Q##__qwidget, public MFocusable { \
    protected: \
        void focusInEvent(QFocusEvent* event) override { \
            QWidget::focusInEvent(event); \
            MFocusable* mw=dynamic_cast<MFocusable*>(this->parentWidget());\
            if(mw) mw->focused(); \
        } \
        virtual void focused() override { \
            MFocusable* mw=dynamic_cast<MFocusable*>(this->parentWidget());\
            if(mw) mw->focused(); \
        } \
};

//! Définition d'objet Qt couramment utilisé.
FOCUS_WIDGET(Widget)        // MWidget
FOCUS_WIDGET(LineEdit)      // MLineEdit
FOCUS_WIDGET(TextEdit)      // MTextEdit
FOCUS_WIDGET(PushButton)    // MPushButton
FOCUS_WIDGET(SpinBox)       // MSpinBox

#endif // MACRO_H
