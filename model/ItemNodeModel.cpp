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
        return m_data.getValidNode(index).data(index.cible(), role, index.num());
    return QVariant();
}

numt ItemNodeModel::dataCount(const NodeIndex & index) const {
    if(checkIndex(index))
        return m_data.getValidNode(index).dataCount(index.cible());
    return ItemNode::NoData;
}

flag ItemNodeModel::flags(const NodeIndex & index) const {
    if(checkIndex(index))
        return m_data.getValidNode(index).flags(index.cible(), index.num());
    return NoFlagNode;
}
NodeIndex ItemNodeModel::createIndex(void * ptr, int cible, numt num) const noexcept{
    NodeIndex index;
    index.m_cible = cible;
    index.m_num = num;
    index.m_ptr = ptr;
    index.m_model = const_cast<ItemNodeModel *>(this);
    return index;
}

NodeIndex ItemNodeModel::index(const NodeIndex &parent, numt pos, int cible, numt num) const {
    if(parent.isValid()) {
        auto iter = m_data.getValidIter(parent);
        iter.toChildU(pos);
        if (iter)
            return createIndex(iter.ptr(),cible,num);
    }
    else if (pos == 0)
        return createIndex(m_data.getRootIter().ptr(),cible,num);
    return NodeIndex();
}

bool ItemNodeModel::insertNodes(const NodeIndex &parent, numt pos, numt count, int type) {
    if (!parent.isValid() || pos > childCount(parent))
        return false;
    beginInsertNodes(parent,pos,count);
        m_data.insertNodes(parent,pos,count,
                           [this,type](const NodeIndex & parentArg, numt posArg){return nodeFactory(parentArg,posArg,type);});
    endInsertNodes();
    return true;
}

bool ItemNodeModel::removeNodes(const NodeIndex &index, numt count) {
    if(count == 0 || !index.isValid() || index.isRoot())
        return false;
    beginRemoveNodes(index, count);
        if(count == 1)
            m_data.tree().erase(m_data.getIter(index));
        else
            m_data.tree().erase(m_data.getIter(index),m_data.getIter(index).toBrotherU(count));
    endRemoveNodes();
    return true;
}

bool ItemNodeModel::setData(const NodeIndex &index, const QVariant &value, int role) {
    if(checkIndex(index)){
        auto changeRole = m_data.getValidNode(index).setData(index.cible(),value,role,index.num());
        if(changeRole)
            dataChanged(index,changeRole);
        return true;
    }
    return false;
}
//////////////////////////////////// NodeIndex ////////////////////////////////
QVariant NodeIndex::data(int role) const
    {return m_model ? m_model->data(*this, role)
                      : QVariant();}

NodeIndex NodeIndex::firstBrother() const noexcept
    {return m_model ? m_model->firstBrother(*this)
                    : NodeIndex();}

flag NodeIndex::flags() const
    {return m_model ? m_model->flags(*this)
                      : Qt::NoItemFlags;}

bool NodeIndex::isRoot() const noexcept
    {return m_model ? m_model->isRoot(*this) : true;}

NodeIndex NodeIndex::lastBrother() const noexcept
    {return m_model ? m_model->lastBrother(*this)
                    : NodeIndex();}

bool NodeIndex::leaf() const
    {return isValid() && m_model->leaf(*this);}

NodeIndex NodeIndex::nextBrother() const noexcept
    {return m_model ? m_model->nextBrother(*this)
                    : NodeIndex();}

NodeIndex NodeIndex::parent() const
    {return m_model ? m_model->parent(*this)
                      : NodeIndex();}

numt NodeIndex::position() const noexcept
    {return m_model ? m_model->position(*this)
                    : 0;}

NodeIndex NodeIndex::prevBrother() const noexcept
    {return m_model ? m_model->prevBrother(*this)
                    : NodeIndex();}

//////////////////////////////////// ItemNodeModel ////////////////////////////
void ItemNodeBddModel::save() {

}

/////////////////////////////////// TreeForNodeModel ///////////////////////////
bool TreeForNodeModel::removeNodes(const NodeIndex &parent, numt pos, numt count) {
    if(!parent.isValid())
        return false;
    numt comp = 0;
    for(numt i = 0; i != count; ++i) {
        std::forward_list<iterator> pile;
        pile.push_front(getIter(parent).toChildU(pos));
        bool remove = true;
        while(!pile.empty() && remove) {
            if(!pile.front().leaf())
                pile.push_front(pile.front().crbeginChild());
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
