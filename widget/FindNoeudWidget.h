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
namespace delegate {
/*! \ingroup groupe_delegate
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
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type abstract_colonnes_model.
 */
class FindNoeudWidget : public QWidget {
    Q_OBJECT
protected:
    // Model &delegate
    using Delegate = delegate::FindNoeudDelegate;
    using find_model = model_base::FindNoeudModel;
    find_model *m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton *m_addButton;      //!< Bouton d'ajout d'une condition de recherche.
    QPushButton *m_delButton;      //!< Bouton de suppresion d'une condition.
    QPushButton *m_find_button;     //!< Bouton de recherche.
    QPushButton *m_reset_button;    //!< Bouton de réinitialisation de la recherche.
    QTreeView *m_view;             //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout *m_main_layout;     //!< Calque principal.
    QHBoxLayout *m_button_layout;  //!< Claque des boutton.
public:
    //! Constructeur.
    FindNoeudWidget(model_base::FindNoeudModel *model = new find_model(), QWidget *parent = nullptr);

    //! Accesseur du Model.
    find_model *find() const
        {return m_model;}

    //! Mutateur du model de recheche.
    void set_find(model_base::FindNoeudModel *model);

    //! Mutateur du model filtré.
    void set_model(model_base::abstract_colonnes_model *model) {
        if(m_model)
            m_model->setModel(model);
    }
};
}
#endif // FINDWIDGET_H
