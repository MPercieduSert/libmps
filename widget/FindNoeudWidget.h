/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/04/2020
 */
#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTreeView>
#include <QVBoxLayout>
#include "FindNoeudModel.h"

//////////////////////////////////////// FindNoeudDelegate /////////////////////////
namespace delegateMPS {
/*! \ingroup groupeDelegate
 *\brief Delegate de la recherche dans un model de type colonnes.
 */
class FindNoeudDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    //! Constructeur.
    FindNoeudDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    //! Fabrique d'éditeur.
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    //! Gestionnaire d'évenement.
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

    //! Hydrate l'éditeur.
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    //! Transmet les données au model.
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};
}

//////////////////////////////////////////////// FindNoeudWidget /////////////////////////////////////
namespace widgetMPS {
/*! \ingroup groupeWidget
 *\brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type AbstractColonnesModel.
 */
class FindNoeudWidget : public QWidget {
    Q_OBJECT
protected:
    // Model &delegate
    using Delegate = delegateMPS::FindNoeudDelegate;
    using FindModel = modelMPS::FindNoeudModel;
    FindModel *m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton *m_addButton;      //!< Bouton d'ajout d'une condition de recherche.
    QPushButton *m_delButton;      //!< Bouton de suppresion d'une condition.
    QPushButton *m_findButton;     //!< Bouton de recherche.
    QPushButton *m_resetButton;    //!< Bouton de réinitialisation de la recherche.
    QTreeView *m_view;             //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout *m_mainLayout;     //!< Calque principal.
    QHBoxLayout *m_buttonsLayout;  //!< Claque des boutton.
public:
    //! Constructeur.
    FindNoeudWidget(modelMPS::FindNoeudModel *model = new FindModel(), QWidget *parent = nullptr);

    //! Accesseur du Model.
    FindModel *findModel() const
        {return m_model;}

    //! Mutateur du model de recheche.
    void set_findModel(modelMPS::FindNoeudModel *model);

    //! Mutateur du model filtré.
    void setModel(modelMPS::AbstractColonnesModel *model) {
        if(m_model)
            m_model->setModel(model);
    }
};
}
#endif // FINDWIDGET_H
