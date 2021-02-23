#include "Standardnode_ptrWidget.h"

using namespace delegateMPS;
using namespace widgetMPS;

////////////////////////////////////////////////// CheckSubnode_ptrWidget //////////////////////////////////////////
CheckSubnode_ptrWidget::CheckSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent)
    : Subnode_ptrWidget(index,parent) {
    m_checkBox = new QCheckBox;
    m_mainLayout->addWidget(m_checkBox);
    connect(m_checkBox,&QCheckBox::stateChanged,[this]() {
        if(connexionEnable())
            m_index.model()->set_data(m_index,m_checkBox->isChecked(),model_base::Check_State_Role);});
}

void CheckSubnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    Subnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::Label_Role))
        m_checkBox->set_text(m_index.data(model_base::Label_Role).to_string());
    if(role.test(model_base::Check_State_Role))
        m_checkBox->setChecked(m_index.data(model_base::Check_State_Role).toBool());
}

////////////////////////////////////////////////// CodeSubnode_ptrWidget //////////////////////////////////////////
CodeSubnode_ptrWidget::CodeSubnode_ptrWidget(const CodeWidget::Cases &cases, const node_ptrIndex &index, QWidget *parent)
    : LabelSubnode_ptrWidget(index,parent) {
    m_codeWidget = new CodeWidget(cases);
    m_mainLayout->addWidget(m_codeWidget);
    connect(m_codeWidget,&CodeWidget::codeChanged,this,[this]() {
        if(connexionEnable())
            m_index.model()->set_data(m_index,m_codeWidget->code().value(),model_base::Num_Role);});
}

void CodeSubnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    LabelSubnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::Num_Role))
        m_codeWidget->set_code(m_index.data(model_base::Num_Role).toUInt());
}

////////////////////////////////////////////////// ComboBoxSubnode_ptrWidget //////////////////////////////////////////
ComboBoxSubnode_ptrWidget::ComboBoxSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent)
    : LabelSubnode_ptrWidget(index,parent) {
    m_comboBox = new QComboBox;
    m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_mainLayout->addWidget(m_comboBox);
    connect(m_comboBox,qOverload<int>(&QComboBox::currentIndexChanged),this,[this]() {
        if(connexionEnable())
            m_index.model()->set_data(m_index,m_comboBox->currentData(),model_base::Int_Role);});
}

void ComboBoxSubnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    LabelSubnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::List_Of_Values)){
        m_comboBox->clear();
        auto map = m_index.data(model_base::List_Of_Values).toMap();
        for (auto iter = map.cbegin(); iter != map.cend(); ++iter)
            m_comboBox->addItem(iter.key(),iter.value());
    }
    if(role.test(model_base::Int_Role))
        m_comboBox->setCurrentIndex(m_comboBox->findData(m_index.data(model_base::Int_Role).toInt()));
}

////////////////////////////////////////////////// LabelSubnode_ptrWidget //////////////////////////////////////////
LabelSubnode_ptrWidget::LabelSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent)
    : Subnode_ptrWidget(index,parent) {
    m_label = new QLabel;
    m_mainLayout->addWidget(m_label);
}

void LabelSubnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    Subnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::Label_Role))
        m_label->set_text(m_index.data(model_base::Label_Role).to_string());
}

////////////////////////////////////////////////// DateSubnode_ptrWidget //////////////////////////////////////////
DateSubnode_ptrWidget::DateSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent)
    : LabelSubnode_ptrWidget(index,parent) {
    m_dateEdit = new QDateEdit;
    m_dateEdit->setCalendarPopup(true);
    m_mainLayout->addWidget(m_dateEdit);
    connect(m_dateEdit,&QDateEdit::dateChanged,this,[this]() {
        if(connexionEnable())
            m_index.model()->set_data(m_index,m_dateEdit->date(),model_base::Date_Role);});
}

void DateSubnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    LabelSubnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::Date_Role))
        m_dateEdit->setDate(m_index.data(model_base::Date_Role).toDate());
}

////////////////////////////////////////////////// LineEditSubnode_ptrWidget //////////////////////////////////////////
LineEditSubnode_ptrWidget::LineEditSubnode_ptrWidget(const node_ptrIndex &index, QWidget *parent)
    : LabelSubnode_ptrWidget(index,parent) {
    m_lineEdit = new QLineEdit;
    m_mainLayout->addWidget(m_lineEdit);
    connect(m_lineEdit,&QLineEdit::textChanged,this,[this]() {
        if(connexionEnable())
            m_index.model()->set_data(m_index,m_lineEdit->text(),model_base::String_Role);});
}

void LineEditSubnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    LabelSubnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::String_Role))
        m_lineEdit->set_text(m_index.data(model_base::String_Role).to_string());
}

////////////////////////////////////////////////// TexteEditSubnode_ptrWidget //////////////////////////////////////////
TexteEditnode_ptrWidget::TexteEditnode_ptrWidget(const node_ptrIndex &index, QWidget *parent)
    : Subnode_ptrWidget(index,parent) {
    m_texteEdit = new QTextEdit;
    m_mainLayout->addWidget(m_texteEdit);
    connect(m_texteEdit,&QTextEdit::textChanged,[this]() {
        if(connexionEnable())
            m_index.model()->set_data(m_index,m_texteEdit->toPlainText(),model_base::String_Role);});
}

void TexteEditnode_ptrWidget::updateDataSubnode_ptr(flag role) {
    Subnode_ptrWidget::updateDataSubnode_ptr(role);
    if(role.test(model_base::String_Role))
        m_texteEdit->setPlainText(m_index.data(model_base::String_Role).to_string());
}

////////////////////////////////////////////////// RoundedArcPainter //////////////////////////////////////////
void RoundedArcPainter::drawArc(Arcnode_ptrViewWidget *arc) const{
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

void RoundedArcPainter::drawToolZone(Arcnode_ptrViewWidget *arc) const {
    QPainter painter(arc);
    QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(WidthLine);
    painter.setPen(pen);
    auto top = arc->nodeWidget()->geometry().bottom() + bottomnode_ptrMargin() + WidthLine / 2;
    auto height =  heightToolZone() - WidthLine;
    auto xo = leftnode_ptrMargin() + WidthLine / 2;
    auto xEx = xo + widthToolZone(node_ptrView::ExpandTool);
    auto xEl = xo + widthToolZone(node_ptrView::ElderTool);
    auto xBr = xo + widthToolZone(node_ptrView::BrotherTool);
    auto xEnd = xo + widthToolZone(node_ptrView::EndOfTool);
    // Rectangle
    painter.drawRoundedRect(xo,top,
                            widthToolZone(node_ptrView::EndOfTool) - WidthLine / 2, height,
                            RayonToolZone, RayonToolZone, Qt::AbsoluteSize);
    //Line
    painter.drawLine(xEx,top,xEx,top + height);
    painter.drawLine(xEl,top,xEl,top + height);
    painter.drawLine(xBr,top,xBr,top + height);
    //Icon
    QPen penDisabled(QGuiApplication::palette().color(QPalette::Disabled,QPalette::WindowText));
    painter.setFont(QFont("FontAwesome"));
    //Icon : Expand
    if(!arc->nodeWidget()->index().flags().test(model_base::ExpendableFLagnode_ptr))
        painter.setPen(penDisabled);
    QString strExpand;
    if(arc->leaf())
        strExpand = "\uf10c";
    else if(arc->expanded())
        strExpand = "\uf151";
    else
        strExpand = "\uf150";
    painter.drawText(xo,top,xEx-xo,height,Qt::AlignCenter,strExpand);
    //Icon : Elder
    if(arc->nodeWidget()->index().flags().test(model_base::ElderEnableFlagnode_ptr))
        painter.setPen(pen);
    else
        painter.setPen(penDisabled);
    painter.drawText(xEx,top,xEl-xEx,height,Qt::AlignCenter,"\uf149");
    //Icon : Brother
    if(arc->nodeWidget()->index().flags().test(model_base::BrotherEnableFlagnode_ptr))
        painter.setPen(pen);
    else
        painter.setPen(penDisabled);
    painter.drawText(xEl,top,xBr-xEl,height,Qt::AlignCenter,"\uf07e");
    //Icon : Del
    if(arc->nodeWidget()->index().flags().test(model_base::DelEnableFlagnode_ptr))
        painter.setPen(pen);
    else
        painter.setPen(penDisabled);
    painter.drawText(xBr,top,xEnd-xBr,height,Qt::AlignCenter,"\uf12d");
}

////////////////////////////////////////////////// Roundednode_ptrPainter //////////////////////////////////////////
void Roundednode_ptrPainter::paint(QWidget *widget) {
    QPen pen(m_colorLine);
    pen.setWidth(m_widthLine);
    QPainter painter(widget);
    painter.setPen(pen);
    painter.drawRoundedRect(m_widthLine / 2, m_widthLine / 2,
                            widget->width() - m_widthLine, widget->height() - m_widthLine,
                            Rayon, Rayon, Qt::AbsoluteSize);
}

void Roundednode_ptrPainter::set_etatSelection(node_ptrWidget::EtatSelection etat) {
    switch (etat) {
    case node_ptrWidget::NoSelected:
        m_widthLine = NoSelectedWidth;
        m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText);
        break;
    case node_ptrWidget::Selected:
        m_widthLine = SelectedWidth;
        m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
        break;
    case node_ptrWidget::Current:
        m_widthLine = CurrentWidth;
        m_colorLine = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
        break;
    case node_ptrWidget::Initial:
        break;
    }
}

//////////////////////////////////////////////// Standardnode_ptrDelegate /////////////////////////////////////
node_ptrWidget *Standardnode_ptrDelegate::createnode_ptr(const node_ptrIndex &index, QWidget *parent) const {
    Q_ASSERT(index.cible() == model_base::Node_Cible);
    auto node = new node_ptrWidget(index,parent);
    node->setPainter(std::make_unique<Roundednode_ptrPainter>());
    auto nbrSubnode_ptr = index.model()->dataCount(index.index(model_base::Sub_Node_Cible));
    for(numt num = 0; num != nbrSubnode_ptr; ++num)
        node->addSubnode_ptrWidget(createSubnode_ptr(index.index(model_base::Sub_Node_Cible,num),node));
    return node;
}

Subnode_ptrWidget *Standardnode_ptrDelegate::createSubnode_ptr(const node_ptrIndex &index, QWidget *parent) const {
    Q_ASSERT(index.cible() == model_base::Sub_Node_Cible);
    auto info = index.data(model_base::Sub_Node_Role).toList();
    auto indexSubnode_ptr = index.index(info.at(model_base::CibleSubnode_ptr).toInt(),info.at(model_base::NumSubnode_ptr).toUInt());
    switch (info.at(model_base::TypeSubnode_ptr).toUInt()) {
    case model_base::CheckSubnode_ptr:
        return new CheckSubnode_ptrWidget(indexSubnode_ptr,parent);
    case model_base::ComboBoxSubnode_ptr:
        return new ComboBoxSubnode_ptrWidget(indexSubnode_ptr,parent);
    case model_base::DateSubnode_ptr:
        return  new DateSubnode_ptrWidget(indexSubnode_ptr,parent);
    case model_base::LineEditSubnode_ptr:
        return new LineEditSubnode_ptrWidget(indexSubnode_ptr,parent);
    case model_base::TexteEditSubnode_ptr:
        return  new TexteEditnode_ptrWidget(indexSubnode_ptr,parent);
    }
    return new Subnode_ptrWidget(indexSubnode_ptr,parent);
}

Subnode_ptrWidget *CodeStandardnode_ptrDelegate::createSubnode_ptr(const node_ptrIndex &index, QWidget *parent) const {
    Q_ASSERT(index.cible() == model_base::Sub_Node_Cible);
    if(index.data(model_base::Sub_Node_Role).toList().at(model_base::TypeSubnode_ptr).toUInt() == model_base::CodeSubnode_ptr) {
        auto info = index.data(model_base::Sub_Node_Role).toList();
        auto indexSubnode_ptr = index.index(info.at(model_base::CibleSubnode_ptr).toInt(),info.at(model_base::NumSubnode_ptr).toUInt());
        return new CodeSubnode_ptrWidget(m_cases,indexSubnode_ptr,parent);
    }
    return Standardnode_ptrDelegate::createSubnode_ptr(index,parent);
}
