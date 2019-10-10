#include "AbstractTab.h"

bool AbstractTab::openTab(const QPair<int, int> &pair)
{
    if(m_mapTab.contains(pair))
    {
        setCurrentWidget(m_mapTab[pair]);
        return true;
    }
    else
    {
        auto tab = createTab(pair);
        if(!tab)
            return false;
        m_mapTab[pair]=tab;
        connectTab(tab);
        QTabWidget::addTab(tab,tab->title());
        return true;
    }
}

void AbstractTab::connectTab(AbstractTabModule * tab) const
{
    connect(tab,&AbstractTabModule::effacerPermis,this,&AbstractTab::effacerPermis);
    connect(tab,&AbstractTabModule::collerPermis,this,&AbstractTab::collerPermis);
    connect(tab,&AbstractTabModule::copierPermis,this,&AbstractTab::copierPermis);
    connect(tab,&AbstractTabModule::couperPermis,this,&AbstractTab::couperPermis);
    connect(tab,&AbstractTabModule::savePermis,this,&AbstractTab::savePermis);
}

void AbstractTab::currentChange()
    {static_cast<AbstractTabModule *>(currentWidget())->becomeCurrent();}


void AbstractTab::removeTab(const QPair<int,int> & pair)
{
    if(m_mapTab.contains(pair))
    {
        QTabWidget::removeTab(indexOf(m_mapTab[pair]));
        m_mapTab.remove(pair);
    }
}
