#include "AbstractNodeModel.h"

using namespace modelMPS;

NodeIndex AbstractNodeModel::createIndex(int row, void * ptr) const {
    NodeIndex index;
    index.m_ptr = ptr;
    index.m_row = row;
    index.m_model = const_cast<AbstractNodeModel *>(this);
    return index;
}

//////////////////////////////////// NodeIndex ////////////////////////////////
QVariant NodeIndex::data(int cible, int role, szt num) const
    {return isValid() ? m_model->data(*this, cible, role, num)
                      : QVariant();}

Qt::ItemFlags NodeIndex::flags(int cible, szt num) const
    {return isValid() ? m_model->flags(*this, cible, num)
                      : Qt::NoItemFlags;}

NodeIndex NodeIndex::parent() const
    {return isValid() ? m_model->parent(*this)
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

bool TreeNodeModel::insertNodes(int type, int row, int count, const NodeIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount(parent))
        return false;
    if(m_data.racine() && !parent.isValid())
        return false;
    beginInsertRows(parent,row,row + count - 1);
        m_data.insertRows([this,type](int row, const NodeIndex & parent){return nodeFactory(type,row,parent);},row,count,parent);
    endInsertRows();
    return true;
}

bool TreeNodeModel::removeRows(int row, int count, const NodeIndex &parent) {
    if(row < 0 || count <= 0 || row >= rowCount(parent))
        return false;
    beginRemoveRows(parent, row, row + count -1);
        if(count == 1)
            m_data.tree().erase(m_data.getIter(parent).toChild(row));
        else
            m_data.tree().erase(m_data.getIter(parent).toChild(row),m_data.getIter(parent).toChild(row + count));
    endRemoveRows();
    return true;
}

bool TreeNodeModel::setData(const NodeIndex &index, int cible, const QVariant &value, int role, szt num) {
    if(checkIndex(index) && m_data.getValidData(index)->setData(cible,value,role,num)) {
        dataChanged(index,cible,num);
        return true;
    }
    return false;
}
