#include "StandardNodeWidget.h"

using namespace delegateMPS;
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
////////////////////////////////////////////////// CheckSubNodeWidget //////////////////////////////////////////
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

void CheckSubNodeWidget::updateData(flag role) {
    AbstractSubNodeWidget::updateData(role);
    if(role.test(modelMPS::LabelRole))
        m_checkBox->setText(m_index.data(modelMPS::LabelRole).toString());
    if(role.test(modelMPS::DataRole))
        m_checkBox->setChecked(m_index.data().toBool());
}

////////////////////////////////////////////////// LabelSubNodeWidget //////////////////////////////////////////
LabelSubNodeWidget::LabelSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : AbstractSubNodeWidget(index,parent) {
    m_label = new QLabel;
    m_mainLayout->addWidget(m_label);
}

void LabelSubNodeWidget::updateData(flag role) {
    AbstractSubNodeWidget::updateData(role);
    if(role.test(modelMPS::LabelRole))
        m_label->setText(m_index.data(modelMPS::LabelRole).toString());
}

////////////////////////////////////////////////// DateSubNodeWidget //////////////////////////////////////////
DateSubNodeWidget::DateSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent)
    : LabelSubNodeWidget(index,parent) {
    m_dateEdit = new QDateEdit;
    m_mainLayout->addWidget(m_dateEdit);
}

void DateSubNodeWidget::connexion() const {
    LabelSubNodeWidget::connexion();
    connect(m_dateEdit,&QDateEdit::dateChanged,this,[this]()
        {m_index.model()->setData(m_index,m_dateEdit->date());});
}


void DateSubNodeWidget::deconnexion() const {
    LabelSubNodeWidget::deconnexion();
    disconnect(m_dateEdit,&QDateEdit::dateChanged,this,nullptr);
}

void DateSubNodeWidget::updateData(flag role) {
    LabelSubNodeWidget::updateData(role);
    if(role.test(modelMPS::DataRole))
        m_dateEdit->setDate(m_index.data().toDate());
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

void LineEditSubNodeWidget::updateData(flag role) {
    LabelSubNodeWidget::updateData(role);
    if(role.test(modelMPS::DataRole))
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
    subNode->updateData(modelMPS::AllRole);
    subNode->connexion();
}

void StandardNodeWidget::updateData() {
    for (auto iter = m_cibleMap.begin(); iter != m_cibleMap.end(); ++iter) {
        iter->second->deconnexion();
        iter->second->updateData(modelMPS::AllRole);
        iter->second->connexion();
    }
}

void StandardNodeWidget::updateData(const NodeIndex & index, flag role) {
    auto iters = m_cibleMap.equal_range(index.subIndex());
    for (auto iter = iters.first; iter != iters.second; ++iter) {
        iter->second->deconnexion();
        iter->second->updateData(role);
        iter->second->connexion();
    }
}

//////////////////////////////////////////////// StandardNodeDelegate /////////////////////////////////////
AbstractNodeWidget * StandardNodeDelegate::createNode(const NodeIndex &index, widgetMPS::ArcNodeViewWidget * parent) const {
    Q_ASSERT(index.cible() == modelMPS::NodeCible);
    auto node = new StandardNodeWidget(index,parent);
    auto nbrSubNode = index.model()->dataCount(index.index(modelMPS::SubNodeCible));
    for(szt num = 0; num != nbrSubNode; ++num)
        node->addSubNodeWidget(createSubNode(index.index(modelMPS::SubNodeCible,num),node));
    return node;
}

AbstractSubNodeWidget * StandardNodeDelegate::createSubNode(const NodeIndex &index, widgetMPS::StandardNodeWidget * parent) const {
    Q_ASSERT(index.cible() == modelMPS::SubNodeCible);
    auto info = index.data(modelMPS::SubNodeRole).toList();
    auto indexSubNode = index.index(info.at(modelMPS::CibleSubNode).toInt(),info.at(modelMPS::NumSubNode).toUInt());
    switch (info.at(modelMPS::TypeSubNode).toInt()) {
    case modelMPS::CheckSubNode:
        return new CheckSubNodeWidget(indexSubNode,parent);
    case modelMPS::DateSubNode:
        return  new DateSubNodeWidget(indexSubNode,parent);
    case modelMPS::LineEditSubNode:
        return new LineEditSubNodeWidget(indexSubNode,parent);
    }
    return new AbstractSubNodeWidget(indexSubNode,parent);
}
