#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:10:01
#
#-------------------------------------------------

QT       += xml

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
    tree.cpp \
    xml.cpp \
    num_to_texte.cpp \
    info_entity.cpp \
    file_interface.cpp \
    config.cpp \
    conteneur_ptr.cpp \
    attribut_simple.cpp \
    attribut_multiple.cpp \
    entity.cpp \
    entity_divers.cpp \
    entity_relation.cpp \
    entity_cible.cpp \
    entity_predef.cpp

HEADERS += \
    type_mps.h \
    tree.h \
    xml.h \
    num_to_texte.h \
    macro_mps.h \
    info_entity.h \
    file_interface.h \
    config.h \
    conteneur_ptr.h \
    attribut_simple.h \
    attribut_multiple.h \
    entity.h \
    entity_divers.h \
    entity_relation.h \
    entity_cible.h \
    entity_predef.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    entities.pro
