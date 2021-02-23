/*Auteur: PERCIE DU SERT Maxime
 *Date: 17/09/2020
 */
#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QTreeWidget>
#include "tree.h"
#include "type_mps.h"

namespace mps {
namespace  widget {

class tree_widget : public QTreeWidget{
    //Q_OBJECT non compatible avec les méthodes template.
public:
    //! Colonne
    enum colonne {Id_Colonne,
                 Id_Role = Qt::UserRole + 1};

    //! Constructeur.
    tree_widget(QWidget *parent = nullptr) : QTreeWidget (parent) {}

    //! Destructeur.
    ~tree_widget() override;

    //! Renvoie l'identifiant du type sélectionné.
    idt id() const
        {return selectionModel()->hasSelection() ? currentItem()->data(Id_Colonne,Id_Role).toUInt() : 0;}

    //! Mutateur du type.
    void set_id(idt id);

    /*! Mutateur des donnés affichés.
     *QTreeWidgetItem *childItem(tree<T>::brother_iterator)
     */
    template<class T, class ChildFactory> void set_tree(const tree<T> &tree, ChildFactory child);

    //! Mutateur des donnés affichés (Nom, Nc) avec id.
    template<class Ent> void set_treenc_nomId(const tree<Ent> &tr) {
        set_tree_ref(tr, [](const Ent &ent)->QTreeWidgetItem *{
                auto item = new QTreeWidgetItem({ent.nom(),ent.nc()});
                item->setData(widget::tree_widget::Id_Colonne,widget::tree_widget::Id_Role,ent.id());
                return item;
            });
    }

    /*! Mutateur des donnés affichés.
     *QTreeWidgetItem *childItem(const T&)
     */
    template<class T, class ChildFactory> void set_tree_ref(const tree<T> &tree, ChildFactory child);

protected:
    //! Ajouter les fils de iter à item avec .
    template<class T, class ChildFactory> void add_childs(typename tree<T>::brother_iterator it,
                                                                       QTreeWidgetItem *item,
                                                                       ChildFactory &child);
};

template<class T, class ChildFactory> void tree_widget::add_childs(typename tree<T>::brother_iterator it,
                                                                               QTreeWidgetItem *item,
                                                                               ChildFactory &child) {
    if(!it.leaf()){
        for (auto child_it = it.begin_child(); child_it; ++child_it) {
            auto *child_item = child(child_it);
            item->addChild(child_item);
            add_childs<T,ChildFactory>(child_it,child_item,child);
        }
    }
}

template<class T, class ChildFactory> void tree_widget::set_tree(const tree<T> &tr, ChildFactory child) {
    auto it = tr.cbegin_brother();
    auto *item = child(it);
    addTopLevelItem(item);
    add_childs<T,ChildFactory>(it,item,child);
}

template<class T, class ChildFactory> void tree_widget::set_tree_ref(const tree<T> &tr, ChildFactory child)
    {set_tree(tr,[child](typename tree<T>::brother_iterator it)->QTreeWidgetItem *{return child(*it);});}
}}
#endif // TREE_WIDGET_H
