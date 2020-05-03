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

class FindWidget : public QWidget {
    Q_OBJECT
protected:
    // Model & delegate
    using Delegate = delegateMPS::FindDelegate;
    using Model = modelMPS::AbstractFindModel;
    Model * m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton * m_addButton;      //!< Bouton d'ajout d'une condition de recherche.
    QPushButton * m_delButton;      //!< Bouton de suppresion d'une condition.
    QPushButton * m_resetButton;    //!< Bouton de réinitialisation de la recherche.
    QTreeView * m_view;             //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout * m_mainLayout;     //!< Calque principal.
    QHBoxLayout * m_buttonsLayout;  //!< Claque des boutton.
public:
    //! Constructeur.
    FindWidget(Model * model = new Model(),QWidget * parent = nullptr);

    //! Accesseur du Model.
    Model * model() const
        {return m_model;}

    //! Mutateur du model.
    void setModel(Model * model) {
        m_view->setModel(model);
        if(m_model)
            delete m_model;
        m_model = model;
        m_model->setParent(this);
    }
};

#endif // FINDWIDGET_H
