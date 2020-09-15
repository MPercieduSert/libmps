#include "TableauForModel.h"

using namespace modelMPS;

/////////////////////////////////////// CompositionTableau /////////////////////
void CompositionTableau::add(szt count) {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->add(count);
}

void CompositionTableau::clear() {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->clear();
}

bool CompositionTableau::egal(szt ligne1, szt ligne2) const
    TEST_COMPOSITION_TABLEAU(EgalModule,egal(ligne1,ligne2))

void CompositionTableau::erase(szt ligne) {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->erase(ligne);
}

void CompositionTableau::erase(szt first, szt last) {
    for(auto iter = m_tableaux.begin(); iter != m_tableaux.end(); ++iter)
        (*iter)->erase(first, last);
}

bool CompositionTableau::existsInternalData(szt ligne) const
    TEST_COMPOSITION_TABLEAU(ExistsModule,existsInternalData(ligne))

void CompositionTableau::hydrate(szt ligne)
    OPERATION_COMPOSITION_TABLEAU(HydrateModule,hydrate(ligne));

bool CompositionTableau::newOrModif(szt ligne) const
    TEST_COMPOSITION_TABLEAU(NewOrModifModule,newOrModif(ligne))

void CompositionTableau::push_back(std::unique_ptr<AbstractTableau> && tableau, bool actif){
    if(!m_tableaux.empty() && tableau->size() != size())
        throw std::runtime_error(QString("void CompositionTableau::push_back(std::unique_ptr<AbstractTableau> && )\n"
                                 "La taille du nouveau tableau (").append(QString::number(tableau->size()))
                                 .append(") ne correspond pas à ceux déjà présents (").append(QString::number(size())).toStdString());
    m_tableaux.push_back(std::move(tableau));
    for (auto iter = m_module.begin(); iter != m_module.cend(); ++iter)
        iter->push_back(actif);
}

bool CompositionTableau::removeInternalData(szt ligne)
    TEST_COMPOSITION_TABLEAU(RemoveModule,removeInternalData(ligne))

void CompositionTableau::save(szt ligne)
    OPERATION_COMPOSITION_TABLEAU(SaveModule,save(ligne))

bool CompositionTableau::valide(szt ligne) const
    TEST_COMPOSITION_TABLEAU(ValideModule,valide(ligne))
