/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef TABGESTIONTYPE_H
#define TABGESTIONTYPE_H

#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include "AbstractTabModuleWithBdd.h"
#include "bdd_predef.h"
#include "code_widget.h"
#include "predef_model.h"
#include "Standardnode_widget.h"

namespace fenMPS {
/*! \ingroup groupeFen
 *\brief Classe de l'onglets de gestion des types.
 */
class TabGestionType : public AbstractTabModuleWithBdd
{
protected:
    model_base::type_permission_model *m_model;    //!< Model.
    widget::node_view *m_view;               //!< Vue.
    QListWidget *m_cibleListWidget;            //!< Sélection des cibles.
    QPushButton *m_saveButton;                 //!< Bouton de sauvegarde.

    // Calque
    QVBoxLayout *m_cible_layout;                //!< Calque des cibles.
    QHBoxLayout *m_main_layout;                 //!< Calque principal.

public:
    //! position des cases.
    enum positionCase {Visible,
                      Attribuable,
                      NbrCase};
    //! Constructeur.
    TabGestionType(b2d::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent = nullptr);

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
}

#endif // TABGESTIONTYPE_H
