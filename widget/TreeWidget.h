/*Auteur: PERCIE DU SERT Maxime
 *Date: 17/09/2020
 */
#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include "Tree.h"
#include "typemps.h"

namespace  widgetMPS {
using namespace typeMPS;

class TreeWidget : public QTreeWidget{
public:
    //! Colonne
    enum colonne {IdColonne,
                 IdRole = Qt::UserRole + 1};

    //! Constructeur.
    TreeWidget(QWidget * parent = nullptr);

    //! Renvoie l'identifiant du type sélectionné.
    idt id() const
        {return selectionModel()->hasSelection() ? currentItem()->data(IdColonne,Qt::UserRole).toUInt() : 0;}

    //! Mutateur du type.
    void setId(idt id);

    /*! Mutateur des donnés affichés.
     * QTreeWidgetItem * childItem(tree<T>::brother_iterator)
     */
    template<class T, class ChildFactory> void setTree(const conteneurMPS::tree<T> & tree, ChildFactory child);

    /*! Mutateur des donnés affichés.
     * QTreeWidgetItem * childItem(const T&)
     */
    template<class T, class ChildFactory> void setTreeRef(const conteneurMPS::tree<T> & tree, ChildFactory child);
protected:
    //! Ajouter les fils de iter à item avec .
    template<class T, class ChildFactory> void addChilds(typename conteneurMPS::tree<T>::brother_iterator iter,
                                                                       QTreeWidgetItem * item,
                                                                       ChildFactory & child);
};

template<class T, class ChildFactory> void TreeWidget::addChilds(typename conteneurMPS::tree<T>::brother_iterator iter,
                                                                               QTreeWidgetItem * item,
                                                                               ChildFactory & child) {
    if(!iter.leaf()){
        for (auto iterChild = iter.beginChild(); iterChild; ++iterChild) {
            auto * itemChild = child(iterChild);
            item->addChild(itemChild);
            addChilds<T,ChildFactory>(iterChild,itemChild,child);
        }
    }
}

template<class T, class ChildFactory> void TreeWidget::setTree(const conteneurMPS::tree<T> & tree, ChildFactory child) {
    auto iter = tree.cbeginBrother();
    auto * item = child(iter);
    addTopLevelItem(item);
    addChilds<T,ChildFactory>(iter,item,child);
}

template<class T, class ChildFactory> void TreeWidget::setTreeRef(const conteneurMPS::tree<T> & tree, ChildFactory child)
    {setTree(tree,[child](typename conteneurMPS::tree<T>::brother_iterator iter)->QTreeWidgetItem *{return child(*iter);});}
}
#endif // TREEWIDGET_H
