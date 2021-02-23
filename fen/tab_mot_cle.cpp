#include "tab_mot_cle.h"

//using namespace fenMPS;

//Tabmot_cle::Tabmot_cle(bddMPS::BddPredef &bd, const tab_index &index, QWidget *parent)
//    : abstract_tabModuleWithBdd(bd, index, parent) {
//    m_treeWidget = new QTreeView;
//    m_treeModel = new modelMPS::TreeModelmot_cle(bdd(), bdd().nomsentityCible(info_entity::Mot_Cle), this);
//    m_treeWidget->setModel(m_treeModel);
//    m_treeWidget->setItemDelegate(new modelMPS::mot_cleDelegate(bdd().nbrentityCible(info_entity::Mot_Cle), this));
//    m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    connect(m_treeWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
//            this,SLOT(selectionMotcle(QItemSelection,QItemSelection)));
//    m_motCleWidget = new widgetMPS::mot_cleWidget(bdd(),bdd().cible(info_entity::Mot_Cle));
//    m_horizontalSplitter = new QSplitter();
//    m_horizontalSplitter->addWidget(m_treeWidget);
//    m_horizontalSplitter->addWidget(m_motCleWidget);
//    m_mainLayout->addWidget(m_horizontalSplitter);
//}

////void Tabmot_cle::become_current() {
////    emit collerPermis(false);
////    emit copierPermis(false);
////    emit couperPermis(false);
////    emit savePermis(false);
////}

//void Tabmot_cle::selectionMotcle(const QItemSelection &selected, const QItemSelection &/*deselected*/) {
//    auto indexList = selected.indexes();
//    std::set<idt> idSet;
//    for(auto i = indexList.cbegin(); i != indexList.cend(); ++i)
//    {
//        if(i->column() == 0 &&std::find(idSet.cbegin(),idSet.cend(),m_treeModel->id(*i)) == idSet.cend())
//            idSet.insert(m_treeModel->id(*i));
//    }
//    m_motCleWidget->set_idSet(idSet);
//}
