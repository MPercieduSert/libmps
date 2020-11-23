#include "AbstractNodeModel.h"

using namespace modelMPS;

NodeIndex AbstractNodeModel::createIndex(void * ptr, int cible, szt num) const noexcept{
    NodeIndex index;
    index.m_cible = cible;
    index.m_num = num;
    index.m_ptr = ptr;
    index.m_model = const_cast<AbstractNodeModel *>(this);
    return index;
}

//////////////////////////////////// NodeIndex ////////////////////////////////
QVariant NodeIndex::data(int role) const
    {return m_model ? m_model->data(*this, role)
                      : QVariant();}

NodeIndex NodeIndex::firstBrother() const noexcept
    {return m_model ? m_model->firstBrother(*this)
                    : NodeIndex();}

Qt::ItemFlags NodeIndex::flags() const
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

uint NodeIndex::position() const noexcept
    {return m_model ? m_model->position(*this)
                    : 0;}

NodeIndex NodeIndex::prevBrother() const noexcept
    {return m_model ? m_model->prevBrother(*this)
                    : NodeIndex();}

/////////////////////////////////// TreeNodeModel ///////////////////////////

TreeNodeModel::AbstractNode::~AbstractNode() = default;

TreeNodeModel::TreeNodeModel(bool racine, QObject *parent) : AbstractNodeModel (parent), m_data(this,racine){}

QVariant TreeNodeModel::data(const NodeIndex &index, int role) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->data(index.cible(), role, index.num());
    return QVariant();
}

szt TreeNodeModel::dataCount(const NodeIndex & index) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->dataCount(index.cible());
    return NoData;
}

Qt::ItemFlags TreeNodeModel::flags(const NodeIndex & index) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->flags(index.cible(), index.num());
    return Qt::NoItemFlags;
}

bool TreeNodeModel::insertNodes(const NodeIndex &parent, szt pos, szt count, int type) {
    if (pos > childCount(parent))
        return false;
    if(m_data.racine() && !parent.isValid())
        return false;
    beginInsertNodes(parent,pos,pos + count - 1);
        m_data.insertNodes([this,type](szt posArg, const NodeIndex & parentArg){return nodeFactory(parentArg,posArg,type);},pos,count,parent);
    endInsertNodes();
    return true;
}

bool TreeNodeModel::removeNodes(const NodeIndex &index, szt count) {
    if(count == 0)
        return false;
    beginRemoveNodes(index, count);
        if(count == 1)
            m_data.tree().erase(m_data.getIter(index));
        else
            m_data.tree().erase(m_data.getIter(index),m_data.getIter(index).toBrotherU(count));
    endRemoveNodes();
    return true;
}

bool TreeNodeModel::setData(const NodeIndex &index, const QVariant &value, int role) {
    if(checkIndex(index) && m_data.getValidData(index)->setData(index.cible(),value,role,index.num())) {
        dataChanged(index);
        return true;
    }
    return false;
}
