#include "standard_node_widget.h"

using namespace mps;
using namespace delegate;
using namespace widget;

////////////////////////////////////////////////// check_sub_node_widget //////////////////////////////////////////
check_sub_node_widget::check_sub_node_widget(const node_index &index, QWidget *parent)
    : sub_node_widget(index,parent) {
    m_check_box = new QCheckBox;
    m_main_layout->addWidget(m_check_box);
    connect(m_check_box,&QCheckBox::stateChanged,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_check_box->isChecked(),model_base::Check_State_Role);});
}

void check_sub_node_widget::update_data_sub_node(flag role) {
    sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Label_Change_Flag))
        m_check_box->setText(m_index.data(model_base::Label_Role).toString());
    if(role.test(model_base::Main_Data_Change_Flag))
        m_check_box->setChecked(m_index.data(model_base::Check_State_Role).toBool());
}

////////////////////////////////////////////////// code_sub_node_widget //////////////////////////////////////////
code_sub_node_widget::code_sub_node_widget(const code_widget::option_code &cases, const node_index &index, QWidget *parent)
    : label_sub_node_widget(index,parent) {
    m_code_widget = new code_widget(cases);
    m_main_layout->addWidget(m_code_widget);
    connect(m_code_widget,&code_widget::code_changed,this,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_code_widget->code().value(),model_base::Num_Role);});
}

void code_sub_node_widget::update_data_sub_node(flag role) {
    label_sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Main_Data_Change_Flag))
        m_code_widget->set_code(m_index.data(model_base::Num_Role).toUInt());
}

////////////////////////////////////////////////// combo_box_sub_node_widget //////////////////////////////////////////
combo_box_sub_node_widget::combo_box_sub_node_widget(const node_index &index, QWidget *parent)
    : label_sub_node_widget(index,parent) {
    m_combo_box = new QComboBox;
    m_combo_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_main_layout->addWidget(m_combo_box);
    connect(m_combo_box,qOverload<int>(&QComboBox::currentIndexChanged),this,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_combo_box->currentData(),model_base::Variant_Role);});
}

void combo_box_sub_node_widget::update_data_sub_node(flag role) {
    label_sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Setting_Change_Flag)){
        m_combo_box->clear();
        auto map = m_index.data(model_base::Map_Role).toMap();
        for (auto it = map.cbegin(); it != map.cend(); ++it)
            m_combo_box->addItem(it.key(),it.value());
    }
    if(role.test(model_base::Main_Data_Change_Flag | model_base::Setting_Change_Flag))
        m_combo_box->setCurrentIndex(m_combo_box->findData(m_index.data(model_base::Variant_Role)));
}

////////////////////////////////////////////////// label_sub_node_widget //////////////////////////////////////////
label_sub_node_widget::label_sub_node_widget(const node_index &index, QWidget *parent)
    : sub_node_widget(index,parent) {
    m_label = new QLabel;
    m_main_layout->addWidget(m_label);
}

void label_sub_node_widget::update_data_sub_node(flag role) {
    sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Label_Change_Flag))
        m_label->setText(m_index.data(model_base::Label_Role).toString());
}

////////////////////////////////////////////////// date_sub_node_widget //////////////////////////////////////////
date_sub_node_widget::date_sub_node_widget(const node_index &index, QWidget *parent)
    : label_sub_node_widget(index,parent) {
    m_dateEdit = new QDateEdit;
    m_dateEdit->setCalendarPopup(true);
    m_main_layout->addWidget(m_dateEdit);
    connect(m_dateEdit,&QDateEdit::dateChanged,this,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_dateEdit->date(),model_base::Date_Role);});
}

void date_sub_node_widget::update_data_sub_node(flag role) {
    label_sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Main_Data_Change_Flag))
        m_dateEdit->setDate(m_index.data(model_base::Date_Role).toDate());
}

////////////////////////////////////////////////// line_edit_sub_node_widget //////////////////////////////////////////
line_edit_sub_node_widget::line_edit_sub_node_widget(const node_index &index, QWidget *parent)
    : label_sub_node_widget(index,parent) {
    m_lineEdit = new QLineEdit;
    m_main_layout->addWidget(m_lineEdit);
    connect(m_lineEdit,&QLineEdit::textChanged,this,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_lineEdit->text(),model_base::String_Role);});
}

void line_edit_sub_node_widget::update_data_sub_node(flag role) {
    label_sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Main_Data_Change_Flag))
        m_lineEdit->setText(m_index.data(model_base::String_Role).toString());
}

////////////////////////////////////////////////// spin_box_sub_node_widget //////////////////////////////////////////
spin_box_sub_node_widget::spin_box_sub_node_widget(const node_index &index, QWidget *parent)
    : label_sub_node_widget(index,parent) {
    m_spin_box = new QSpinBox;
    m_main_layout->addWidget(m_spin_box);
    connect(m_spin_box,qOverload<int>(&QSpinBox::valueChanged),this,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_spin_box->value(),model_base::Int_Role);});
}

void spin_box_sub_node_widget::update_data_sub_node(flag role) {
    label_sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Main_Data_Change_Flag))
        m_spin_box->setValue(m_index.data(model_base::Int_Role).toInt());
    if(role.test(model_base::Setting_Change_Flag)) {
        m_spin_box->setMinimum(m_index.data(model_base::Min_role).toInt());
        m_spin_box->setMaximum(m_index.data(model_base::Max_role).toInt());
    }
}

////////////////////////////////////////////////// TexteEditsub_node_widget //////////////////////////////////////////
texte_edit_node_widget::texte_edit_node_widget(const node_index &index, QWidget *parent)
    : label_sub_node_widget(index,parent) {
    m_texteEdit = new QTextEdit;
    m_main_layout->addWidget(m_texteEdit);
    connect(m_texteEdit,&QTextEdit::textChanged,[this](){
        if(connexion_enable())
            m_index.model()->set_data(m_index,m_texteEdit->toPlainText(),model_base::String_Role);});
}

void texte_edit_node_widget::update_data_sub_node(flag role) {
    label_sub_node_widget::update_data_sub_node(role);
    if(role.test(model_base::Main_Data_Change_Flag))
        m_texteEdit->setPlainText(m_index.data(model_base::String_Role).toString());
}

////////////////////////////////////////////////// rounded_arc_painter //////////////////////////////////////////
void rounded_arc_painter::draw_arc(arc_node_view_widget *arc) const{
    QPainter painter(arc);
    QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(Width_Line);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    auto x = arc->node()->geometry().left() + Width_Line / 2;
    auto y = 0;
    for (auto it = arc->arc_child().cbegin(); it != arc->arc_child().cend(); ++it) {
        auto node_geo = (*it)->node()->geometry();
        y = std::min<int>(((*it)->mapToParent(node_geo.topLeft()).y() + (*it)->mapToParent(node_geo.bottomLeft()).y() )/ 2,
                          (*it)->mapToParent(node_geo.topLeft()).y() + HMax_Line);
        painter.drawLine(x,y, x + Left_Expand_Margin - HSpacing - Width_Line, y);
    }
    painter.drawLine(x,
                     arc->arc_child().front()->mapToParent(arc->arc_child().front()->node()->geometry().topLeft()).y()
                                                           + Width_Line / 2,
                     x ,y);
}

void rounded_arc_painter::draw_tool_zone(arc_node_view_widget *arc) const {
    QPainter painter(arc);
    QPen pen(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText));
    pen.setWidth(Width_Line);
    painter.setPen(pen);
    auto top = arc->node()->geometry().bottom() + bottom_node_margin() + Width_Line / 2;
    auto height =  height_tool_zone() - Width_Line;
    auto xo = left_node_margin() + Width_Line / 2;
    auto xEx = xo + width_tool_zone(node_view::Expand_Tool);
    auto xEl = xo + width_tool_zone(node_view::Elder_Tool);
    auto xBr = xo + width_tool_zone(node_view::Brother_Tool);
    auto xEnd = xo + width_tool_zone(node_view::End_Of_Tool);
    // Rectangle
    painter.drawRoundedRect(xo,top,
                            width_tool_zone(node_view::End_Of_Tool) - Width_Line / 2, height,
                            Rayon_Tool_Zone, Rayon_Tool_Zone, Qt::AbsoluteSize);
    //Line
    painter.drawLine(xEx,top,xEx,top + height);
    painter.drawLine(xEl,top,xEl,top + height);
    painter.drawLine(xBr,top,xBr,top + height);
    //Icon
    QPen pen_disabled(QGuiApplication::palette().color(QPalette::Disabled,QPalette::WindowText));
    painter.setFont(QFont("FontAwesome"));
    //Icon : Expand
    if(!arc->node()->flags().test(model_base::Expendable_FLag_Node))
        painter.setPen(pen_disabled);
    QString str_expand;
    if(arc->leaf())
        str_expand = "\uf10c";
    else if(arc->expanded())
        str_expand = "\uf151";
    else
        str_expand = "\uf150";
    painter.drawText(xo,top,xEx-xo,height,Qt::AlignCenter,str_expand);
    //Icon : Elder
    if(arc->node()->flags().test(model_base::Elder_Enable_Flag_Node))
        painter.setPen(pen);
    else
        painter.setPen(pen_disabled);
    painter.drawText(xEx,top,xEl-xEx,height,Qt::AlignCenter,"\uf149");
    //Icon : Brother
    if(arc->node()->flags().test(model_base::Brother_Enable_Flag_Node))
        painter.setPen(pen);
    else
        painter.setPen(pen_disabled);
    painter.drawText(xEl,top,xBr-xEl,height,Qt::AlignCenter,"\uf07e");
    //Icon : Del
    if(arc->node()->flags().test(model_base::Del_Enable_Flag_Node))
        painter.setPen(pen);
    else
        painter.setPen(pen_disabled);
    painter.drawText(xBr,top,xEnd-xBr,height,Qt::AlignCenter,"\uf12d");
}

////////////////////////////////////////////////// rounded_node_painter //////////////////////////////////////////
void rounded_node_painter::paint(QWidget *widget) {
    QPen pen(m_color_line);
    pen.setWidth(m_width_line);
    QPainter painter(widget);
    painter.setPen(pen);
    painter.drawRoundedRect(m_width_line / 2, m_width_line / 2,
                            widget->width() - m_width_line, widget->height() - m_width_line,
                            Rayon, Rayon, Qt::AbsoluteSize);
}

void rounded_node_painter::set_etat_selection(node_widget::etat_selection etat) {
    switch (etat) {
    case node_widget::No_Selected:
        m_width_line = No_Selected_Width;
        m_color_line = QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText);
        break;
    case node_widget::Selected:
        m_width_line = Selected_Width;
        m_color_line = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
        break;
    case node_widget::Current:
        m_width_line = Current_Width;
        m_color_line = QGuiApplication::palette().color(QPalette::Active,QPalette::Highlight);
        break;
    case node_widget::Initial:
        break;
    }
}

//////////////////////////////////////////////// standard_node_delegate /////////////////////////////////////
node_widget *standard_node_delegate::create_node(const node_index &index, QWidget *parent) const {
    Q_ASSERT(index.cible() == model_base::Node_Cible);
    auto node = new node_widget(index,parent);
    node->set_painter(std::make_unique<rounded_node_painter>());
    auto cibles = index.data(model_base::Cibles_Role).toList();
    for(auto it = cibles.cbegin(); it != cibles.cend(); ++it) {
        if(it->canConvert<QVariantList>()) {
            auto l = it->toList();
            node->add_sub_node_widget(create_sub_node(index.index(l.front().toInt(),l.back().toUInt()),node));
        }
        else
            node->add_sub_node_widget(create_sub_node(index.index(it->toInt()),node));
    }
    return node;
}

sub_node_widget *standard_node_delegate::create_sub_node(const node_index &index, QWidget *parent) const {
    switch (index.data(model_base::Type_Role).toInt()) {
    case model_base::Check_Sub_Node:
        return new check_sub_node_widget(index,parent);
    case model_base::Combo_Box_Sub_Node:
        return new combo_box_sub_node_widget(index,parent);
    case model_base::Date_Sub_Node:
        return  new date_sub_node_widget(index,parent);
    case model_base::Label_Sub_Node:
        return  new label_sub_node_widget(index,parent);
    case model_base::Line_Edit_Sub_Node:
        return new line_edit_sub_node_widget(index,parent);
    case model_base::Spin_Box_Sub_Node:
        return new spin_box_sub_node_widget(index,parent);
    case model_base::Texte_Edit_Sub_Node:
        return  new texte_edit_node_widget(index,parent);
    }
    return new sub_node_widget(index,parent);
}

sub_node_widget *code_standard_node_delegate::create_sub_node(const node_index &index, QWidget *parent) const {
    if(index.data(model_base::Type_Role).toInt() == model_base::Code_Sub_Node)
        return new code_sub_node_widget(m_option,index,parent);
    return standard_node_delegate::create_sub_node(index,parent);
}
