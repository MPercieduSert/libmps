#include "PredefModel.h"

using namespace modelMPS;

////////////////////////////////////////////// AbstractPermissionModel ////////////////////////////////////
AbstractPermissionModel::AbstractPermissionModel(bddMPS::BddPredef & bdd, szt offset, QObject * parent)
    : TreeNodeModel(false, parent), m_offset(offset), m_bdd(bdd) {
    m_idNomVec.reserve(m_bdd.nbrEntity());
    for(szt i = 0; i < m_bdd.nbrEntity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_idNomVec.emplace_back(m_bdd.cible(i),m_bdd.managers().info(i).name());
    }
    std::sort(m_idNomVec.begin(),m_idNomVec.end(),
              [](const std::pair<int, QString> & p1, const std::pair<int, QString> & p2)->bool {return p1.second < p2.second;});
}

QVariant AbstractPermissionModel::data(const NodeIndex &index, int role) const {
    if(index.cible() == CibleCible){
        if(role == modelMPS::LabelRole && index.num() < m_idNomVec.size())
            return m_idNomVec.at(index.num()).second;
        if(role == CheckStateRole && index.num() < m_idNomVec.size())
            return std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),index.num()) != m_cibleVec.cend();

    }
    return TreeNodeModel::data(index,role);
}

szt AbstractPermissionModel::dataCount(const NodeIndex & index) const {
    switch (index.cible()) {
    case AbstractPermissionModel::NcCible:
        return 1;
    case AbstractPermissionModel::NomCible:
        return 1;
    case AbstractPermissionModel::PermissionCible:
        return static_cast<szt>(m_cibleVec.size());
    case AbstractPermissionModel::CibleCible:
        return static_cast<szt>(m_idNomVec.size());
    case SubNodeCible:
        return static_cast<szt>(m_cibleVec.size()) + m_offset;
    }
    return TreeNodeModel::dataCount(index);
}
