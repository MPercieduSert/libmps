#include "AbstractTabZoneCentrale.h"

using namespace fenMPS;

AbstractTabZoneCentrale::AbstractTabZoneCentrale(AbstractTab *tab, QWidget *parent)
    : AbstractZoneCentrale (parent), m_tab(tab) {
    connect(m_tab,&AbstractTab::actionPermise,this,&AbstractTabZoneCentrale::actionPermise);
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_tab);
}
