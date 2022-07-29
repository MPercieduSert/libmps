/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/02/2020
 */
#ifndef CHECK_LIST_H
#define CHECK_LIST_H

#include <QListWidget>
#include "bdd.h"

namespace mps {
namespace widget {

/*! \ingroup groupe_Widget
 *\brief Classe affichant une liste à checker.
 */
class check_list : public QListWidget {
    Q_OBJECT
public:
    template<class Ent> class Save {
    public:
        template<class Hydrate> static void save(b2d::bdd &bdd, check_list *list, const Hydrate &hydrate);
    };

    using QListWidget::QListWidget;

    //! Constructeur.
    check_list(const std::map<idt,QString> &map, QWidget *parent = nullptr)
        : check_list(parent)
        {set_labels(map);}
    
    //! Constructeur.
    template<class Ent> check_list(const vector_ptr<Ent> &vec,
                                  const QString &(*F)(const Ent &) = [](const Ent &ent)->const QString &{return ent.nom();},
                                  QWidget *parent = nullptr)
        : check_list(parent)
        {set_labels(vec,F);}

    //! Constructeur.
    template<class Ent> check_list(vector_ptr<Ent> &&vec,
                                  const QString &(*F)(const Ent &) = [](const Ent &ent)->const QString &{return ent.nom();},
                                  QWidget *parent = nullptr)
        : check_list(vec,F,parent) {}
    
    //! Renvoie l'identifiant d'un item.
    idt id(int i) const
        {return item(i)->data(Qt::UserRole).toUInt();}

    //! Mutateur des check de la liste.
    template<class Ent> void set_checks_by_id(const vector_ptr<Ent> &vec, idt (*F)(const Ent &));

    //! Mutateur des check de la liste.
    template<class Ent> void set_checks_by_id(vector_ptr<Ent> &&vec, idt (*F)(const Ent &))
        {set_checks_by_id(vec,F);}

    //! Mutateur de la liste des labels.
    void set_labels(const std::map<idt,QString> &map);

    //! Mutateur de la liste des labels.
    template<class Ent> void set_labels(const vector_ptr<Ent> &vec,
                                       const QString &(*F)(const Ent &) = [](const Ent &ent)->const QString &{return ent.nom();});

    //! Mutateur de la liste des labels.
    template<class Ent> void set_labels(vector_ptr<Ent> &&vec,
                                       const QString &(*F)(const Ent &) = [](const Ent &ent)->const QString &{return ent.nom();})
        {set_labels(vec,F);}
};

template<class Ent> template<class Hydrate> void check_list::Save<Ent>::save(b2d::bdd &bdd, check_list *list, const Hydrate &hydrate) {
    for (auto i = 0; i != list->count(); ++i){
        Ent ent;
        hydrate(ent,list->id(i));
        if(list->item(i)->checkState() == Qt::Checked)
            bdd.save_unique(ent);
        else if (bdd.exists_unique(ent))
            bdd.del(ent);
    }
}

template<class Ent> void check_list::set_checks_by_id(const vector_ptr<Ent> &vec, idt (*F)(const Ent &)) {
    for(auto i = 0; i != count(); ++i) {
        auto *it = item(i);
        auto j = vec.cbegin();
        while(j != vec.cend() &&F(*j) != it->data(Qt::UserRole))
            j++;
        if(j == vec.cend())
            it->setCheckState(Qt::Unchecked);
        else
            it->setCheckState(Qt::Checked);
    }
}

template<class Ent> void check_list::set_labels(const vector_ptr<Ent> &vec, const QString &(*F)(const Ent &)) {
    for(auto i = vec.cbegin(); i != vec.cend(); ++i) {
        auto *item = new QListWidgetItem(F(*i));
        item->setData(Qt::UserRole,i->id());
        item->setCheckState(Qt::Unchecked);
        addItem(item);
    }
}
}}
#endif // CHECK_LIST_H
