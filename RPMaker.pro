QT += \
    widgets \
    core

HEADERS += \
    ihm/item/perso_info.h \
    ihm/item/player_cell.h \
    tools/config.h \
    tools/codexmanager.h \
    tools/des.h \
    tools/macro.h \
    tools/urgparsor.h \
    ihm/item/item_init.h \
    ihm/item/init_area.h \
    ihm/mot.h \
    ihm/divers.h \
    ihm/assistant.h \
    ihm/fenetre.h \
    ihm/item.h \
    ihm/item/monster_cell.h \
    ihm/parametre.h \
    model/perso.h \
    model/player.h \
    model/pnj.h \
    tools/mmatrix.h \
    tools/donjongenerator.h

SOURCES += \
    ihm/item/perso_info.cpp \
    ihm/item/player_cell.cpp \
    main.cpp \
    ihm/item/item_init.cpp \
    ihm/item/init_area.cpp \
    ihm/fenetre.cpp \
    ihm/mot.cpp \
    ihm/divers.cpp \
    ihm/parametre.cpp \
    ihm/assistant.cpp \
    ihm/item.cpp \
    ihm/item/monster_cell.cpp \
    tools/config.cpp \
    tools/codexmanager.cpp \
    tools/des.cpp \
    tools/urgparsor.cpp \
    tools/donjongenerator.cpp

CONFIG += c++14

RC_FILE += icon.rc

RESOURCES += res.qrc
