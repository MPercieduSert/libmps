#include "TabMotCle.h"

TabMotCle::TabMotCle(QWidget *parent): AbstractTabModuleWithBdd(parent)
{
    m_treeWidget = new QTreeView;
    m_treeModel = new TreeModelMotCle(bdd(), bdd()->nomsEntityCible(InfoEntity::MotCleId), this);
    m_treeWidget->setModel(m_treeModel);
    m_treeWidget->setItemDelegate(new MotCleDelegate(bdd()->nbrEntityCible(InfoEntity::MotCleId), this));
    m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(m_treeWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,SLOT(selectionMotcle(QItemSelection,QItemSelection)));
    m_motCleWidget = new MotCleWidget(bdd(),bdd()->cible(InfoEntity::MotCleId));
    m_horizontalSplitter = new QSplitter();
    m_horizontalSplitter->addWidget(m_treeWidget);
    m_horizontalSplitter->addWidget(m_motCleWidget);
    m_mainLayout->addWidget(m_horizontalSplitter);
}

void TabMotCle::becomeCurrent()
{
    emit collerPermis(false);
    emit copierPermis(false);
    emit couperPermis(false);
    emit savePermis(false);
}

void TabMotCle::selectionMotcle(const QItemSelection &selected, const QItemSelection &/*deselected*/)
{
    QModelIndexList indexList = selected.indexes();
    QSet<int> idSet;
    for(QModelIndexList::const_iterator i = indexList.cbegin(); i != indexList.cend(); ++i)
    {
        if((*i).column() == 0 && !idSet.contains(m_treeModel->id(*i)))
            idSet.insert(m_treeModel->id(*i));
    }
    m_motCleWidget->setIdSet(idSet);
}
