#include "item_node_model.h"

using namespace mps;
using namespace model_base;

//////////////////////////////////// item_node ////////////////////////////////
item_node::~item_node() = default;

QVariant item_node::data(int cible, int role, numt num) const {
    if(cible == Node_Cible && num == Node_Num) {
        switch (role) {
        case Orientation_Role:
            return Qt::Horizontal;
        case Type_Role:
            return type();
        case Cibles_Role:
            return QList<QVariant>();
        }
    }
    return QVariant();
}

//////////////////////////////////// item_bdd_node ////////////////////////////
item_bdd_node::~item_bdd_node() = default;

//////////////////////////////////// item_node_model ////////////////////////////
QVariant item_node_model::data(const node_index &index, int role) const {
    if(check_index(index))
        return m_data.get_node(index).data(index.cible(), role, index.num());
    return QVariant();
}

flag item_node_model::flags(const node_index &index) const {
    if(check_index(index))
        return m_data.get_node(index).flags(index.cible(), index.num());
    return No_Flag_Node;
}
node_index item_node_model::create_index(node_iter iter, int cible, numt num) const noexcept{
    node_index index;
    index.m_cible = cible;
    index.m_num = num;
    index.m_iter = iter;
    index.m_model = const_cast<item_node_model *>(this);
    return index;
}

node_index item_node_model::index(const node_index &parent, numt pos, int cible, numt num) const {
    if(parent.is_valid()) {
        auto iter = index_to_iterator(parent);
        iter.to_child_u(pos);
        if (iter)
            return create_index(iter,cible,num);
    }
    else if (pos == 0)
        return create_index(m_data.cbegin(),cible,num);
    return node_index();
}

std::list<node_iter> item_node_model::insert(const node_index &parent, numt pos, numt count, int type) {
    if (check_index(parent) && pos <= parent.child_count()) {
        begin_insert_nodes(parent,pos,count);
            auto r = m_data.insert_nodes(parent,pos,count,
                               [this,type](const node_index &parentArg, numt posArg){return node_factory(parentArg,posArg,type);});
        end_insert_nodes();
        return r;
    }
    return std::list<node_iter>();
}

bool item_node_model::remove(const node_index &index, numt count) {
    if(count == 0 || !check_index(index) || index.is_root())
        return false;
    begin_remove_nodes(index, count);
        if(count == 1)
            m_data.erase(index);
        else
            m_data.erase(index,count);
    end_remove_nodes();
    return true;
}

bool item_node_model::set(const node_index &index, const QVariant &value, int role) {
    if(check_index(index)){
        auto changeRole = m_data.get_node(index).set_data(index.cible(),value,role,index.num());
        if(changeRole) {
            emit data_changed(index,changeRole);
            return true;
        }
    }
    return false;
}
//////////////////////////////////// node_index ////////////////////////////////
QVariant node_index::data(int role) const
    {return m_model ? m_model->data(*this, role) : QVariant();}

node_index node_index::first() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_first();
        return ind;
    }
    return node_index();
}

node_index node_index::first_brother() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_first_brother();
        return ind;
    }
    return node_index();
}

flag node_index::flags() const
    {return m_model ? m_model->flags(*this)
                      : Qt::NoItemFlags;}

node_index node_index::last() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_last();
        return ind;
    }
    return node_index();
}

node_index node_index::last_brother() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_last_brother();
        return ind;
    }
    return node_index();
}

node_index node_index::next() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_next();
        return ind;
    }
    return node_index();
}

node_index node_index::parent() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_parent();
        return ind;
    }
    return node_index();
}

node_index node_index::prev() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_prev();
        return ind;
    }
    return node_index();
}

//////////////////////////////////// item_node_bdd_model ////////////////////////////
void item_node_bdd_model::save() {
    std::set<node_iter,decltype(&node_iter::Less_ptr)> change_set(&node_iter::Less_ptr);
    std::list<node_iter> insert_list;
    for(auto iter = m_change_list.cbegin(); iter != m_change_list.cend(); ++iter)
        switch (iter->type) {
        case Set_Data:
            change_set.insert(iter->iter);
            break;
        case Insert_Node:
            insert_list.push_back(iter->iter);
            break;
        }
    insert_list.sort();
    for(auto iter = insert_list.cbegin(); iter != insert_list.cend(); ++iter){
        static_cast<item_bdd_node &>(***iter).insert(bdd());
        change_set.erase(*iter);
    }
    for(auto iter = change_set.cbegin(); iter != change_set.cend(); ++iter)
        static_cast<item_bdd_node &>(***iter).save(bdd());

}

/////////////////////////////////// tree_for_node_model ///////////////////////////
node_ptr tree_for_node_model::move_node(const node_index &index, node_ptr &&node){
    node_ptr old = take_node(index);
    set_node(index,std::move(node));
    return old;
}

node_ptr tree_for_node_model::move_node(const node_index &from, const node_index &to) {
    node_ptr old = take_node(to);
    move(from,to);
    return old;
}

bool tree_for_node_model::remove_nodes(const node_index &parent, numt pos, numt count) {
    if(!parent.is_valid())
        return false;
    numt comp = 0;
    for(numt i = 0; i != count; ++i) {
        std::forward_list<iterator> pile;
        auto iter = parent.m_iter;
        pile.push_front(iter.to_child_u(pos));
        bool remove = true;
        while(!pile.empty() &&remove) {
            if(!pile.front().leaf())
                pile.push_front(pile.front().crbegin_child());
            else {
                remove = (**pile.front()).del();
                if(remove) {
                    m_tree.erase(pile.front());
                    pile.pop_front();
                }
            }
        }
        if(pile.empty())
            ++comp;
        else
            ++pos;
    }
    return comp == count;
}
