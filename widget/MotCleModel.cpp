#include "MotCleModel.h"

MotCleModel::MotCleModel(BddPredef *bd, int cible, QObject *parent)
    : TreeModelReadTemp<QPair<MotCle,int>>(parent),
      m_bdd(bd),
      m_cible(cible)
    {setTreeMotCle();}

QVariant MotCleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    switch (index.column()) {
    case CheckColumn:
        return getItem(index)->data().second;
    case NomColumn:
        return getItem(index)->data().first.nom();
    default:
        return QVariant();
    }
}

QString MotCleModel::dataListeNomMotCle() const
{
    QString str;
    for(QMap<int,bool>::const_iterator i = m_idMotCle.cbegin(); i != m_idMotCle.cend(); ++i)
    {
        MotCle mot(i.key());
        m_bdd->get(mot);
        str.append(mot.nom());
        if(i.value())
            str.append("*");
        str.append(",");
    }
    str.chop(1);
    return str;
}

QVariant MotCleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == CheckColumn)
            return QVariant();
        if(section == NomColumn)
            return tr("Mots clés");
    }
    return QVariant();
}

void MotCleModel::setIdSet(const QSet<int> & idSet)
{
    m_idSet = idSet;
    m_idMotCle.clear();
    int newValue = Tous;
    for(QSet<int>::const_iterator i = m_idSet.cbegin(); i != m_idSet.cend(); ++i)
    {
        ListPtr<CibleMotCle> cibleMotcleList(m_bdd->getList<CibleMotCle>(CibleMotCle::Cible,m_cible,CibleMotCle::IdCible,*i));
        for(ListPtr<CibleMotCle>::iterator j = cibleMotcleList.begin(); j != cibleMotcleList.end(); ++j)
        {
            QMap<int,bool>::iterator k = m_idMotCle.find((*j).idMotCle());
            if(k == m_idMotCle.end())
                m_idMotCle[(*j).idMotCle()] = newValue;
            else if(*k == TousJusquaPresent)
                *k = Tous;
        }
        for(QMap<int,bool>::iterator k = m_idMotCle.begin(); k != m_idMotCle.end(); ++k)
        {
            if(*k == TousJusquaPresent)
                *k = PasTous;
        }
        newValue = PasTous;
    }
    emit changedIdSet(m_idSet);
}

void MotCleModel::setTreeMotCle()
{
    setDataTree(Tree<QPair<MotCle,int>>(m_bdd->getArbre<MotCle>().elagageFeuilleData(
                                 [this](const MotCle & motCle)->bool
                                 {
                                    MotClePermission perm(motCle.id(),m_cible);
                                    m_bdd->getUnique(perm);
                                    return perm.in(bdd::motClePermissionNum::InterditMCNum);
    }),
                [](const MotCle & motCle)->QPair<MotCle,int>{return QPair<MotCle,int>(motCle,Aucun);}));
}
