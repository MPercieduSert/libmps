#include "StandardNodeWidget.h"

using namespace delegateMPS;
using namespace widgetMPS;

////////////////////////////////////////////////// CheckSubNodeWidget //////////////////////////////////////////
CheckSubNodeWidget::CheckSubNodeWidget(const NodeIndex & index, QWidget * parent)
    : SubNodeWidget(index,parent) {
    m_checkBox = new QCheckBox;
    m_mainLayout->addWidget(m_checkBox);
    connect(m_checkBox,&QCheckBox::stateChanged,[this]() {
        if(connexionEnable())
            m_index.model()->setData(m_index,m_checkBox->checkState(),modelMPS::CheckStateRole);});
}

void CheckSubNodeWidget::updateDataSubNode(flag role) {
    SubNodeWidget::updateDataSubNode(role);
    if(role.test(modelMPS::LabelRole))
        m_checkBox->setText(m_index.data(modelMPS::LabelRole).toString());
    if(role.test(modelMPS::CheckStateRole))
        m_checkBox->setChecked(m_index.data(modelMPS::CheckStateRole).toBool());
}

////////////////////////////////////////////////// CodeSubNodeWidget //////////////////////////////////////////
CodeSubNodeWidget::CodeSubNodeWidget(const CodeWidget::Cases & cases, const NodeIndex & index, QWidget * parent)
    : LabelSubNodeWidget(index,parent) {
    m_codeWidget = new CodeWidget(cases);
    m_mainLayout->addWidget(m_codeWidget);
    connect(m_codeWidget,&CodeWidget::codeChanged,this,[this]() {
        if(connexionEnable())
            m_index.model()->setData(m_index,m_codeWidget->code().value(),modelMPS::NumRole);});
}

void CodeSubNodeWidget::updateDataSubNode(flag role) {
    LabelSubNodeWidget::updateDataSubNode(role);
    if(role.test(modelMPS::NumRole))
        m_codeWidget->setCode(m_index.data(modelMPS::NumRole).toUInt());
}

////////////////////////////////////////////////// ComboBoxSubNodeWidget //////////////////////////////////////////
ComboBoxSubNodeWidget::ComboBoxSubNodeWidget(const NodeIndex & index, QWidget * parent)
    : LabelSubNodeWidget(index,parent) {
    m_comboBox = new QComboBox;
    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_mainLayout->addWidget(m_comboBox);
    connect(m_comboBox,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]() {
        if(connexionEnable())
            m_index.model()->setData(m_index,m_comboBox->currentData(),modelMPS::IntRole);});
}

void ComboBoxSubNodeWidget::updateDataSubNode(flag role) {
    LabelSubNodeWidget::updateDataSubNode(role);
    if(role.test(modelMPS::ListOfValues)){
        m_comboBox->clear();
        auto map = m_index.data(modelMPS::ListOfValues).toMap();
        for (auto iter = map.cbegin(); iter != map.cend(); ++iter)
            m_comboBox->addItem(iter.key(),iter.value());
    }
    if(role.test(modelMPS::IntRole))
        m_comboBox->setCurrentIndex(m_comboBox->findData(m_index.data(modelMPS::IntRole).toInt()));
}

////////////////////////////////////////////////// LabelSubNodeWidget //////////////////////////////////////////
LabelSubNodeWidget::LabelSubNodeWidget(const NodeIndex & index, QWidget * parent)
    : SubNodeWidget(index,parent) {
    m_label = new QLabel;
    m_mainLayout->addWidget(m_label);
}

void LabelSubNodeWidget::updateDataSubNode(flag role) {
    SubNodeWidget::updateDataSubNode(role);
    if(role.test(modelMPS::LabelRole))
        m_label->setText(m_index.data(modelMPS::LabelRole).toString());
}

////////////////////////////////////////////////// DateSubNodeWidget //////////////////////////////////////////
DateSubNodeWidget::DateSubNodeWidget(const NodeIndex & index, QWidget * parent)
    : LabelSubNodeWidget(index,parent) {
    m_dateEdit = new QDateEdit;
    m_mainLayout->addWidget(m_dateEdit);
    connect(m_dateEdit,&QDateEdit::dateChanged,this,[this]() {
        if(connexionEnable())
            m_index.model()->setData(m_index,m_dateEdit->date(),modelMPS::DateRole);});
}

void DateSubNodeWidget::updateDataSubNode(flag role) {
    LabelSubNodeWidget::updateDataSubNode(role);
    if(role.test(modelMPS::DateRole))
        m_dateEdit->setDate(m_index.data(modelMPS::DateRole).toDate());
}

////////////////////////////////////////////////// LineEditSubNodeWidget //////////////////////////////////////////
LineEditSubNodeWidget::LineEditSubNodeWidget(const NodeIndex & index, QWidget * parent)
    : LabelSubNodeWidget(index,parent) {
    m_lineEdit = new QLineEdit;
    m_mainLayout->addWidget(m_lineEdit);
    connect(m_lineEdit,&QLineEdit::textChanged,this,[this]() {
        if(connexionEnable())
            m_index.model()->setData(m_index,m_lineEdit->text(),modelMPS::StringRole);});
}

void LineEditSubNodeWidget::updateDataSubNode(flag role) {
    LabelSubNodeWidget::updateDataSubNode(role);
    if(role.test(modelMPS::StringRole))
        m_lineEdit->setText(m_index.data(modelMPS::StringRole).toString());
}

////////////////////////////////////////////////// RoundedArcPainter //////////////////////////////////////////
void RoundedArcPainter::drawArc(ArcNodeViewWidget * arc) const{
    QPainter painter(arc);
    QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(WidthLine);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    auto x = arc->nodeWidget()->geometry().left() + WidthLine / 2;
    auto y = 0;
    for (auto iter = arc->arcChild().cbegin(); iter != arc->arcChild().cend(); ++iter) {
        auto nodeGeo = (*iter)->nodeWidget()->geometry();
        y = std::min<int>(((*iter)->mapToParent(nodeGeo.topLeft()).y() + (*iter)->mapToParent(nodeGeo.bottomLeft()).y() )/ 2,
                          (*iter)->mapToParent(nodeGeo.topLeft()).y() + HMaxLine);
        painter.drawLine(x,y, x + LeftExpandMargin - HSpacing - WidthLine, y);
    }
    painter.drawLine(x,
                     arc->arcChild().front()->mapToParent(arc->arcChild().front()->nodeWidget()->geometry().topLeft()).y() + WidthLine / 2,
                     x ,y);
}

void RoundedArcPainter::drawExpandZone(ArcNodeViewWidget * arc) const {
    QPainter painter(arc);
    QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(WidthCircle);
    painter.setPen(pen);
    painter.setBrush(QBrush(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText)));
    for (auto i = 0; i < NbrCircle; ++i) {
        QPoint pt(arc->nodeWidget()->geometry().left() + WidthCircle + Rayon + Ecart * i,
                  arc->nodeWidget()->geometry().bottom() + VSpacing + WidthCircle + Rayon);
        painter.drawEllipse(pt,Rayon,Rayon);
    }
}

////////////////////////////////////////////////// RoundedNodePainter //////////////////////////////////////////
void RoundedNodePainter::paint(QWidget * widget) {
    QPen pen(m_colorLine);
    pen.setWidth(m_widthLine);
    QPainter painter(widget);
    painter.setPen(pen);
    painter.drawRoundedRect(m_widthLine / 2, m_widthLine / 2,
                            widget->width() - m_widthLine, widget->height() - m_widthLine,
                            Rayon, Rayon, Qt::AbsoluteSize);
}

void RoundedNodePainter::setEtatSelection(NodeWidget::EtatSelection etat) {
    switch (etat) {
    case NodeWidget::NoSelected:
        m_widthLine = NoSelectedWidth;
        m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText);
        break;
    case NodeWidget::Selected:
        m_widthLine = SelectedWidth;
        m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
        break;
    case NodeWidget::Current:
        m_widthLine = CurrentWidth;
        m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
        break;
    case NodeWidget::Initial:
        break;
    }
}

//////////////////////////////////////////////// StandardNodeDelegate /////////////////////////////////////
NodeWidget * StandardNodeDelegate::createNode(const NodeIndex &index, QWidget * parent) const {
    Q_ASSERT(index.cible() == modelMPS::NodeCible);
    auto node = new NodeWidget(index,parent);
    node->setPainter(std::make_unique<RoundedNodePainter>());
    auto nbrSubNode = index.model()->dataCount(index.index(modelMPS::SubNodeCible));
    for(szt num = 0; num != nbrSubNode; ++num)
        node->addSubNodeWidget(createSubNode(index.index(modelMPS::SubNodeCible,num),node));
    return node;
}

SubNodeWidget * StandardNodeDelegate::createSubNode(const NodeIndex &index, QWidget *parent) const {
    Q_ASSERT(index.cible() == modelMPS::SubNodeCible);
    auto info = index.data(modelMPS::SubNodeRole).toList();
    auto indexSubNode = index.index(info.at(modelMPS::CibleSubNode).toInt(),info.at(modelMPS::NumSubNode).toUInt());
    switch (info.at(modelMPS::TypeSubNode).toUInt()) {
    case modelMPS::CheckSubNode:
        return new CheckSubNodeWidget(indexSubNode,parent);
    case modelMPS::DateSubNode:
        return  new DateSubNodeWidget(indexSubNode,parent);
    case modelMPS::LineEditSubNode:
        return new LineEditSubNodeWidget(indexSubNode,parent);
    case modelMPS::ComboBoxSubNode:
        return new ComboBoxSubNodeWidget(indexSubNode,parent);
    }
    return new SubNodeWidget(indexSubNode,parent);
}

SubNodeWidget * CodeStandardNodeDelegate::createSubNode(const NodeIndex &index, QWidget *parent) const {
    Q_ASSERT(index.cible() == modelMPS::SubNodeCible);
    if(index.data(modelMPS::SubNodeRole).toList().at(modelMPS::TypeSubNode).toUInt() == modelMPS::CodeSubNode) {
        auto info = index.data(modelMPS::SubNodeRole).toList();
        auto indexSubNode = index.index(info.at(modelMPS::CibleSubNode).toInt(),info.at(modelMPS::NumSubNode).toUInt());
        return new CodeSubNodeWidget(m_cases,indexSubNode,parent);
    }
    return StandardNodeDelegate::createSubNode(index,parent);
}
