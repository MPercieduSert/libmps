/*Auteur: PERCIE DU SERT Maxime
 *Date: 02/12/2020
 */
#ifndef PREDEFMODEL_H
#define PREDEFMODEL_H

#include "AbstractNodeModel.h"
#include "BddPredef.h"

namespace modelMPS {
/*! \ingroup groupeModel
 * \brief Model de gestion des types.
 */
class PermissionModel : public TreeNodeModelWithBdd {
    Q_OBJECT
protected:
    const szt m_offset;                                 //!< Offset de postion des cibles.
    //bddMPS::BddPredef & m_bdd;                          //!< Référence à la base de donnée
    std::vector<szt> m_cibleVec;                        //!< Vecteur de cibles à afficher.
    std::vector<std::pair<int,QString>> m_idNomVec;     //!< Vecteur d'information sur les cibles.

public:
    enum {TypeNode = 1,
         NcNomOffset = 2};
    //! Cible des données du model.
    enum dataCible {NcCible,
                    NomCible,
                    RefCible,
                    PermissionCible,
                    CibleCible = ExterneCible};
    //! Position des sous-noeud.
    enum positionNode{ZeroPosition,
                      UnPosition,
                      DeuxPosition,
                      NomPosition = ZeroPosition,
                      NcPosition = UnPosition,
                      RefPosition = DeuxPosition};
    //! Constructeur.
    PermissionModel(bddMPS::BddPredef &bdd, szt offset = NcNomOffset, QObject * parent = nullptr);

    //! Accesseur la donnée associé à un couple (index,role).
    QVariant data(const NodeIndex &index, int role) const override;

    //! Nombre de donné associé à une cible.
    szt dataCount(const NodeIndex & index) const override;

    //! Drapeaux assossiés à une donnée.
    flag flags(const NodeIndex & index) const override{
        if(index.isRoot())
            return NoFlagNode;
        return TreeNodeModelWithBdd::flags(index);
    }

    //! Nom d'une cible.
    const QString & nomCible(szt num) const
        {return m_idNomVec.at(m_cibleVec.at(num)).second;}

    //! Numero de cible.
    int cible(szt num) const
        {return m_idNomVec.at(m_cibleVec.at(num)).first;}

    //! Accesseur de la liste des cibles.
    const std::vector<std::pair<int,QString>> & idNomVec() const noexcept
        {return m_idNomVec;}

    //! Accesseur de l'offset de postion des cibles
    szt offset() const noexcept
        {return m_offset;}

    //! Muateur de la présence d'une cible.
    void setCible(szt num, bool visible);
};

/*! \ingroup groupeModel
 * \brief Classe mère des neuds d'un model de permission.
 */
template<class Ent, class Permission> class PermissionNode : public TreeNodeModel::AbstractNode {
protected:
    Ent m_ent;                              //!< Type associé au noeud.
    std::map<int,flag> m_permissionMap;     //!< Map des permission du type.
    PermissionModel * m_model;      //!< Pointeur sur le model.
public:
    //! Constructeur.
    PermissionNode(PermissionModel * model)
        : AbstractNode(PermissionModel::TypeNode), m_model(model) {}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, szt num = 0) const override;

    //! Mutateur des données du noeud.
    flag setData(int cible, const QVariant & value, int role, szt num = 0) override;

    //! Mutateur de l'entité.
    void setEnt(const Ent & entity);
};

/*! \ingroup groupeModel
 * \brief Classe des neuds du model de  Type-permission.
 */
class TypePermissionNode : public PermissionNode<entityMPS::Type,entityMPS::TypePermission> {
protected:
    using PermNode = PermissionNode<entityMPS::Type,entityMPS::TypePermission>;
public:
    //! Constructeur.
    using PermNode::PermissionNode;

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role, szt num = 0) const override;

    //! Mutateur des données du noeud.
    flag setData(int cible, const QVariant & value, int role, szt num = 0) override;
};

/*! \ingroup groupeModel
 * \brief Model de gestion des type et de leur permission.
 */
class TypePermissionModel : public PermissionModel {
    Q_OBJECT
public:
    enum {NcNomRefOffset = 3};
    //! Constructeur.
    TypePermissionModel(bddMPS::BddPredef &bdd, QObject * parent = nullptr);

    //! Nombre de donné associé à une cible.
    szt dataCount(const NodeIndex & index) const override;

};

////////////////////////////////////////////////////////// MakeArbreNode ////////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe mère des neuds de recherche.
 */
template<class Ent, class Permission, class Node> class MakeArbreNode {
public:
    static TreeNodeModel::Tree arbre(PermissionModel * model);
};


template<class Ent, class Permission, class Node> TreeNodeModel::Tree MakeArbreNode<Ent,Permission,Node>::arbre(PermissionModel *model){
    return TreeNodeModel::Tree(model->bdd().getArbre<Ent>(),[model](const Ent & entity) {
        auto node = std::make_unique<Node>(model);
        node->setEnt(entity);
        return node;
    });
}

///////////////////////////////////////////////// definition PermissionNode ///////////////////////////////////
template<class Ent, class Permission> QVariant PermissionNode<Ent,Permission>::data(int cible, int role, szt num) const {
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
    case PermissionModel::PermissionCible:
        if(role == LabelRole)
            return m_model->nomCible(num);
        if(role == NumRole)
            return m_permissionMap.at(m_model->cible(num)).value();
        break;
    case SubNodeCible:
        if(role == SubNodeRole){
            if(num == PermissionModel::NomPosition) {
                QList<QVariant> init;
                init.append(PermissionModel::NomCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
            if(num == PermissionModel::NcPosition) {
                QList<QVariant> init;
                init.append(PermissionModel::NcCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
            if(num >= m_model->offset()) {
                QList<QVariant> init;
                init.append(PermissionModel::CibleCible);
                init.append(num - m_model->offset());
                init.append(LineEditSubNode);
                return init;
            }
        }
    }
    return AbstractNode::data(cible,role,num);
}

template<class Ent, class Permission> flag PermissionNode<Ent,Permission>::setData(int cible, const QVariant &value, int role, szt num) {
    if(cible == PermissionModel::NcCible && role == StringRole) {
        m_ent.setNc(value.toString());
        return StringRole;
    }
    if(cible == PermissionModel::NomCible && role == StringRole) {
        m_ent.setNom(value.toString());
        return StringRole;
    }
    if(cible == PermissionModel::CibleCible && role == NumRole) {
        m_permissionMap[m_model->cible(num)] = value.toUInt();
        return NumRole;
    }
    return AbstractNode::setData(cible,value,role,num);
}

template<class Ent, class Permission> void PermissionNode<Ent,Permission>::setEnt(const Ent & entity) {
    m_ent = entity;
    for(auto iter = m_permissionMap.begin(); iter != m_permissionMap.end(); ++iter) {
        Permission perm;
        perm.setIdType(entity.id());
        perm.setCible(iter->first);
        m_model->bdd().getUnique(perm);
        iter->second = perm.code();
    }
}
}
#endif // PREDEFMODEL_H
