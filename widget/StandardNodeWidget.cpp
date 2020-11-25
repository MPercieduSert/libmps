#include "StandardNodeWidget.h"

using namespace widgetMPS;

AbstractSubNodeWidget::AbstractSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : QWidget (parent), m_index(index)
{}

AbstractSubNodeWidget::~AbstractSubNodeWidget() {
    auto node = static_cast<StandardNodeWidget*>(parent());
    auto iter = node->m_cibleMap.cbegin();
    while (iter != node->m_cibleMap.cend() && iter->second != this)
        ++iter;
    if(iter != node->m_cibleMap.cend())
        node->m_cibleMap.erase(iter);
}

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

void StandardNodeWidget::updateData(const NodeIndex & index) {
    auto iters = m_cibleMap.equal_range(index.subIndex());
    for (auto iter = iters.first; iter != iters.second; ++iter)
        iter->second->updateData();
}
