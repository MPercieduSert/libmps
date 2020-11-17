#include "NodeView.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace widgetMPS;

////////////////////////////////////// AbstractNodeDelegate /////////////////////////////////
AbstractNodeDelegate::AbstractNodeDelegate(QObject * parent)
    : QObject(parent) {}

////////////////////////////////////// ArcNodeViewWidget //////////////////////////////////////////
ArcNodeViewWidget::ArcNodeViewWidget(NodeWidget *node, NodeView *view, QWidget * parent, bool root)
    : QWidget (parent), m_root(root), m_view(view) {
    setNodeWidget(node);
    m_leaf = m_nodeWidget->index().leaf();
}

void ArcNodeViewWidget::drawNode(bool next) {
    adjustSize();
    if(!m_root) {
        auto parentArc = static_cast<ArcNodeViewWidget *>(parentWidget());
        auto draw = false;
        auto y = 0;
        for (auto iter = parentArc->m_arcVec.begin(); iter != parentArc->m_arcVec.end(); ++iter) {
            if (!next && !draw && *iter == this) {
                draw = true;
                if(iter ==  parentArc->m_arcVec.begin())
                    y = parentArc->nodeWidget()->geometry().bottom();
                else
                    y = (*std::prev(iter))->geometry().bottom();
            }
            if(draw) {
                (*iter)->move(LeftExpandMargin,y);
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

void ArcNodeViewWidget::insertNodes(szt first, szt last){
    if(last >= first) {
        setLeaf(false);
        if(m_expanded) {
            auto iterfirst = m_arcVec.insert(std::next(m_arcVec.cbegin(),first),static_cast<szt>(last - first + 1), nullptr);
            auto iter = iterfirst;
            auto child = m_nodeWidget->index().model()->index(first,m_nodeWidget->index());
            while (first <= last && child.isValid()) {
                *iter = new ArcNodeViewWidget(child,m_view,this);
                ++first;
                ++iter;
                child = child.nextBrother();
            }
            (*iterfirst)->drawNode();
        }
    }
}

void ArcNodeViewWidget::mousePressEvent(QMouseEvent *event) {
    if(!m_leaf && event->button() == Qt::LeftButton)
        if((m_expanded && event->x() < LeftExpandMargin && event->y() > m_nodeWidget->geometry().bottom())
                || (! m_expanded && event->x() < RigthCircles && event->y() > m_nodeWidget->geometry().bottom()))
        setExpanded(!m_expanded);
}

void ArcNodeViewWidget::paintEvent(QPaintEvent * /*event*/) {
    if(!m_leaf) {
        QPainter painter(this);
        QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
        if(m_expanded){
            pen.setWidth(WidthLine);
            pen.setJoinStyle(Qt::RoundJoin);
            pen.setCapStyle(Qt::RoundCap);
            painter.setPen(pen);
            auto x = m_nodeWidget->geometry().left() + WidthLine / 2;
            auto y = 0;
            for (auto iter = m_arcVec.begin(); iter != m_arcVec.end(); ++iter) {
                auto nodeGeo = (*iter)->nodeWidget()->geometry();
                y = std::min<int>(((*iter)->mapToParent(nodeGeo.topLeft()).y() + (*iter)->mapToParent(nodeGeo.bottomLeft()).y() )/ 2,
                                  (*iter)->mapToParent(nodeGeo.topLeft()).y() + HMaxLine);
                painter.drawLine(x,y, x + LeftExpandMargin - HSpacing - WidthLine, y);
            }
            painter.drawLine(x,
                             m_arcVec.front()->mapToParent(m_arcVec.front()->nodeWidget()->geometry().topLeft()).y() + WidthLine / 2,
                             x ,y);
        }
        else {
            pen.setWidth(WidthCircle);
            painter.setPen(pen);
            painter.setBrush(QBrush(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText)));
            for (auto i = 0; i < NbrCircle; ++i) {
                QPoint pt(m_nodeWidget->geometry().left() + WidthCircle + Rayon + Ecart * i,
                          m_nodeWidget->geometry().bottom() + VSpacing + WidthCircle + Rayon);
                painter.drawEllipse(pt,Rayon,Rayon);
            }
        }
    }
}

//! Suppresion du noeud et de count frère.
void ArcNodeViewWidget::removeNodes(szt first, szt last) {
    if(first <= last && last < m_arcVec.size()) {
        auto firstIter = std::next(m_arcVec.begin(), first);
        auto lastIter = std::next(m_arcVec.begin(), last + 1);
        for (auto iter = firstIter; iter != lastIter ; ++iter)
            (*iter)->deleteLater();
        m_arcVec.erase(firstIter,lastIter);
        if(m_arcVec.size()) {
            if(m_arcVec.size() != first)
                m_arcVec[first]->drawNode();
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
            m_arcVec.resize(index.model()->childCount(index));
            auto child = index.model()->index(0,index);
            for (auto iterVec = m_arcVec.begin(); iterVec != m_arcVec.end(); ++iterVec) {
                *iterVec = new ArcNodeViewWidget(child,m_view,this);
                child = child.nextBrother();
            }
            m_arcVec.front()->drawNode();
        }
        else {
            for (auto iter = m_arcVec.begin(); iter != m_arcVec.end(); ++iter)
                (*iter)->deleteLater();
            m_arcVec.clear();
            drawNode();
        }
    }
}

void ArcNodeViewWidget::setLeaf(bool bb) {
    if(bb && !m_leaf){
        m_leaf = bb;
        m_expanded = false;
        drawNode(true);
    }
    else if (m_leaf && !bb) {
        m_leaf = bb;
        drawNode(true);
    }
}

void ArcNodeViewWidget::setNodeWidget(NodeWidget * widget) {
    auto draw = m_nodeWidget && m_nodeWidget->isVisible();
    if(m_nodeWidget) {
        m_view->m_arcMap.erase(m_nodeWidget->index());
        m_nodeWidget->hide();
        m_nodeWidget->deleteLater();
    }
    m_nodeWidget = widget;
    m_nodeWidget->setParent(this);
    if(m_view->selectionModel()->isCurrentIndex(m_nodeWidget->index()))
        m_nodeWidget->setEtatSelection(NodeWidget::Current);
    else if(m_view->selectionModel()->isSelected(m_nodeWidget->index()))
        m_nodeWidget->setEtatSelection(NodeWidget::Selected);
    m_nodeWidget->updateData();
    m_nodeWidget->connexion();
    m_view->m_arcMap[m_nodeWidget->index()] = this;
    m_nodeWidget->move(LeftNodeMargin,TopNodeMargin);
    m_nodeWidget->setVisible(true);
    if(draw)
        drawNode();
    else
        adjustSize();
}

QSize ArcNodeViewWidget::sizeHint() const{
    if(m_expanded) {
        QSize sz(m_nodeWidget->geometry().right(),m_arcVec.back()->geometry().bottom() + BottomMargin);
        for (auto iter = m_arcVec.cbegin(); iter != m_arcVec.cend(); ++iter) {
            if((*iter)->geometry().right() > sz.width())
                sz.rwidth() = (*iter)->geometry().right();
        }
        sz.rwidth() += RightMargin;
        return sz;
    }
    auto sz = m_nodeWidget->sizeHint();
    sz.rwidth() += LeftNodeMargin + RightMargin;
    sz.rheight() += TopNodeMargin;
    if(!m_leaf)
        sz.rheight() += 2 * VSpacing + VSizeCircle;
    return sz;
}

/////////////////////////////////////// NodeView //////////////////////////////////////////
NodeView::NodeView(QWidget * parent)
    : QScrollArea (parent) {}

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
        auto iter = m_arcMap.find(current);
        if(iter != m_arcMap.end())
            iter->second->nodeWidget()->setEtatSelection(NodeWidget::Current);
    }
    if(previous.isValid()) {
        auto iter = m_arcMap.find(previous);
        if(iter != m_arcMap.end())
            iter->second->nodeWidget()->setEtatSelection(NodeWidget::NoSelected);
    }
}

void NodeView::deleteRoot(){
    m_arcMap.clear();
    delete takeWidget();
}

void NodeView::insertNodes(const NodeIndex & parent, szt first, szt last) {
    auto iter = m_arcMap.find(parent);
    if(iter != m_arcMap.end())
        iter->second->insertNodes(first,last);
}

void NodeView::removeNodes(const NodeIndex & parent, szt first, szt last) {
    auto iter = m_arcMap.find(parent);
    if(iter != m_arcMap.end())
        iter->second->removeNodes(first, last);
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
    connect(m_model,qOverload<const NodeIndex &>(&Model::dataChanged),this,&NodeView::updateData);
    connect(m_model,&Model::nodesAboutToBeRemoved,this,&NodeView::removeNodes);
    connect(m_model,&Model::modelAboutToBeReset,this,&NodeView::deleteRoot);
    connect(m_model,&Model::modelReset,this,&NodeView::resetRoot);
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
    setWidget(new ArcNodeViewWidget(m_model->index(0),this,this,true));
}

void NodeView::updateData(const NodeIndex & index) {
    auto iter = m_arcMap.find(index);
    if(iter != m_arcMap.end()){
        if(index.data(NodeTypeCible) == iter->second->nodeWidget()->type()) {
            iter->second->nodeWidget()->deconnexion();
            iter->second->nodeWidget()->updateData();
            iter->second->nodeWidget()->connexion();
        }
        else
            iter->second->setNodeWidget(index);
    }
}

////////////////////////////////////////// NodeWidget /////////////////////////////////////
NodeWidget::NodeWidget(const NodeIndex & index, ArcNodeViewWidget *parent, int tp)
    : QWidget(parent),
      m_colorLine(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText)),
      m_type(tp), m_index(index)
    {}

void NodeWidget::mousePressEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton)
        static_cast<ArcNodeViewWidget *>(parentWidget())->view()->clickLeftOn(m_index);
}

void NodeWidget::paintEvent(QPaintEvent * /*event*/) {
    QPen pen(m_colorLine);
    pen.setWidth(m_widthLine);
    QPainter painter(this);
    painter.setPen(pen);
    painter.drawRoundedRect(m_widthLine / 2,m_widthLine / 2,width() - m_widthLine,height() - m_widthLine,Rayon,Rayon,Qt::AbsoluteSize);
}

void NodeWidget::setEtatSelection(EtatSelection etat) {
    if(m_etatSelection != etat){
        switch (etat) {
        case NoSelected:
            m_widthLine = NoSelectedWidth;
            m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText);
            break;
        case Selected:
            m_widthLine = SelectedWidth;
            m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
            break;
        case Current:
            m_widthLine = CurrentWidth;
            m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
            break;
        case Initial:
            break;
        }
        repaint();
    }
}
