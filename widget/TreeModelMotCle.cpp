#include "TreeModelMotCle.h"

TreeModelMotCle::TreeModelMotCle(BddPredef * bdd, const QMap<int,QString> & header, QObject *parent)
    : TreeModelEditEntity<MotCle>(QList<MotCle::Position>(), parent), m_nbrEntity(header.count()), m_bdd(bdd), m_cibleList(m_nbrEntity)
{
    setHeaderData(NomMotCleIndex, Qt::Horizontal, QString(tr("Mot Clé")), Qt::DisplayRole);
    setHeaderData(NcMotCleIndex, Qt::Horizontal, QString(tr("Abrégé")), Qt::DisplayRole);
    {int j = 0;
    for(QMap<int,QString>::const_iterator i = header.cbegin(); i != header.cend(); ++i, ++j)
    {
        m_cibleList[j] = m_bdd->cible(i.key());
        setHeaderData(NbrColumnMotCle + j, Qt::Horizontal, i.value(), Qt::DisplayRole);
    }}

    setDataTree(m_bdd->getArbre<MotCle>());
    for(TreeItem<MotCle>::iterator i = m_tree.begin(); i != m_tree.end(); ++i)
    {
        auto id = (*i)->data().id();
        m_permission[id] = QVector<codeType>(m_nbrEntity,bdd::motClePermissionNum::InterditMCNum);
        for(int j = 0; j != m_nbrEntity; ++j)
        {
            MotClePermission permission(id, m_cibleList[j]);
            m_bdd->get(permission);
            m_permission[id][j] = permission.code();
        }
    }
}

bool TreeModelMotCle::autorisation(const QModelIndex & index, bdd::Autorisation autoris) const
{
    if(autoris == bdd::Autorisation::Suppr)
        return m_bdd->getAutorisation(getItem(index)->data(),autoris);
    if(autoris == bdd::Autorisation::Modif)
    {
        if(index.column() < NbrColumnMotCle)
            return m_bdd->getAutorisation(getItem(index)->data(),autoris);
        else
        {
            MotClePermission perm(getItem(index)->data().id(),cible(index));
            return !m_bdd->getUnique(perm) || m_bdd->getAutorisation(perm,bdd::Modif);
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

    TreeItem<MotCle> *item = getItem(index);
    if(index.column() ==  NomMotCleIndex)
    {
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return item->data().nom();
    }
    else if(index.column() ==  NcMotCleIndex)
    {
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return item->data().nc();
    }
    else if(index.column() < NbrColumnMotCle + m_nbrEntity)
    {
        switch (role) {
        case Qt::DisplayRole:
            return m_permission.value(item->data().id()).at(index.column() - NbrColumnMotCle) == bdd::motClePermissionNum::InterditMCNum ?
                        QString("\uF051") : QString("\uF052");
        case Qt::EditRole:
            return m_permission.value(item->data().id()).at(index.column() - NbrColumnMotCle);
        case Qt::FontRole:
            return QFont("Wingdings 2");
        case Qt::ForegroundRole:
            return m_permission.value(item->data().id()).at(index.column() - NbrColumnMotCle) == bdd::motClePermissionNum::InterditMCNum ?
                        QColor(Qt::red) : QColor(Qt::green);
        }
    }
    return QVariant();
}

Qt::ItemFlags TreeModelMotCle::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    if(!autorisation(index,bdd::Autorisation::Modif))
        return QAbstractItemModel::flags(index);
    return TreeModelEditEntity<MotCle>::flags(index);
}

QVariant TreeModelMotCle::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == NomMotCleIndex)
            return tr("Nom");
        if(section == NcMotCleIndex)
            return tr("Nom court");
        if(NbrColumnMotCle <= section && section <NbrColumnMotCle + m_nbrEntity)
            return m_headerPerm.at(section - NbrColumnMotCle);
    }
    return QVariant();
}

bool TreeModelMotCle::hydrateNewEntity(MotCle & entity, int row, const QModelIndex &parent)
{
    NewMotCleDialog diag(getItem(parent)->data().nom(),row+1);
    if(diag.exec())
    {
        NewMotCleDialog::dialogResult result = diag.value();
        entity.setNom(result.nom);
        entity.setNc(result.nc);
        m_bdd->save(entity,getItem(parent)->data(),row);
        std::fill(m_permission[entity.id()].begin(),m_permission[entity.id()].end(),bdd::motClePermissionNum::InterditMCNum);
        return true;
    }
    else
        return false;
}

idt TreeModelMotCle::id(const QModelIndex & index) const
{
    if(!index.isValid())
        return 0;
    return getItem(index)->data().id();
}

bool TreeModelMotCle::removeEntity(const MotCle & entity)
{
    if(QMessageBox::question(nullptr,tr("Suppression d'un mot clé"),
                             QString(tr("Êtes-vous certain de vouloir supprimer le mot clé : ").append(entity.nom()).append(".")),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        return m_bdd->del(entity);
    return false;
}

bool TreeModelMotCle::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || role != Qt::EditRole)
        return false;
    if(index.column() == NomMotCleIndex)
    {
        getItem(index)->modifData().setNom(value.toString());
        m_bdd->save(getItem(index)->data());
        return true;
    }
    else if(index.column() == NcMotCleIndex)
    {
        getItem(index)->modifData().setNc(value.toString());
        m_bdd->save(getItem(index)->data());
        return true;
    }
    else if(NbrColumnMotCle <= index.column() && index.column() < NbrColumn)
    {
        m_permission[getItem(index)->data().id()][index.column() - NbrColumnMotCle] = value.toInt();
        MotClePermission perm(getItem(index)->data().id(), cible(index) , value.toInt());
        m_bdd->saveUnique(perm);
        return true;
    }
    return false;
}
