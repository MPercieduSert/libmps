/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/12/2019
 */
#ifndef TAB_GESTION_BDD_H
#define TAB_GESTION_BDD_H

#include <QListWidget>
#include <QPushButton>
#include <QSqlTableModel>
#include <QTableView>
#include "abstract_tab_module.h"
#include "bdd_predef.h"
#include "code_widget.h"
#include "predef_model.h"
#include "standard_node_widget.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Classe des onglets de gestion des tables de la base de donnée.
 */
class tab_gestion_bdd : public abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    manager::info_bdd m_info;                   //!< Information sur la table.
    std::vector<QString> m_attribut_names;      //!< Noms des attributs.
    QSqlTableModel *m_model;                    //!< Model.
    QTableView *m_view;                         //!< Vue.
    QPushButton *m_insert_bouton;               //!< Bouton d'insertion d'une ligne.
    QPushButton *m_refresh_bouton;              //!< Bouton rafraichisant l'affichage.
    QPushButton *m_save_bouton;                 //!< Bouton de sauvegarde.
    QPushButton *m_suppr_bouton;                //!< Bouton de suppression d'une entité.
    QVBoxLayout *m_main_layout;                 //!< Calque principal.

public:
    tab_gestion_bdd(b2d::bdd &bdd, const tab_index &index, QWidget *parent = nullptr);

    //! Accesseur du titre.
    QString title() const
        {return QString("Table : ").append(m_info.name());}
};

/*! \ingroup groupe_fen
 *\brief Classe de l'onglets de gestion des types.
 */
class tab_gestion_type : public abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    model_base::type_permission_model *m_model;    //!< Model.
    widget::node_view *m_view;               //!< Vue.
    QListWidget *m_cible_list_widget;            //!< Sélection des cibles.
    QPushButton *m_save_bouton;                 //!< Bouton de sauvegarde.

    // Calque
    QVBoxLayout *m_cible_layout;                //!< Calque des cibles.
    QHBoxLayout *m_main_layout;                 //!< Calque principal.

public:
    //! position des cases.
    enum position_case {Visible,
                      Attribuable,
                      NbrCase};
    //! Constructeur.
    tab_gestion_type(b2d::bdd &bdd, const tab_index &index, QWidget *parent = nullptr);

    //! Accesseur du titre.
    QString title() const override
        {return QString("Gestion des types");}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;

    //! Slot coorepondant à l'action sauver.
    void sauver() override
        {m_model->save();}
};
}}
#endif // TAB_GESTION_BDD_H
