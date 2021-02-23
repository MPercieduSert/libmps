/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/04/2020
 */
#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include "abstract_colonnes_model.h"
#include "colonnes_for_model.h"
#include "find_model.h"
#include "node_ptrView.h"
#include "Standardnode_ptrWidget.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 *\brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type abstract_colonnes_model.
 */
class FindWidget : public QWidget {
    Q_OBJECT
protected:
    // Model &delegate
    using Delegate = delegateMPS::Standardnode_ptrDelegate;
    using find_model = model_base::find_model;
    find_model *m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton *m_findButton;     //!< Bouton de recherche.
    QPushButton *m_resetButton;    //!< Bouton de réinitialisation de la recherche.
    node_ptrView *m_view;              //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout *m_mainLayout;     //!< Calque principal.
    QHBoxLayout *m_buttonsLayout;  //!< Claque des boutton.
public:
    //! Constructeur.
    FindWidget(find_model *model = new find_model(),QWidget *parent = nullptr);

    //! Constructeur.
    FindWidget(model_base::abstract_colonnes_model *model,QWidget *parent = nullptr)
        : FindWidget(new find_model(model),parent) {}

    //! Accesseur du Model.
    find_model *find_model() const
        {return m_model;}

    //! Mutateur du model de recheche.
    void set_find_model(find_model *model);

    //! Mutateur du model filtré.
    void setModel(model_base::abstract_colonnes_model *model) {
        if(m_model)
            m_model->setModel(model);
    }
};
}
#endif // FINDWIDGET_H
