/*Auteur: PERCIE DU SERT Maxime
 *Date: 29/02/2020
 */
#ifndef CHECKLIST_H
#define CHECKLIST_H

#include <QListWidget>
#include "Bdd.h"

namespace widgetMPS {
using namespace typeMPS;

/*! \ingroup groupeWidget
 * \brief Classe affichant une liste à checker.
 */
class Checklist : public QListWidget
{
    Q_OBJECT
public:
    template<class Ent> class Save {
    public:
        template<class Hydrate> static void save(bddMPS::Bdd & bdd, Checklist * list, const Hydrate & hydrate);
    };

    using QListWidget::QListWidget;

    //! Constructeur.
    Checklist(const std::map<idt,QString> & map, QWidget * parent = nullptr) 
        : Checklist(parent)
        {setLabels(map);}
    
    //! Constructeur.
    template<class Ent> Checklist(const conteneurMPS::VectorPtr<Ent> & vec,
                                  const QString & (*F)(const Ent &) = [](const Ent & ent)->const QString &{return ent.nom();},
                                  QWidget * parent = nullptr)
        : Checklist(parent)
        {setLabels(vec,F);}

    //! Constructeur.
    template<class Ent> Checklist(conteneurMPS::VectorPtr<Ent> && vec,
                                  const QString & (*F)(const Ent &) = [](const Ent & ent)->const QString &{return ent.nom();},
                                  QWidget * parent = nullptr)
        : Checklist(vec,F,parent) {}
    
    //! Renvoie l'identifiant d'un item.
    idt id(int i) const
        {return item(i)->data(Qt::UserRole).toUInt();}

    //! Mutateur des check de la liste.
    template<class Ent> void setChecksById(const conteneurMPS::VectorPtr<Ent> & vec, idt (*F)(const Ent &));

    //! Mutateur des check de la liste.
    template<class Ent> void setLChecksById(conteneurMPS::VectorPtr<Ent> && vec, idt (*F)(const Ent &))
        {setChecksById(vec,F);}

    //! Mutateur de la liste des labels.
    void setLabels(const std::map<idt,QString> & map);

    //! Mutateur de la liste des labels.
    template<class Ent> void setLabels(const conteneurMPS::VectorPtr<Ent> & vec,
                                       const QString & (*F)(const Ent &) = [](const Ent & ent)->const QString &{return ent.nom();});

    //! Mutateur de la liste des labels.
    template<class Ent> void setLabels(conteneurMPS::VectorPtr<Ent> && vec,
                                       const QString & (*F)(const Ent &) = [](const Ent & ent)->const QString &{return ent.nom();})
        {setLabels(vec,F);}
};

template<class Ent> template<class Hydrate> void Checklist::Save<Ent>::save(bddMPS::Bdd & bdd, Checklist * list, const Hydrate & hydrate) {
    for (auto i = 0; i != list->count(); ++i){
        Ent ent;
        hydrate(ent,list->id(i));
        if(list->item(i)->checkState() == Qt::Checked)
            bdd.saveUnique(ent);
        else if (bdd.existsUnique(ent))
            bdd.del(ent);
    }
}

template<class Ent> void Checklist::setChecksById(const conteneurMPS::VectorPtr<Ent> & vec, idt (*F)(const Ent &)) {
    for(auto i = 0; i != count(); ++i) {
        auto * it = item(i);
        auto j = vec.cbegin();
        while(j != vec.cend() && F(*j) != it->data(Qt::UserRole))
            j++;
        if(j == vec.cend())
            it->setCheckState(Qt::Unchecked);
        else
            it->setCheckState(Qt::Checked);
    }
}

template<class Ent> void Checklist::setLabels(const conteneurMPS::VectorPtr<Ent> & vec, const QString & (*F)(const Ent &)) {
    for(auto i = vec.cbegin(); i != vec.cend(); ++i) {
        auto * item = new QListWidgetItem(F(*i));
        item->setData(Qt::UserRole,i->id());
        item->setCheckState(Qt::Unchecked);
        addItem(item);
    }
}
}
#endif // CHECKLIST_H
