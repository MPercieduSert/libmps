#include "AbstractTab.h"

using namespace fenMPS;

bool AbstractTab::openTab(const std::pair<int, int> &pair, const std::vector<QVariant> &args) {
    auto iter = m_mapTab.find(pair);
    if(iter != m_mapTab.cend()) {
        setCurrentWidget(iter->second);
        return true;
    }
    else {
        auto tab = createTab(pair, args);
        if(!tab)
            return false;
        m_mapTab[pair]=tab;
        connectTab(tab);
        QTabWidget::addTab(tab,tab->title());
        setCurrentWidget(tab);
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

void AbstractTab::currentChange() {
    if(currentWidget())
        static_cast<AbstractTabModule *>(currentWidget())->becomeCurrent();
}

void AbstractTab::removeTab(int index) {
    m_mapTab.erase(static_cast<AbstractTabModule*>(widget(index))->pairIndex());
    QTabWidget::removeTab(index);
}

void AbstractTab::removeTab(const std::pair<int,int> & pair) {
    auto iter = m_mapTab.find(pair);
    if(iter != m_mapTab.cend()) {
        QTabWidget::removeTab(indexOf(iter->second));
        m_mapTab.erase(iter);
    }
}
