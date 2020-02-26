/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/12/2019
 */
#ifndef TABGESTIONBDD_H
#define TABGESTIONBDD_H

#include <QPushButton>
#include <QSqlTableModel>
#include <QTableView>
#include "AbstractTabModuleWithBdd.h"


namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Classe des onglets de gestion des tables de la base de donnée.
 */
class TabGestionBdd : public AbstractTabModuleWithBdd
{
protected:
    managerMPS::InfoBdd m_info;                 //!< Information sur la table.
    std::vector<QString> m_namesAttributs;      //!< Noms des attributs.
    QSqlTableModel * m_model;                   //!< Model.
    QTableView * m_view;                        //!< Vue.
    QPushButton * m_insertButton;               //!< Bouton d'insertion d'une ligne.
    QPushButton * m_refreshButton;              //!< Bouton rafraichisant l'affichage.
    QPushButton * m_saveButton;                 //!< Bouton de sauvegarde.
    QPushButton * m_supprButton;                //!< Bouton de suppression d'une entité.
    QVBoxLayout * m_mainLayout;                 //!< Calque principal.

public:
    TabGestionBdd(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget * parent = nullptr);

    //! Accesseur du titre.
    QString title() const
        {return QString("Table : ").append(m_info.name());}
};
}
#endif // TABGESTIONBDD_H
