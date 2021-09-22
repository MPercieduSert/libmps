/*Auteur: PERCIE DU SERT Maxime
 *Date: 16/09/2016
 */
#ifndef MANAGER_FOR_ARBRE_H
#define MANAGER_FOR_ARBRE_H

#include <cstdlib>
#include "entity_divers.h"
#include "manager_sql.h"
#include "unique_sql_base.h"

//! \ingroup groupeManager
//! Coprs des deux methodes save pour les arbres.
#define SAVE_ARBRE if(node.is_valid()) \
    {if(node.is_new()) add(node); \
    else \
        {if(!same_in_bdd(node)) modify(node);}} \
else \
    throw entities::invalide_entity_exception(QString("void manager_for_arbre::save(arbre &)"),node);

//! \ingroup groupeManager
//! Coprs des deux methodes save_st_unstable pour les arbres.
#define SAVESTUNSTABLE_ARBRE if(node.is_valid()) \
    {if(node.is_new()) add_unstable(node); \
    else \
        {if(!same_in_bdd(node)) modify_stable_unstable(node);}} \
else \
    throw entities::invalide_entity_exception(QString("void manager_for_arbre::save_st_unstable(arbre &)"),node);

//! \ingroup groupeManager
//! Coprs des deux methodes save_unstable pour les arbres.
#define SAVEUNSTABLE_ARBRE if(node.is_valid()) \
    {if(node.is_new()) add_unstable(node); \
    else \
        {if(!same_in_bdd(node)) modify_unstable(node);}} \
else \
    throw entities::invalide_entity_exception(QString("void manager_for_arbre::save_unstable(arbre &)"),node);

namespace mps {
namespace manager {
/*! \ingroup groupeManager
 *\brief Classe des manageurs pour les entités de type arbre.
 */
class manager_for_arbre : public manager_sql<entities_base::arbre> {
protected:
    using arbre = entities_base::arbre;
    using ma_sql_arbre = manager_sql<arbre>;
    QString m_get_parent;     //!< Requéte sql de demande de l'identifiant du parent.

public:
    using ma_sql_arbre::get_list;
    using ma_sql_arbre::save;
    using ma_sql_arbre::fonction_agrega;
    using ma_sql_arbre::same_in_bdd;

    //! Constructeur.
    manager_for_arbre(const info_bdd &info);

    //! Destructeur.
    ~manager_for_arbre() override = default;

    //! Teste si le noeud d'identifiant id_node est un descendant du noeud d'identifiant id_ancestry.
    bool are_related(idt id_node, idt id_ancestry) {
        while(id_node != 0 &&id_node != id_ancestry)
            id_node = get_parent(id_node);
        return id_node != 0;
    }

    //! Supprime de la base de donnée le noeud d'identifiant id seulement si c'est une feuille (opération stable).
    bool del(idt id) override;

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui d'identifiant id en
    //! commençant par les descendants.
    template<class Fct> bool foreach_begin_leaf(idt id, const Fct &fct, bool ordre = true);

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendants celui d'identifiant id.
    template<class Fct> bool foreach_node(idt id, const Fct &fct, bool ordre = true);

    //! Returne l'identifiant du parent du noeud d'identifiant id.
    idt get_parent(idt id) {
        prepare(m_get_parent);
        bind_value(0,id);
        exec();
        if(next())
            return value<idt>();
        else
            return 0;
    }

    //! Renvoie le nombre de descendants directs.
    int size(idt id_parent)
        {return fonction_agrega(b2d::agrega::Nombre,arbre::Parent,arbre::Parent,id_parent).toInt();}

    //! Renvoie le nombre de frères.
    int size_brother(idt id) {
        arbre arb(id);
        if(get(arb)) {
            if(arb.parent())
                return size(arb.parent());
            else
                return 1;
        }
        return 0;
    }
    
    //! Teste s'il y a dans la base de donnée un noeud ayant exactement les mêmes attributs id, num et parent.
    bool same_in_bdd(const arbre &node) override {
        arbre node_bdd(node.id());
        if(get(node_bdd))
            return node_bdd.num() == node.num() &&node_bdd.parent() == node.parent();
        else
            return false;
    }

    //! Réimplémentation des la méthode save.
    void save(arbre &node) override
        {SAVE_ARBRE}

    //! Réimplémentation des la méthode save.
    void save(const arbre &node) override
        {SAVE_ARBRE}

    //! Réimplémentation des la méthode save.
    void save_st_unstable(arbre &node)
        {SAVESTUNSTABLE_ARBRE}

    //! Réimplémentation des la méthode save.
    void save_st_unstable(const arbre &node)
        {SAVESTUNSTABLE_ARBRE}

    //! Réimplémentation des la méthode save.
    void save_unstable(arbre &node)
        {SAVEUNSTABLE_ARBRE}

    //! Réimplémentation des la méthode save.
    void save_unstable(const arbre &node)
        {SAVEUNSTABLE_ARBRE}

protected:
    //! Réimplémentation de la méthode add(opération stable).
    void add(arbre &node) override;

    //! Réimplémentation de la méthode add(opération stable).
    void add(const arbre &node) override;

    //! Réimplémentation de la méthode add.
    void add_unstable(arbre &node);

    //! Réimplémentation de la méthode add.
    void add_unstable(const arbre &node);

    //! Modifie en base de données le noeud de de couple (parent, num) identique à node en (parent,-num).
    //! Renvoie false si le noeud de couple (parent, num) correspond à node.
    bool deplace(const arbre &node);

    //! Réimplémentation de la méthode modify.
    void modify(const arbre &node) override;

    //! Réimplémentation de la méthode modify.
    void modify_unstable(const arbre &node);

    //! Réimplémentation de la méthode modify.
    void modify_stable_unstable(const arbre &node);
};

template<class Fct> bool manager_for_arbre::foreach_begin_leaf(idt id, const Fct &fct, bool ordre) {
    vector_ptr<arbre> childs = get_list(arbre::Parent,id,arbre::Num,ordre);
    auto it = childs.cbegin();
    while (it != childs.end() &&
               ((it->feuille() && fct(it->id()))
               || foreach_begin_child(it->id(),fct,ordre)
               ))
           ++it;
    return it == childs.end() && fct(id);
}

template<class Fct> bool manager_for_arbre::foreach_node(idt id, const Fct &fct, bool ordre) {
    if(fct(id)) {
        vector_ptr<arbre> childs = get_list(arbre::Parent,id,arbre::Num,ordre);
        auto i = childs.cbegin();
        while (i != childs.end() (
                    (i->feuille() &&fct(i->id()))
                   || foreach_node(i->id(),fct,ordre)
                    ))
               ++i;
        return i == childs.end();
    }
    return false;
}
}}
#endif // MANAGER_FOR_ARBRE_H
