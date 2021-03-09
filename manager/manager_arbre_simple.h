/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGER_ARBRE_SIMPLE_H
#define MANAGER_ARBRE_SIMPLE_H

#include "abstract_manager_arbre.h"

namespace mps {
namespace manager {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbreSimple.
 */
template<class Ent> class manager_arbre_simple : public abstract_manager_arbre<Ent> {
protected:
    using ma_sql = manager_sql<Ent>;
public:
    using abstract_manager_arbre<Ent>::abstract_manager_arbre;
    using ma_sql::exists;
    using ma_sql::get;
    using ma_sql::get_list;
    using ma_sql::get_list_id;
    using ma_sql::name;

    //! Destructeur.
    ~manager_arbre_simple() override = default;

    //! Supprime de la base de donnée le noeud d'identifiant id ainsi que tous ses déscendants (opération stable).
    bool del(idt id) override;

    //! Renvoie le descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    Ent get_child(idt id_parent, int num) override {
        auto ids = get_list_childs_id(id_parent);
        Ent ent;
        if(!ids.empty()) {
            if(num == 0)
                ent.set_id(ids.front());
            if(num >= 0 && static_cast<szt>(num) < ids.size()) {
                auto it = std::next(ids.cbegin(),num);
                ent.set_id(*it);
            }
            else if(num < 0 && static_cast<szt>(-num) <= ids.size()) {
                auto it = std::next(ids.crbegin(),-num);
                ent.set_id(*it);
            }
            if(ent.id())
                get(ent);
        }
        return ent;
    }

    //! Renvoie l'identifiant du descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    idt get_id_child(idt id_parent, int num) override {
        auto ids = get_list_childs_id(id_parent);
        if(!ids.empty()) {
            if(num == 0)
                return ids.front();
            if(num == -1)
                return ids.back();
            if(num >= 0 && static_cast<szt>(num) < ids.size()) {
                auto it = std::next(ids.cbegin(),num);
                return *it;
            }
            else if(num < 0 && static_cast<szt>(-num) <= ids.size()) {
                auto it = std::next(ids.crbegin(),-num);
                return *it;
            }
        }
        return 0;
    }

    //! Renvoie le liste des descendant direct d'entity.
    vector_ptr<Ent> get_list_childs(const Ent &ent, typename Ent::position ordre = Ent::Ordre) override {
        return ent.id() ? get_list(Ent::Parent,ent.id(),ordre)
                        : get_list(Ent::Parent,QVariant(QVariant::Int),ordre,mps::b2d::Is);
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    std::list<idt> get_list_childs_id(idt id,typename Ent::position ordre = Ent::Ordre) override{
        return id ? get_list_id(Ent::Parent,id,ordre)
                  : get_list_id(Ent::Parent,QVariant(QVariant::Int),ordre,mps::b2d::Is);
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    std::vector<std::pair<idt,bool>> get_list_childs_id_leaf(idt id) override {
        auto childs = get_list_childs_id(id);
        std::vector<std::pair<idt,bool>> liste(childs.size());
        auto it_list = liste.begin();
        for(auto it = childs.begin(); it != childs.end(); ++it) {
            it_list->first = *it;
            it_list->second = exists(Ent::Parent,*it);
        }
        return liste;
    }

    //! Teste si le noeud d'identifiant id est une feuille dans la base de donnée.
    //! Retourne vrai s'il n'existe pas.
    bool is_leaf(idt id) override
        {return !exists(Ent::Parent,id);}

    //! Test la validité de la paternité.
    bool is_valid_parent(const Ent &ent)
        {return ent.parent() == 0 || exists(Ent(ent.parent()));}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override {
        exception_parent_invalid(ent);
        ma_sql::save(ent);
    }

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override {
        exception_parent_invalid(ent);
        ma_sql::save(ent);
    }

    //! Retourne le type du manager.
    virtual flag type_manager() const
        {return b2d::Arbre_Simple_Type_Manager;}

protected:
    //! Lance une execption si la paternité n'est pas valide.
    void exception_parent_invalid(const Ent &ent) {
        if(!is_valid_parent(ent))
            throw entities::invalide_entity_exception(QString("void manager_arbre_simple<").append(name())
                                                     .append(">::exception_parent_invalid(const ").append(name()).append(" &)")
                                                     .append("L'entité transmise en argument de get_arbre n'est pas valide, "
                                                             "le parent spécifié n'existe pas."),ent);
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    virtual void save_ext(typename tree<Ent>::iterator /*iter*/, idt /*id_root*/) override {}

    //! Sauve la racine de l'arbre (donnée et structure).
    void save_root(tree<Ent> &tr) override
        {save(*tr.begin());}

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds
    //! et des permutations à l'interieur de l'arbre.
    void save_without_delete(typename tree<Ent>::iterator iter) override;
};

template<class Ent> bool manager_arbre_simple<Ent>::del(idt id) {
    if(!exists(Ent::Parent,id))
        return ma_sql::del(id);
    else
        return false;
    /*list_ptr<Ent> childs = get_list(Ent::Parent,id);
    typename list_ptr<Ent>::iterator i = childs.begin();
    while(i != childs.end() &&del((*i).id()))
        ++i;
    if(i == childs.end())
        return ma_sql::del(id);
    else
        return false;*/
}

template<class Ent> void manager_arbre_simple<Ent>::save_without_delete(typename tree<Ent>::iterator it) {
    for(auto child = it.begin_child(); child; ++child) {
        child->set_parent(it->id());
        save(*child);
        save_without_delete(child);
    }
}
}}
#endif // MANAGER_ARBRE_SIMPLE_H
