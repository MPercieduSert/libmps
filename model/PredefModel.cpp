#include "PredefModel.h"

using namespace modelMPS;

////////////////////////////////////////////// PermissionModel ////////////////////////////////////
PermissionModel::PermissionModel(b2d::bdd_predef &bdd, enumt offset, QObject *parent)
    : ItemNodeBddModel(bdd, parent), m_offset(offset) {
    m_idNomVec.reserve(m_bdd.nbr_entity());
    for(entidt i = 0; i < m_bdd.nbr_entity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_idNomVec.emplace_back(bdd.cible(i),m_bdd.managers().info(i).name());
    }
    std::sort(m_idNomVec.begin(),m_idNomVec.end(),
              [](const std::pair<int, QString> &p1, const std::pair<int, QString> &p2)->bool {return p1.second < p2.second;});
}

numt PermissionModel::dataCount(const NodeIndex &index) const {
    switch (index.cible()) {
    case PermissionModel::NcCible:
    case PermissionModel::NomCible:
    case PermissionModel::RefCible:
        return 1;
    case PermissionModel::PermissionCible:
        return static_cast<numt>(m_cibleVec.size());
    case SubNodeCible:
        return static_cast<numt>(m_cibleVec.size()) + m_offset;
    }
    return ItemNodeModel::dataCount(index);
}

void PermissionModel::set_cible(entidt num, bool visible){
    if(visible) {
        if(std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),num) == m_cibleVec.cend()) {
            beginReset_data();
                m_cibleVec.push_back(num);
                for (auto iter = m_data.cbegin(); iter; ++iter)
                    static_cast<AbstractPermissionNode &>(**iter).addCible(m_idNomVec.at(num).first);
            endReset_data();
        }
    }
    else {
        auto iter = std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),num);
        if(iter != m_cibleVec.cend()) {
            modelAboutToReset_data();
                m_cibleVec.erase(iter);
            modelReset_data();
        }
    }
}

//////////////////////////////////////////////////////// mot_cle_permissionModel ///////////////////////////////
mot_cle_permissionModel::mot_cle_permissionModel(b2d::bdd_predef &bdd, QObject *parent)
    : PermissionModel(bdd,nc_nom_refOffset,parent) {
    m_data.setTree(bdd.get_arbre<entities::mot_cle>(),[this](const entities::mot_cle &ent) {
        auto node = std::make_unique<mot_cle_permissionNode>(this);
        node->setEnt(ent);
        return node;
    });
}

//////////////////////////////////////////////////////// type_permissionModel ///////////////////////////////
type_permissionModel::type_permissionModel(b2d::bdd_predef &bdd, QObject *parent)
    : PermissionModel(bdd,nc_nom_refOffset,parent) {
    m_data.setTree(bdd.get_arbre<entities::Type>(),[this](const entities::Type &ent) {
        auto node = std::make_unique<type_permissionNode>(this);
        node->setEnt(ent);
        return node;
    });
}
