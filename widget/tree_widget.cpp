#include "tree_widget.h"

using namespace mps;
using namespace widget;

tree_widget::~tree_widget() = default;

void tree_widget::set_id(idt id){
    collapseAll();
    QTreeWidgetItemIterator it(this);
    while (*it &&(*it)->data(Id_Colonne,Id_Role).toUInt() != id)
        ++it;
    if(*it)
        setCurrentItem(*it);
}
