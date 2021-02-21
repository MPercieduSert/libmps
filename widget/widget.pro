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
    AbstractentitySelectWidget.cpp \
    #BarPlotter.cpp \
    Checklist.cpp \
    #EditTreeWidget.cpp \
    FindNoeudWidget.cpp \
    #Histogramme.cpp \
    IdComboBox.cpp \
    #mot_cleDelegate.cpp \
    #mot_cleModel.cpp \
    #mot_cleWidget.cpp \
    NewModifDialog.cpp \
    NewModifPredef.cpp \
    #Newmot_cleDialog.cpp \
    #Plotter.cpp \
    #ReadTreeWidget.cpp \
    #SelectdonneeDialog.cpp \
    SelectInListBox.cpp \
    SpinBox.cpp \
    TableentityView.cpp \
    #TreeModelEditentity.cpp \
    #TreeModelmot_cle.cpp \
    #TreeModelReadentity.cpp \
    #TreeModelRead_temp.cpp \
    #TreeModel_temp.cpp \
    TreeWidget.cpp \
    NodeView.cpp \
    FindWidget.cpp \
    StandardNodeWidget.cpp \
    CodeWidget.cpp \
    dialog_predef.cpp

HEADERS += \
    AbstractentitySelectWidget.h \
    #BarPlotter.h \
    Checklist.h \
    #EditTreeWidget.h \
    FindNoeudWidget.h \
    #Histogramme.h \
    IdComboBox.h \
    #mot_cleDelegate.h \
    #mot_cleModel.h \
    #mot_cleWidget.h \
    NewModifDialog.h \
    NewModifPredef.h \
    #Newmot_cleDialog.h \
    #Plotter.h \
    #ReadTreeWidget.h \
    #SelectdonneeDialog.h \
    SelectInListBox.h \
    SpinBox.h \
    TableentityView.h \
    #TreeModelEditentity.h \
    #TreeModelmot_cle.h \
    #TreeModelReadentity.h \
    #TreeModelRead_temp.h \
    #TreeModel_temp.h \
    TreeWidget.h \
    NodeView.h \
    FindWidget.h \
    StandardNodeWidget.h \
    CodeWidget.h \
    dialog_predef.h

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
