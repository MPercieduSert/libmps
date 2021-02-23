/*Auteur: PERCIE DU SERT Maxime
 *Date: 17/09/2020
 */
#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include "tree.h"
#include "type_mps.h"

namespace  widgetMPS {
using namespace type_mps;

class TreeWidget : public QTreeWidget{
    //Q_OBJECT non compatible avec les méthodes template.
public:
    //! Colonne
    enum colonne {IdColonne,
                 Id_Role = Qt::UserRole + 1};

    //! Constructeur.
    TreeWidget(QWidget *parent = nullptr) : QTreeWidget (parent) {}

    //! Destructeur.
    ~TreeWidget() override;

    //! Renvoie l'identifiant du type sélectionné.
    idt id() const
        {return selectionModel()->hasSelection() ? currentItem()->data(IdColonne,Id_Role).toUInt() : 0;}

    //! Mutateur du type.
    void set_id(idt id);

    /*! Mutateur des donnés affichés.
     *QTreeWidgetItem *childItem(tree<T>::brother_iterator)
     */
    template<class T, class ChildFactory> void set_tree(const tree<T> &tree, ChildFactory child);

    //! Mutateur des donnés affichés (Nom, Nc) avec id.
    template<class Ent> void set_treenc_nomId(const tree<Ent> &tr) {
        set_treeRef(tree, [](const Ent &ent)->QTreeWidgetItem *{
                auto item = new QTreeWidgetItem({ent.nom(),ent.nc()});
                item->set_data(widgetMPS::TreeWidget::IdColonne,widgetMPS::TreeWidget::Id_Role,ent.id());
                return item;
            });
    }

    /*! Mutateur des donnés affichés.
     *QTreeWidgetItem *childItem(const T&)
     */
    template<class T, class ChildFactory> void set_treeRef(const tree<T> &tree, ChildFactory child);

protected:
    //! Ajouter les fils de iter à item avec .
    template<class T, class ChildFactory> void addChilds(typename tree<T>::brother_iterator iter,
                                                                       QTreeWidgetItem *item,
                                                                       ChildFactory &child);
};

template<class T, class ChildFactory> void TreeWidget::addChilds(typename tree<T>::brother_iterator iter,
                                                                               QTreeWidgetItem *item,
                                                                               ChildFactory &child) {
    if(!iter.leaf()){
        for (auto iterChild = iter.begin_child(); iterChild; ++iterChild) {
            auto *itemChild = child(iterChild);
            item->addChild(itemChild);
            addChilds<T,ChildFactory>(iterChild,itemChild,child);
        }
    }
}

template<class T, class ChildFactory> void TreeWidget::set_tree(const tree<T> &tree, ChildFactory child) {
    auto iter = tree.cbegin_brother();
    auto *item = child(iter);
    addTopLevelItem(item);
    addChilds<T,ChildFactory>(iter,item,child);
}

template<class T, class ChildFactory> void TreeWidget::set_treeRef(const tree<T> &tree, ChildFactory child)
    {set_tree(tree,[child](typename tree<T>::brother_iterator iter)->QTreeWidgetItem *{return child(*iter);});}
}
#endif // TREEWIDGET_H
