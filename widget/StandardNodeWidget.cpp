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
CheckSubNodeWidget::CheckSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : AbstractSubNodeWidget(index,parent) {
    m_checkBox = new QCheckBox;
    m_mainLayout->addWidget(m_checkBox);
}

void CheckSubNodeWidget::connexion() const {
    AbstractSubNodeWidget::connexion();
    connect(m_checkBox,&QCheckBox::stateChanged,[this]()
        {m_index.model()->setData(m_index,m_checkBox->isChecked());});
}


void CheckSubNodeWidget::deconnexion() const {
    AbstractSubNodeWidget::deconnexion();
    disconnect(m_checkBox,&QCheckBox::stateChanged,this,nullptr);
}

void CheckSubNodeWidget::updateData() {
    AbstractSubNodeWidget::updateData();
    m_checkBox->setText(m_index.data(modelMPS::LabelRole).toString());
    m_checkBox->setChecked(m_index.data().toBool());
}

////////////////////////////////////////////////// LabelSubNodeWidget //////////////////////////////////////////
LabelSubNodeWidget::LabelSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : AbstractSubNodeWidget(index,parent) {
    m_label = new QLabel;
    m_mainLayout->addWidget(m_label);
}

void LabelSubNodeWidget::updateData() {
    AbstractSubNodeWidget::updateData();
    m_label->setText(m_index.data(modelMPS::LabelRole).toString());
}

////////////////////////////////////////////////// LineEditSubNodeWidget //////////////////////////////////////////
LineEditSubNodeWidget::LineEditSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : LabelSubNodeWidget(index,parent) {
    m_lineEdit = new QLineEdit;
    m_mainLayout->addWidget(m_lineEdit);
}

void LineEditSubNodeWidget::connexion() const {
    LabelSubNodeWidget::connexion();
    connect(m_lineEdit,&QLineEdit::textChanged,this,[this]()
        {m_index.model()->setData(m_index,m_lineEdit->text());});
}


void LineEditSubNodeWidget::deconnexion() const {
    LabelSubNodeWidget::deconnexion();
    disconnect(m_lineEdit,&QLineEdit::textChanged,this,nullptr);
}

void LineEditSubNodeWidget::updateData() {
    LabelSubNodeWidget::updateData();
    m_lineEdit->setText(m_index.data().toString());
}

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
    subNode->updateData();
    subNode->connexion();
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
