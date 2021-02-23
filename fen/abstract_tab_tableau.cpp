#include "abstract_tab_tableau.h"

using namespace mps;
using namespace fenetre;

abstract_tab_tableau::abstract_tab_tableau(b2d::bdd &bdd, const tab_index &index, QWidget *parent)
    : abstract_tab_module_with_bdd(bdd,index, parent) {}

void abstract_tab_tableau::become_current()
    {emit action_permise(Editer_Action);}

void abstract_tab_tableau::coller()
    {m_model->coller(m_view->selectionModel()->selectedIndexes());}

void abstract_tab_tableau::copier()
    {m_model->copier(m_view->selectionModel()->selectedIndexes());}

void abstract_tab_tableau::couper()
    {m_model->couper(m_view->selectionModel()->selectedIndexes());}

void abstract_tab_tableau::effacer()
    {m_model->effacer(m_view->selectionModel()->selectedIndexes());}

void abstract_tab_tableau::supprimer() {
    if(model()->remove_rows_selected(m_view->selectionModel()->selectedIndexes()))
        m_view->selectionModel()->clear();
    else
        QMessageBox::warning(this,tr("Erreur de sélection"),
                             tr("Vous devez séléctioner des lignes entières pour supprimer des éléves"));
}
