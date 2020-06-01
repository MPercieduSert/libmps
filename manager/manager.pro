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
    AbstractLinkSql.cpp \
    AbstractManager.cpp \
    AbstractUniqueSql.cpp \
    Bdd.cpp \
    BddPredef.cpp \
    InfoBdd.cpp \
    ManagerForArbre.cpp \
    Managers.cpp \
    ManagersPredef.cpp \
    ManagerSql.cpp \
    ReqSql.cpp \
    UniqueSqlBase.cpp \
    ManagerArbre.cpp \
    ManagerArbreModifControle.cpp \
    ManagerArbreSimple.cpp \
    ManagerArbreSimpleModifControle.cpp \
    ManagerModifControle.cpp \
    ManagerPermission.cpp \
    ManagerPermissionModifControle.cpp \
    AbstractManagerArbre.cpp \
    AbstractGestionRestriction.cpp \
    GestionRestrictionCible.cpp

HEADERS += \
    AbstractLinkSql.h \
    AbstractManager.h \
    AbstractUniqueSql.h \
    Bdd.h \
    BddPredef.h \
    InfoBdd.h \
    ManagerForArbre.h \
    Managers.h \
    ManagersPredef.h \
    ManagerSql.h \
    ReqSql.h \
    UniqueSqlBase.h \
    ManagerArbre.h \
    ManagerArbreModifControle.h \
    ManagerArbreSimple.h \
    ManagerArbreSimpleModifControle.h \
    ManagerModifControle.h \
    ManagerPermission.h \
    ManagerPermissionModifControle.h \
    AbstractManagerArbre.h \
    AbstractGestionRestriction.h \
    GestionRestrictionCible.h
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
