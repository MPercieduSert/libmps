#include "AbstractNodeModel.h"

using namespace modelMPS;

NodeIndex AbstractNodeModel::createIndex(void * ptr) const {
    NodeIndex index;
    index.m_ptr = ptr;
    index.m_model = const_cast<AbstractNodeModel *>(this);
    return index;
}

//////////////////////////////////// NodeIndex ////////////////////////////////
QVariant NodeIndex::data(int cible, int role, szt num) const
    {return m_model ? m_model->data(*this, cible, role, num)
                      : QVariant();}

NodeIndex NodeIndex::firstBrother() const noexcept
    {return m_model ? m_model->firstBrother(*this)
                    : NodeIndex();}

Qt::ItemFlags NodeIndex::flags(int cible, szt num) const
    {return m_model ? m_model->flags(*this, cible, num)
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

QVariant TreeNodeModel::data(const NodeIndex &index, int cible, int role, szt num) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->data(cible,role,num);
    return QVariant();
}

szt TreeNodeModel::dataCount(const NodeIndex & index, int cible) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->dataCount(cible);
    return NoData;
}

Qt::ItemFlags TreeNodeModel::flags(const NodeIndex & index, int cible, szt num) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->flags(cible, num);
    return Qt::NoItemFlags;
}

bool TreeNodeModel::insertNodes(int type, szt pos, szt count, const NodeIndex &parent) {
    if (pos > childCount(parent))
        return false;
    if(m_data.racine() && !parent.isValid())
        return false;
    beginInsertNodes(parent,pos,pos + count - 1);
        m_data.insertNodes([this,type](szt row, const NodeIndex & parent){return nodeFactory(type,row,parent);},pos,count,parent);
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

bool TreeNodeModel::setData(const NodeIndex &index, int cible, const QVariant &value, int role, szt num) {
    if(checkIndex(index) && m_data.getValidData(index)->setData(cible,value,role,num)) {
        dataChanged(index,cible,num);
        return true;
    }
    return false;
}
