#include "id_combo_box.h"

using namespace mps;
using namespace widget;

void id_combo_box::set_current_index_id(idt id) {
    auto index = findData(id);
    if(index != -1)
        setCurrentIndex(index);
}
