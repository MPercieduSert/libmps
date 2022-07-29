#include "tree_colonnes_model.h"

using namespace mps;
using namespace model_base;

tree_colonnes_model::tree_colonnes_model(bool racine, QObject *parent) : abstract_colonnes_model (parent), m_tree(this,racine){}

void tree_colonnes_model::find(abstract_find_model *find_model) {
    if(find_model) {
        beginResetModel();
            m_tree.set_tree(m_full_tree);
            if(find_model->root_leaf())
                m_tree.tree_data().remove_leaf_if([find_model](tree<szt>::leaf_iterator it)->bool
                    {return find_model->test_root(*it);});
            else
                m_tree.tree_data().remove_leaf_if([find_model](tree<szt>::leaf_iterator it)->bool
                    {return find_model->test_tree(*it);});
            if(m_colonne_sorted >=0 &&m_colonne_sorted < columnCount())
                sort(m_colonne_sorted,m_order_sort);
        endResetModel();
    }
}

bool tree_colonnes_model::insertRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || row > rowCount())
        return false;
    auto nbrLigneOld = nbr_lignes();
    m_data->add(static_cast<szt>(count));
    // Insertion des nouvelles lignes dans le vecteur des indices.
    beginInsertRows(parent, row, row + count - 1);
        auto it = m_tree.get_iter(parent).to_child(row);
        auto full_it = m_full_tree.begin().to_path(it.path());
        for(szt ligne = nbrLigneOld; ligne != nbr_lignes(); ++ligne) {
            m_tree.tree_data().insert(it,ligne);
            m_full_tree.insert(full_it,ligne);
        }
    endInsertRows();
    return true;
}

bool tree_colonnes_model::removeRows(int /*row*/, int /*count*/, const QModelIndex &/*parent*/) {
//    count = std::min(count, row_count(parent)-row);
//    if(count <= 0 || row < 0)
//        return false;
//    beginRemoveRows(parent,row,row + count - 1);
//        auto first = m_tree.get_iter(parent).to_child(row);
//        // Suppression d'une ligne.
//        if (count == 1 &&first.leaf()) {
//            auto ligne = *first;
//            if(!m_data->exists_internal_data(ligne) || m_data->remove_internal_data(ligne)){
//                m_tree.tree().erase(first);
//                removeLine(ligne,m_tree.tree().begin(),static_cast<tree<szt>::iterator>(m_tree.tree().end()));
//            }
//        }
//        // Suppression de plusieur lignes.
//        else {
//            // Récupération des indices des lignes à supprimer et suppression des lignes du vecteur des lignes.
//            std::list<szt> list_id;
//            auto end = first;
//            end.to_brother(count - 1);
//            auto read = first;
//            for(auto i = 0; i != count; ++i, ++read){
//                if(!m_data->exists_internal_data(*read) ||m_data->remove_internal_data(*read)){
//                    if(newPlage)
//                        first = last = read;
//                    vec_id.push_back(*read);
//                    ++last;
//                }
//                else {
//                    m_row_to_ligne.erase(first,last);
//                    newPlage = true;
//                }
//            }
//            if(!newPlage)
//                m_row_to_ligne.erase(first,last);
//            // Mise à jour du vecteur des lignes.
//            std::sort(vec_id.begin(),vec_id.end());
//            for (auto write = m_row_to_ligne.begin(); write != m_row_to_ligne.end(); ++write) {
//                szt pos = 0;
//                read = vec_id.begin();
//                while (read != vec_id.end() &&*write > *read) {
//                    ++pos;
//                    ++read;
//                }
//                *write -= pos;
//            }
//            szt first_ligne = vec_id.back();
//            szt last_ligne = first_ligne;
//            newPlage = true;
//            // Suppréssion des lignes de données par numéro décroissant.
//            for(auto iter = std::next(vec_id.crbegin(),1); iter != vec_id.crend(); ++iter){
//                if (*iter == first_ligne - 1)
//                    --first_ligne;
//                else {
//                    if(first_ligne == last_ligne)
//                        m_data->erase(first_ligne);
//                    else
//                        m_data->erase(first_ligne, last_ligne + 1);
//                    first_ligne = last_ligne = *iter;
//                }
//            }
//            if(first_ligne == last_ligne)
//                m_data->erase(first_ligne);
//            else
//                m_data->erase(first_ligne, last_ligne + 1);
//        }
//    endRemoveRows();
    return true;
}
