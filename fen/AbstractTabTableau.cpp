#include "AbstractTabTableau.h"

using namespace fenMPS;

AbstractTabTableau::AbstractTabTableau(QWidget *parent)
    : AbstractTabModule(parent) {}

void AbstractTabTableau::coller()
    {m_model->coller(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::copier()
    {m_model->copier(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::couper()
    {m_model->couper(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::effacer()
    {m_model->effacer(m_view->selectionModel()->selectedIndexes());}
