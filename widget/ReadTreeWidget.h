/*Auteur: PERCIE DU SERT Maxime
 *Date: 12/09/2018
 */
#ifndef READTREEWIDGET_H
#define READTREEWIDGET_H

#include <QScrollArea>
#include <QTreeView>
#include <QWidget>
#include "TreeModelReadEntity.h"

/*! \ingroup groupeWidget
 * \brief Classe template des widget de type arbre non-modifiable.
 */
template<class Ent> class ReadTreeWidget : public QTreeView
{
public:
    //! Constructeur.
    ReadTreeWidget(const Tree<Ent> & tree,
                   const QList<typename Ent::Position> atts = QList<typename Ent::Position>(),
                   QWidget * parent = nullptr);

    //! Destructeur.
    ~ReadTreeWidget() override = default;

    //! Renvoie un pointeur sur le model
    TreeModelReadEntity<Ent> * model() const
        {return static_cast<TreeModelReadEntity<Ent> *>(QTreeView::model());}

    //! Selectionne l'item correspondant au première item d'identifiant id dans l'arbre.
    void select(idt id);

    //! Modifie les données affichées par le widget.
    void setDataTree(const Tree<Ent> & tree)
        {model()->setDataTree(tree);}

    //! renvoie l'élément sélectionnée.
    Ent value() const
    {
        QModelIndex index = currentIndex();
        if (index.isValid())
            return static_cast<TreeItem<Ent>*>(index.internalPointer())->data();
        else
            return Ent();
    }
};

template<class Ent> ReadTreeWidget<Ent>::ReadTreeWidget(const Tree<Ent> &tree,const QList<typename Ent::Position> atts, QWidget * parent)
    : QTreeView(parent)
{ 
    setModel(new TreeModelReadEntity<Ent>(atts, parent));
    model()->setDataTree(tree);
}

template<class Ent> void ReadTreeWidget<Ent>::select(idt id)
    {selectionModel()->select(model()->foundId(id),QItemSelectionModel::Select);}

#endif // READTREEWIDGET_H
