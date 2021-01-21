#include "PredefModel.h"

using namespace modelMPS;

////////////////////////////////////////////// PermissionModel ////////////////////////////////////
PermissionModel::PermissionModel(bddMPS::BddPredef & bdd, enumt offset, QObject * parent)
    : ItemNodeBddModel(bdd, parent), m_offset(offset) {
    m_idNomVec.reserve(m_bdd.nbrEntity());
    for(entidt i = 0; i < m_bdd.nbrEntity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_idNomVec.emplace_back(bdd.cible(i),m_bdd.managers().info(i).name());
    }
    std::sort(m_idNomVec.begin(),m_idNomVec.end(),
              [](const std::pair<int, QString> & p1, const std::pair<int, QString> & p2)->bool {return p1.second < p2.second;});
}

numt PermissionModel::dataCount(const NodeIndex & index) const {
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

void PermissionModel::setCible(entidt num, bool visible){
    if(visible) {
        if(std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),num) == m_cibleVec.cend()) {
            beginResetData();
                m_cibleVec.push_back(num);
                for (auto iter = m_data.cbegin(); iter; ++iter)
                    static_cast<AbstractPermissionNode &>(**iter).addCible(m_idNomVec.at(num).first);
            endResetData();
        }
    }
    else {
        auto iter = std::find(m_cibleVec.cbegin(),m_cibleVec.cend(),num);
        if(iter != m_cibleVec.cend()) {
            modelAboutToResetData();
                m_cibleVec.erase(iter);
            modelResetData();
        }
    }
}

//////////////////////////////////////////////////////// MotClePermissionModel ///////////////////////////////
MotClePermissionModel::MotClePermissionModel(bddMPS::BddPredef &bdd, QObject * parent)
    : PermissionModel(bdd,NcNomRefOffset,parent) {
    m_data.setTree(bdd.getArbre<entityMPS::MotCle>(),[this](const entityMPS::MotCle & entity) {
        auto node = std::make_unique<MotClePermissionNode>(this);
        node->setEnt(entity);
        return node;
    });
}

//////////////////////////////////////////////////////// TypePermissionModel ///////////////////////////////
TypePermissionModel::TypePermissionModel(bddMPS::BddPredef &bdd, QObject * parent)
    : PermissionModel(bdd,NcNomRefOffset,parent) {
    m_data.setTree(bdd.getArbre<entityMPS::Type>(),[this](const entityMPS::Type & entity) {
        auto node = std::make_unique<TypePermissionNode>(this);
        node->setEnt(entity);
        return node;
    });
}
