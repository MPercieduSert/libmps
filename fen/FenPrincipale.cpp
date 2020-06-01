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
    // Chercher
    m_chercherAction = new QAction(tr("Chercher"),this);
    m_chercherAction->setShortcut(QKeySequence::Find);
    //connect(m_chercherAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::chercher);

    // Coller
    m_collerAction = new QAction(tr("Coller"),this);
    m_collerAction->setShortcut(QKeySequence::Paste);
    connect(m_collerAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::coller);

    // Copier
    m_copierAction = new QAction(tr("Copier"),this);
    m_copierAction->setShortcut(QKeySequence::Copy);
    connect(m_copierAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::copier);

    // Couper
    m_couperAction = new QAction(tr("Couper"),this);
    m_couperAction->setShortcut(QKeySequence::Cut);
    connect(m_couperAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::couper);

    // Effacer
    m_effacerAction = new QAction(tr("Effacer"),this);
    m_effacerAction->setShortcut(QKeySequence::Delete);
    connect(m_effacerAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::effacer);

    // Sauver
    m_sauverAction = new QAction(tr("Sauvegarder"),this);
    m_sauverAction->setShortcut(QKeySequence::Save);
    connect(m_sauverAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::sauver);

    // Supprimer
    m_supprimerAction = new QAction(tr("Supprimer"),this);
    //m_supprimerAction->setShortcut(QKeySequence::);
    connect(m_supprimerAction,&QAction::triggered,m_zoneCentrale,&AbstractZoneCentrale::supprimer);

    setAction(NoActions);
    connect(m_zoneCentrale,&AbstractZoneCentrale::actionPermise,this,&FenPrincipale::setAction);
}

void FenPrincipale::createMenu() {
    m_fichierMenu = menuBar()->addMenu(tr("&Fichier"));
    //m_menuNew = m_menuFichier->addMenu(tr("&Nouveau"));
    m_fichierMenu->addAction(m_sauverAction );

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_collerAction);
    m_editMenu->addAction(m_copierAction);
    m_editMenu->addAction(m_couperAction);
    m_editMenu->addAction(m_effacerAction);
    m_editMenu->addAction(m_supprimerAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_chercherAction);
    m_newModifMenu = menuBar()->addMenu(tr("&Créer / Modifier"));
}

void FenPrincipale::createToolBar()
    {m_toolBar = addToolBar(tr("Module"));}


void FenPrincipale::setAction(flag action) {
    if(!action) {
        m_copierAction->setEnabled(false);
        m_collerAction->setEnabled(false);
        m_couperAction->setEnabled(false);
        m_effacerAction->setEnabled(false);
        m_sauverAction->setEnabled(false);
        m_chercherAction->setEnabled(false);
    }
    else {
        m_copierAction->setEnabled(action.test(CopierAction));
        m_collerAction->setEnabled(action.test(CollerAction));
        m_couperAction->setEnabled(action.test(CouperAction));
        m_effacerAction->setEnabled(action.test(EffacerAction));
        m_sauverAction->setEnabled(action.test(SauverAction));
        m_supprimerAction->setEnabled(action.test(SupprimerAction));
        m_chercherAction->setEnabled(action.test(ChercherAction));
    }
}
