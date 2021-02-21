#include "TreeColonnesModel.h"

using namespace modelMPS;

TreeColonnesModel::TreeColonnesModel(bool racine, QObject *parent) : AbstractColonnesModel (parent), m_tree(this,racine){}

void TreeColonnesModel::find(AbstractFindModel *findModel) {
    if(findModel) {
        beginResetModel();
            m_tree.setTree(m_fullTree);
            if(findModel->root_leaf())
                m_tree.tree().remove_leaf_if([findModel](tree<szt>::leaf_iterator iter)->bool{return findModel->testRoot(*iter);});
            else
                m_tree.tree().remove_leaf_if([findModel](tree<szt>::leaf_iterator iter)->bool{return findModel->testTree(*iter);});
            if(m_colonneSorted >=0 &&m_colonneSorted < columnCount())
                sort(m_colonneSorted,m_orderSort);
        endResetModel();
    }
}

bool TreeColonnesModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount())
        return false;
    auto nbrLigneOld = nbrLignes();
    m_data->add(static_cast<szt>(count));
    // Insertion des nouvelles lignes dans le vecteur des indices.
    beginInsertRows(parent, row, row + count - 1);
        auto iter = m_tree.getIter(parent).to_child(row);
        auto iterFull = m_fullTree.begin().to_path(iter.path());
        for(szt ligne = nbrLigneOld; ligne != nbrLignes(); ++ligne) {
            m_tree.tree().insert(iter,ligne);
            m_fullTree.insert(iterFull,ligne);
        }
    endInsertRows();
    return true;
}

bool TreeColonnesModel::removeRows(int /*row*/, int /*count*/, const QModelIndex &/*parent*/) {
//    count = std::min(count, rowCount(parent)-row);
//    if(count <= 0 || row < 0)
//        return false;
//    beginRemoveRows(parent,row,row + count - 1);
//        auto first = m_tree.getIter(parent).to_child(row);
//        // Suppression d'une ligne.
//        if (count == 1 &&first.leaf()) {
//            auto ligne = *first;
//            if(!m_data->existsInternalData(ligne) || m_data->removeInternalData(ligne)){
//                m_tree.tree().erase(first);
//                removeLine(ligne,m_tree.tree().begin(),static_cast<conteneurMPS::tree<szt>::iterator>(m_tree.tree().end()));
//            }
//        }
//        // Suppression de plusieur lignes.
//        else {
//            // Récupération des indices des lignes à supprimer et suppression des lignes du vecteur des lignes.
//            std::list<szt> listId;
//            auto end = first;
//            end.to_brother(count - 1);
//            auto read = first;
//            for(auto i = 0; i != count; ++i, ++read){
//                if(!m_data->existsInternalData(*read) ||m_data->removeInternalData(*read)){
//                    if(newPlage)
//                        first = last = read;
//                    vecId.push_back(*read);
//                    ++last;
//                }
//                else {
//                    m_rowToLigne.erase(first,last);
//                    newPlage = true;
//                }
//            }
//            if(!newPlage)
//                m_rowToLigne.erase(first,last);
//            // Mise à jour du vecteur des lignes.
//            std::sort(vecId.begin(),vecId.end());
//            for (auto write = m_rowToLigne.begin(); write != m_rowToLigne.end(); ++write) {
//                szt pos = 0;
//                read = vecId.begin();
//                while (read != vecId.end() &&*write > *read) {
//                    ++pos;
//                    ++read;
//                }
//                *write -= pos;
//            }
//            szt firstLigne = vecId.back();
//            szt lastLigne = firstLigne;
//            newPlage = true;
//            // Suppréssion des lignes de données par numéro décroissant.
//            for(auto iter = std::next(vecId.crbegin(),1); iter != vecId.crend(); ++iter){
//                if (*iter == firstLigne - 1)
//                    --firstLigne;
//                else {
//                    if(firstLigne == lastLigne)
//                        m_data->erase(firstLigne);
//                    else
//                        m_data->erase(firstLigne, lastLigne + 1);
//                    firstLigne = lastLigne = *iter;
//                }
//            }
//            if(firstLigne == lastLigne)
//                m_data->erase(firstLigne);
//            else
//                m_data->erase(firstLigne, lastLigne + 1);
//        }
//    endRemoveRows();
    return true;
}
