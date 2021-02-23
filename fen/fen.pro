#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:40:03
#
#-------------------------------------------------

QT       += widgets sql xml

TARGET = fen
TEMPLATE = lib
CONFIG += staticlib c++17 create_prl

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fen_flags.cpp \
    abstract_tab_module.cpp \
    abstract_tab.cpp \
    fen_found_file.cpp \
    abstract_noyau.cpp \
    abstract_tab_gestion_bdd.cpp \
    abstract_tab_menu.cpp \
    abstract_tab_tableau.cpp \
    abstract_zone_centrale.cpp \
    abstract_tab_zone_centrale.cpp \
    fen.cpp \
    fen_principale.cpp \
    predef_fen_principale.cpp \
    tab_mot_cle.cpp \
    tab_gestion_predef.cpp \
    predef_tab.cpp \
    predef_zone_centrale.cpp

HEADERS += \
    fen_flags.h \
    abstract_tab_module.h \
    abstract_tab.h \
    fen_found_file.h \
    abstract_noyau.h \
    abstract_tab_gestion_bdd.h \
    abstract_tab_menu.h \
    abstract_tab_tableau.h \
    abstract_zone_centrale.h \
    abstract_tab_zone_centrale.h \
    fen.h \
    fen_principale.h \
    predef_fen_principale.h \
    tab_mot_cle.h \
    tab_gestion_predef.h \
    predef_tab.h \
    predef_zone_centrale.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    fen.pro

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../entities/release/ -lentities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../entities/debug/ -lentities
else:unix: LIBS += -L$$OUT_PWD/../entities/ -lentities

INCLUDEPATH += $$PWD/../entities
DEPENDPATH += $$PWD/../entities

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/release/libentities.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/debug/libentities.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/release/entities.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../entities/debug/entities.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../entities/libentities.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../manager/release/ -lmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../manager/debug/ -lmanager
else:unix: LIBS += -L$$OUT_PWD/../manager/ -lmanager

INCLUDEPATH += $$PWD/../manager
DEPENDPATH += $$PWD/../manager

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/release/libmanager.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/debug/libmanager.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/release/manager.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../manager/debug/manager.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../manager/libmanager.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../widget/release/ -lwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../widget/debug/ -lwidget
else:unix: LIBS += -L$$OUT_PWD/../widget/ -lwidget

INCLUDEPATH += $$PWD/../widget
DEPENDPATH += $$PWD/../widget

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widget/release/libwidget.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widget/debug/libwidget.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widget/release/widget.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widget/debug/widget.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../widget/libwidget.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../model/release/libmodel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../model/debug/libmodel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../model/release/model.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../model/debug/model.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../model/libmodel.a
