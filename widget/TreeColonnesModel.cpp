#include "TreeColonnesModel.h"

using namespace modelMPS;

TreeColonnesModel::TreeColonnesModel(bool racine, QObject *parent) : AbstractColonnesModel (parent), m_tree(this,racine){}

void TreeColonnesModel::find(FindModel * findModel) {
    if(findModel) {
        beginResetModel();
            m_tree.setTree(m_fullTree);
            if(findModel->rootLeaf())
                m_tree.tree().removeLeafIf([findModel](tree<szt>::leaf_iterator iter)->bool{return findModel->testRoot(*iter);});
            else
                m_tree.tree().removeLeafIf([findModel](tree<szt>::leaf_iterator iter)->bool{return findModel->testTree(*iter);});
            if(m_colonneSorted >=0 && m_colonneSorted < columnCount())
                sort(m_colonneSorted,m_orderSort);
        endResetModel();
    }
}

bool TreeColonnesModel::insertRows(int row, int count, const QModelIndex & parent) {
    if (count <= 0 || row < 0 || row > rowCount())
        return false;
    auto nbrLigneOld = nbrLignes();
    m_data->add(static_cast<szt>(count));
    // Insertion des nouvelles lignes dans le vecteur des indices.
    beginInsertRows(parent, row, row + count - 1);
        auto iter = m_tree.getIter(parent).toChild(row);
        auto iterFull = m_fullTree.begin().toPath(iter.path());
        for(szt ligne = nbrLigneOld; ligne != nbrLignes(); ++ligne) {
            m_tree.tree().insert(iter,ligne);
            m_fullTree.insert(iterFull,ligne);
        }
    endInsertRows();
    return true;
}
