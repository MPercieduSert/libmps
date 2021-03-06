#include "AbstractEntityTableau.h"

using namespace modelMPS;

void AbstractEntityTableau::add(szt count) {
    auto oldNbrLignes = size();
    auto newNbrLignes = oldNbrLignes + count;
    szt j = 0;
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter,++j)
        iter->resize(newNbrLignes,[this,j,i = oldNbrLignes]()mutable{return entityFactory(i++,j);});
}

bool AbstractEntityTableau::egal(szt ligne1, szt ligne2) const {
    auto iter = m_data.begin();
    while(iter != m_data.end() && (*iter)[ligne1] == (*iter)[ligne2])
        ++iter;
    return iter == m_data.end();
}

void AbstractEntityTableau::erase(szt ligne) {
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter)
        iter->erase(std::next(iter->cbegin(),ligne));
}


//! Supprime les lignes [first,last] des données du model.
void AbstractEntityTableau::erase(szt first, szt last) {
    for (auto iter = m_data.begin(); iter != m_data.end(); ++iter)
        iter->erase(std::next(iter->cbegin(),first),std::next(iter->cbegin(),last));
}
