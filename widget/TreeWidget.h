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
                 IdRole = Qt::UserRole + 1};

    //! Constructeur.
    TreeWidget(QWidget *parent = nullptr) : QTreeWidget (parent) {}

    //! Destructeur.
    ~TreeWidget() override;

    //! Renvoie l'identifiant du type sélectionné.
    idt id() const
        {return selectionModel()->hasSelection() ? currentItem()->data(IdColonne,IdRole).toUInt() : 0;}

    //! Mutateur du type.
    void set_id(idt id);

    /*! Mutateur des donnés affichés.
     *QTreeWidgetItem *childItem(tree<T>::brother_iterator)
     */
    template<class T, class ChildFactory> void setTree(const conteneurMPS::tree<T> &tree, ChildFactory child);

    //! Mutateur des donnés affichés (Nom, Nc) avec id.
    template<class Ent> void setTreenc_nomId(const conteneurMPS::tree<Ent> &tr) {
        setTreeRef(tree, [](const Ent &ent)->QTreeWidgetItem *{
                auto item = new QTreeWidgetItem({ent.nom(),ent.nc()});
                item->set_data(widgetMPS::TreeWidget::IdColonne,widgetMPS::TreeWidget::IdRole,ent.id());
                return item;
            });
    }

    /*! Mutateur des donnés affichés.
     *QTreeWidgetItem *childItem(const T&)
     */
    template<class T, class ChildFactory> void setTreeRef(const conteneurMPS::tree<T> &tree, ChildFactory child);

protected:
    //! Ajouter les fils de iter à item avec .
    template<class T, class ChildFactory> void addChilds(typename conteneurMPS::tree<T>::brother_iterator iter,
                                                                       QTreeWidgetItem *item,
                                                                       ChildFactory &child);
};

template<class T, class ChildFactory> void TreeWidget::addChilds(typename conteneurMPS::tree<T>::brother_iterator iter,
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

template<class T, class ChildFactory> void TreeWidget::setTree(const conteneurMPS::tree<T> &tree, ChildFactory child) {
    auto iter = tree.cbegin_brother();
    auto *item = child(iter);
    addTopLevelItem(item);
    addChilds<T,ChildFactory>(iter,item,child);
}

template<class T, class ChildFactory> void TreeWidget::setTreeRef(const conteneurMPS::tree<T> &tree, ChildFactory child)
    {setTree(tree,[child](typename conteneurMPS::tree<T>::brother_iterator iter)->QTreeWidgetItem *{return child(*iter);});}
}
#endif // TREEWIDGET_H
