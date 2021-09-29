/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef FEN_PRINCIPALE_H
#define FEN_PRINCIPALE_H

#include <QAction>
#include <QComboBox>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QToolBar>
#include <QVBoxLayout>
#include <stdexcept>
#include "abstract_noyau.h"
#include "abstract_zone_centrale.h"
#include "fen_flags.h"
#include "new_modif_dialog.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Fenêtre principale de l'application.
 *
 * Fenêtre principale:
 * + Crée les différents menus.
 */
class fen_principale : public QMainWindow {
    Q_OBJECT
protected:
    // Action
    QAction *m_chercher_action;  //!< Action chercher.
    QAction *m_coller_action;    //!< Action coller.
    QAction *m_copier_action;    //!< Action copier.
    QAction *m_couper_action;    //!< Action couper.
    QAction *m_effacer_action;   //!< Action effacer.
    QAction *m_sauver_action;    //!< Action sauvegarder.
    QAction *m_supprimer_action; //!< Action supprimer.

    // Menu
    QMenu *m_edit_menu;              //!< Menu édition.
    QMenu *m_fichier_menu;           //!< Menu Fichier.
    QMenu *m_new_modif_menu;          //!< Création modification.

    // Tools Bar
    QToolBar *m_tool_bar;            //!< Barre d'outils.

    abstract_zone_centrale *m_zone_centrale;    //!< Zone centrale.

    // Noyau
    abstract_noyau *m_noyau;                //!< Noyau de l'application.

public:
    //! Constructeur.
    explicit fen_principale(abstract_noyau *noyau, std::unique_ptr<b2d::bdd> &&bdd, abstract_zone_centrale *central_zone,
                           const QString &bdd_path_xml,
                           const QString &config_path = QDir::currentPath().append("/Config.xml"),
                           QWidget *parent = nullptr);

    //! Destructeur.
    ~fen_principale() override = default;

    //! Accesseur du noyau.
    virtual abstract_noyau *noyau()
        {return m_noyau;}

public slots:
    //! Mutateur des actions.
    void set_action(flag action);

protected:
    //! Crée le menu bdd.
    void add_menu_bdd();

    //! Connecte une QAction à l'ouverture d'un dialogue de création et modification.
    template<class From> void connect_action_to_new_modif_dialog(QAction *action, bool new_ent);

    //! Crée les différente action des menus et toolbars de la fenêtre principale.
    void create_action();

    //! Crée les menus de la fenêtre principale.
    void create_menu();

    //! Crée les toolbars de la fenêtre principale.
    void create_tool_bar();
};

template<class Form> void fen_principale::connect_action_to_new_modif_dialog(QAction *action, bool new_ent) {
    connect(action,&QAction::triggered,this, [this,new_ent](){
                            auto *form = new Form(noyau()->bdd(), new_ent);
                            dialogue::new_modif_dialog diag(form,this);
                            diag.exec();
                        });
    if(!new_ent)
        action->setDisabled(noyau()->bdd().table_empty<typename Form::entity_of_form>());
}
}}
#endif // FEN_PRINCIPALE_H
