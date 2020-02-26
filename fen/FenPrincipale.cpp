#include "FenPrincipale.h"

using namespace fenMPS;

FenPrincipale::FenPrincipale(AbstractNoyau * noyau, std::unique_ptr<bmps::Bdd> && bdd, AbstractZoneCentrale * zoneCentrale,
                             const QString & bddPathXML, const QString & configPath, QWidget *parent) :
    QMainWindow(parent), m_zoneCentrale(zoneCentrale), m_noyau(noyau) {
    m_noyau->setParent(this);
    m_noyau->setConfigByPath(configPath,this);
    m_noyau->setBdd(std::move(bdd),bddPathXML,this);

    //m_menuNew = new QMenu(this);
    //m_menuNew->setParent(this);
    // Initialise la zone centrale.

    //zoneCentrale->init(this);
    setCentralWidget(zoneCentrale);
    //! + Création des Menus.
    // et toolbar.
    createAction();
    createMenu();
    //createToolBar();
}

void FenPrincipale::createAction() {
    // Coller
    m_actionEditColler = new QAction(tr("Coller"),this);
    m_actionEditColler->setShortcut(QKeySequence::Paste);
    m_actionEditColler->setEnabled(false);
    connect(m_actionEditColler,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::coller);
    connect(m_noyau,&AbstractNoyau::collerPermis,m_actionEditColler,&QAction::setEnabled);

    // Copier
    m_actionEditCopier = new QAction(tr("Copier"),this);
    m_actionEditCopier->setShortcut(QKeySequence::Copy);
    m_actionEditCopier->setEnabled(false);
    connect(m_actionEditCopier,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::copier);
    connect(m_noyau,&AbstractNoyau::copierPermis,m_actionEditCopier,&QAction::setEnabled);

    // Couper
    m_actionEditCouper = new QAction(tr("Couper"),this);
    m_actionEditCouper->setShortcut(QKeySequence::Cut);
    m_actionEditCouper->setEnabled(false);
    connect(m_actionEditCouper,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::couper);
    connect(m_noyau,&AbstractNoyau::couperPermis,m_actionEditCouper,&QAction::setEnabled);

    // Effacer
    m_actionEditEffacer = new QAction(tr("Effacer"),this);
    m_actionEditEffacer->setShortcut(QKeySequence::Delete);
    m_actionEditEffacer->setEnabled(false);
    connect(m_actionEditEffacer,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::effacer);
    connect(m_noyau,&AbstractNoyau::effacerPermis,m_actionEditEffacer,&QAction::setEnabled);

    // Save
    m_actionSave = new QAction(tr("Sauvegarder"),this);
    m_actionSave->setShortcut(QKeySequence::Save);
    m_actionSave->setEnabled(false);
    connect(m_actionSave,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::save);
    connect(m_noyau,&AbstractNoyau::savePermis,m_actionSave,&QAction::setEnabled);
}

void FenPrincipale::createMenu() {
    m_menuFichier = menuBar()->addMenu(tr("&Fichier"));
    //m_menuNew = m_menuFichier->addMenu(tr("&Nouveau"));
    m_menuFichier->addAction(m_actionSave );

    m_menuEdit = menuBar()->addMenu("&Edit");
    m_menuEdit->addAction(m_actionEditColler);
    m_menuEdit->addAction(m_actionEditCopier);
    m_menuEdit->addAction(m_actionEditCouper);
    m_menuEdit->addAction(m_actionEditEffacer);
}

void FenPrincipale::createToolBar()
    {m_toolBar = addToolBar(tr("Module"));}
