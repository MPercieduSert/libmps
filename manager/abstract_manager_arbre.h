/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/11/2019
 */
#ifndef ABSTRACT_MANAGER_ARBRE_H
#define ABSTRACT_MANAGER_ARBRE_H

#include "manager_sql.h"
#include "tree.h"

namespace mps {
namespace manager {
/*! \ingroup groupeManager
 *\brief Classe template abstraite parent des différents manageurs pour les entités de type arbre.
 */
template<class Ent> class abstract_manager_arbre : public virtual manager_sql<Ent> {
protected:
    using ma_sql = manager_sql<Ent>;
    using ma_sql::del;
public:
    using ma_sql::manager_sql;
    using ma_sql::get_list_childs_id;
    using ma_sql::get;
    using ma_sql::save;

    //! Renvoie l'arbre de toutes les entités.
    tree<Ent> get_arbre() override {
        auto racines = get_list_racines_id();
        tree<Ent> tree;
        auto it = tree.begin();
        for(auto it_racine = racines.cbegin(); it_racine != racines.cend(); ++it_racine)
            get(*tree.emplace_back(it,*it_racine));
        while(it){
            if(!opti_leaf(it->id())){
                auto listChilds = get_list_childs_id(it->id());
                for(auto it_child = listChilds.cbegin(); it_child != listChilds.cend(); ++it_child)
                    get(*tree.emplace_back(it,*it_child));
            }
            ++it;
        }
        return tree;
    }

    //! Renvoie l'arbre de racine entity.
    tree<Ent> get_arbre(const Ent &ent) override
        {return get_arbre(ent.id());}

    //! Renvoie l'arbre l'entity d'identifiant.
    tree<Ent> get_arbre(idt id) override {
        Ent ent(id);
        if(get(ent)) {
            tree<Ent> tree(ent);
            auto it = tree.begin();
            while(it){
                if(!opti_leaf(it->id())){
                    auto listChilds = get_list_childs_id(it->id());
                    for(auto it_child = listChilds.cbegin(); it_child != listChilds.cend(); ++it_child)
                        get(*tree.emplace_back(it,*it_child));
                }
                ++it;
            }
            return tree;
        }
        else
            throw std::invalid_argument("L'identifiant transmise en argument de get_arbre ne correspond à aucune entité.");
    }

    //! Renvoie la liste des identifiants des racines.
    virtual std::list<idt> get_list_racines_id() = 0;

    //! Enregistre l'arbre d'entités.
    void save(tree<Ent> &tree, b2d::tree_save n = b2d::tree_save::External_Change) override;

protected:
    //! Supprime de la Base de données les noeuds hors de l'arbre.
    virtual void delete_leaf_out_of(typename tree<Ent>::iterator it);

    //! Teste si le noeud d'identifiant id est une feuille dans la base de donnée en vue d'optimisation.
    virtual bool opti_leaf(idt /*id*/)
        {return false;}

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds.
    virtual void save_add_leaf(typename tree<Ent>::iterator it)
        {save_without_delete(it);}

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    virtual void save_ext(typename tree<Ent>::iterator it, idt id_root) = 0;

    //! Sauve la racine de l'arbre (donnée et structure).
    virtual void save_root(tree<Ent> &tr) = 0;

    //! Sauve un arbre où le changement de structure consite seulement
    //! l'ajout de nouveaux noeuds et des permutations à l'interieur de l'arbre.
    virtual void save_without_delete(typename tree<Ent>::iterator it) = 0;
};

template<class Ent> void abstract_manager_arbre<Ent>::delete_leaf_out_of(typename tree<Ent>::iterator it) {
    while(it) {
        if(it.leaf()) {
            auto childs = get_list_childs_id(it->id());
            for(auto i = childs.cbegin(); i != childs.cend(); ++i)
                del(*i);
        }
//        else {
//            auto childs = get_list_childs_id(it->id());
//            for(auto i = childs.cbegin(); i != childs.cend(); ++i){
//                if(i->num() < 0 || i->num() >= it->size_child())
//                    del(*i);
//            }
//        }
        ++it;
    }
}

template<class Ent> void abstract_manager_arbre<Ent>::save(tree<Ent> &tr, b2d::tree_save n) {
    if(n == b2d::tree_save::Entity_Only) {
        for(auto it = tr.begin(); it ; ++it)
            save(*it);
    }
    else if(n == b2d::tree_save::Entity_Only_Whitout_Root) {
        auto it = tr.begin();
        ++it;
        while (it) {
            save(*it);
            ++it;
        }
    }
    else {
        if(n < b2d::tree_save::Entity_Only_Whitout_Root)
            save_root(tr);
        switch (n) {
        case b2d::tree_save::Add_Leaf:
        case b2d::tree_save::Add_Leaf_Whitout_Root:
            save_add_leaf(tr.begin());
            break;

        case b2d::tree_save::Without_Delete:
        case b2d::tree_save::Without_Delete_Whitout_Root:
            save_without_delete(tr.begin());
            break;

        case b2d::tree_save::Internal_Change:
        case b2d::tree_save::Internal_Change_Whitout_Root:
            save_without_delete(tr.begin());
            delete_leaf_out_of(tr.begin());
            break;

        case b2d::tree_save::External_Change:
        case b2d::tree_save::External_Change_Whitout_Root:
            save_ext(tr.begin(),tr.begin()->id());
            delete_leaf_out_of(tr.begin());
            break;

        case b2d::tree_save::Entity_Only:
        case b2d::tree_save::Entity_Only_Whitout_Root:
            break;
        }
    }
}
}}
#endif // ABSTRACTMANAGERARBRE_H
