#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:18:28
#
#-------------------------------------------------

QT       += sql xml

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
    abstract_manager.cpp \
    info_bdd.cpp \
    unique_sql_base.cpp \
    manager_arbre.cpp \
    manager_arbre_modif_controle.cpp \
    req_sql.cpp \
    abstract_link_sql.cpp \
    abstract_unique_sql.cpp \
    manager_sql.cpp \
    abstract_gestion_restriction.cpp \
    manager_permission.cpp \
    gestion_restriction_cible.cpp \
    abstract_manager_arbre.cpp \
    manager_for_arbre.cpp \
    manager_modif_controle.cpp \
    manager_permission_modif_controle.cpp \
    manager_arbre_simple.cpp \
    manager_arbre_simple_modif_controle.cpp \
    managers_base.cpp \
    managers_predef.cpp \
    bdd.cpp \
    bdd_predef.cpp

HEADERS += \
    abstract_manager.h \
    info_bdd.h \
    unique_sql_base.h \
    manager_arbre.h \
    manager_arbre_modif_controle.h \
    req_sql.h \
    abstract_link_sql.h \
    abstract_unique_sql.h \
    manager_sql.h \
    abstract_gestion_restriction.h \
    manager_permission.h \
    gestion_restriction_cible.h \
    abstract_manager_arbre.h \
    manager_for_arbre.h \
    manager_modif_controle.h \
    manager_permission_modif_controle.h \
    manager_arbre_simple.h \
    manager_arbre_simple_modif_controle.h \
    managers_base.h \
    managers_predef.h \
    bdd.h \
    bdd_predef.h
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
