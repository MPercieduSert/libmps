#include "TreeModelMotCle.h"

using namespace modelMPS;
using MotClePermission = emps::MotClePermission;

TreeModelMotCle::TreeModelMotCle(bmps::BddPredef * bdd, const std::map<int,QString> & header, QObject *parent)
    : TreeModelEditEntity<MotCle>({}, parent), m_nbrEntity(header.size()), m_bdd(bdd), m_cibleList(m_nbrEntity) {
    setHeaderData(NomMotCleIndex, Qt::Horizontal, QString(tr("Mot Clé")), Qt::DisplayRole);
    setHeaderData(NcMotCleIndex, Qt::Horizontal, QString(tr("Abrégé")), Qt::DisplayRole);
    {
        int j = 0;
        for(auto i = header.cbegin(); i != header.cend(); ++i, ++j) {
            m_cibleList[j] = m_bdd->cible(i->first);
            setHeaderData(NbrColumnMotCle + j, Qt::Horizontal, i->second, Qt::DisplayRole);
        }
    }

    setDataTree(m_bdd->getArbre<MotCle>());
    for(auto i = m_tree.begin(); i != m_tree.end(); ++i) {
        auto id = i->id();
        m_permission[id] = std::vector<codeType>(m_nbrEntity,bmps::motClePermissionNum::InterditMCNum);
        for(int j = 0; j != m_nbrEntity; ++j) {
            MotClePermission permission(id, m_cibleList[j]);
            m_bdd->get(permission);
            m_permission[id][j] = permission.code();
        }
    }
}

bool TreeModelMotCle::autorisation(const QModelIndex & index, bmps::autorisation autoris) const {
    if(autoris == bmps::autorisation::Suppr)
        return m_bdd->getAutorisation(getData(index),autoris);
    if(autoris == bmps::autorisation::Modif) {
        if(index.column() < NbrColumnMotCle)
            return m_bdd->getAutorisation(getData(index),autoris);
        else {
            MotClePermission perm(getData(index).id(),cible(index));
            return !m_bdd->getUnique(perm) || m_bdd->getAutorisation(perm,bmps::Modif);
        }
    }
    return TreeModelMotCle::autorisation(index,autoris);
}


QVariant TreeModelMotCle::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::FontRole && role != Qt::ForegroundRole)
        return QVariant();

    auto item = getIter(index);
    if(index.column() ==  NomMotCleIndex) {
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return getData(index).nom();
    }
    else if(index.column() ==  NcMotCleIndex) {
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return getData(index).nc();
    }
    else if(index.column() < NbrColumnMotCle + m_nbrEntity) {
        switch (role) {
        case Qt::DisplayRole:
            return m_permission.at(getData(index).id())[index.column() - NbrColumnMotCle] == bmps::motClePermissionNum::InterditMCNum ?
                        QString("\uF051") : QString("\uF052");
        case Qt::EditRole:
            return m_permission.at(getData(index).id())[index.column() - NbrColumnMotCle];
        case Qt::FontRole:
            return QFont("Wingdings 2");
        case Qt::ForegroundRole:
            return m_permission.at(getData(index).id())[index.column() - NbrColumnMotCle] == bmps::motClePermissionNum::InterditMCNum ?
                        QColor(Qt::red) : QColor(Qt::green);
        }
    }
    return QVariant();
}

Qt::ItemFlags TreeModelMotCle::flags(const QModelIndex &index) const {
    if(!index.isValid())
        return Qt::NoItemFlags;
    if(!autorisation(index,bmps::autorisation::Modif))
        return QAbstractItemModel::flags(index);
    return TreeModelEditEntity<MotCle>::flags(index);
}

QVariant TreeModelMotCle::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == NomMotCleIndex)
            return tr("Nom");
        if(section == NcMotCleIndex)
            return tr("Nom court");
        if(NbrColumnMotCle <= section && section <NbrColumnMotCle + m_nbrEntity)
            return m_headerPerm.at(section - NbrColumnMotCle);
    }
    return QVariant();
}

bool TreeModelMotCle::hydrateNewEntity(MotCle & entity, int row, const QModelIndex &parent) {
    dialogMPS::NewMotCleDialog diag(getData(parent).nom(),row+1);
    if(diag.exec()) {
        auto result = diag.value();
        entity.setNom(result.nom);
        entity.setNc(result.nc);
        m_bdd->save(entity,getData(parent),row);
        std::fill(m_permission[entity.id()].begin(),m_permission[entity.id()].end(),bmps::motClePermissionNum::InterditMCNum);
        return true;
    }
    else
        return false;
}

idt TreeModelMotCle::id(const QModelIndex & index) const {
    if(!index.isValid())
        return 0;
    return getData(index).id();
}

bool TreeModelMotCle::removeEntity(const MotCle & entity) {
    if(QMessageBox::question(nullptr,tr("Suppression d'un mot clé"),
                             QString(tr("Êtes-vous certain de vouloir supprimer le mot clé : ").append(entity.nom()).append(".")),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        return m_bdd->del(entity);
    return false;
}

bool TreeModelMotCle::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!index.isValid() || role != Qt::EditRole)
        return false;
    if(index.column() == NomMotCleIndex) {
        getData(index).setNom(value.toString());
        m_bdd->save(getData(index));
        return true;
    }
    else if(index.column() == NcMotCleIndex) {
        getData(index).setNc(value.toString());
        m_bdd->save(getData(index));
        return true;
    }
    else if(NbrColumnMotCle <= index.column() && index.column() < NbrColumn) {
        m_permission[getData(index).id()][index.column() - NbrColumnMotCle] = value.value<idt>();
        MotClePermission perm(getData(index).id(), cible(index) , value.value<idt>());
        m_bdd->saveUnique(perm);
        return true;
    }
    return false;
}
