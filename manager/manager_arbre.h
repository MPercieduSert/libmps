/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef MANAGER_ARBRE_H
#define MANAGER_ARBRE_H

#include "abstract_manager_arbre.h"
#include "manager_for_arbre.h"

namespace mps {
namespace manager {
/*! \ingroup groupeManager
 *\brief Classe template mère des différents manageurs pour les entités de type arbre.
 */
template<class Ent> class manager_arbre : public abstract_manager_arbre<Ent> {
protected:
    manager_for_arbre m_manager_arbre;         //!< Manager de la structure d'arbre.

    using arbre = entities_base::arbre;
    using ma_sql = manager_sql<Ent>;

    using ma_sql::m_link;
    using ma_sql::m_add;
    using ma_sql::exec_finish;

    using ma_sql::prepare;
    using ma_sql::message_erreurs;
    using ma_sql::modify;
    using ma_sql::save_unique;
    using ma_sql::table;

public:
    using ma_sql::del;
    using ma_sql::attribut;
    using ma_sql::exists_unique;
    using ma_sql::exists;
    using ma_sql::get;
    using ma_sql::get_unique;
    using ma_sql::name;
    using ma_sql::nbr_att;
    using ma_sql::same_in_bdd;
    using ma_sql::save;

    //! Constructeur.
    manager_arbre(const info_bdd &info, const info_bdd &info_arbre,
                  std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique
                    = std::make_unique<no_unique_sql<Ent>>());

    //! Destructeur.
    ~manager_arbre() override = default;

    //! Creer la table.
    void creer() override {
        m_manager_arbre.creer();
        ma_sql::creer();
    }

    //! Supprime de la table en base de donnée l'entité d'identifiant id seulement si c'est une feuille.
    bool del(idt id) override {
        arbre node(id);
        if(m_manager_arbre.get(node)) {
            if(node.feuille() &&ma_sql::del(id))
                return  m_manager_arbre.del(id);
            else
                return false;
        }
        else
            return ma_sql::del(id);
    }

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui
    //! d'identifiant id en commençant par les descendants.
    template<class Fct> bool foreach_begin_child(idt id, const Fct &fct, bool ordre = true)
        {return m_manager_arbre.foreach_begin_leaf(id,fct,ordre);}

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant celui d'identifiant id.
    template<class Fct> bool foreach_node(idt id, const Fct &fct, bool ordre = true)
        {return m_manager_arbre.foreach_node(id,fct,ordre);}

    //! Renvoie le descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    Ent get_child(idt id_parent, int num) override {
        arbre node(num, id_parent);
        if(m_manager_arbre.exists_unique(node)){
            Ent ent(node.id());
            get(ent);
            return ent;
        }
        else
            return Ent();
    }

    //! Renvoie l'identifiant du descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    idt get_id_child(idt id_parent, int num) override {
        arbre node(num, id_parent);
        m_manager_arbre.exists_unique(node);
        return node.id();
    }

    //! Renvoie l'identifiant du parent (si le manager est de type arbre).
    idt get_id_parent(idt id) override
        {return m_manager_arbre.get_parent(id);}

    //! Renvoie le liste des descendant direct d'entity.
    vector_ptr<Ent> get_list_childs(const Ent &ent, typename Ent::position ordre = Ent::Id) override {
        return ent.id() ? ma_sql::get_list_join(m_manager_arbre.table(),m_manager_arbre.attribut(arbre::Id),
                                     m_manager_arbre.attribut(arbre::Parent),ent.id(),ordre)
                        : ma_sql::get_list_join(m_manager_arbre.table(),m_manager_arbre.attribut(arbre::Id),
                                                m_manager_arbre.attribut(arbre::Parent),QVariant(QVariant::Int),ordre,b2d::Is);
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id (ordre inactif) .
    std::list<idt> get_list_childs_id(idt id,typename Ent::position /*ordre*/ = Ent::Id) override {
        return id ? m_manager_arbre.get_list_id(arbre::Parent,id,arbre::Num)
                  : m_manager_arbre.get_list_id(arbre::Parent,QVariant(QVariant::Int),arbre::Id,mps::b2d::Is);
    }

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    std::vector<std::pair<idt,bool>> get_list_childs_id_leaf(idt id) override
        {return m_manager_arbre.get_list(arbre::Parent,id,arbre::Num,id ? b2d::Egal : b2d::Is)
                .vector_of([](const arbre &node){return std::pair<idt,bool>(node.id(),node.feuille());});}

    //! Renvoie les informations de la table arbre associée au manager.
    virtual info_bdd info_arbre() const
        {return m_manager_arbre.info();}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(Ent &ent, idt id_parent, int num = 0) override;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const Ent &ent, idt id_parent, int num = 0) override;

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override
        {save_const(ent);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {save_const(ent);}

    //! Renvoie le nombre de frères du noed..
    int size_brother(idt id) override
        {return m_manager_arbre.size_brother(id);}

    //! Renvoie le nombre de fils du noeud.
    int size_child(idt id) override
        {return m_manager_arbre.size(id);}

    //! Retourne le type du manager.
    virtual flag type_manager() const override
        {return b2d::Arbre_Type_Manager;}

protected:
    //! Constructeur.
    manager_arbre(const info_bdd &info_arbre);

    //! Insert la nouvelle entité entity dans la base de donnée.
    void add(const Ent &ent) override {
        prepare(m_add);
        m_link.bind_values(ent);
        m_link.set_id(ent,nbr_att()-1);
        exec_finish();
    }

    //! Teste si le noeud d'identifiant id est une feuille dans la base de donnée.
    //! Retourne vrai s'il n'existe pas.
    bool is_leaf(idt id) override {
        arbre node(id);
        return !m_manager_arbre.get(node) || node.feuille();
    }

    //! Sauve un arbre où le changement de structure consite seulement l'ajout de nouveaux noeuds.
    void save_add_leaf(typename tree<Ent>::iterator iter) override;

    //! Enregistre l'entité entity en base de donnée.
    void save_const(const Ent &ent) {
        if(ent.is_valid()) {
            if(ent.is_new())
                throw std::invalid_argument("Les nouvelles entités de type arbre ne peut être sauvé "
                                            "dans la base de données uniquement à travers un arbre.");
            else
                save_valide(ent);
        }
        else
            throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_const(const ")
                                                     .append(name()).append("&)"),ent);
    }

    //! Sauve un arbre où le changement de structure consite en l'ajout de nouveaux noeuds,
    //! des permutations à l'interieur de l'arbre et le déplasement de noeuds extérieur à l'arbre.
    void save_ext(typename tree<Ent>::iterator iter, idt id_root) override;

    //! Sauve la racine de l'arbre (donnée et structure).
    void save_root(tree<Ent> &tr) override;

    //! Enregistre l'entité valide et d'identifiant non nul entity en base de donnée.
    void save_valide(const Ent &ent) {
        auto controle = b2d::None;
        if(exists(ent)) {
            if(!same_in_bdd(ent)) {
                controle = exists_unique(ent);
                if(controle <= b2d::Meme) {
                    modify(ent);
                    controle = b2d::None;
                }
            }
        }
        else {
            if(exists_unique(ent) == b2d::Aucun) {
                add(ent);
                controle = b2d::None;
            }
        }
        if(controle != b2d::None){
            if(controle == b2d::Autre) {
                Ent ent_bdd(ent);
                ent_bdd.set_id(0);
                get_unique(ent_bdd);
                throw unique_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_valideConst(const")
                                                                            .append(name()).append("&):\n"),ent,ent_bdd);
            }
            else
                throw unique_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_valideConst(const")
                                                                            .append(name()).append("&):\n"),ent);
        }
    }

    //! Sauve un arbre où le changement de structure consite seulement
    //! l'ajout de nouveaux noeuds et des permutations à l'interieur de l'arbre.
    void save_without_delete(typename tree<Ent>::iterator iter) override;

    //! Réécrie la chaine de requète m_add.
    void write_string_sql();
};

template<class Ent> manager_arbre<Ent>::manager_arbre(const info_bdd &info, const info_bdd &info_arbre,
                                                        std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique)
    : ma_sql(info,std::move(unique)),
      m_manager_arbre(info_arbre)
    {write_string_sql();}

template<class Ent> manager_arbre<Ent>::manager_arbre(const info_bdd &info_arbre)
    : m_manager_arbre(info_arbre)
    {write_string_sql();}



template<class Ent> void manager_arbre<Ent>::insert(Ent &ent, idt id_parent, int num) {
    if(ent.is_valid()) {
        if(ent.is_new()) {
            arbre node(num,id_parent);
            m_manager_arbre.save(node);
            ent.set_id(node.id());
        }
        else
            m_manager_arbre.save(arbre(num,id_parent,ent.id()));
        save_valide(ent);
    }
    else
        throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save(")
                                                 .append(name()).append("&,const ").append(name())
                                                 .append(" &,int)"),ent);
}

template<class Ent> void manager_arbre<Ent>::insert(const Ent &ent, idt id_parent, int num) {
    if(ent.is_valid()) {
        if(ent.is_new()) {
            arbre node(num,id_parent);
            m_manager_arbre.save(node);
            Ent entity_temp(ent);
            entity_temp.set_id(node.id());
            save_valide(entity_temp);
        }
        else {
            m_manager_arbre.save(arbre(num,id_parent,ent.id()));
            save_valide(ent);
        }
    }
    else
        throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save(const ")
                                                 .append(name()).append("&,const ").append(name())
                                                 .append(" &,int)"),ent);
}

template<class Ent> void manager_arbre<Ent>::save_add_leaf(typename tree<Ent>::iterator iter) {
    auto i = 0;
    for(auto child = iter.begin_child(); iter; ++i, ++child) {
        if(child->is_valid()) {
            if(child->is_new()) {
                arbre node(i,iter->id());
                m_manager_arbre.save(node);
                child->set_id(node.id());
            }
            save_valide(*child);
            if(!child.leaf())
                save_add_leaf(child);
        }
        else
            throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_add_leaf(tree<")
                                                     .append(name()).append(">::iterator)"),*child);
    }
}

template<class Ent> void manager_arbre<Ent>::save_root(tree<Ent> &tr) {
    auto it = tr.begin();
    if(it->is_valid()) {
        if(it->is_new()) {
            arbre node(0,0);
            m_manager_arbre.save(node);
            it->set_id(node.id());
        }
        save_valide(*it);
    }
    else
        throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_root(tree<")
                                                 .append(name()).append("> &)"),*it);
}

template<class Ent> void manager_arbre<Ent>::save_without_delete(typename tree<Ent>::iterator iter) {
    auto i = 0;
    for(auto child = iter.begin_child(); child; ++i, ++child) {
        if(child->is_valid()) {
            arbre node(i,iter->id(),child->id());
            m_manager_arbre.save_unstable(node);
            if(child->is_new())
                child->set_id(node.id());
            save_valide(*child);
            if(!child.leaf())
                save_without_delete(child);
        }
        else
            throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_without_delete(tree<")
                                                     .append(name()).append(">::iterator)"),*child);
    }
}

template<class Ent> void manager_arbre<Ent>::save_ext(typename tree<Ent>::iterator iter, idt id_root) {
    auto i = 0;
    for(auto child = iter.begin_child(); child; ++i, ++child) {
        if(child->is_valid()) {
            arbre node(i,iter->id(),child->id());
            if(node.id() == 0 || m_manager_arbre.are_related(iter->id(),id_root))
                m_manager_arbre.save_unstable(node);
            else
                m_manager_arbre.save_st_unstable(node);
            if(child->is_new())
                child->set_id(node.id());
            save_valide(*child);
            if(!child.leaf())
                save_ext(child,id_root);
        }
        else
            throw entities::invalide_entity_exception(QString("void manager_arbre<").append(name()).append(">::save_ext(tree<")
                                                     .append(name()).append(">::iterator,idt)"),*child);
    }
}

template<class Ent> void manager_arbre<Ent>::write_string_sql()
{
    m_add.clear();
    m_add.append("INSERT INTO ").append(table()).append("(");
    for(post i = 1; i != nbr_att(); ++i)
        m_add.append(attribut(i)).append(",");
    m_add.append(attribut(Ent::Id));
    m_add.append(") VALUES(");
    for(post i = 0; i != nbr_att(); ++i)
        m_add.append("?,");
    m_add.chop(1);
    m_add.append(")");
    m_add.squeeze();
}
}}
#endif // MANAGER_ARBRE_H
