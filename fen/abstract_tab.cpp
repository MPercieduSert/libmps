#include "abstract_tab.h"

using namespace mps;
using namespace fenetre;

bool abstract_tab::open_tab(const tab_index &pair, const std::vector<QVariant> &args) {
    auto iter = m_map_tab.find(pair);
    if(iter != m_map_tab.cend()) {
        setCurrentWidget(iter->second);
        return true;
    }
    else {
        auto tab = create_tab(pair, args);
        if(!tab)
            return false;
        m_map_tab[pair]=tab;
        connect_tab(tab);
        QTabWidget::addTab(tab,tab->title());
        setCurrentWidget(tab);
        return true;
    }
}

void abstract_tab::connect_tab(abstract_tab_module *tab){
    connect(tab,&abstract_tab_module::action_permise,this,&abstract_tab::action_permise);
    connect(tab,&abstract_tab_module::open_tab_requested,this,[this](const tab_index &pair){open_tab(pair);});
}

void abstract_tab::current_change() {
    if(currentWidget())
        static_cast<abstract_tab_module *>(currentWidget())->become_current();
}

void abstract_tab::remove_tab(int index) {
    m_map_tab.erase(static_cast<abstract_tab_module*>(widget(index))->index());
    QTabWidget::removeTab(index);
}

void abstract_tab::remove_tab(const tab_index &pair) {
    auto iter = m_map_tab.find(pair);
    if(iter != m_map_tab.cend()) {
        QTabWidget::removeTab(indexOf(iter->second));
        m_map_tab.erase(iter);
    }
}
