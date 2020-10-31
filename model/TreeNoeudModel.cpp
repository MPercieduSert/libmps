#include "TreeNoeudModel.h"

using namespace modelMPS;

TreeNoeudModel::AbstractNoeud::~AbstractNoeud() = default;

TreeNoeudModel::TreeNoeudModel(bool racine, QObject *parent) : AbstractModel (parent), m_data(this,racine){}

int TreeNoeudModel::columnCount(const QModelIndex & parent) const {
    if(parent.isValid())
        return m_data.getValidData(parent)->childColumnCount();
    else
        return m_data.getRootData()->childColumnCount();
}

QVariant TreeNoeudModel::data(const QModelIndex &index, int role) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->data(index.column(),role);
    return QVariant();
}

Qt::ItemFlags TreeNoeudModel::flags(const QModelIndex & index) const {
    if(checkIndex(index))
        return m_data.getValidData(index)->flags(index.column());
    return Qt::NoItemFlags;
}

bool TreeNoeudModel::insertNoeuds(int type, int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount(parent))
        return false;
    if(m_data.racine() && !parent.isValid())
        return false;
    beginInsertRows(parent,row,row + count - 1);
        m_data.insertRows([this,type](int row, const QModelIndex & parent){return nodeFactory(type,row,parent);},row,count,parent);
    endInsertRows();
    return true;
}

bool TreeNoeudModel::removeRows(int row, int count, const QModelIndex &parent) {
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

bool TreeNoeudModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(checkIndex(index) && m_data.getValidData(index)->setData(index.column(),value,role)) {
        dataChanged(index,index);
        return true;
    }
    return false;
}
