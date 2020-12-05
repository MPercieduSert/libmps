#include "PredefModel.h"

using namespace modelMPS;

AbstractPermissionModel::AbstractPermissionModel(bddMPS::BddPredef & bdd, szt offset, QObject * parent)
    : TreeNodeModel(false, parent), m_offset(offset), m_bdd(bdd) {}

szt AbstractPermissionModel::dataCount(const NodeIndex & index) const {
    switch (index.cible()) {
    case AbstractPermissionModel::NcCible:
        return 1;
    case AbstractPermissionModel::NomCible:
        return 1;
    case AbstractPermissionModel::CibleCible:
        return static_cast<szt>(m_cibleVec.size());
    case SubNodeCible:
        return static_cast<szt>(m_cibleVec.size()) + m_offset;
    }
    return TreeNodeModel::dataCount(index);
}

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
    if(cible == AbstractPermissionModel::NcCible && role == DataRole) {
        m_ent.setNc(value.toString());
        return DataRole;
    }
    if(cible == AbstractPermissionModel::NomCible && role == DataRole) {
        m_ent.setNom(value.toString());
        return DataRole;
    }
    if(cible == AbstractPermissionModel::CibleCible && role == DataRole) {
        m_permissionMap[m_model->cible(num)] = value.toUInt();
        return DataRole;
    }
    return AbstractNode::setData(cible,value,role,num);
}
