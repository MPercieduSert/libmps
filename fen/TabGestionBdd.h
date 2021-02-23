/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/12/2019
 */
#ifndef TABGESTIONBDD_H
#define TABGESTIONBDD_H

#include <QPushButton>
#include <QSqltable_model>
#include <QTableView>
#include "AbstractTabModuleWithBdd.h"


namespace fenMPS {
/*! \ingroup groupeFen
 *\brief Classe des onglets de gestion des tables de la base de donnée.
 */
class TabGestionBdd : public AbstractTabModuleWithBdd
{
protected:
    manager::info_bdd m_info;                 //!< Information sur la table.
    std::vector<QString> m_Names_attributs;      //!< Noms des attributs.
    QSqltable_model *m_model;                   //!< Model.
    QTableView *m_view;                        //!< Vue.
    QPushButton *m_insertButton;               //!< Bouton d'insertion d'une ligne.
    QPushButton *m_refreshButton;              //!< Bouton rafraichisant l'affichage.
    QPushButton *m_saveButton;                 //!< Bouton de sauvegarde.
    QPushButton *m_suppr_bouton;                //!< Bouton de suppression d'une entité.
    QVBoxLayout *m_main_layout;                 //!< Calque principal.

public:
    TabGestionBdd(b2d::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent = nullptr);

    //! Accesseur du titre.
    QString title() const
        {return QString("Table : ").append(m_info.name());}
};
}
#endif // TABGESTIONBDD_H
