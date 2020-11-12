#include "NodeView.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace widgetMPS;
using namespace nodeViewWidget;

////////////////////////////////////// AbstractNodeDelegate /////////////////////////////////
AbstractNodeDelegate::AbstractNodeDelegate(QObject * parent)
    : QObject(parent) {}

////////////////////////////////////// ArcWidget //////////////////////////////////////////
ArcWidget::ArcWidget(NodeWidget *node, NodeView *view, QWidget * parent)
    : QWidget (parent), m_view(view) {
    m_mainLayout = new QVBoxLayout(this);
    setNodeWidget(node);
    setLeaf(m_nodeWidget->index().leaf());
}

void ArcWidget::insertRows(int first, int last){
    setLeaf(false);
    if(expandEtat()) {
        auto index = m_nodeWidget->index().model()->index(first,m_nodeWidget->index());
        while (index.isValid() && first <= last) {
            m_expandLayout->insertWidget(first + 1,new ArcWidget(index,m_view,this));
            ++first;
            index = index.nextBrother();
        }
    }
}

void ArcWidget::setExpandEtat(bool /*bb*/) {
    resize(sizeHint());
//       for(int i = m_secondLayout->count() - 1; i != 0; --i) {
//           auto item = m_secondLayout->itemAt(i);
//           m_secondLayout->removeItem(item);
//           delete item;
//       }
}

void ArcWidget::setLeaf(bool bb) {
    if(bb && !m_leaf){
        m_leaf = bb;
        m_mainLayout->removeItem(m_brancheLayout);
        m_brancheLayout->deleteLater();
        m_brancheWgt->deleteLater();
        m_expandLayout->deleteLater();
        resize(sizeHint());
    }
    else if (m_leaf && !bb) {
        m_leaf = bb;
        m_brancheWgt = new BrancheWidget(false,this);
        m_expandLayout = new QVBoxLayout;
        m_brancheLayout = new QHBoxLayout;
        m_brancheLayout->setAlignment(Qt::AlignLeft);
        m_brancheLayout->addWidget(m_brancheWgt);
        m_brancheLayout->addLayout(m_expandLayout);
        m_mainLayout->addLayout(m_brancheLayout);
        setExpandEtat(false);
        connect(m_brancheWgt,&BrancheWidget::expandStateChanged,this,&ArcWidget::setExpandEtat);
        resize(sizeHint());
    }
}

void ArcWidget::setNodeWidget(NodeWidget * widget) {
    if(m_nodeWidget) {
        m_mainLayout->replaceWidget(m_nodeWidget,widget,Qt::FindDirectChildrenOnly);
        m_view->m_arcMap.erase(m_nodeWidget->index());
        m_nodeWidget->deleteLater();
    }
    m_nodeWidget = widget;
    m_nodeWidget->updateData();
    m_nodeWidget->connexion();
    m_view->m_arcMap[m_nodeWidget->index()] = this;
    m_mainLayout->insertWidget(NodeWidgetIndice,m_nodeWidget);
    resize(sizeHint());
}

QSize ArcWidget::sizeHint() const{
    auto sz = m_nodeWidget->sizeHint();
    auto marge = m_mainLayout->contentsMargins();
    sz.rwidth() += marge.left() + marge.right();
    sz.rheight() += marge.bottom() + marge.top();
    if(m_leaf)
        return sz;
    else if (m_brancheWgt->expanded()){
        sz.rheight() += m_brancheWgt->sizeHint().height();
        sz.rheight() += m_mainLayout->spacing();
        return sz;
    }
    else {
        sz.rheight() += m_brancheWgt->sizeHint().height();
        sz.rheight() += m_mainLayout->spacing();
        return sz;
    }
//    return m_nodeWidget ? QSize(m_nodeWidget->sizeHint().width(),
//                 m_nodeWidget->sizeHint().height()
//                 + (m_leaf ? 0 : m_brancheWgt->height()))
//                        : QSize(0,0);
}

//////////////////////////////////////// BrancheWidget ////////////////////////////////////
BrancheWidget::BrancheWidget(bool expandState, QWidget * parent)
    : QWidget(parent), m_expanded(expandState) {
    if(m_expanded)
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
    else
        setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void BrancheWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton)
        setExpanded(!m_expanded);
}

void BrancheWidget::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    if(m_expanded){
        pen.setWidth(WidthLine);
        painter.setPen(pen);
        painter.drawLine(width()/2,0,width()/2,height());
    }
    else {
        pen.setWidth(WidthCircle);
        painter.setPen(pen);
        for (auto i = 0; i < NbrCircle; ++i) {
            QPoint pt(WidthCircle / 2+ Rayon + Ecart * i, WidthCircle / 2+ Rayon);
            painter.drawEllipse(pt,Rayon,Rayon);
        }
    }
}

void BrancheWidget::setExpanded(bool bb){
    if(m_expanded != bb) {
        m_expanded = bb;
        if(m_expanded)
            setSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
        else
            setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        updateGeometry();
        emit expandStateChanged(m_expanded);
    }
}

QSize BrancheWidget::sizeHint() const{
    return m_expanded ? QSize(HSize,VSize)
                      : QSize(HSizeCircle,VSizeCircle);
}
/////////////////////////////////////// NodeView //////////////////////////////////////////
NodeView::NodeView(QWidget * parent)
    : QScrollArea (parent) {}

void NodeView::deleteRoot(){
    m_arcMap.clear();
    delete takeWidget();
}

void NodeView::insertRows(const NodeIndex & parent, int first, int last) {
    auto iter = m_arcMap.find(parent);
    if(iter != m_arcMap.end())
        iter->second->insertRows(first,last);
}

void NodeView::setDelegate(AbstractNodeDelegate * delegate) {
    if(m_delegate)
        delete m_delegate;
    m_delegate = delegate;
    m_delegate->setParent(this);
    if(m_delegate && m_model)
        resetRoot();
}

void NodeView::setModel(AbstractNodeModel * model) {
    if(m_model && static_cast<QObject*>(m_model->parent()) == static_cast<QObject*>(this))
        m_model->deleteLater();
    m_model = model;
    if(!m_model->parent())
        m_model->setParent(this);
    connect(m_model,qOverload<const NodeIndex &>(&AbstractNodeModel::dataChanged),this,&NodeView::updateData);
    connect(m_model,&AbstractNodeModel::modelAboutToBeReset,this,&NodeView::deleteRoot);
    connect(m_model,&AbstractNodeModel::modelReset,this,&NodeView::resetRoot);
    connect(m_model,&AbstractNodeModel::rowsInserted,this,&NodeView::insertRows);
    if(m_delegate && m_model)
        resetRoot();
}

void NodeView::resetRoot(){
    if(widget())
        deleteRoot();
    setWidget(new ArcWidget(m_delegate->createWidget(m_model->index(0)),this));
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
NodeWidget::NodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : QFrame (parent), m_type(tp), m_index(index) {
    setFrameShape(Box);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    }
