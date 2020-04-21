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
    m_collerAction = new QAction(tr("Coller"),this);
    m_collerAction->setShortcut(QKeySequence::Paste);
    m_collerAction->setEnabled(false);
    connect(m_collerAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::coller);
    connect(m_zoneCentrale,&AbstractZoneCentrale::collerPermis,m_collerAction,&QAction::setEnabled);

    // Copier
    m_copierAction = new QAction(tr("Copier"),this);
    m_copierAction->setShortcut(QKeySequence::Copy);
    m_copierAction->setEnabled(false);
    connect(m_copierAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::copier);
    connect(m_zoneCentrale,&AbstractZoneCentrale::copierPermis,m_copierAction,&QAction::setEnabled);

    // Couper
    m_couperAction = new QAction(tr("Couper"),this);
    m_couperAction->setShortcut(QKeySequence::Cut);
    m_couperAction->setEnabled(false);
    connect(m_couperAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::couper);
    connect(m_zoneCentrale,&AbstractZoneCentrale::couperPermis,m_couperAction,&QAction::setEnabled);

    // Effacer
    m_effacerAction = new QAction(tr("Effacer"),this);
    m_effacerAction->setShortcut(QKeySequence::Delete);
    m_effacerAction->setEnabled(false);
    connect(m_effacerAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::effacer);
    connect(m_zoneCentrale,&AbstractZoneCentrale::effacerPermis,m_effacerAction,&QAction::setEnabled);

    // Save
    m_saveAction = new QAction(tr("Sauvegarder"),this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setEnabled(false);
    connect(m_saveAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::save);
    connect(m_zoneCentrale,&AbstractZoneCentrale::savePermis,m_saveAction,&QAction::setEnabled);
}

void FenPrincipale::createMenu() {
    m_fichierMenu = menuBar()->addMenu(tr("&Fichier"));
    //m_menuNew = m_menuFichier->addMenu(tr("&Nouveau"));
    m_fichierMenu->addAction(m_saveAction );

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_collerAction);
    m_editMenu->addAction(m_copierAction);
    m_editMenu->addAction(m_couperAction);
    m_editMenu->addAction(m_effacerAction);

    m_newModifMenu = menuBar()->addMenu(tr("&Créer / Modifier"));
}

void FenPrincipale::createToolBar()
    {m_toolBar = addToolBar(tr("Module"));}
