#include "AbstractTabZoneCentrale.h"

using namespace fenMPS;

AbstractTabZoneCentrale::AbstractTabZoneCentrale(AbstractTab * tab, QWidget *parent)
    : AbstractZoneCentrale (parent), m_tab(tab) {
    connect(m_tab,&AbstractTab::effacerPermis,this,&AbstractZoneCentrale::effacerPermis);
    connect(m_tab,&AbstractTab::collerPermis,this,&AbstractZoneCentrale::collerPermis);
    connect(m_tab,&AbstractTab::copierPermis,this,&AbstractZoneCentrale::copierPermis);
    connect(m_tab,&AbstractTab::couperPermis,this,&AbstractZoneCentrale::couperPermis);
    connect(m_tab,&AbstractTab::savePermis,this,&AbstractZoneCentrale::savePermis);

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_tab);
}
