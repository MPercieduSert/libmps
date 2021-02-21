/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef PREDEFMODEL_H
#define PREDEFMODEL_H

#include "ItemNodeModel.h"
#include "BddPredef.h"

namespace modelMPS {
/*! \ingroup groupeModel
 *\brief Model de gestion des types.
 */
class PermissionModel : public ItemNodeBddModel {
    Q_OBJECT
protected:
    const enumt m_offset;                               //!< Offset de postion des cibles.
    std::vector<entidt> m_cibleVec;                     //!< Vecteur de cibles à afficher.
    std::vector<std::pair<int,QString>> m_idNomVec;     //!< Vecteur d'information sur les cibles (cible en Bdd, info).

public:
    enum {TypeNode = 1,
         nc_nomOffset = 2,
         nc_nom_refOffset = 3};
    //! Cible des données du model.
    enum dataCible {NcCible,
                    NomCible,
                    RefCible,
                    PermissionCible};
    //! position des sous-noeud.
    enum positionNode{Zeroposition,
                      Unposition,
                      Deuxposition,
                      Nomposition = Zeroposition,
                      Ncposition = Unposition,
                      Refposition = Deuxposition};
    //! Constructeur.
    PermissionModel(bddMPS::BddPredef &bdd, enumt offset = nc_nomOffset, QObject *parent = nullptr);

    //! Accesseur de la base de données.
    bddMPS::BddPredef &bdd() const
        {return static_cast<bddMPS::BddPredef &>(m_bdd);}

    //! Nombre de donné associé à une cible.
    numt dataCount(const NodeIndex &index) const override;

    //! Nom d'une cible.
    const QString &nomCible(entidt num) const
        {return m_idNomVec.at(m_cibleVec.at(num)).second;}

    //! Numero de cible.
    int cible(entidt num) const
        {return m_idNomVec.at(m_cibleVec.at(num)).first;}

    //! Accesseur de la liste des cibles.
    const std::vector<std::pair<int,QString>> &idNomVec() const noexcept
        {return m_idNomVec;}

    //! Accesseur de l'offset de postion des cibles
    enumt offset() const noexcept
        {return m_offset;}

    //! Muateur de la présence d'une cible.
    void set_cible(entidt num, bool visible);
};

/*! \ingroup groupeModel
 *\brief Classe abstraite mère des neuds d'un model de permission.
 */
class AbstractPermissionNode : public ItemBddNode {
public:
    //! Constructeur.
    using ItemBddNode::ItemBddNode;

    //! Ajoute une cible aux permissions.
    virtual void addCible(int cible) = 0;
};

/*! \ingroup groupeModel
 *\brief Classe mère des neuds d'un model de permission.
 */
template<class Ent, class Permission> class PermissionNode : public AbstractPermissionNode {
protected:
    Ent m_ent;                              //!< Type associé au noeud.
    std::map<int,flag> m_permissionMap;     //!< Map des permission du type.
    PermissionModel *m_model;              //!< Pointeur sur le model.
public:
    enum {NoFlag = 0};
    //! Constructeur.
    PermissionNode(PermissionModel *model)
        : AbstractPermissionNode(PermissionModel::TypeNode), m_model(model) {}

    NODE_COPIE(PermissionNode)

    //! Ajoute une cible aux permissions.
    void addCible(int cible) override;

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, numt num = 0) const override;

    //! Accesseur des drapeaux associés à column.
    flag flags(int cible, numt num = 0) const override;

    //! Revoie l'identifiant de l'entiée.
    idt idEnt() const noexcept
        {return m_ent.id();}

    //! Enregistre les données du noeud.
    void save(bddMPS::Bdd &bdd) override{
        bdd.save(m_ent);
        savePermission(bdd);
    }

    //! Enregistre les données du noeud.
    void savePermission(bddMPS::Bdd &bdd);

    //! Mutateur des données du noeud.
    flag set_data(int cible, const QVariant &value, int role, numt num = 0) override;

    //! Mutateur de l'entité.
    void setEnt(const Ent &ent);
};

/*! \ingroup groupeModel
 *\brief Classe des neuds du model de permission pour une entité de type arbre.
 */
template<class Ent, class Permission> class arbrePermissionNode : public PermissionNode<Ent,Permission> {
protected:
    using PermNode = PermissionNode<Ent,Permission>;
    using PermNode::m_iter;
    using PermNode::m_ent;
    using PermNode::savePermission;
public:
    //! Constructeur.
    using PermNode::PermissionNode;

    NODE_COPIE(arbrePermissionNode)

    //! Enregistre les données du noeud.
    void insert(bddMPS::Bdd &bdd) override{
        idt idParent = 0;
        if(!m_iter.parent().root())
            idParent = static_cast<const PermNode &>(**m_iter.parent()).idEnt();
        bdd.insert(m_ent,idParent,m_iter.position());
        savePermission(bdd);
    }
};

/*! \ingroup groupeModel
 *\brief Classe des neuds du model de permission pour une entité de type arbre simple.
 */
template<class Ent, class Permission> class arbre_simplePermissionNode : public PermissionNode<Ent,Permission> {
protected:
    using PermNode = PermissionNode<Ent,Permission>;
    using PermNode::m_iter;
    using PermNode::m_ent;
    using PermNode::savePermission;
public:
    //! Constructeur.
    using PermNode::PermissionNode;

    NODE_COPIE(arbre_simplePermissionNode)

    //! Enregistre les données du noeud.
    void insert(bddMPS::Bdd &bdd) override{
        if(!m_iter.parent().root())
            m_ent.set_parent(static_cast<const PermNode &>(**m_iter.parent()).idEnt());
        bdd.save(m_ent);
        savePermission(bdd);
    }
};

/*! \ingroup groupeModel
 *\brief Model de gestion des type et de leur permission.
 */
class mot_cle_permissionModel : public PermissionModel {
    Q_OBJECT
public:
    using mot_cle_permissionNode = arbrePermissionNode<entities::mot_cle,entities::mot_cle_permission>;
    //! Constructeur.
    mot_cle_permissionModel(bddMPS::BddPredef &bdd, QObject *parent = nullptr);

    //! Fabrique des noeuds.
    Node nodeFactory(const NodeIndex &/*parent*/, numt /*pos*/, int /*type*/) override
        {return std::make_unique<mot_cle_permissionNode>(this);}
};

/*! \ingroup groupeModel
 *\brief Model de gestion des type et de leur permission.
 */
class type_permissionModel : public PermissionModel {
    Q_OBJECT
public:
    using type_permissionNode = arbre_simplePermissionNode<entities::Type,entities::type_permission>;
    //! Constructeur.
    type_permissionModel(bddMPS::BddPredef &bdd, QObject *parent = nullptr);

    //! Fabrique des noeuds.
    Node nodeFactory(const NodeIndex &/*parent*/, numt /*pos*/, int /*type*/) override
        {return std::make_unique<type_permissionNode>(this);}
};

///////////////////////////////////////////////// definition PermissionNode ///////////////////////////////////
template<class Ent, class Permission> void PermissionNode<Ent,Permission>::addCible(int cible) {
    if(m_permissionMap.find(cible) == m_permissionMap.end()) {
        Permission perm;
        perm.set_cible(cible);
        perm.set_id_1(m_ent.id());
        if(m_model->bdd().getUnique(perm))
            m_permissionMap[cible] = perm.code();
        else
            m_permissionMap[cible] = NoFlag;
    }
}
template<class Ent, class Permission> QVariant PermissionNode<Ent,Permission>::data(int cible, int role, numt num) const {
    switch (cible) {
    case PermissionModel::NcCible:
        if(role == LabelRole)
            return "Nom abrégé :";
        if(role == StringRole)
            return m_ent.nc();
        break;
    case PermissionModel::NomCible:
        if(role == LabelRole)
            return "Nom :";
        if(role == StringRole)
            return m_ent.nom();
        break;
    case PermissionModel::RefCible:
        if(role == LabelRole)
            return "Référence :";
        if(role == StringRole)
            return m_ent.ref();
        break;
    case PermissionModel::PermissionCible:
        if(role == LabelRole)
            return m_model->nomCible(num);
        if(role == NumRole)
            return m_permissionMap.at(m_model->cible(num)).value();
        break;
    case SubNodeCible:
        if(role == SubNodeRole){
            QList<QVariant> init;
            if(num >= m_model->offset()) {
                init.append(PermissionModel::PermissionCible);
                init.append(num - m_model->offset());
                init.append(CodeSubNode);
                return init;
            }
            switch (num) {
            case PermissionModel::Nomposition:
                init.append(PermissionModel::NomCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            case PermissionModel::Ncposition:
                init.append(PermissionModel::NcCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            case PermissionModel::Refposition:
                init.append(PermissionModel::RefCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
        }
    }
    return ItemNode::data(cible,role,num);
}

template<class Ent, class Permission> flag PermissionNode<Ent,Permission>::flags(int cible, numt num) const {
    if(m_iter.root())
        return NoFlagNode;
    auto flag = AbstractPermissionNode::flags(cible,num);
    if(cible == NodeCible || cible == PermissionModel::NomCible
            || cible == PermissionModel::NcCible || cible == PermissionModel::RefCible) {
        entities::Restriction restriction;
        restriction.set_cible(m_model->bdd().cible(Ent::ID));
        restriction.set_id_cible(m_ent.id());
        m_model->bdd().getUnique(restriction);
        if(restriction.code().test(bddMPS::Modif)
                &&(cible == PermissionModel::NomCible || cible == PermissionModel::NcCible || cible == PermissionModel::RefCible))
            flag ^= EnableFlagNode;
        if(restriction.code().test(bddMPS::Suppr) &&(cible == NodeCible))
            flag ^= DelEnableFlagNode;
    }
    if(cible == PermissionModel::PermissionCible){
        Permission perm;
        perm.set_cible(m_model->cible(num));
        perm.set_id_1(m_ent.id());
        if(m_model->bdd().existsUnique(perm)) {
            entities::Restriction restriction;
            restriction.set_cible(m_model->bdd().cible(Permission::ID));
            restriction.set_id_cible(perm.id());
            m_model->bdd().getUnique(restriction);
            if(restriction.code().test(bddMPS::Modif))
                flag ^= EnableFlagNode;
        }
    }
    return flag;
}
template<class Ent, class Permission> void PermissionNode<Ent,Permission>::savePermission(bddMPS::Bdd &bdd) {
    for(auto iter = m_permissionMap.cbegin(); iter != m_permissionMap.cend(); ++iter) {
        Permission perm;
        perm.set_id_1(m_ent.id());
        perm.set_cible(iter->first);
        perm.set_code(iter->second);
        bdd.save(perm);
    }
}

template<class Ent, class Permission> flag PermissionNode<Ent,Permission>::
                set_data(int cible, const QVariant &value, int role, numt num) {
    switch (cible) {
    case PermissionModel::NcCible:
        if(role == StringRole) {
            m_ent.set_nc(value.to_string());
            return StringRole;
        }
        break;
    case PermissionModel::NomCible:
        if(role == StringRole) {
            m_ent.set_nom(value.to_string());
            return StringRole;
        }
        break;
    case PermissionModel::RefCible:
        if(role == StringRole) {
            m_ent.set_ref(value.to_string());
            return StringRole;
        }
        break;
    case PermissionModel::PermissionCible:
        if(role == NumRole) {
            m_permissionMap[m_model->cible(num)] = value.toUInt();
            return NumRole;
        }
        break;
    }
    return ItemNode::set_data(cible,value,role,num);
}

template<class Ent, class Permission> void PermissionNode<Ent,Permission>::setEnt(const Ent &ent) {
    m_ent = entity;
    for(auto iter = m_permissionMap.begin(); iter != m_permissionMap.end(); ++iter) {
        Permission perm;
        perm.set_id_1(entity.id());
        perm.set_cible(iter->first);
        m_model->bdd().getUnique(perm);
        iter->second = perm.code();
    }
}
}
#endif // PREDEFMODEL_H
