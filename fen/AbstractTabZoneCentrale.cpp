#include "AbstractTabZoneCentrale.h"

using namespace fenMPS;

AbstractTabZoneCentrale::AbstractTabZoneCentrale(AbstractTab *tab, QWidget *parent)
    : AbstractZoneCentrale (parent), m_tab(tab) {
    connect(m_tab,&AbstractTab::actionPermise,this,&AbstractTabZoneCentrale::actionPermise);
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_tab);
}
