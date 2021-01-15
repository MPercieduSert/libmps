#include "NodeView.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace widgetMPS;

////////////////////////////////////// AbstractNodeDelegate /////////////////////////////////
AbstractNodeDelegate::AbstractNodeDelegate(QObject * parent) : QObject(parent) {}

////////////////////////////////////// ArcNodeViewWidget //////////////////////////////////////////
ArcNodeViewWidget::ArcNodeViewWidget(NodeWidget *node, NodeView *view, QWidget * parent, bool root, bool nodeArcVisible)
    : QWidget (parent), m_leaf(node->index().leaf()), m_root(root), m_nodeArcVisible(nodeArcVisible), m_view(view)
    {setNodeWidget(node);}

void ArcNodeViewWidget::drawNode(bool next) {
    adjustSize();
    if(!m_root) {
        auto parentArc = static_cast<ArcNodeViewWidget *>(parentWidget());
        auto draw = false;
        auto y = 0;
        auto & arcPainter = *m_view->m_arcPainter;
        for (auto iter = parentArc->m_arcChild.begin(); iter != parentArc->m_arcChild.end(); ++iter) {
            if (!next && !draw && *iter == this) {
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
                    (*iter)->move(NodeView::ArcPainter::NoMargin,y);
                (*iter)->setVisible(true);
                y = (*iter)->geometry().bottom();
            }
            else if(next && *iter == this) {
                draw = true;
                y = (*iter)->geometry().bottom();
            }
        }
        parentArc->drawNode(true);
    }
}

void ArcNodeViewWidget::insertNodes(numt pos, numt count){
    if(count > 0 && pos <= m_arcChild.size()) {
        setLeaf(false);
        if(m_expanded) {
            auto iterfirst = m_arcChild.insert(std::next(m_arcChild.cbegin(),pos), count, nullptr);
            auto iter = iterfirst;
            auto child = m_nodeWidget->index().model()->index(m_nodeWidget->index(),pos);
            while (count && child.isValid()) {
                *iter = new ArcNodeViewWidget(child,m_view,this);
                --count;
                ++iter;
                child = child.nextBrother();
            }
            (*iterfirst)->drawNode();
        }
    }
}

void ArcNodeViewWidget::mousePressEvent(QMouseEvent *event) {
    if(m_nodeArcVisible && event->button() == Qt::LeftButton) {
        if(event->y() > m_nodeWidget->geometry().bottom() + m_view->m_arcPainter->bottomNodeMargin()
                && event->y() <= m_nodeWidget->geometry().bottom() + m_view->m_arcPainter->bottomNodeMargin() + m_view->m_arcPainter->heightToolZone()
                && event->x() < m_view->m_arcPainter->widthToolZone(NodeView::EndOfTool)){
            if(event->x() < m_view->m_arcPainter->widthToolZone(NodeView::ExpandTool)) {
                if(!m_leaf && m_nodeWidget->index().flags().test(ExpendableFLagNode))
                    setExpanded(!m_expanded);
            }
            else if (event->x() < m_view->m_arcPainter->widthToolZone(NodeView::ElderTool)) {
                if(m_nodeWidget->index().flags().test(ElderEnableFlagNode))
                    m_nodeWidget->index().model()->insertNodes(m_nodeWidget->index(),0,1);
            }
            else if (event->x() < m_view->m_arcPainter->widthToolZone(NodeView::BrotherTool)) {
                if(m_nodeWidget->index().flags().test(BrotherEnableFlagNode))
                    m_nodeWidget->index().model()->insertNodes(m_nodeWidget->index().parent(),m_nodeWidget->index().position() + 1,1);

            }
            else if (event->x() < m_view->m_arcPainter->widthToolZone(NodeView::DelTool)) {
                if(m_nodeWidget->index().flags().test(DelEnableFlagNode))
                    m_nodeWidget->index().model()->removeNodes(m_nodeWidget->index());
            }
        }
        else if((m_expanded && event->x() < m_view->m_arcPainter->leftExpandedMargin() && event->y() > m_nodeWidget->geometry().bottom()))
            setExpanded(false);
    }
}

void ArcNodeViewWidget::paintEvent(QPaintEvent * /*event*/) {
    if(m_nodeArcVisible)
        m_view->m_arcPainter->drawToolZone(this);
    if(!m_leaf && m_nodeArcVisible && m_expanded)
        m_view->m_arcPainter->drawArc(this);
}

void ArcNodeViewWidget::removeNodes(numt pos, numt count) {
    if(count && pos < m_arcChild.size()) {
        auto iter = std::next(m_arcChild.begin(), pos);
        if(count == 1) {
            (*iter)->deleteLater();
            m_arcChild.erase(iter);
        }
        else {
            auto firstIter = iter;
            while(count && iter != m_arcChild.end()) {
                (*iter)->deleteLater();
                ++iter;
                --count;
            }
            m_arcChild.erase(firstIter,iter);
        }
        if(m_arcChild.size()) {
            if(m_arcChild.size() != pos)
                m_arcChild[pos]->drawNode();
            else {
                adjustSize();
                drawNode(true);
            }
        }
        else
            setLeaf(true);
    }
}

void ArcNodeViewWidget::setExpanded(bool bb){
    if(m_expanded != bb) {
        m_expanded = bb;
        if(m_expanded) {
            auto index = m_nodeWidget->index();
            m_arcChild.resize(index.childCount());
            auto child = index.model()->index(index,0);
            for (auto iterVec = m_arcChild.begin(); iterVec != m_arcChild.end(); ++iterVec) {
                *iterVec = new ArcNodeViewWidget(child,m_view,this);
                child = child.nextBrother();
            }
            m_arcChild.front()->drawNode();
        }
        else {
            for (auto iter = m_arcChild.begin(); iter != m_arcChild.end(); ++iter)
                (*iter)->deleteLater();
            m_arcChild.clear();
            drawNode();
        }
    }
}

void ArcNodeViewWidget::setLeaf(bool bb) {
    if((bb && !m_leaf) || (m_leaf && !bb)){
        m_leaf = bb;
        m_expanded = false;
        repaint();
    }
}

void ArcNodeViewWidget::setNodeWidget(NodeWidget * widget) {
    auto draw = m_nodeWidget && m_nodeWidget->isVisible();
    if(m_nodeWidget) {
        if(m_nodeWidget->index().internalPointer() != widget->index().internalPointer())
            m_view->m_arcMap.erase(m_nodeWidget->index().internalPointer());
        m_nodeWidget->hide();
        m_nodeWidget->deleteLater();
    }
    m_nodeWidget = widget;
    m_view->m_arcMap[m_nodeWidget->index().internalPointer()] = this;
    m_nodeWidget->setParent(this);
    if(m_view->selectionModel()->isCurrentIndex(m_nodeWidget->index()))
        m_nodeWidget->setEtatSelection(NodeWidget::Current);
    else if(m_view->selectionModel()->isSelected(m_nodeWidget->index()))
        m_nodeWidget->setEtatSelection(NodeWidget::Selected);
    m_nodeWidget->updateData(m_nodeWidget->index(),AllRole);
    connect(m_nodeWidget,&NodeWidget::leftClicked,this,[this]()
        {m_view->clickLeftOn(m_nodeWidget->index());});
    if(m_nodeArcVisible)
        m_nodeWidget->move(m_view->m_arcPainter->leftNodeMargin(),m_view->m_arcPainter->topNodeMargin());
    if(draw)
        drawNode();
    else
        adjustSize();
}

QSize ArcNodeViewWidget::sizeHint() const{
    auto & arcPainter = *m_view->m_arcPainter;
    if(m_expanded) {
        QSize sz(0,m_arcChild.back()->geometry().bottom() + arcPainter.bottomToolsZoneMargin());
        if(m_nodeArcVisible)
            sz.rwidth() = m_nodeWidget->geometry().right();
        for (auto iter = m_arcChild.cbegin(); iter != m_arcChild.cend(); ++iter) {
            if((*iter)->geometry().right() > sz.width())
                sz.rwidth() = (*iter)->geometry().right();
        }
        if(m_nodeArcVisible)
            sz.rwidth() += arcPainter.rightNodeMargin();
        return sz;
    }
    auto sz = m_nodeWidget->sizeHint();
    if(sz.width() < arcPainter.widthToolZone(NodeView::EndOfTool))
        sz.rwidth() = arcPainter.widthToolZone(NodeView::EndOfTool);
    sz.rwidth() += arcPainter.leftNodeMargin() + arcPainter.rightNodeMargin();
    sz.rheight() += arcPainter.topNodeMargin() + arcPainter.bottomNodeMargin()
                    + arcPainter.heightToolZone() + arcPainter.bottomToolsZoneMargin();
    return sz;
}

///////////////////////////////////////// IndexWidget /////////////////////////////////////////
IndexWidget::IndexWidget(const NodeIndex & index, QWidget * parent)
    : QWidget(parent), m_index(index) {
    if(index.data(modelMPS::OrientationRole).toUInt() == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
}

/////////////////////////////////////// NodeView //////////////////////////////////////////
NodeView::NodeView(std::unique_ptr<ArcPainter> &&arcPainter, QWidget * parent)
    : QScrollArea (parent), m_arcPainter(std::move(arcPainter)) {}

void NodeView::clickLeftOn(const NodeIndex & index) {
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

void NodeView::currentChanged(const NodeIndex & current, const NodeIndex & previous){
    if(current.isValid()) {
        auto iter = m_arcMap.find(current.internalPointer());
        if(iter != m_arcMap.end())
            iter->second->nodeWidget()->setEtatSelection(NodeWidget::Current);
    }
    if(previous.isValid()) {
        auto iter = m_arcMap.find(previous.internalPointer());
        if(iter != m_arcMap.end())
            iter->second->nodeWidget()->setEtatSelection(NodeWidget::NoSelected);
    }
}

void NodeView::deleteRoot(){
    m_arcMap.clear();
    delete takeWidget();
}

void NodeView::insertNodes(const NodeIndex & parent, numt pos, numt count) {
    auto iter = m_arcMap.find(parent.internalPointer());
    if(iter != m_arcMap.end())
        iter->second->insertNodes(pos,count);
}

void NodeView::removeNodes(const NodeIndex & parent, numt pos, numt count) {
    auto iter = m_arcMap.find(parent.internalPointer());
    if(iter != m_arcMap.end())
        iter->second->removeNodes(pos, count);
}

void NodeView::setCurrentIndex(const NodeIndex & index){
    m_selectionModel->setCurrentIndex(index,modelMPS::NodeSelectionModel::Select);
}

void NodeView::setDelegate(Delegate * delegate) {
    if(m_delegate)
        delete m_delegate;
    m_delegate = delegate;
    m_delegate->setParent(this);
    if(m_delegate && m_model)
        resetRoot();
}

void NodeView::setModel(Model *model) {
    if(m_model && static_cast<QObject*>(m_model->parent()) == static_cast<QObject*>(this))
        m_model->deleteLater();
    m_model = model;
    if(!m_model->parent())
        m_model->setParent(this);
    connect(m_model,&Model::dataChanged,this,&NodeView::updateData);
    connect(m_model,&Model::nodesAboutToBeRemoved,this,&NodeView::removeNodes);
    connect(m_model,&Model::modelAboutToBeReset,this,&NodeView::deleteRoot);
    connect(m_model,&Model::modelAboutToResetData,this,[this](){m_connexionUpdateData = false;});
    connect(m_model,&Model::modelReset,this,&NodeView::resetRoot);
    connect(m_model,&Model::modelResetData,this,[this](){
        m_connexionUpdateData = true;
        updateAllData();});
    connect(m_model,&Model::nodesInserted,this,&NodeView::insertNodes);
    if(m_delegate && m_model)
        resetRoot();
    setSelectionModel(new SelectionModel(m_model,this));
}

void NodeView::setSelectionModel(SelectionModel * selectionModel) {
    if(!m_selectionMode)
        disconnect(m_selectionModel,nullptr,this,nullptr);
    m_selectionModel = selectionModel;
    connect(m_selectionModel,&SelectionModel::currentChanged,this,&NodeView::currentChanged);
}
void NodeView::resetRoot(){
    if(widget())
        deleteRoot();
    auto rootIndex = m_model->index(NodeIndex(),0);
    if(rootIndex.flags().test(modelMPS::VisibleFlagNode))
        setWidget(new ArcNodeViewWidget(rootIndex,this,this,true));
    else
        setWidget(new RootNodeViewWidget(rootIndex,this));
}

void NodeView::updateAllData() {
    if(m_connexionUpdateData)
        for (auto iter =m_arcMap.begin(); iter != m_arcMap.cend(); ++iter)
            iter->second->setNodeWidget(iter->second->nodeWidget()->index());
}

void NodeView::updateData(const NodeIndex & index, flag role) {
    if(m_connexionUpdateData) {
        auto iter = m_arcMap.find(index.internalPointer());
        if(iter != m_arcMap.end()){
            if(index.cible() != NodeCible)
                iter->second->nodeWidget()->updateData(index,role);
            else
                iter->second->setNodeWidget(index);
        }
    }
}

/////////////////////////////////////////////// NodeWidget ///////////////////////////////////////////////////
NodeWidget::~NodeWidget() {
    for (auto iter = m_cibleMap.cbegin(); iter != m_cibleMap.cend(); ++iter)
        disconnect(iter->second,&QObject::destroyed,this,&NodeWidget::removeSubNodeWidget);
}

void NodeWidget::addSubNodeWidget(SubNodeWidget * subNode) {
    m_mainLayout->addWidget(subNode);
    m_cibleMap.insert({subNode->index().subIndex(),subNode});
    connect(subNode,&QObject::destroyed,this,&NodeWidget::removeSubNodeWidget);
    subNode->updateData(modelMPS::AllRole);
}

void NodeWidget::updateData() {
    IndexWidget::updateData(modelMPS::AllRole);
    for (auto iter = m_cibleMap.begin(); iter != m_cibleMap.end(); ++iter)
        iter->second->updateData(modelMPS::AllRole);
}

void NodeWidget::updateData(const modelMPS::NodeIndex & index, flag role) {
    if(index.cible() == modelMPS::NodeCible)
        IndexWidget::updateData(role);
    else {
        auto iters = m_cibleMap.equal_range(index.subIndex());
        for (auto iter = iters.first; iter != iters.second; ++iter)
            iter->second->updateData(role);
    }
}

/////////////////////////////////////////////// RootNodeViewWidget ///////////////////////////////////////////////////

