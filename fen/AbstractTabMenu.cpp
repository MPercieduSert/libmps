#include "AbstractTabMenu.h"

using namespace fenMPS;

AbstractTabMenu::AbstractTabMenu(QWidget *parent)
    : AbstractTabModule(parent) {
        m_bddButton = new QPushButton(tr("Base de Données"));
        m_motCleButton = new QPushButton(tr("Mots Clés"));
        m_mainLayout = new QHBoxLayout(this);
        m_mainLayout->addWidget(m_motCleButton);
        m_mainLayout->addWidget(m_bddButton);
        connect(m_bddButton,&QPushButton::clicked,
                [this]{emit this->openTabRequested(std::pair<int,int>(BddTabIndex,SingleTab));});
        connect(m_motCleButton,&QPushButton::clicked,
                [this]{emit this->openTabRequested(std::pair<int,int>(MotCleTabIndex,SingleTab));});
}
