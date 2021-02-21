#include "Checklist.h"

using namespace widgetMPS;

void Checklist::setLabels(const std::map<idt,QString> &map) {
    for(auto i = map.cbegin(); i != map.cend(); ++i) {
        auto *item = new QListWidgetItem(i->second);
        item->set_data(Qt::UserRole,i->first);
        item->setCheckState(Qt::Unchecked);
        addItem(item);
    }
}
