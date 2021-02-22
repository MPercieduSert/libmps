#include "AbstractTabTableau.h"

using namespace fenMPS;

AbstractTabTableau::AbstractTabTableau(b2d::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd(bdd,pairIndex, parent) {}

void AbstractTabTableau::become_current()
    {emit actionPermise(EditerActions);}

void AbstractTabTableau::coller()
    {m_model->coller(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::copier()
    {m_model->copier(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::couper()
    {m_model->couper(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::effacer()
    {m_model->effacer(m_view->selectionModel()->selectedIndexes());}

void AbstractTabTableau::supprimer() {
    if(model()->removeRowsSelected(m_view->selectionModel()->selectedIndexes()))
        m_view->selectionModel()->clear();
    else
        QMessageBox::warning(this,tr("Erreur de sélection"),
                             tr("Vous devez séléctioner des lignes entières pour supprimer des éléves"));
}
