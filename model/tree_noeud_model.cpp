#include "tree_noeud_model.h"

using namespace mps;
using namespace model_base;

tree_noeud_model::abstract_noeud::~abstract_noeud() = default;

tree_noeud_model::tree_noeud_model(bool racine, QObject *parent) : abstract_model (parent), m_data(this,racine){}

int tree_noeud_model::columnCount(const QModelIndex &parent) const {
    if(parent.isValid())
        return m_data.get_valid_data(parent)->child_column_count();
    else
        return m_data.get_root_data()->child_column_count();
}

QVariant tree_noeud_model::data(const QModelIndex &index, int role) const {
    if(checkIndex(index))
        return m_data.get_valid_data(index)->data(index.column(),role);
    return QVariant();
}

Qt::ItemFlags tree_noeud_model::flags(const QModelIndex &index) const {
    if(checkIndex(index))
        return m_data.get_valid_data(index)->flags(index.column());
    return Qt::NoItemFlags;
}

bool tree_noeud_model::insert_noeuds(int type, int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount(parent))
        return false;
    if(m_data.racine() &&!parent.isValid())
        return false;
    beginInsertRows(parent,row,row + count - 1);
        m_data.insert_rows([this,type](int row, const QModelIndex &parent){return node_factory(type,row,parent);},row,count,parent);
    endInsertRows();
    return true;
}

bool tree_noeud_model::removeRows(int row, int count, const QModelIndex &parent) {
    if(row < 0 || count <= 0 || row >= rowCount(parent))
        return false;
    beginRemoveRows(parent, row, row + count -1);
        if(count == 1)
            m_data.tree_data().erase(m_data.get_iter(parent).to_child(row));
        else
            m_data.tree_data().erase(m_data.get_iter(parent).to_child(row),m_data.get_iter(parent).to_child(row + count));
    endRemoveRows();
    return true;
}

bool tree_noeud_model::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(checkIndex(index) &&m_data.get_valid_data(index)->set_data(index.column(),value,role)) {
        dataChanged(index,index);
        return true;
    }
    return false;
}
