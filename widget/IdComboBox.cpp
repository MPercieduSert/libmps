#include "IdComboBox.h"

using namespace widgetMPS;

void IdComboBox::setCurrentIndexId(idt id) {
    auto index = findData(id);
    if(index != -1)
        setCurrentIndex(index);
}
