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
    AbstractNoyau.cpp \
    AbstractTab.cpp \
    AbstractTabGestionBdd.cpp \
    AbstractTabMenu.cpp \
    AbstractTabModule.cpp \
    AbstractTabModuleWithBdd.cpp \
    AbstractTabTableau.cpp \
    AbstractTabZoneCentrale.cpp \
    AbstractZoneCentrale.cpp \
    Fen.cpp \
    FenFoundFile.cpp \
    FenPrincipale.cpp \
    PredefTab.cpp \
    Tabmot_cle.cpp \
    TabGestionBdd.cpp \
    PredefZoneCentrale.cpp \
    PredefFenPrincipale.cpp \
    FenFlags.cpp \
    TabGestionType.cpp

HEADERS += \
    AbstractNoyau.h \
    AbstractTab.h \
    AbstractTabGestionBdd.h \
    AbstractTabMenu.h \
    AbstractTabModule.h \
    AbstractTabModuleWithBdd.h \
    AbstractTabTableau.h \
    AbstractTabZoneCentrale.h \
    AbstractZoneCentrale.h \
    Fen.h \
    FenFoundFile.h \
    FenPrincipale.h \
    PredefTab.h \
    Tabmot_cle.h \
    TabGestionBdd.h \
    PredefZoneCentrale.h \
    PredefFenPrincipale.h \
    FenFlags.h \
    TabGestionType.h
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
