#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:24:26
#
#-------------------------------------------------

QT       += sql widgets

TARGET = widget
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
    #BarPlotter.cpp \
    #Edittree_widget.cpp \
    #FindNoeudWidget.cpp \
    #Histogramme.cpp \
    #mot_cleDelegate.cpp \
    #mot_cleModel.cpp \
    #mot_cleWidget.cpp \
    #Newmot_cleDialog.cpp \
    #Plotter.cpp \
    #Readtree_widget.cpp \
    #SelectdonneeDialog.cpp \
    #TreeModelEditent.cpp \
    #TreeModelmot_cle.cpp \
    #TreeModelReadent.cpp \
    #TreeModelRead_temp.cpp \
    #TreeModel_temp.cpp \
    dialog_predef.cpp \
    id_combo_box.cpp \
    abstract_entity_select_widget.cpp \
    check_list.cpp \
    code_widget.cpp \
    spin_box.cpp \
    node_view.cpp \
    standard_node_widget.cpp \
    find_widget.cpp \
    select_in_list_box.cpp \
    table_entity_view.cpp \
    new_modif_dialog.cpp \
    tree_widget.cpp \
    new_modif_predef.cpp

HEADERS += \
    #BarPlotter.h \
    #Edittree_widget.h \
    #FindNoeudWidget.h \
    #Histogramme.h \
    #mot_cleDelegate.h \
    #mot_cleModel.h \
    #mot_cleWidget.h \
    #Newmot_cleDialog.h \
    #Plotter.h \
    #Readtree_widget.h \
    #SelectdonneeDialog.h \
    #TreeModelEditentity.h \
    #TreeModelmot_cle.h \
    #TreeModelReadentity.h \
    #TreeModelRead_temp.h \
    #TreeModel_temp.h \
    dialog_predef.h \
    id_combo_box.h \
    abstract_entity_select_widget.h \
    check_list.h \
    code_widget.h \
    spin_box.h \
    node_view.h \
    standard_node_widget.h \
    find_widget.h \
    select_in_list_box.h \
    table_entity_view.h \
    new_modif_dialog.h \
    tree_widget.h \
    new_modif_predef.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    widget.pro

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
