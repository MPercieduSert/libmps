#include "StandardNodeWidget.h"

using namespace widgetMPS;

///////////////////////////////////////// AbstractSubNodeWidget /////////////////////////////////////////
AbstractSubNodeWidget::AbstractSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : QWidget (parent), m_index(index) {
    if(index.data(modelMPS::OrientationRole).toUInt() == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
}

AbstractSubNodeWidget::~AbstractSubNodeWidget() {
    auto node = static_cast<StandardNodeWidget*>(parent());
    auto iter = node->m_cibleMap.cbegin();
    while (iter != node->m_cibleMap.cend() && iter->second != this)
        ++iter;
    if(iter != node->m_cibleMap.cend())
        node->m_cibleMap.erase(iter);
}
////////////////////////////////////////////////// LabelSubNodeWidget //////////////////////////////////////////
LabelSubNodeWidget::LabelSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : AbstractSubNodeWidget(index,parent) {
    m_label = new QLabel;
    m_mainLayout->addWidget(m_label);
}

void LabelSubNodeWidget::updateData()
    {m_label->setText(m_index.data(modelMPS::LabelRole).toString());}

////////////////////////////////////////////// StandardNodeWidget //////////////////////////////////////////
StandardNodeWidget::StandardNodeWidget(const NodeIndex & index, ArcNodeViewWidget * parent, int tp)
    : AbstractNodeWidget (index,parent,tp) {
    if(index.data(modelMPS::OrientationRole).toUInt() == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
}

void StandardNodeWidget::addSubNodeWidget(AbstractSubNodeWidget *subNode) {
    m_mainLayout->addWidget(subNode);
    m_cibleMap.insert({subNode->index().subIndex(),subNode});
}

void StandardNodeWidget::updateData() {
    for (auto iter = m_cibleMap.begin(); iter != m_cibleMap.end(); ++iter) {
        iter->second->deconnexion();
        iter->second->updateData();
        iter->second->connexion();
    }
}

void StandardNodeWidget::updateData(const NodeIndex & index) {
    auto iters = m_cibleMap.equal_range(index.subIndex());
    for (auto iter = iters.first; iter != iters.second; ++iter) {
        iter->second->deconnexion();
        iter->second->updateData();
        iter->second->connexion();
    }
}
