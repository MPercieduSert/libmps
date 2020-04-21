#include "AbstractTabTableau.h"

using namespace fenMPS;

AbstractTabTableau::AbstractTabTableau(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd(bdd,pairIndex, parent) {}

void AbstractTabTableau::becomeCurrent() {
    emit effacerPermis(true);
    emit collerPermis(true);
    emit copierPermis(true);
    emit couperPermis(true);
    emit savePermis(true);
}

void AbstractTabTableau::coller()
    {m_model->coller(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::copier()
    {m_model->copier(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::couper()
    {m_model->couper(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::effacer()
    {m_model->effacer(m_view->selectionModel()->selectedIndexes());}
