#include "abstract_tab_menu.h"

using namespace mps;
using namespace fenetre;

abstract_tab_menu::abstract_tab_menu(const tab_index &index, QWidget *parent)
    : abstract_tab_module(index,parent) {
        m_bdd_bouton = new QPushButton(tr("Base de Données"));
        m_mot_cle_bouton = new QPushButton(tr("Mots Clés"));
        m_main_layout = new QHBoxLayout(this);
        m_main_layout->addWidget(m_mot_cle_bouton);
        m_main_layout->addWidget(m_bdd_bouton);
        connect(m_bdd_bouton,&QPushButton::clicked,
                [this]{emit this->open_tab_requested(tab_index(Bdd_Tab_Index,Single_Tab));});
        connect(m_mot_cle_bouton,&QPushButton::clicked,
                [this]{emit this->open_tab_requested(tab_index(Mot_Cle_Tab_Index,Single_Tab));});
}
