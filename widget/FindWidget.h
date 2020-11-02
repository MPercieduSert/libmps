/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/04/2020
 */
#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "FindModel.h"
#include "NodeView.h"

//////////////////////////////////////// FindDelegate /////////////////////////
namespace delegateMPS {
/*! \ingroup groupeDelegate
 * \brief Delegate de la recherche dans un model de type colonnes.
 */
class FindDelegate : public AbstractNodeDelegate {
    Q_OBJECT
public:
    //! Constructeur.
    FindDelegate(QObject * parent = nullptr)
        : AbstractNodeDelegate (parent) {}

    //! Fabrique d'éditeur.
    NodeWidget * createWidget(const NodeIndex &index) const override;
};
}

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type AbstractColonnesModel.
 */
class FindWidget : public QWidget {
    Q_OBJECT
protected:
    // Model & delegate
    using Delegate = delegateMPS::FindDelegate;
    using FindModel = modelMPS::FindModel;
    FindModel * m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton * m_addButton;      //!< Bouton d'ajout d'une condition de recherche.
    QPushButton * m_delButton;      //!< Bouton de suppresion d'une condition.
    QPushButton * m_findButton;     //!< Bouton de recherche.
    QPushButton * m_resetButton;    //!< Bouton de réinitialisation de la recherche.
    NodeView * m_view;             //!< Vue de l'arbre de recherche.

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
/////////////////////////////////////////////////// Noeud de Recherche /////////////////////////////////
//! Espace de noms des noeud de recheches.
namespace findNodeWidget {
/*! \ingroup groupeModel
 * \brief Classe mère des widgets associés aux neuds de recherche.
 */
class FindNodeWidget : public NodeWidget {
    Q_OBJECT
public:
    //! Constructeur
    using NodeWidget::NodeWidget;
};

/*! \ingroup groupeModel
 * \brief Classe mère des widget de noeuds de recherche avec négation.
 */
class NegationNodeWidget : public FindNodeWidget {
    Q_OBJECT
protected:
    QCheckBox * m_nonCheckBox;      //! CheckBox de négation du prédicat de recherche.
    QHBoxLayout * m_mainLayout;     //! Calque principale du noeud.
public:
    //! Constructeur
    NegationNodeWidget(const NodeIndex & index, QWidget * parent);

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override
        {m_nonCheckBox->setChecked(m_index.data(modelMPS::findNodeModel::NegType,Qt::DisplayRole).toBool());}
};

/*! \ingroup groupeModel
 * \brief Classe mère des widget de noeuds de recherche définissant une condition.
 */
class ConditionNodeWidget : public NegationNodeWidget {
    Q_OBJECT
protected:
    QCheckBox * m_nonCheckBox;      //! CheckBox de négation du prédicat de recherche.
    QHBoxLayout * m_mainLayout;     //! Calque principale du noeud.
public:
    //! Constructeur
    NegationNodeWidget(const NodeIndex & index, QWidget * parent);

    //! Met à jour les données du widget à partir des données du model.
    void updateData() override
        {m_nonCheckBox->setChecked(m_index.data(modelMPS::findNodeModel::NegType,Qt::DisplayRole).toBool());}
};
}
}
#endif // FINDWIDGET_H
