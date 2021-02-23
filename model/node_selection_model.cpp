#include "node_selection_model.h"

using namespace mps;
using namespace model_base;

node_selection_model::node_selection_model(item_node_model *model, QObject *parent)
    : QObject(parent), m_model(model){

}

void node_selection_model::set_current_index(const node_index &index, flag selectionFlag) {
    if (m_model->check_index(index)) {
        if(selectionFlag.test(Select) &&index.flags().test(Selectable_Flag_Node)) {
            auto old = m_current_index;
            m_current_index = index;
            m_has_selection = true;
            emit current_changed(m_current_index,old);
        }
        if(selectionFlag.test(Deselect) &&m_current_index == index) {
            auto old = m_current_index;
            m_current_index = node_index();
            m_has_selection = false;
            emit current_changed(m_current_index,old);
        }
        if(selectionFlag.test(Toggle))
            set_current_index(index,m_current_index == index ? Deselect : Select);
    }
}
