#include "TabMotCle.h"

using namespace fenMPS;

TabMotCle::TabMotCle(bddMPS::BddPredef &bd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd(bd, pairIndex, parent) {
    m_treeWidget = new QTreeView;
    m_treeModel = new modelMPS::TreeModelMotCle(bdd(), bdd().nomsEntityCible(infoEntity::MotCleId), this);
    m_treeWidget->setModel(m_treeModel);
    m_treeWidget->setItemDelegate(new modelMPS::MotCleDelegate(bdd().nbrEntityCible(infoEntity::MotCleId), this));
    m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(m_treeWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,SLOT(selectionMotcle(QItemSelection,QItemSelection)));
    m_motCleWidget = new widgetMPS::MotCleWidget(bdd(),bdd().cible(infoEntity::MotCleId));
    m_horizontalSplitter = new QSplitter();
    m_horizontalSplitter->addWidget(m_treeWidget);
    m_horizontalSplitter->addWidget(m_motCleWidget);
    m_mainLayout->addWidget(m_horizontalSplitter);
}

//void TabMotCle::becomeCurrent() {
//    emit collerPermis(false);
//    emit copierPermis(false);
//    emit couperPermis(false);
//    emit savePermis(false);
//}

void TabMotCle::selectionMotcle(const QItemSelection &selected, const QItemSelection &/*deselected*/) {
    auto indexList = selected.indexes();
    std::set<idt> idSet;
    for(auto i = indexList.cbegin(); i != indexList.cend(); ++i)
    {
        if(i->column() == 0 && std::find(idSet.cbegin(),idSet.cend(),m_treeModel->id(*i)) == idSet.cend())
            idSet.insert(m_treeModel->id(*i));
    }
    m_motCleWidget->setIdSet(idSet);
}
