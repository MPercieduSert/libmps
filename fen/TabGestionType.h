/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef TABGESTIONTYPE_H
#define TABGESTIONTYPE_H

#include <QPushButton>
#include "AbstractTabModuleWithBdd.h"
#include "NodeView.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Classe de l'onglets de gestion des types.
 */
class TabGestionType : public AbstractTabModuleWithBdd
{
protected:
    std::vector<QString> m_namesAttributs;      //!< Noms des attributs.
     * m_model;                   //!< Model.
    widgetMPS::NodeView * m_view;               //!< Vue.
    QPushButton * m_insertButton;               //!< Bouton d'insertion d'une ligne.
    QPushButton * m_saveButton;                 //!< Bouton de sauvegarde.
    QPushButton * m_supprButton;                //!< Bouton de suppression d'une entité.
    QVBoxLayout * m_mainLayout;                 //!< Calque principal.

public:
    TabGestionType(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget * parent = nullptr);

    //! Accesseur du titre.
    QString title() const
        {return QString("Gestion des types");}
};
}

#endif // TABGESTIONTYPE_H
