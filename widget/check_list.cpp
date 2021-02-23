#include "check_list.h"

using namespace mps;
using namespace widget;

void check_list::set_labels(const std::map<idt,QString> &map) {
    for(auto i = map.cbegin(); i != map.cend(); ++i) {
        auto *item = new QListWidgetItem(i->second);
        item->setData(Qt::UserRole,i->first);
        item->setCheckState(Qt::Unchecked);
        addItem(item);
    }
}
