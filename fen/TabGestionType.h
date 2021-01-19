/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef TABGESTIONTYPE_H
#define TABGESTIONTYPE_H

#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include "AbstractTabModuleWithBdd.h"
#include "BddPredef.h"
#include "CodeWidget.h"
#include "PredefModel.h"
#include "StandardNodeWidget.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Classe de l'onglets de gestion des types.
 */
class TabGestionType : public AbstractTabModuleWithBdd
{
protected:
    modelMPS::TypePermissionModel * m_model;    //!< Model.
    widgetMPS::NodeView * m_view;               //!< Vue.
    QListWidget * m_cibleListWidget;            //!< Sélection des cibles.
    QPushButton * m_saveButton;                 //!< Bouton de sauvegarde.

    // Calque
    QVBoxLayout * m_cibleLayout;                //!< Calque des cibles.
    QHBoxLayout * m_mainLayout;                 //!< Calque principal.

public:
    //! Position des cases.
    enum positionCase {Visible,
                      Attribuable,
                      NbrCase};
    //! Constructeur.
    TabGestionType(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget * parent = nullptr);

    //! Accesseur du titre.
    QString title() const
        {return QString("Gestion des types");}
};
}

#endif // TABGESTIONTYPE_H
