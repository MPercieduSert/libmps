#include "NodeView.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace widgetMPS;

////////////////////////////////////// AbstractNodeDelegate /////////////////////////////////
AbstractNodeDelegate::AbstractNodeDelegate(QObject * parent)
    : QObject(parent) {}

////////////////////////////////////// ArcWidget //////////////////////////////////////////
ArcWidget::ArcWidget(NodeWidget *node, NodeView *view, QWidget * parent)
    : QWidget (parent), m_view(view) {
    m_secondLayout = new QVBoxLayout;
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addLayout(m_secondLayout,1);
    m_mainLayout->addStretch();
    setNodeWidget(node);
    if(!m_nodeWidget->index().leaf()) {
        m_expandButton = new QPushButton;
        m_expandLayout = new QVBoxLayout;
        m_expandLayout->addWidget(m_expandButton);
        m_expandLayout->addStretch();
        m_mainLayout->insertLayout(0,m_expandLayout);
        setExpandEtat(false);
    }
}

void ArcWidget::setExpandEtat(bool bb) {
    m_expandEtat = bb;
    if(bb) {
        m_expandButton->setText("-");
    }
    else {
        m_expandButton->setText("+");
    }
}

void ArcWidget::setNodeWidget(NodeWidget * widget) {
    if(m_nodeWidget) {
        m_secondLayout->removeWidget(m_nodeWidget);
        m_view->m_arcMap.erase(m_nodeWidget->index());
        m_nodeWidget->deleteLater();
    }
    m_nodeWidget = widget;
    m_nodeWidget->updateData();
    m_nodeWidget->connexion();
    m_view->m_arcMap[m_nodeWidget->index()] = this;
    m_secondLayout->insertWidget(NodeWidgetIndice,m_nodeWidget);
}

QSize ArcWidget::sizeHint() const{
    return m_nodeWidget->sizeHint();
}

/////////////////////////////////////// NodeView //////////////////////////////////////////
NodeView::NodeView(QWidget * parent)
    : QScrollArea (parent) {}

void NodeView::deleteRoot(){
    m_arcMap.clear();
    delete takeWidget();
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
        delete m_model;
    m_model = model;
    if(!m_model->parent())
        m_model->setParent(this);
    connect(m_model,qOverload<const NodeIndex &>(&AbstractNodeModel::dataChanged),this,&NodeView::updateData);
    connect(m_model,&AbstractNodeModel::modelAboutToBeReset,this,&NodeView::deleteRoot);
    connect(m_model,&AbstractNodeModel::modelReset,this,&NodeView::resetRoot);
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
    }
