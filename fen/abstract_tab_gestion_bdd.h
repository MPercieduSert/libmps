/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef ABSTRACT_TAB_GESTION_BDD_H
#define ABSTRACT_TAB_GESTION_BDD_H

#include <QListWidget>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSplitter>
#include <QStackedWidget>
#include <QTableView>
#include "abstract_tab_module.h"

//! \ingroup groupe_fen
//! Défini les nom et bdd_nom des entitée.
#define DECL_TABLE_ENTITY_NOM(ENTITY,NOM) m_tables[info_entity::ENTITY##Id].nom = NOM; \
    m_tables[info_entity::ENTITY##Id].bdd_nom = info_bdd<ENTITY>::table(); \
    attribut_bdd_noms = info_bdd<ENTITY>::attribut(); \
    for(int i = 0; i != ENTITY::Nbr_Att; ++i) m_tables[info_entity::ENTITY##Id].attribut_noms \
.append(ENTITY::nomAttribut(i).append("(").append(attribut_bdd_noms.value(i)).append(")"));

#define DECL_TABLE_ENTITY(ENTITY) DECL_TABLE_ENTITY_NOM(ENTITY,ENTITY::Name())
#define DECL_TABLE_ENTITY_NAME(ENTITY) DECL_TABLE_ENTITY_NOM(ENTITY,#ENTITY)

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Onglet de gestion de la base de donnée.
 */
class abstract_tab_gestion_bdd : public abstract_tab_module {
    Q_OBJECT
protected:
//    enum {Arbre_Cours_Id,
//          arbre_Donnee_Id,
//          Arbre_Exercice_Id,
//          Arbre_Mot_Cle,
//          Nbr_Table
//    };
    //! Structure contenant les donnée et variable associé à chaque table.
    struct table{
        QStringList attribut_noms;  //!< Liste des noms des attributs.
        QSqlTableModel *model = nullptr;   //!< Model.
        QString nom;        //!< Nom de la table.
        QString bdd_nom;     //!< Nom de la table dans la base de donnée.
        QTableView *view = nullptr;       //!< Vue.
    };

    using Tables = std::vector<table>;
    const entidt m_nbr_entity;
    Tables m_tables; //!< Ensemble des liens avec les tables.
    QListWidget *m_name_list;   //!< Liste des noms des entitées
    QSplitter *m_splitter;     //!< Splitter entre la liste des noms des entités et les tables
    QStackedWidget *m_stack;   //!< Staked widget.
    QHBoxLayout *m_main_layout;     //!< Calque principal.

public:
    //! Constructeur.
    explicit abstract_tab_gestion_bdd(QSqlDatabase &bdd, entidt nbr_entity,
                                   const tab_index &index, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_tab_gestion_bdd() override = default;
};
}}
#endif // ABSTRACT_TAB_GESTION_BDD_H
