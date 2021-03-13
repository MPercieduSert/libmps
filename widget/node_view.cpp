#include "node_view.h"

using namespace mps;
using namespace delegate;
using namespace model_base;
using namespace widget;

////////////////////////////////////// abstract_node_delegate /////////////////////////////////
abstract_node_delegate::abstract_node_delegate(QObject *parent) : QObject(parent) {}

////////////////////////////////////// arc_node_view_widget //////////////////////////////////////////
arc_node_view_widget::arc_node_view_widget(node_widget *node, node_view *view, QWidget *parent, bool root, bool node_arc_visible)
    : QWidget (parent), m_leaf(node->index().leaf()), m_root(root), m_node_arc_visible(node_arc_visible), m_view(view)
    {set_node_widget(node);}

void arc_node_view_widget::draw_node(bool next) {
    adjustSize();
    if(!m_root) {
        auto parent_arc = static_cast<arc_node_view_widget *>(parentWidget());
        auto draw = false;
        auto y = 0;
        auto &arc_painter = *m_view->m_arc_painter;
        for (auto it = parent_arc->m_arc_child.begin(); it != parent_arc->m_arc_child.end(); ++it) {
            if (!next &&!draw &&*it == this) {
                draw = true;
                if(it ==  parent_arc->m_arc_child.begin()) {
                    if(parent_arc->m_node_arc_visible)
                        y = parent_arc->node()->geometry().bottom()
                                + arc_painter.height_tool_zone() + arc_painter.bottom_tools_zone_margin();
                }
                else
                    y = (*std::prev(it))->geometry().bottom();
            }
            if(draw) {
                if(parent_arc->m_node_arc_visible)
                    (*it)->move(arc_painter.left_expanded_margin(),y);
                else
                    (*it)->move(node_view::arc_painter::No_Margin,y);
                (*it)->setVisible(true);
                y = (*it)->geometry().bottom();
            }
            else if(next && *it == this) {
                draw = true;
                y = (*it)->geometry().bottom();
            }
        }
        parent_arc->draw_node(true);
    }
}

void arc_node_view_widget::insert_nodes(numt pos, numt count){
    if(count > 0 &&pos <= m_arc_child.size()) {
        set_leaf(false);
        if(m_expanded) {
            auto first_it = m_arc_child.insert(std::next(m_arc_child.cbegin(),pos), count, nullptr);
            auto it = first_it;
            auto child = m_node->index().model()->index(m_node->index(),pos);
            while (count &&child.is_valid()) {
                *it = new arc_node_view_widget(child,m_view,this);
                --count;
                ++it;
                child.to_next();
            }
            (*first_it)->draw_node();
        }
    }
}

void arc_node_view_widget::mousePressEvent(QMouseEvent *event) {
    if(m_node_arc_visible && event->button() == Qt::LeftButton) {
        if(event->y() > m_node->geometry().bottom() + m_view->m_arc_painter->bottom_node_margin()
                && event->y() <= m_node->geometry().bottom() + m_view->m_arc_painter->bottom_node_margin()
                                                             + m_view->m_arc_painter->height_tool_zone()
                && event->x() < m_view->m_arc_painter->width_tool_zone(node_view::End_Of_Tool)){
            if(event->x() < m_view->m_arc_painter->width_tool_zone(node_view::Expand_Tool)) {
                if(!m_leaf && m_node->flags().test(Expendable_FLag_Node))
                    set_expanded(!m_expanded);
            }
            else if (event->x() < m_view->m_arc_painter->width_tool_zone(node_view::Elder_Tool)) {
                if(m_node->flags().test(Elder_Enable_Flag_Node))
                    m_node->index().model()->insert_nodes(m_node->index(),0,1);
            }
            else if (event->x() < m_view->m_arc_painter->width_tool_zone(node_view::Brother_Tool)) {
                if(m_node->flags().test(Brother_Enable_Flag_Node))
                    m_node->index().model()->insert_nodes(m_node->index().parent(),m_node->index().position() + 1,1);
            }
            else if (event->x() < m_view->m_arc_painter->width_tool_zone(node_view::Del_Tool)) {
                if(m_node->flags().test(Del_Enable_Flag_Node))
                    m_node->index().model()->remove_nodes(m_node->index());
            }
        }
        else if((m_expanded && event->x() < m_view->m_arc_painter->left_expanded_margin()
                            && event->y() > m_node->geometry().bottom()))
            set_expanded(false);
    }
}

void arc_node_view_widget::paintEvent(QPaintEvent */*event*/) {
    if(m_node_arc_visible)
        m_view->m_arc_painter->draw_tool_zone(this);
    if(!m_leaf && m_node_arc_visible && m_expanded)
        m_view->m_arc_painter->draw_arc(this);
}

void arc_node_view_widget::remove_nodes(numt pos, numt count) {
    if(count && pos < m_arc_child.size()) {
        auto it = std::next(m_arc_child.begin(), pos);
        if(count == 1) {
            (*it)->deleteLater();
            m_arc_child.erase(it);
        }
        else {
            auto firstIter = it;
            while(count && it != m_arc_child.end()) {
                (*it)->deleteLater();
                ++it;
                --count;
            }
            m_arc_child.erase(firstIter,it);
        }
        if(m_arc_child.size()) {
            if(m_arc_child.size() != pos)
                m_arc_child[pos]->draw_node();
            else {
                adjustSize();
                draw_node(true);
            }
        }
        else
            set_leaf(true);
    }
}

void arc_node_view_widget::set_expanded(bool bb){
    if(m_expanded != bb && !m_leaf) {
        m_expanded = bb && !m_leaf;
        if(m_expanded) {
            auto index = m_node->index();
            m_arc_child.resize(index.child_count());
            auto child = index.model()->index(index,0);
            for (auto vec_it = m_arc_child.begin(); vec_it != m_arc_child.end(); ++vec_it) {
                *vec_it = new arc_node_view_widget(child,m_view,this);
                child.to_next();
            }
            m_arc_child.front()->draw_node();
        }
        else {
            for (auto it = m_arc_child.begin(); it != m_arc_child.end(); ++it)
                (*it)->deleteLater();
            m_arc_child.clear();
            draw_node();
        }
    }
}

void arc_node_view_widget::set_leaf(bool bb) {
    if((bb && !m_leaf) || (m_leaf && !bb)){
        m_leaf = bb;
        m_expanded = false;
        repaint();
    }
}

void arc_node_view_widget::set_node_widget(node_widget *widget) {
    auto draw = m_node && m_node->isVisible();
    if(m_node) {
        if(m_node->index().internal_pointer() != widget->index().internal_pointer())
            m_view->m_arc_map.erase(m_node->index().internal_pointer());
        m_node->hide();
        m_node->deleteLater();
    }
    m_node = widget;
    m_view->m_arc_map[m_node->index().internal_pointer()] = this;
    m_node->setParent(this);
    if(m_view->selection_model()->is_current_index(m_node->index()))
        m_node->set_etat_selection(node_widget::Current);
    else if(m_view->selection_model()->is_selected(m_node->index()))
        m_node->set_etat_selection(node_widget::Selected);
    m_node->update_data(m_node->index(),All_Data_Change_Flag);
    connect(m_node,&node_widget::left_clicked,this,[this]()
        {m_view->click_left_on(m_node->index());});
    if(m_node_arc_visible)
        m_node->move(m_view->m_arc_painter->left_node_margin(),m_view->m_arc_painter->top_node_margin());
    if(draw)
        draw_node();
    else
        adjustSize();
}

QSize arc_node_view_widget::sizeHint() const{
    auto &arc_painter = *m_view->m_arc_painter;
    if(m_expanded) {
        QSize sz(0,m_arc_child.back()->geometry().bottom() + arc_painter.bottom_tools_zone_margin());
        if(m_node_arc_visible)
            sz.rwidth() = m_node->geometry().right();
        for (auto it = m_arc_child.cbegin(); it != m_arc_child.cend(); ++it) {
            if((*it)->geometry().right() > sz.width())
                sz.rwidth() = (*it)->geometry().right();
        }
        if(m_node_arc_visible)
            sz.rwidth() += arc_painter.right_node_margin();
        return sz;
    }
    auto sz = m_node->sizeHint();
    if(sz.width() < arc_painter.width_tool_zone(node_view::End_Of_Tool))
        sz.rwidth() = arc_painter.width_tool_zone(node_view::End_Of_Tool);
    sz.rwidth() += arc_painter.left_node_margin() + arc_painter.right_node_margin();
    sz.rheight() += arc_painter.top_node_margin() + arc_painter.bottom_node_margin()
                    + arc_painter.height_tool_zone() + arc_painter.bottom_tools_zone_margin();
    return sz;
}

///////////////////////////////////////// indexed_widget /////////////////////////////////////////
indexed_widget::indexed_widget(const node_index &index, QWidget *parent)
    : QWidget(parent), m_index(index) {
    m_index.set_source(this);
    if(index.data(model_base::Orientation_Role).toUInt() == Qt::Horizontal)
        m_main_layout = new QHBoxLayout(this);
    else
        m_main_layout = new QVBoxLayout(this);
}

/////////////////////////////////////// node_view //////////////////////////////////////////
node_view::node_view(std::unique_ptr<arc_painter> &&arc_painter, QWidget *parent)
    : QScrollArea (parent), m_arc_painter(std::move(arc_painter)) {}

void node_view::click_left_on(const node_index &index) {
    switch (m_selection_mode) {
    case Single_Selection:
        m_selection_model->set_current_index(index,select_model::Toggle);
        break;
    case Multi_Selection:
        break;
    case Continous_Selection:
        break;
    case Extended_Selection:
        break;
    case No_Selection:
        break;
    }
}

void node_view::current_changed(const node_index &current, const node_index &previous){
    if(current.is_valid()) {
        auto it = m_arc_map.find(current.internal_pointer());
        if(it != m_arc_map.end())
            it->second->node()->set_etat_selection(node_widget::Current);
    }
    if(previous.is_valid()) {
        auto it = m_arc_map.find(previous.internal_pointer());
        if(it != m_arc_map.end())
            it->second->node()->set_etat_selection(node_widget::No_Selected);
    }
}

void node_view::delete_root(){
    m_arc_map.clear();
    delete takeWidget();
}

void node_view::insert_nodes(const node_index &parent, numt pos, numt count) {
    auto it = m_arc_map.find(parent.internal_pointer());
    if(it != m_arc_map.end())
        it->second->insert_nodes(pos,count);
}

void node_view::remove_nodes(const node_index &parent, numt pos, numt count) {
    auto it = m_arc_map.find(parent.internal_pointer());
    if(it != m_arc_map.end())
        it->second->remove_nodes(pos, count);
}

void node_view::set_current_index(const node_index &index){
    m_selection_model->set_current_index(index,model_base::node_selection_model::Select);
}

void node_view::set_delegate(node_delegate *delegate) {
    if(m_delegate)
        delete m_delegate;
    m_delegate = delegate;
    m_delegate->setParent(this);
    if(m_delegate && m_model)
        reset_root();
}

void node_view::set_model(node_model *model) {
    if(m_model && static_cast<QObject*>(m_model->parent()) == static_cast<QObject*>(this))
        m_model->deleteLater();
    m_model = model;
    if(!m_model->parent())
        m_model->setParent(this);
    connect(m_model,&node_model::data_changed,this,&node_view::update_data);
    connect(m_model,&node_model::nodes_about_to_be_removed,this,&node_view::remove_nodes);
    connect(m_model,&node_model::model_about_to_be_reset,this,&node_view::delete_root);
    connect(m_model,&node_model::model_about_to_reset_data,this,[this](){m_connexion_update_data = false;});
    connect(m_model,&node_model::model_reset,this,&node_view::reset_root);
    connect(m_model,&node_model::model_reset_data,this,[this](){
        m_connexion_update_data = true;
        updateAllData();});
    connect(m_model,&node_model::nodes_inserted,this,&node_view::insert_nodes);
    if(m_delegate &&m_model)
        reset_root();
    set_selection_model(new select_model(m_model,this));
}

void node_view::set_selection_model(select_model *selection_model) {
    if(!m_selection_mode)
        disconnect(m_selection_model,nullptr,this,nullptr);
    m_selection_model = selection_model;
    connect(m_selection_model,&select_model::current_changed,this,&node_view::current_changed);
}
void node_view::reset_root(){
    if(widget())
        delete_root();
    auto root_index = m_model->index(node_index(),0);
    if(root_index.flags().test(model_base::Visible_Flag_Node))
        setWidget(new arc_node_view_widget(root_index,this,this,true));
    else
        setWidget(new root_node_view_widget(root_index,this));
}

void node_view::updateAllData() {
    if(m_connexion_update_data)
        for (auto it = m_arc_map.begin(); it != m_arc_map.cend(); ++it)
            it->second->set_node_widget(it->second->node()->index());
}

void node_view::update_data(const node_index &index, flag role) {
    if(m_connexion_update_data) {
        auto it = m_arc_map.find(index.internal_pointer());
        if(it != m_arc_map.end()){
            if(index.cible() == Node_Cible && index.num() == model_base::Node_Num && role & Type_Change_Flag)
                it->second->set_node_widget(index);
            else
                it->second->node()->update_data(index,role);  
        }
    }
}

/////////////////////////////////////////////// node_widget ///////////////////////////////////////////////////
node_widget::~node_widget() {
    for (auto it = m_cible_map.cbegin(); it != m_cible_map.cend(); ++it)
        disconnect(it->second,&QObject::destroyed,this,&node_widget::remove_sub_node_widget);
}

void node_widget::add_sub_node_widget(sub_node_widget *sub_node) {
    m_main_layout->addWidget(sub_node);
    m_cible_map.insert({sub_node->index().sub(),sub_node});
    connect(sub_node,&QObject::destroyed,this,&node_widget::remove_sub_node_widget);
    sub_node->update_data(model_base::All_Data_Change_Flag);
}

void node_widget::update_data() {
    indexed_widget::update_data(model_base::All_Data_Change_Flag);
    for (auto it = m_cible_map.begin(); it != m_cible_map.end(); ++it)
        it->second->update_data(model_base::All_Data_Change_Flag);
}

void node_widget::update_data(const model_base::node_index &index, flag role) {
    if(index.cible() == model_base::Node_Cible && index.num() == model_base::Node_Num)
        indexed_widget::update_data(role);
    else {
        auto cible_it = m_cible_map.equal_range(index.sub());
        for (auto it = cible_it.first; it != cible_it.second; ++it)
            if(!(role & Same_Change_Flag) || !index.is_source(it->second))
                it->second->update_data(role);
    }
}
