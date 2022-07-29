#include "tableau_for_model.h"

using namespace mps;
using namespace model_base;

/////////////////////////////////////// composition_tableau /////////////////////
void composition_tableau::add(szt count) {
    for(auto it = m_tableaux.begin(); it != m_tableaux.end(); ++it)
        (*it)->add(count);
}

void composition_tableau::clear() {
    for(auto it = m_tableaux.begin(); it != m_tableaux.end(); ++it)
        (*it)->clear();
}

bool composition_tableau::egal(szt ligne_1, szt ligne_2) const
    TEST_COMPOSITION_TABLEAU(Egal_Module,egal(ligne_1,ligne_2))

void composition_tableau::erase(szt ligne) {
    for(auto it = m_tableaux.begin(); it != m_tableaux.end(); ++it)
        (*it)->erase(ligne);
}

void composition_tableau::erase(szt first, szt last) {
    for(auto it = m_tableaux.begin(); it != m_tableaux.end(); ++it)
        (*it)->erase(first, last);
}

bool composition_tableau::exists_internal_data(szt ligne) const
    TEST_COMPOSITION_TABLEAU(Exists_Module,exists_internal_data(ligne))

void composition_tableau::hydrate(szt ligne)
    OPERATION_COMPOSITION_TABLEAU(Hydrate_Module,hydrate(ligne));

bool composition_tableau::new_or_modif(szt ligne) const
    TEST_COMPOSITION_TABLEAU(New_Or_Modif_Module,new_or_modif(ligne))

void composition_tableau::push_back(std::unique_ptr<abstract_tableau> &&tableau, bool actif){
    if(!m_tableaux.empty() &&tableau->size() != size())
        throw std::runtime_error(QString("void composition_tableau::push_back(std::unique_ptr<abstract_tableau> &&)\n"
                                 "La taille du nouveau tableau (").append(QString::number(tableau->size()))
                                 .append(") ne correspond pas à ceux déjà présents (")
                                 .append(QString::number(size())).toStdString());
    m_tableaux.push_back(std::move(tableau));
    for (auto it = m_module.begin(); it != m_module.cend(); ++it)
        it->push_back(actif);
}

bool composition_tableau::remove_internal_data(szt ligne)
    TEST_COMPOSITION_TABLEAU(Remove_Module,remove_internal_data(ligne))

void composition_tableau::save(szt ligne)
    OPERATION_COMPOSITION_TABLEAU(Save_Module,save(ligne))

bool composition_tableau::valide(szt ligne) const
    TEST_COMPOSITION_TABLEAU(Valide_Module,valide(ligne))
