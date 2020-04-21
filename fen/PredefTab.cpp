#include "PredefTab.h"

using namespace fenMPS;

AbstractTabModule * PredefTab::createTab(const std::pair<int,int> & pair, const std::vector<QVariant> & /*args*/) const {
    switch (pair.first) {
        case GestionBddTabId:
            return new TabGestionBdd(m_bdd , pair);


        //case AbstractTabMenu::MotCleTabIndex:
        //    return new TabMotCle;


        default:
            return nullptr;
    }
}
