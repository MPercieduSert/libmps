#include "abstract_tab_zone_centrale.h"

using namespace mps;
using namespace fenetre;

abstract_tab_zone_centrale::abstract_tab_zone_centrale(abstract_tab *tab, QWidget *parent)
    : abstract_zone_centrale (parent), m_tab(tab) {
    connect(m_tab,&abstract_tab::action_permise,this,&abstract_tab_zone_centrale::action_permise);
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_tab);
}
