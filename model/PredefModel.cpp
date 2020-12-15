#include "PredefModel.h"

using namespace modelMPS;

////////////////////////////////////////////// PermissionModel ////////////////////////////////////
PermissionModel::PermissionModel(bddMPS::BddPredef & bdd, szt offset, QObject * parent)
    : TreeNodeModelWithBdd(bdd, parent), m_offset(offset) {
    m_idNomVec.reserve(m_bdd.nbrEntity());
    for(szt i = 0; i < m_bdd.nbrEntity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_idNomVec.emplace_back(bdd.cible(i),m_bdd.managers().info(i).name());
    }
    std::sort(m_idNomVec.begin(),m_idNomVec.end(),
              [](const std::pair<int, QString> & p1, const std::pair<int, QString> & p2)->bool {return p1.second < p2.second;});
}

QVariant PermissionModel::data(const NodeIndex &index, int role) const {
    if(index.cible() == CibleCible){
        if(role == modelMPS::LabelRole && index.num() < m_idNomVec.size())
            return m_idNomVec.at(index.num()).second;
        if(role == CheckStateRole && index.num() < m_idNomVec.size())
            return std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),index.num()) != m_cibleVec.cend() ? Qt::Checked : Qt::Unchecked;
    }
    return TreeNodeModel::data(index,role);
}

szt PermissionModel::dataCount(const NodeIndex & index) const {
    switch (index.cible()) {
    case PermissionModel::NcCible:
        return 1;
    case PermissionModel::NomCible:
        return 1;
    case PermissionModel::PermissionCible:
        return static_cast<szt>(m_cibleVec.size());
    case PermissionModel::CibleCible:
        return static_cast<szt>(m_idNomVec.size());
    case SubNodeCible:
        return static_cast<szt>(m_cibleVec.size()) + m_offset;
    }
    return TreeNodeModel::dataCount(index);
}

void PermissionModel::setCible(szt num, bool visible){
    if(visible) {
        if(std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),num) == m_cibleVec.cend())
            m_cibleVec.push_back(num);
    }
    else {
        auto iter = std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),num);
        if(iter != m_cibleVec.cend())
            m_cibleVec.erase(iter);
    }
}

//////////////////////////////////////////////////////// TypePermissionNode ///////////////////////////////
QVariant TypePermissionNode::data(int cible, int role, szt num) const {
    if(cible == PermissionModel::RefCible) {
        if(role == LabelRole)
            return "Référence :";
        if(role == StringRole)
            return m_ent.ref();
    }
    else if(cible == SubNodeCible && role == SubNodeRole && num == PermissionModel::RefPosition) {
        QList<QVariant> init;
        init.append(PermissionModel::RefCible);
        init.append(0);
        init.append(LineEditSubNode);
        return init;
    }
    return PermNode::data(cible,role,num);
}

flag TypePermissionNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == PermissionModel::RefCible && role == StringRole) {
        m_ent.setRef(value.toString());
        return StringRole;
    }
    return PermNode::setData(cible,value,role,num);
}

//////////////////////////////////////////////////////// TypePermissionModel ///////////////////////////////
TypePermissionModel::TypePermissionModel(bddMPS::BddPredef &bdd, QObject * parent)
    : PermissionModel(bdd,NcNomRefOffset,parent)
    {m_data.setTree(MakeArbreNode<entityMPS::Type,entityMPS::TypePermission,TypePermissionNode>::arbre(this));}

szt TypePermissionModel::dataCount(const NodeIndex & index) const {
    if(index.cible() == PermissionModel::RefCible)
        return 1;
    return PermissionModel::dataCount(index);
}
