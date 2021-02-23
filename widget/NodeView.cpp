#include "node_ptrView.h"

using namespace delegateMPS;
using namespace model_base;
using namespace widgetMPS;

////////////////////////////////////// Abstractnode_ptrDelegate /////////////////////////////////
Abstractnode_ptrDelegate::Abstractnode_ptrDelegate(QObject *parent) : QObject(parent) {}

////////////////////////////////////// Arcnode_ptrViewWidget //////////////////////////////////////////
Arcnode_ptrViewWidget::Arcnode_ptrViewWidget(node_ptrWidget *node, node_ptrView *view, QWidget *parent, bool root, bool nodeArcVisible)
    : QWidget (parent), m_leaf(node->index().leaf()), m_root(root), m_nodeArcVisible(nodeArcVisible), m_view(view)
    {setnode_ptrWidget(node);}

void Arcnode_ptrViewWidget::drawnode_ptr(bool next) {
    adjustSize();
    if(!m_root) {
        auto parentArc = static_cast<Arcnode_ptrViewWidget *>(parentWidget());
        auto draw = false;
        auto y = 0;
        auto &arcPainter = *m_view->m_arcPainter;
        for (auto iter = parentArc->m_arcChild.begin(); iter != parentArc->m_arcChild.end(); ++iter) {
            if (!next &&!draw &&*iter == this) {
                draw = true;
                if(iter ==  parentArc->m_arcChild.begin()) {
                    if(parentArc->m_nodeArcVisible)
                        y = parentArc->nodeWidget()->geometry().bottom()
                                + arcPainter.heightToolZone() + arcPainter.bottomToolsZoneMargin();
                }
                else
                    y = (*std::prev(iter))->geometry().bottom();
            }
            if(draw) {
                if(parentArc->m_nodeArcVisible)
                    (*iter)->move(arcPainter.leftExpandedMargin(),y);
                else
                    (*iter)->move(node_ptrView::ArcPainter::NoMargin,y);
                (*iter)->setVisible(true);
                y = (*iter)->geometry().bottom();
            }
            else if(next &&*iter == this) {
                draw = true;
                y = (*iter)->geometry().bottom();
            }
        }
        parentArc->drawnode_ptr(true);
    }
}

void Arcnode_ptrViewWidget::insert_nodes(numt pos, numt count){
    if(count > 0 &&pos <= m_arcChild.size()) {
        setLeaf(false);
        if(m_expanded) {
            auto iterfirst = m_arcChild.insert(std::next(m_arcChild.cbegin(),pos), count, nullptr);
            auto iter = iterfirst;
            auto child = m_nodeWidget->index().model()->index(m_nodeWidget->index(),pos);
            while (count &&child.is_valid()) {
                *iter = new Arcnode_ptrViewWidget(child,m_view,this);
                --count;
                ++iter;
                child = child.nextBrother();
            }
            (*iterfirst)->drawnode_ptr();
        }
    }
}

void Arcnode_ptrViewWidget::mousePressEvent(QMouseEvent *event) {
    if(m_nodeArcVisible &&event->button() == Qt::LeftButton) {
        if(event->y() > m_nodeWidget->geometry().bottom() + m_view->m_arcPainter->bottomnode_ptrMargin()
                &&event->y() <= m_nodeWidget->geometry().bottom() + m_view->m_arcPainter->bottomnode_ptrMargin() + m_view->m_arcPainter->heightToolZone()
                &&event->x() < m_view->m_arcPainter->widthToolZone(node_ptrView::EndOfTool)){
            if(event->x() < m_view->m_arcPainter->widthToolZone(node_ptrView::ExpandTool)) {
                if(!m_leaf &&m_nodeWidget->index().flags().test(ExpendableFLagnode_ptr))
                    setExpanded(!m_expanded);
            }
            else if (event->x() < m_view->m_arcPainter->widthToolZone(node_ptrView::ElderTool)) {
                if(m_nodeWidget->index().flags().test(ElderEnableFlagnode_ptr))
                    m_nodeWidget->index().model()->insert_nodes(m_nodeWidget->index(),0,1);
            }
            else if (event->x() < m_view->m_arcPainter->widthToolZone(node_ptrView::BrotherTool)) {
                if(m_nodeWidget->index().flags().test(BrotherEnableFlagnode_ptr))
                    m_nodeWidget->index().model()->insert_nodes(m_nodeWidget->index().parent(),m_nodeWidget->index().position() + 1,1);

            }
            else if (event->x() < m_view->m_arcPainter->widthToolZone(node_ptrView::DelTool)) {
                if(m_nodeWidget->index().flags().test(DelEnableFlagnode_ptr))
                    m_nodeWidget->index().model()->removenode_ptrs(m_nodeWidget->index());
            }
        }
        else if((m_expanded &&event->x() < m_view->m_arcPainter->leftExpandedMargin() &&event->y() > m_nodeWidget->geometry().bottom()))
            setExpanded(false);
    }
}

void Arcnode_ptrViewWidget::paintEvent(QPaintEvent */*event*/) {
    if(m_nodeArcVisible)
        m_view->m_arcPainter->drawToolZone(this);
    if(!m_leaf &&m_nodeArcVisible &&m_expanded)
        m_view->m_arcPainter->drawArc(this);
}

void Arcnode_ptrViewWidget::removenode_ptrs(numt pos, numt count) {
    if(count &&pos < m_arcChild.size()) {
        auto iter = std::next(m_arcChild.begin(), pos);
        if(count == 1) {
            (*iter)->deleteLater();
            m_arcChild.erase(iter);
        }
        else {
            auto firstIter = iter;
            while(count &&iter != m_arcChild.end()) {
                (*iter)->deleteLater();
                ++iter;
                --count;
            }
            m_arcChild.erase(firstIter,iter);
        }
        if(m_arcChild.size()) {
            if(m_arcChild.size() != pos)
                m_arcChild[pos]->drawnode_ptr();
            else {
                adjustSize();
                drawnode_ptr(true);
            }
        }
        else
            setLeaf(true);
    }
}

void Arcnode_ptrViewWidget::setExpanded(bool bb){
    if(m_expanded != bb) {
        m_expanded = bb;
        if(m_expanded) {
            auto index = m_nodeWidget->index();
            m_arcChild.resize(index.child_count());
            auto child = index.model()->index(index,0);
            for (auto iterVec = m_arcChild.begin(); iterVec != m_arcChild.end(); ++iterVec) {
                *iterVec = new Arcnode_ptrViewWidget(child,m_view,this);
                child = child.nextBrother();
            }
            m_arcChild.front()->drawnode_ptr();
        }
        else {
            for (auto iter = m_arcChild.begin(); iter != m_arcChild.end(); ++iter)
                (*iter)->deleteLater();
            m_arcChild.clear();
            drawnode_ptr();
        }
    }
}

void Arcnode_ptrViewWidget::setLeaf(bool bb) {
    if((bb &&!m_leaf) || (m_leaf &&!bb)){
        m_leaf = bb;
        m_expanded = false;
        repaint();
    }
}

void Arcnode_ptrViewWidget::setnode_ptrWidget(node_ptrWidget *widget) {
    auto draw = m_nodeWidget &&m_nodeWidget->isVisible();
    if(m_nodeWidget) {
        if(m_nodeWidget->index().internal_pointer() != widget->index().internal_pointer())
            m_view->m_arcMap.erase(m_nodeWidget->index().internal_pointer());
        m_nodeWidget->hide();
        m_nodeWidget->deleteLater();
    }
    m_nodeWidget = widget;
    m_view->m_arcMap[m_nodeWidget->index().internal_pointer()] = this;
    m_nodeWidget->set_parent(this);
    if(m_view->selectionModel()->isCurrentIndex(m_nodeWidget->index()))
        m_nodeWidget->set_etatSelection(node_ptrWidget::Current);
    else if(m_view->selectionModel()->isSelected(m_nodeWidget->index()))
        m_nodeWidget->set_etatSelection(node_ptrWidget::Selected);
    m_nodeWidget->updateData(m_nodeWidget->index(),All_Role);
    connect(m_nodeWidget,&node_ptrWidget::leftClicked,this,[this]()
        {m_view->clickLeftOn(m_nodeWidget->index());});
    if(m_nodeArcVisible)
        m_nodeWidget->move(m_view->m_arcPainter->leftnode_ptrMargin(),m_view->m_arcPainter->topnode_ptrMargin());
    if(draw)
        drawnode_ptr();
    else
        adjustSize();
}

QSize Arcnode_ptrViewWidget::sizeHint() const{
    auto &arcPainter = *m_view->m_arcPainter;
    if(m_expanded) {
        QSize sz(0,m_arcChild.back()->geometry().bottom() + arcPainter.bottomToolsZoneMargin());
        if(m_nodeArcVisible)
            sz.rwidth() = m_nodeWidget->geometry().right();
        for (auto iter = m_arcChild.cbegin(); iter != m_arcChild.cend(); ++iter) {
            if((*iter)->geometry().right() > sz.width())
                sz.rwidth() = (*iter)->geometry().right();
        }
        if(m_nodeArcVisible)
            sz.rwidth() += arcPainter.rightnode_ptrMargin();
        return sz;
    }
    auto sz = m_nodeWidget->sizeHint();
    if(sz.width() < arcPainter.widthToolZone(node_ptrView::EndOfTool))
        sz.rwidth() = arcPainter.widthToolZone(node_ptrView::EndOfTool);
    sz.rwidth() += arcPainter.leftnode_ptrMargin() + arcPainter.rightnode_ptrMargin();
    sz.rheight() += arcPainter.topnode_ptrMargin() + arcPainter.bottomnode_ptrMargin()
                    + arcPainter.heightToolZone() + arcPainter.bottomToolsZoneMargin();
    return sz;
}

///////////////////////////////////////// IndexWidget /////////////////////////////////////////
IndexWidget::IndexWidget(const node_ptrIndex &index, QWidget *parent)
    : QWidget(parent), m_index(index) {
    if(index.data(model_base::Orientation_Role).toUInt() == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
}

/////////////////////////////////////// node_ptrView //////////////////////////////////////////
node_ptrView::node_ptrView(std::unique_ptr<ArcPainter> &&arcPainter, QWidget *parent)
    : QScrollArea (parent), m_arcPainter(std::move(arcPainter)) {}

void node_ptrView::clickLeftOn(const node_ptrIndex &index) {
    switch (m_selectionMode) {
    case SingleSelection:
        m_selectionModel->setCurrentIndex(index,SelectionModel::Toggle);
        break;
    case MultiSelection:
        break;
    case ContinousSelection:
        break;
    case ExtendedSelection:
        break;
    case NoSelection:
        break;
    }
}

void node_ptrView::currentChanged(const node_ptrIndex &current, const node_ptrIndex &previous){
    if(current.is_valid()) {
        auto iter = m_arcMap.find(current.internal_pointer());
        if(iter != m_arcMap.end())
            iter->second->nodeWidget()->set_etatSelection(node_ptrWidget::Current);
    }
    if(previous.is_valid()) {
        auto iter = m_arcMap.find(previous.internal_pointer());
        if(iter != m_arcMap.end())
            iter->second->nodeWidget()->set_etatSelection(node_ptrWidget::NoSelected);
    }
}

void node_ptrView::deleteRoot(){
    m_arcMap.clear();
    delete takeWidget();
}

void node_ptrView::insert_nodes(const node_ptrIndex &parent, numt pos, numt count) {
    auto iter = m_arcMap.find(parent.internal_pointer());
    if(iter != m_arcMap.end())
        iter->second->insert_nodes(pos,count);
}

void node_ptrView::removenode_ptrs(const node_ptrIndex &parent, numt pos, numt count) {
    auto iter = m_arcMap.find(parent.internal_pointer());
    if(iter != m_arcMap.end())
        iter->second->removenode_ptrs(pos, count);
}

void node_ptrView::setCurrentIndex(const node_ptrIndex &index){
    m_selectionModel->setCurrentIndex(index,model_base::node_ptrSelectionModel::Select);
}

void node_ptrView::setDelegate(Delegate *delegate) {
    if(m_delegate)
        delete m_delegate;
    m_delegate = delegate;
    m_delegate->set_parent(this);
    if(m_delegate &&m_model)
        resetRoot();
}

void node_ptrView::setModel(Model *model) {
    if(m_model &&static_cast<QObject*>(m_model->parent()) == static_cast<QObject*>(this))
        m_model->deleteLater();
    m_model = model;
    if(!m_model->parent())
        m_model->set_parent(this);
    connect(m_model,&Model::dataChanged,this,&node_ptrView::updateData);
    connect(m_model,&Model::nodesAboutToBeRemoved,this,&node_ptrView::removenode_ptrs);
    connect(m_model,&Model::modelAboutToBeReset,this,&node_ptrView::deleteRoot);
    connect(m_model,&Model::modelAboutToReset_data,this,[this](){m_connexionUpdateData = false;});
    connect(m_model,&Model::modelReset,this,&node_ptrView::resetRoot);
    connect(m_model,&Model::modelReset_data,this,[this](){
        m_connexionUpdateData = true;
        updateAllData();});
    connect(m_model,&Model::nodesInserted,this,&node_ptrView::insert_nodes);
    if(m_delegate &&m_model)
        resetRoot();
    setSelectionModel(new SelectionModel(m_model,this));
}

void node_ptrView::setSelectionModel(SelectionModel *selectionModel) {
    if(!m_selectionMode)
        disconnect(m_selectionModel,nullptr,this,nullptr);
    m_selectionModel = selectionModel;
    connect(m_selectionModel,&SelectionModel::currentChanged,this,&node_ptrView::currentChanged);
}
void node_ptrView::resetRoot(){
    if(widget())
        deleteRoot();
    auto rootIndex = m_model->index(node_ptrIndex(),0);
    if(rootIndex.flags().test(model_base::VisibleFlagnode_ptr))
        setWidget(new Arcnode_ptrViewWidget(rootIndex,this,this,true));
    else
        setWidget(new Rootnode_ptrViewWidget(rootIndex,this));
}

void node_ptrView::updateAllData() {
    if(m_connexionUpdateData)
        for (auto iter =m_arcMap.begin(); iter != m_arcMap.cend(); ++iter)
            iter->second->setnode_ptrWidget(iter->second->nodeWidget()->index());
}

void node_ptrView::updateData(const node_ptrIndex &index, flag role) {
    if(m_connexionUpdateData) {
        auto iter = m_arcMap.find(index.internal_pointer());
        if(iter != m_arcMap.end()){
            if(index.cible() != Node_Cible)
                iter->second->nodeWidget()->updateData(index,role);
            else
                iter->second->setnode_ptrWidget(index);
        }
    }
}

/////////////////////////////////////////////// node_ptrWidget ///////////////////////////////////////////////////
node_ptrWidget::~node_ptrWidget() {
    for (auto iter = m_cibleMap.cbegin(); iter != m_cibleMap.cend(); ++iter)
        disconnect(iter->second,&QObject::destroyed,this,&node_ptrWidget::removeSubnode_ptrWidget);
}

void node_ptrWidget::addSubnode_ptrWidget(Subnode_ptrWidget *subnode_ptr) {
    m_mainLayout->addWidget(subnode_ptr);
    m_cibleMap.insert({subnode_ptr->index().subIndex(),subnode_ptr});
    connect(subnode_ptr,&QObject::destroyed,this,&node_ptrWidget::removeSubnode_ptrWidget);
    subnode_ptr->updateData(model_base::All_Role);
}

void node_ptrWidget::updateData() {
    IndexWidget::updateData(model_base::All_Role);
    for (auto iter = m_cibleMap.begin(); iter != m_cibleMap.end(); ++iter)
        iter->second->updateData(model_base::All_Role);
}

void node_ptrWidget::updateData(const model_base::node_ptrIndex &index, flag role) {
    if(index.cible() == model_base::Node_Cible)
        IndexWidget::updateData(role);
    else {
        auto iters = m_cibleMap.equal_range(index.subIndex());
        for (auto iter = iters.first; iter != iters.second; ++iter)
            iter->second->updateData(role);
    }
}

/////////////////////////////////////////////// Rootnode_ptrViewWidget ///////////////////////////////////////////////////

