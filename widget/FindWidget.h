/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/04/2020
 */
#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>
#include "AbstractFindModel.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type AbstractColonnesModel.
 */
class FindWidget : public QWidget {
    Q_OBJECT
protected:
    // Model & delegate
    using Delegate = delegateMPS::FindDelegate;
    using FindModel = modelMPS::AbstractFindModel;
    FindModel * m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton * m_addButton;      //!< Bouton d'ajout d'une condition de recherche.
    QPushButton * m_delButton;      //!< Bouton de suppresion d'une condition.
    QPushButton * m_findButton;     //!< Bouton de recherche.
    QPushButton * m_resetButton;    //!< Bouton de réinitialisation de la recherche.
    QTreeView * m_view;             //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout * m_mainLayout;     //!< Calque principal.
    QHBoxLayout * m_buttonsLayout;  //!< Claque des boutton.
public:
    //! Constructeur.
    FindWidget(FindModel * model = new FindModel(),QWidget * parent = nullptr);

    //! Accesseur du Model.
    FindModel * findModel() const
        {return m_model;}

    //! Mutateur du model de recheche.
    void setFindModel(FindModel * model);

    //! Mutateur du model filtré.
    void setModel(modelMPS::AbstractColonnesModel * model) {
        if(m_model)
            m_model->setModel(model);
    }
};
}
#endif // FINDWIDGET_H
