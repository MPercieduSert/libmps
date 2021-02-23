#include "AbstractTabMenu.h"

using namespace fenMPS;

AbstractTabMenu::AbstractTabMenu(const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModule(pairIndex,parent) {
        m_bddButton = new QPushButton(tr("Base de Données"));
        m_motCleButton = new QPushButton(tr("Mots Clés"));
        m_main_layout = new QHBoxLayout(this);
        m_main_layout->addWidget(m_motCleButton);
        m_main_layout->addWidget(m_bddButton);
        connect(m_bddButton,&QPushButton::clicked,
                [this]{emit this->openTabRequested(std::pair<int,int>(BddTabIndex,SingleTab));});
        connect(m_motCleButton,&QPushButton::clicked,
                [this]{emit this->openTabRequested(std::pair<int,int>(mot_cleTabIndex,SingleTab));});
}
