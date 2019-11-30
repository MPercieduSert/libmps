#include "AbstractTab.h"

using namespace fenMPS;

bool AbstractTab::openTab(const std::pair<int, int> &pair) {
    auto iter = m_mapTab.find(pair);
    if(iter != m_mapTab.cend()) {
        setCurrentWidget(iter->second);
        return true;
    }
    else {
        auto tab = createTab(pair);
        if(!tab)
            return false;
        m_mapTab[pair]=tab;
        connectTab(tab);
        QTabWidget::addTab(tab,tab->title());
        return true;
    }
}

void AbstractTab::connectTab(AbstractTabModule * tab) const {
    connect(tab,&AbstractTabModule::effacerPermis,this,&AbstractTab::effacerPermis);
    connect(tab,&AbstractTabModule::collerPermis,this,&AbstractTab::collerPermis);
    connect(tab,&AbstractTabModule::copierPermis,this,&AbstractTab::copierPermis);
    connect(tab,&AbstractTabModule::couperPermis,this,&AbstractTab::couperPermis);
    connect(tab,&AbstractTabModule::savePermis,this,&AbstractTab::savePermis);
}

void AbstractTab::currentChange()
    {static_cast<AbstractTabModule *>(currentWidget())->becomeCurrent();}


void AbstractTab::removeTab(const std::pair<int,int> & pair) {
    auto iter = m_mapTab.find(pair);
    if(iter != m_mapTab.cend()) {
        QTabWidget::removeTab(indexOf(iter->second));
        m_mapTab.erase(iter);
    }
}
