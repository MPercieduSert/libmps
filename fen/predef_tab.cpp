#include "predef_tab.h"

using namespace mps;
using namespace fenetre;

abstract_tab_module *predef_tab::create_tab(const tab_index &pair, const std::vector<QVariant> &/*args*/) const {
    switch (pair.first) {
        case Gestion_Bdd_Tab_Id:
            return new tab_gestion_bdd(m_bdd, pair);
        case Gestion_Type_Tab_Id:
            return new tab_gestion_type(m_bdd, pair);
        //case abstract_tab_menu::Mot_Cle_Tab_Index:
        //    return new Tabmot_cle;
        default:
            return nullptr;
    }
}
