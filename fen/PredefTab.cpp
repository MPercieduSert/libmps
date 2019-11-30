#include "PredefTab.h"

using namespace fenMPS;

AbstractTabModule * PredefTab::createTab(std::pair<int,int> pair) const {
    switch (pair.first) {
        case AbstractTabMenu::MotCleTabIndex:
            return new TabMotCle;


        default:
            return nullptr;
    }
}
