#include "MotCleModel.h"

using namespace modelMPS;
using MotCle = emps::MotCle;
using MotCleCible = emps::MotCleCible;
using MotClePermission = emps::MotClePermission;

MotCleModel::MotCleModel(bddMPS::BddPredef &bd, int cible, QObject *parent)
    : TreeModelReadTemp<std::pair<MotCle,int>>(parent),
      m_bdd(bd),
      m_cible(cible)
    {setTreeMotCle();}

QVariant MotCleModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    switch (index.column()) {
    case CheckColumn:
        return getIter(index)->second;
    case NomColumn:
        return getIter(index)->first.nom();
    default:
        return QVariant();
    }
}

QString MotCleModel::dataListeNomMotCle() const {
    QString str;
    for(auto i = m_idMotCle.cbegin(); i != m_idMotCle.cend(); ++i) {
        MotCle mot(i->first);
        m_bdd.get(mot);
        str.append(mot.nom());
        if(i->second)
            str.append("*");
        str.append(",");
    }
    str.chop(1);
    return str;
}

QVariant MotCleModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == CheckColumn)
            return QVariant();
        if(section == NomColumn)
            return tr("Mots clés");
    }
    return QVariant();
}

void MotCleModel::setIdSet(const std::set<idt> & idSet) {
    m_idSet = idSet;
    m_idMotCle.clear();
    int newValue = Tous;
    for(auto i = m_idSet.cbegin(); i != m_idSet.cend(); ++i) {
        auto cibleMotcleList = m_bdd.getList<MotCleCible>(MotCleCible::Cible,m_cible,MotCleCible::IdCible,*i);
        for(auto j = cibleMotcleList.begin(); j != cibleMotcleList.end(); ++j) {
            auto k = m_idMotCle.find(j->idMotCle());
            if(k == m_idMotCle.end())
                m_idMotCle[j->idMotCle()] = newValue;
            else if(k->second == TousJusquaPresent)
                k->second = Tous;
        }
        for(auto k = m_idMotCle.begin(); k != m_idMotCle.end(); ++k)
        {
            if(k->second == TousJusquaPresent)
                k->second = PasTous;
        }
        newValue = PasTous;
    }
    emit changedIdSet(m_idSet);
}

void MotCleModel::setTreeMotCle() {
    auto tree = m_bdd.getArbre<MotCle>();
    tree.removeLeafIfData(
                [this](const MotCle & motCle)->bool
                {
                   MotClePermission perm(motCle.id(),m_cible);
                   m_bdd.getUnique(perm);
                   return perm.in(bmps::code::Interdit);
                });
    setDataTree(cmps::tree<std::pair<MotCle,int>>(tree,
                [](const MotCle & motCle)->std::pair<MotCle,int>{return std::pair<MotCle,int>(motCle,Aucun);}));
}
