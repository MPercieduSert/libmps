#include "NodeView.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace widgetMPS;

////////////////////////////////////// AbstractNodeDelegate /////////////////////////////////
AbstractNodeDelegate::AbstractNodeDelegate(QObject * parent)
    : QObject(parent) {}

////////////////////////////////////// ArcWidget //////////////////////////////////////////
NodeView::ArcWidget::ArcWidget(NodeWidget *node, NodeView *view, QWidget * parent)
    : QWidget (parent), m_view(view) {
    m_expandButton = new QPushButton;
    m_expandLayout = new QVBoxLayout;
    m_expandLayout->addWidget(m_expandButton);
    m_expandLayout->addStretch();
    m_secondLayout = new QVBoxLayout;
    setNodeWidget(node);
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addLayout(m_expandLayout);
    m_mainLayout->addLayout(m_secondLayout);
}

void NodeView::ArcWidget::setExpandEtat(bool bb) {
    m_expandEtat = bb;
    if(bb) {
        m_expandButton->setText("-");
    }
    else {
        m_expandButton->setText("+");
    }
}

void NodeView::ArcWidget::setNodeWidget(NodeWidget * widget) {
    if(m_nodeWidget) {
        m_secondLayout->removeWidget(m_nodeWidget);
        m_view->m_arcMap.erase(m_nodeWidget->index());
        delete m_nodeWidget;
    }
    m_nodeWidget = widget;
    m_view->m_arcMap[m_nodeWidget->index()] = this;
    m_secondLayout->insertWidget(NodeWidgetIndice,widget);
}

/////////////////////////////////////// NodeView //////////////////////////////////////////
NodeView::NodeView(QWidget * parent)
    : QScrollArea (parent) {
    setWidget(new ArcWidget(m_delegate->createWidget(m_model->index(0)),this));
}

void NodeView::setDelegate(AbstractNodeDelegate * delegate) {
    if(m_delegate)
        delete m_delegate;
    m_delegate = delegate;
    m_delegate->setParent(this);
}

void NodeView::setModel(AbstractNodeModel *model) {
    if(m_model->parent() == this)
        delete m_model;
    m_model = model;
    if(!m_model->parent())
        m_model->setParent(this);
    connect(m_model,qOverload<const NodeIndex &>(&AbstractNodeModel::dataChanged),this,&NodeView::updateData);
}

void NodeView::updateData(const NodeIndex & index) {
    auto iter = m_arcMap.find(index);
    if(iter != m_arcMap.end()){
        if(index.data(NodeType) == iter->second->nodeWidget()->type())
            iter->second->nodeWidget()->updateData();
        else
            iter->second->setNodeWidget(index);
    }
}

////////////////////////////////////////// NodeWidget /////////////////////////////////////
NodeWidget::NodeWidget(const NodeIndex & index, QWidget * parent, int tp)
    : QWidget (parent), m_type(tp), m_index(index)
    {}
