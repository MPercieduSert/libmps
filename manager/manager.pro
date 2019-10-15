#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:18:28
#
#-------------------------------------------------

QT       += sql xml

QT       -= gui

TARGET = manager
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
    AbstractGestionAutorisation.cpp \
    AbstractLinkSql.cpp \
    AbstractManager.cpp \
    AbstractUniqueSql.cpp \
    Bdd.cpp \
    BddPredef.cpp \
    GestionAutorisationCible.cpp \
    InfoBdd.cpp \
    InfoBddBase.cpp \
    ManagerArbre.cpp \
    ManagerOfArbre.cpp \
    ManagerOfArbreModifControle.cpp \
    ManagerOfArbreSimple.cpp \
    ManagerOfArbreSimpleModifControle.cpp \
    ManagerOfModifControle.cpp \
    ManagerOfModifControlePermission.cpp \
    ManagerOfPermission.cpp \
    Managers.cpp \
    ManagersPredef.cpp \
    ManagerSql.cpp \
    ReqSql.cpp \
    UniqueSqlBase.cpp

HEADERS += \
    AbstractGestionAutorisation.h \
    AbstractLinkSql.h \
    AbstractManager.h \
    AbstractUniqueSql.h \
    Bdd.h \
    BddPredef.h \
    GestionAutorisationCible.h \
    InfoBdd.h \
    InfoBddBase.h \
    ManagerArbre.h \
    ManagerOfArbre.h \
    ManagerOfArbreModifControle.h \
    ManagerOfArbreSimple.h \
    ManagerOfArbreSimpleModifControle.h \
    ManagerOfModifControle.h \
    ManagerOfModifControlePermission.h \
    ManagerOfPermission.h \
    Managers.h \
    ManagersPredef.h \
    ManagerSql.h \
    ReqSql.h \
    UniqueSqlBase.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    manager.pro


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
