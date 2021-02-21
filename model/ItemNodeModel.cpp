#include "ItemNodeModel.h"

using namespace modelMPS;

//////////////////////////////////// ItemNode ////////////////////////////////
ItemNode::~ItemNode() = default;

QVariant ItemNode::data(int cible, int role, numt /*num*/) const {
    if(cible == NodeCible){
        switch (role) {
        case TypeRole:
            return type();
        case OrientationRole:
            return Qt::Horizontal;
        }
    }
    return QVariant();
}

//////////////////////////////////// ItemNodeModel ////////////////////////////
QVariant ItemNodeModel::data(const NodeIndex &index, int role) const {
    if(checkIndex(index))
        return m_data.getNode(index).data(index.cible(), role, index.num());
    return QVariant();
}

numt ItemNodeModel::dataCount(const NodeIndex &index) const {
    if(checkIndex(index))
        return m_data.getNode(index).dataCount(index.cible());
    return ItemNode::NoData;
}

flag ItemNodeModel::flags(const NodeIndex &index) const {
    if(checkIndex(index))
        return m_data.getNode(index).flags(index.cible(), index.num());
    return NoFlagNode;
}
NodeIndex ItemNodeModel::createIndex(IterNode iter, int cible, numt num) const noexcept{
    NodeIndex index;
    index.m_cible = cible;
    index.m_num = num;
    index.m_iter = iter;
    index.m_model = const_cast<ItemNodeModel *>(this);
    return index;
}

NodeIndex ItemNodeModel::index(const NodeIndex &parent, numt pos, int cible, numt num) const {
    if(parent.is_valid()) {
        auto iter = indexToIterator(parent);
        iter.to_childU(pos);
        if (iter)
            return createIndex(iter,cible,num);
    }
    else if (pos == 0)
        return createIndex(m_data.cbegin(),cible,num);
    return NodeIndex();
}

std::list<IterNode> ItemNodeModel::insert(const NodeIndex &parent, numt pos, numt count, int type) {
    if (checkIndex(parent) &&pos <= parent.childCount()) {
        beginInsertNodes(parent,pos,count);
            auto r = m_data.insertNodes(parent,pos,count,
                               [this,type](const NodeIndex &parentArg, numt posArg){return nodeFactory(parentArg,posArg,type);});
        endInsertNodes();
        return r;
    }
    return std::list<IterNode>();
}

bool ItemNodeModel::remove(const NodeIndex &index, numt count) {
    if(count == 0 || !checkIndex(index) || index.isRoot())
        return false;
    beginRemoveNodes(index, count);
        if(count == 1)
            m_data.erase(index);
        else
            m_data.erase(index,count);
    endRemoveNodes();
    return true;
}

bool ItemNodeModel::set(const NodeIndex &index, const QVariant &value, int role) {
    if(checkIndex(index)){
        auto changeRole = m_data.getNode(index).set_data(index.cible(),value,role,index.num());
        if(changeRole)
            dataChanged(index,changeRole);
        return true;
    }
    return false;
}
//////////////////////////////////// NodeIndex ////////////////////////////////
QVariant NodeIndex::data(int role) const
    {return m_model ? m_model->data(*this, role) : QVariant();}

NodeIndex NodeIndex::first() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_first_brother();
        return ind;
    }
    return NodeIndex();
}

flag NodeIndex::flags() const
    {return m_model ? m_model->flags(*this)
                      : Qt::NoItemFlags;}

NodeIndex NodeIndex::last() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_last_brother();
        return ind;
    }
    return NodeIndex();
}

NodeIndex NodeIndex::nextBrother() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_next();
        return ind;
    }
    return NodeIndex();
}

NodeIndex NodeIndex::parent() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_parent();
        return ind;
    }
    return NodeIndex();
}

NodeIndex NodeIndex::prevBrother() const noexcept {
    if(is_valid()) {
        auto ind = *this;
        ind.m_iter.to_prev();
        return ind;
    }
    return NodeIndex();
}

//////////////////////////////////// ItemNodeBddModel ////////////////////////////
void ItemNodeBddModel::save() {
    std::set<IterNode,decltype (&IterNode::Less_ptr)> changeSet(&IterNode::Less_ptr);
    std::list<IterNode> insertList;
    for(auto iter = m_changeList.cbegin(); iter != m_changeList.cend(); ++iter)
        switch (iter->type) {
        case SetData:
            changeSet.insert(iter->iter);
            break;
        case InsertNode:
            insertList.push_back(iter->iter);
            break;
        }
    insertList.sort();
    for(auto iter = insertList.cbegin(); iter != insertList.cend(); ++iter){
        static_cast<ItemBddNode &>(***iter).insert(bdd());
        changeSet.erase(*iter);
    }
    for(auto iter = changeSet.cbegin(); iter != changeSet.cend(); ++iter)
        static_cast<ItemBddNode &>(***iter).save(bdd());

}

/////////////////////////////////// TreeForNodeModel ///////////////////////////
Node TreeForNodeModel::moveNode(const NodeIndex &index, Node &&node){
    Node old = takeNode(index);
    setNode(index,std::move(node));
    return old;
}

Node TreeForNodeModel::moveNode(const NodeIndex &from, const NodeIndex &to) {
    Node old = takeNode(to);
    move(from,to);
    return old;
}

bool TreeForNodeModel::removeNodes(const NodeIndex &parent, numt pos, numt count) {
    if(!parent.is_valid())
        return false;
    numt comp = 0;
    for(numt i = 0; i != count; ++i) {
        std::forward_list<iterator> pile;
        auto iter = parent.m_iter;
        pile.push_front(iter.to_childU(pos));
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
