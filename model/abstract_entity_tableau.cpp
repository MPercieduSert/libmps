#include "abstract_entity_tableau.h"

using namespace mps;
using namespace model_base;

void abstract_entity_tableau::add(szt count) {
    auto old_nbr_lignes = size();
    auto new_nbr_lignes = old_nbr_lignes + count;
    szt j = 0;
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter,++j)
        iter->resize(new_nbr_lignes,[this,j,i = old_nbr_lignes]()mutable{return entity_factory(i++,j);});
}

bool abstract_entity_tableau::egal(szt ligne_1, szt ligne_2) const {
    auto iter = m_data.begin();
    while(iter != m_data.end() &&(*iter)[ligne_1] == (*iter)[ligne_2])
        ++iter;
    return iter == m_data.end();
}

void abstract_entity_tableau::erase(szt ligne) {
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter)
        iter->erase(std::next(iter->cbegin(),ligne));
}

//! Supprime les lignes [first,last] des données du model.
void abstract_entity_tableau::erase(szt first, szt last) {
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter)
        iter->erase(std::next(iter->cbegin(),first),std::next(iter->cbegin(),last));
}
