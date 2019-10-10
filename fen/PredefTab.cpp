#include "PredefTab.h"

AbstractTabModule * PredefTab::createTab(QPair<int,int> pair) const
{
    switch (pair.first)
    {
        case AbstractTabMenu::MotCleTabIndex:
            return new TabMotCle;


        default:
            return nullptr;
    }
}
