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
class AbstractPermissionModel : public TreeNodeModel {
    Q_OBJECT
protected:
    const szt m_offset;                                 //!< Offset de postion des cibles.
    bddMPS::BddPredef & m_bdd;                          //!< Référence à la base de donnée
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
                      NomPosition = ZeroPosition,
                      NcPosition = UnPosition};
    //! Constructeur.
    AbstractPermissionModel(bddMPS::BddPredef & bdd, szt offset = NcNomOffset, QObject * parent = nullptr);

    //! Accesseur la donnée associé à un couple (index,role).
    QVariant data(const NodeIndex &index, int role = DataRole) const override;

    //! Nombre de donné associé à une cible.
    szt dataCount(const NodeIndex & index) const override;

    //! Accesseur des cibles.
    int cible(szt num) const
        {return m_idNomVec.at(m_cibleVec.at(num)).first;}

    //! Accesseur de l'offset de postion des cibles
    szt offset() const noexcept
        {return m_offset;}
};

/*! \ingroup groupeModel
 * \brief Classe mère des neuds de recherche.
 */
template<class Ent> class PermissionNode : public TreeNodeModel::AbstractNode {
protected:
    Ent m_ent;                   //!< Type associé au noeud.
    std::map<int,flag> m_permissionMap;     //!< Map des permission du type.
    AbstractPermissionModel * m_model;      //!< Pointeur sur le model.
public:
    //! Constructeur.
    PermissionNode(AbstractPermissionModel * model)
        : AbstractNode(AbstractPermissionModel::TypeNode), m_model(model) {}

    //! Accesseur des données du noeud.
    QVariant data(int cible, int role = DataRole, szt num = 0) const override;

    //! Mutateur des données du noeud.
    flag setData(int cible, const QVariant & value, int role = DataRole, szt num = 0) override;
};

template<class Ent> QVariant PermissionNode<Ent>::data(int cible, int role, szt num) const {
    switch (cible) {
    case AbstractPermissionModel::NcCible:
        if(role == LabelRole)
            return "Nom abrégé :";
        if(role == DisplayRole)
            return m_ent.nc();
        break;
    case AbstractPermissionModel::NomCible:
        if(role == LabelRole)
            return "Nom :";
        if(role == DisplayRole)
            return m_ent.nom();
        break;
    case AbstractPermissionModel::CibleCible:
        if(role == LabelRole)
            return "Nom :";
        if(role == DataRole)
            return m_permissionMap.at(m_model->cible(num)).value();
        break;
    case SubNodeCible:
        if(role == SubNodeRole){
            if(num == AbstractPermissionModel::NomPosition) {
                QList<QVariant> init;
                init.append(AbstractPermissionModel::NomCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
            if(num == AbstractPermissionModel::NcPosition) {
                QList<QVariant> init;
                init.append(AbstractPermissionModel::NcCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
            if(num >= m_model->offset()) {
                QList<QVariant> init;
                init.append(AbstractPermissionModel::CibleCible);
                init.append(num - m_model->offset());
                init.append(LineEditSubNode);
                return init;
            }
        }
    }
    return AbstractNode::data(cible,role,num);
}

template<class Ent> flag PermissionNode<Ent>::setData(int cible, const QVariant &value, int role, szt num) {
    if(cible == AbstractPermissionModel::NcCible && role == DisplayRole) {
        m_ent.setNc(value.toString());
        return DisplayRole;
    }
    if(cible == AbstractPermissionModel::NomCible && role == DisplayRole) {
        m_ent.setNom(value.toString());
        return DisplayRole;
    }
    if(cible == AbstractPermissionModel::CibleCible && role == DataRole) {
        m_permissionMap[m_model->cible(num)] = value.toUInt();
        return DataRole;
    }
    return AbstractNode::setData(cible,value,role,num);
}
}
#endif // PREDEFMODEL_H
