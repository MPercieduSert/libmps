#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:10:01
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = entities
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
    Arbre.cpp \
    AttributMultiple.cpp \
    AttributSimple.cpp \
    Config.cpp \
    Entity.cpp \
    EntityCible.cpp \
    EntityDivers.cpp \
    EntityRelation.cpp \
    FileInterface.cpp \
    InfoEntity.cpp \
    ListPtr.cpp \
    MapPtr.cpp \
    Tree.cpp \
    TreeItem.cpp \
    VectorPtr.cpp \
    xmlmps.cpp \
    EntityPredef.cpp #\
    #EntitiesPredef.cpp

HEADERS += \
    Arbre.h \
    AttributMultiple.h \
    AttributSimple.h \
    Config.h \
    Entity.h \
    EntityCible.h \
    EntityDivers.h \
    EntityPredef.h \
    EntityRelation.h \
    FileInterface.h \
    InfoEntity.h \
    ListPtr.h \
    macrolibmps.h \
    MapPtr.h \
    Tree.h \
    TreeItem.h \
    VectorPtr.h \
    xmlmps.h #\
    #EntitiesPredef.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    entities.pro
