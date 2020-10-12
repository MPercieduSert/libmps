#include "TreeWidget.h"

using namespace widgetMPS;

TreeWidget::~TreeWidget() = default;

void TreeWidget::setId(idt id){
    collapseAll();
    QTreeWidgetItemIterator iter(this);
    while (*iter && (*iter)->data(IdColonne,IdRole).toUInt() != id)
        ++iter;
    if(*iter)
        setCurrentItem(*iter);
}
