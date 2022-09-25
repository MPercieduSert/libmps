/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef PREDEF_MODEL_H
#define PREDEF_MODEL_H

#include "item_node_model.h"
#include "bdd_predef.h"

namespace mps {
namespace model_base {
/*! \ingroup groupe_model
 *\brief Model de gestion des permissions.
 */
class permission_model : public item_node_bdd_model {
    Q_OBJECT
protected:
    std::map<numt,QString> m_id_nom;   //!< Vecteur d'information sur les cibles (cible en bdd, info).

public:
    enum {Type_Node = 1,
          Nc_Nom_Offset = 2,
          Nc_Nom_Ref_Offset = 3};

    //! Cible des données du model.
    enum data_cible {
        Nc_Cible,
        Nom_Cible,
        Ref_Cible,
        Permission_Cible};

    //! Constructeur.
    permission_model(b2d::bdd_predef &bdd, QObject *parent = nullptr);

    //! Accesseur de la base de données.
    b2d::bdd_predef &bdd() const
        {return static_cast<b2d::bdd_predef &>(m_bdd);}

    //! Nombre de cible.
    szt cible_size() const
        {return m_id_nom.size();}

    //! Hydrate les permissions d'un noeud.
    void hydrate_permission(const node_index &index);

    //! Accesseur de la liste des cibles.
    const auto &id_nom() const noexcept
        {return m_id_nom;}

    //! Nom d'une cible.
    const QString &nom_cible(numt num) const
        {return m_id_nom.at(num);}

//    //! Muateur de la présence d'une cible.
//    void set_cible(entidt num, bool visible);
};

/*! \ingroup groupe_model
 *\brief Model interface de gestion des cibles de permissions.
 */
class cible_permission_interface_model : public current_index_interface_node_model {
    Q_OBJECT
public:
    //! Cible des données du model.
    enum data_cible {
        Nom_Cible,
        Permission_Cible};

    //! Constructeur.
    cible_permission_interface_model(permission_model *model, QObject *parent = nullptr);

    //! Fabrique des noeuds.
    node_ptr node_factory(const node_index &/*parent*/, numt /*pos*/, int /*type*/) override;

    //! Acceseur du model.
    permission_model *model() const override
        {return static_cast<permission_model *>(m_model);}

public slots:
    //! Mutateur de l'index.
    void set_current(const model_base::node_index &current,
                     const model_base::node_index &previous = node_index()) override {
        model()->hydrate_permission(current);
        current_index_interface_node_model::set_current(current, previous);
    }
};

/*! \ingroup groupe_model
 *\brief Classe des neuds du model d'interface des cibles de permission.
 */
class cible_permission_interface_node : public item_node {
protected:
    numt m_cible;                                   //! Numero de cible.
    cible_permission_interface_model *m_model;      //! Pointeur sur le model.

public:
public:
    enum {Type_Node = 1};

    //! Constructeur.
    cible_permission_interface_node(cible_permission_interface_model *model)
        : item_node(cible_permission_interface_node::Type_Node), m_model(model) {}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Mutateur de la cible.
    void set_cible(numt cible)
        {m_cible = cible;}
};

/*! \ingroup groupe_model
 *\brief Classe abstraite mère des neuds d'un model de permission.
 */
class abstract_permission_node : public item_bdd_node {
protected:
    std::map<numt,flag> m_permission_map;       //!< Map des permission du type.
    permission_model *m_model;                  //!< Pointeur sur le model.
public:
    enum {NoFlag = 0};
    //! Constructeur.
    abstract_permission_node(permission_model *model)
        : item_bdd_node(permission_model::Type_Node), m_model(model) {}

    //! Constructeur.
    using item_bdd_node::item_bdd_node;

    //! Hydrate les permissions.
    virtual void hydrate_permission(bool if_empty = true) = 0;

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;
};

/*! \ingroup groupe_model
 *\brief Classe mère des neuds d'un model de permission.
 */
template<class Ent, class Permission> class permission_node : public abstract_permission_node {
protected:
    Ent m_ent;                              //!< Type associé au noeud.
public:
    //! Constructeur.
    using abstract_permission_node::abstract_permission_node;

    NODE_COPIE(permission_node)

    //! Ajoute une cible aux permissions.
    void hydrate_permission(bool if_empty = true) override;

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Revoie l'identifiant de l'entiée.
    idt id_ent() const noexcept
        {return m_ent.id();}

    //! Enregistre les données du noeud.
    void save(b2d::bdd &bd) override{
        bd.save(m_ent);
        save_permission(bd);
    }

    //! Enregistre les données du noeud.
    void save_permission(b2d::bdd &bdd);

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Mutateur de l'entité.
    void set_ent(const Ent &ent);
};

/*! \ingroup groupe_model
 *\brief Classe des neuds du model de permission pour une entité de type arbre.
 */
template<class Ent, class Permission> class arbre_permission_node : public permission_node<Ent,Permission> {
protected:
    using perm_node = permission_node<Ent,Permission>;
    using perm_node::m_iter;
    using perm_node::m_ent;
    using perm_node::save_permission;
public:
    //! Constructeur.
    using perm_node::permission_node;

    NODE_COPIE(arbre_permission_node)

    //! Enregistre les données du noeud.
    void insert(b2d::bdd &bd) override{
        idt id_parent = 0;
        if(!m_iter.parent().root())
            id_parent = static_cast<const perm_node &>(**m_iter.parent()).id_ent();
        bd.insert(m_ent,id_parent,m_iter.position());
        save_permission(bd);
    }
};

/*! \ingroup groupe_model
 *\brief Classe des neuds du model de permission pour une entité de type arbre simple.
 */
template<class Ent, class Permission> class arbre_simple_permission_node : public permission_node<Ent,Permission> {
protected:
    using perm_node = permission_node<Ent,Permission>;
    using perm_node::m_iter;
    using perm_node::m_ent;
    using perm_node::save_permission;
public:
    //! Constructeur.
    using perm_node::permission_node;

    NODE_COPIE(arbre_simple_permission_node)

    //! Enregistre les données du noeud.
    void insert(b2d::bdd &bd) override{
        if(!m_iter.parent().root())
            m_ent.set_parent(static_cast<const perm_node &>(**m_iter.parent()).id_ent());
        bd.save(m_ent);
        save_permission(bd);
    }
};

/*! \ingroup groupe_model
 *\brief Model de gestion des mots clés et de leur permission.
 */
class mot_cle_permission_model : public permission_model {
    Q_OBJECT
public:
    using mot_cle_permission_node = arbre_permission_node<entities::mot_cle,entities::mot_cle_permission>;
    //! Constructeur.
    mot_cle_permission_model(b2d::bdd_predef &bdd, QObject *parent = nullptr);

    //! Fabrique des noeuds.
    node_ptr node_factory(const node_index &/*parent*/, numt /*pos*/, int /*type*/) override
        {return std::make_unique<mot_cle_permission_node>(this);}
};

/*! \ingroup groupe_model
 *\brief Model de gestion des type et de leur permission.
 */
class type_permission_model : public permission_model {
    Q_OBJECT
public:
    using type_permission_node = arbre_simple_permission_node<entities::type,entities::type_permission>;
    //! Constructeur.
    type_permission_model(b2d::bdd_predef &bdd, QObject *parent = nullptr);

    //! Fabrique des noeuds.
    node_ptr node_factory(const node_index &/*parent*/, numt /*pos*/, int /*type*/) override
        {return std::make_unique<type_permission_node>(this);}
};

///////////////////////////////////////////////// definition permission_node ///////////////////////////////////
template<class Ent, class Permission> void permission_node<Ent,Permission>::hydrate_permission(bool if_empty) {
    if(!if_empty || m_permission_map.empty()){
        for(auto iter = m_model->id_nom().cbegin(); iter != m_model->id_nom().cend(); ++iter) {
            Permission perm;
            perm.set_cible(static_cast<int>(iter->first));
            perm.set_id_1(m_ent.id());
            if(m_model->bdd().get_unique(perm))
                m_permission_map[iter->first] = perm.code();
        }
    }
}

template<class Ent, class Permission> QVariant permission_node<Ent,Permission>::data(int cible, int role, numt num) const {
    if(role == String_Role){
        switch (cible) {
        case permission_model::Nc_Cible:
            return m_ent.nc();
        case permission_model::Nom_Cible:
            return m_ent.nom();
        case permission_model::Ref_Cible:
            return m_ent.ref();
        }
    }
    return abstract_permission_node::data(cible,role,num);
}

template<class Ent, class Permission> flag permission_node<Ent,Permission>::flags(int cible, numt num) const {
    if(m_iter.root())
        return No_Flag_Node;
    auto flag = abstract_permission_node::flags(cible,num);
    if(cible == Node_Cible || cible == permission_model::Nom_Cible
            || cible == permission_model::Nc_Cible || cible == permission_model::Ref_Cible) {
        entities::restriction restriction;
        restriction.set_cible(m_model->bdd().cible(Ent::ID));
        restriction.set_id_cible(m_ent.id());
        m_model->bdd().get_unique(restriction);
        if(restriction.code().test(b2d::Modif)
                && (cible == permission_model::Nom_Cible || cible == permission_model::Nc_Cible
                    || cible == permission_model::Ref_Cible))
            flag ^= Enable_Flag_Node;
        if(restriction.code().test(b2d::Suppr) &&(cible == Node_Cible))
            flag ^= Del_Enable_Flag_Node;
    }
    if(cible == permission_model::Permission_Cible){
        Permission perm;
        perm.set_cible(static_cast<int>(num));
        perm.set_id_1(m_ent.id());
        if(m_model->bdd().exists_unique(perm)) {
            entities::restriction restriction;
            restriction.set_cible(m_model->bdd().cible(Permission::ID));
            restriction.set_id_cible(perm.id());
            m_model->bdd().get_unique(restriction);
            if(restriction.code().test(b2d::Modif))
                flag ^= Enable_Flag_Node;
        }
    }
    return flag;
}

template<class Ent, class Permission> void permission_node<Ent,Permission>::save_permission(b2d::bdd &bdd) {
    for(auto iter = m_permission_map.cbegin(); iter != m_permission_map.cend(); ++iter) {
        Permission perm;
        perm.set_id_1(m_ent.id());
        perm.set_cible(static_cast<int>(iter->first));
        perm.set_code(iter->second);
        bdd.save(perm);
    }
}

template<class Ent, class Permission> flag permission_node<Ent,Permission>::
                set_data(int cible, const QVariant &value, int role, numt num) {
    switch (cible) {
    case permission_model::Nc_Cible:
        if(role == String_Role) {
            m_ent.set_nc(value.toString());
            return Main_Same_Change_Flag;
        }
        break;
    case permission_model::Nom_Cible:
        if(role == String_Role) {
            m_ent.set_nom(value.toString());
            return Main_Same_Change_Flag;
        }
        break;
    case permission_model::Ref_Cible:
        if(role == String_Role) {
            m_ent.set_ref(value.toString());
            return Main_Same_Change_Flag;
        }
        break;
    case permission_model::Permission_Cible:
        if(role == Num_Role) {
            m_permission_map[num] = value.toUInt();
            return Main_Same_Change_Flag;
        }
        break;
    }
    return item_node::set_data(cible,value,role,num);
}

template<class Ent, class Permission> void permission_node<Ent,Permission>::set_ent(const Ent &ent) {
    m_ent = ent;
    for(auto iter = m_permission_map.begin(); iter != m_permission_map.end(); ++iter) {
        Permission perm;
        perm.set_id_1(ent.id());
        perm.set_cible(iter->first);
        m_model->bdd().get_unique(perm);
        iter->second = perm.code();
    }
}
}}
#endif // PREDEF_MODEL_H
