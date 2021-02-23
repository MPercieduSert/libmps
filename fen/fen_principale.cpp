#include "fen_principale.h"

using namespace mps;
using namespace fenetre;

fen_principale::fen_principale(abstract_noyau *noyau, std::unique_ptr<b2d::bdd> &&bdd, abstract_zone_centrale *zoneCentrale,
                             const QString &bdd_path_xml, const QString &config_path, QWidget *parent) :
    QMainWindow(parent), m_zone_centrale(zoneCentrale), m_noyau(noyau) {
    m_noyau->setParent(this);
    m_noyau->set_config_by_path(config_path,this);
    m_noyau->set_bdd(std::move(bdd),bdd_path_xml,this);

    //zoneCentrale->init(this);
    setCentralWidget(zoneCentrale);
    //! + Création des Menus.
    // et toolbar.
    create_action();
    create_menu();
    //create_tool_bar();
}

void fen_principale::create_action() {
    // Chercher
    m_chercher_action = new QAction(tr("Chercher"),this);
    m_chercher_action->setShortcut(QKeySequence::Find);
    //connect(m_chercher_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::chercher);

    // Coller
    m_coller_action = new QAction(tr("Coller"),this);
    m_coller_action->setShortcut(QKeySequence::Paste);
    connect(m_coller_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::coller);

    // Copier
    m_copier_action = new QAction(tr("Copier"),this);
    m_copier_action->setShortcut(QKeySequence::Copy);
    connect(m_copier_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::copier);

    // Couper
    m_couper_action = new QAction(tr("Couper"),this);
    m_couper_action->setShortcut(QKeySequence::Cut);
    connect(m_couper_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::couper);

    // Effacer
    m_effacer_action = new QAction(tr("Effacer"),this);
    m_effacer_action->setShortcut(QKeySequence::Delete);
    connect(m_effacer_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::effacer);

    // Sauver
    m_sauver_action = new QAction(tr("Sauvegarder"),this);
    m_sauver_action->setShortcut(QKeySequence::Save);
    connect(m_sauver_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::sauver);

    // Supprimer
    m_supprimer_action = new QAction(tr("Supprimer"),this);
    //m_supprimer_action->setShortcut(QKeySequence::);
    connect(m_supprimer_action,&QAction::triggered,m_zone_centrale,&abstract_zone_centrale::supprimer);

    set_action(No_Action);
    connect(m_zone_centrale,&abstract_zone_centrale::action_permise,this,&fen_principale::set_action);
}

void fen_principale::create_menu() {
    m_fichier_menu = menuBar()->addMenu(tr("&Fichier"));
    //m_menuNew = m_menuFichier->addMenu(tr("&Nouveau"));
    m_fichier_menu->addAction(m_sauver_action );

    m_edit_menu = menuBar()->addMenu(tr("&Edit"));
    m_edit_menu->addAction(m_coller_action);
    m_edit_menu->addAction(m_copier_action);
    m_edit_menu->addAction(m_couper_action);
    m_edit_menu->addAction(m_effacer_action);
    m_edit_menu->addAction(m_supprimer_action);
    m_edit_menu->addSeparator();
    m_edit_menu->addAction(m_chercher_action);
    m_new_modif_menu = menuBar()->addMenu(tr("&Créer / Modifier"));
}

void fen_principale::create_tool_bar()
    {m_tool_bar = addToolBar(tr("Module"));}


void fen_principale::set_action(flag action) {
    if(!action) {
        m_copier_action->setEnabled(false);
        m_coller_action->setEnabled(false);
        m_couper_action->setEnabled(false);
        m_effacer_action->setEnabled(false);
        m_sauver_action->setEnabled(false);
        m_chercher_action->setEnabled(false);
    }
    else {
        m_copier_action->setEnabled(action.test(Copier_Action));
        m_coller_action->setEnabled(action.test(Coller_Action));
        m_couper_action->setEnabled(action.test(Couper_Action));
        m_effacer_action->setEnabled(action.test(Effacer_Action));
        m_sauver_action->setEnabled(action.test(Sauver_Action));
        m_supprimer_action->setEnabled(action.test(Supprimer_Action));
        m_chercher_action->setEnabled(action.test(Chercher_action));
    }
}
