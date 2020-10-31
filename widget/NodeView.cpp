#include "NodeView.h"

using namespace widgetMPS;

NodeView::NodeView(QWidget * parent) : QScrollArea (parent) {

}

void NodeView::setModel(modelMPS::AbstractNodeModel *model) {
    if(m_model->parent() == this)
        delete m_model;
    m_model = model;
    if(!m_model->parent())
        m_model->setParent(this);
}

////////////////////////////////////// NodeWidget //////////////////////////////////////////
NodeView::NodeWidget::NodeWidget(QWidget * parent) : QWidget (parent) {
    m_expandButton = new QPushButton;
    m_expandLayout = new QVBoxLayout;
    m_expandLayout->addWidget(m_expandButton);
    m_expandLayout->addStretch();
    m_secondLayout = new QVBoxLayout;
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addLayout(m_expandLayout);
    m_mainLayout->addLayout(m_expandLayout);
}

void NodeView::NodeWidget::setExpandEtat(bool bb) {
    m_expandEtat = bb;
    if(bb) {
        m_expandButton->setText("-");
    }
    else {
        m_expandButton->setText("+");
    }
}
