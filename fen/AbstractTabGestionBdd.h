/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef ABSTRACTTABGESTIONBDD_H
#define ABSTRACTTABGESTIONBDD_H

#include <QListWidget>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSplitter>
#include <QStackedWidget>
#include <QTableView>
#include "AbstractTabModule.h"

//! \ingroup groupeFen
//! Défini les nom et nomBdd des entitée.
#define DECL_TABLE_ENTITY_NOM(ENTITY,NOM) m_tables[info_entity::ENTITY##Id].nom = NOM; \
    m_tables[info_entity::ENTITY##Id].nomBdd = info_bdd<ENTITY>::table(); \
    nomAttributsBdd = info_bdd<ENTITY>::attribut(); \
    for(int i = 0; i != ENTITY::Nbr_Att; ++i) m_tables[info_entity::ENTITY##Id].nomAttributs \
.append(ENTITY::nomAttribut(i).append("(").append(nomAttributsBdd.value(i)).append(")"));

#define DECL_TABLE_ENTITY(ENTITY) DECL_TABLE_ENTITY_NOM(ENTITY,ENTITY::Name())
#define DECL_TABLE_ENTITY_NAME(ENTITY) DECL_TABLE_ENTITY_NOM(ENTITY,#ENTITY)

namespace fenMPS {
/*! \ingroup groupeFen
 *\brief Onglet de gestion de la base de donnée.
 */
class AbstractTabGestionBdd : public AbstractTabModule {
    Q_OBJECT
protected:
    enum {arbreCoursId,
          arbredonnee,
          arbreExerciceId,
          arbreMot_Cle,
          NbrTable
    };
    //! Structure contenant les donnée et variable associé à chaque table.
    struct table{
        QStringList nomAttributs;  //!< Liste des noms des attributs.
        QSqlTableModel *model = nullptr;   //!< Model.
        QString nom;        //!< Nom de la table.
        QString nomBdd;     //!< Nom de la table dans la base de donnée.
        QTableView *view = nullptr;       //!< Vue.
    };

    using Tables = std::vector<table>;
    const entidt m_nbr_entity;
    Tables m_tables; //!< Ensemble des liens avec les tables.
    QListWidget *m_listName;   //!< Liste des noms des entitées
    QSplitter *m_splitter;     //!< Splitter entre la liste des noms des entités et les tables
    QStackedWidget *m_stack;   //!< Staked widget.
    QHBoxLayout *m_mainLayout;     //!< Calque principal.
    //QStackedLayout *m_stack;   //!< Staked widget.
    //QHBoxLayout *m_secondLayout;   //!< Calque secondaire.

public:
    //! Constructeur.
    explicit AbstractTabGestionBdd(QSqlDatabase &bdd, entidt nbr_entity,
                                   const std::pair<int, int> &pairIndex, QWidget *parent = nullptr);

    //! Destructeur.
    ~AbstractTabGestionBdd() override = default;
};
}
#endif // ABSTRACTTABGESTIONBDD_H
