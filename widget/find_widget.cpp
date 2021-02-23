#include "find_widget.h"

using namespace mps;
using namespace delegate;
using namespace model_base;
using namespace find_node_model;
using namespace widget;

/////////////////////////////////////////// find_widget //////////////////////////////////////
find_widget::find_widget(find_model *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_find_button = new QPushButton(tr("Chercher"));
    m_reset_button = new QPushButton(tr("Réinitialiser"));
    m_view = new node_view(std::make_unique<widget::rounded_arc_painter>());
    set_find_model(model);
    m_view->set_delegate(new node_delegate(this));

    // Calque
    m_button_layout = new QHBoxLayout;
    m_button_layout->addWidget(m_find_button);
    m_button_layout->addWidget(m_reset_button);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_button_layout);
}

void find_widget::set_find_model(find_model *model) {
    m_view->set_model(model);
    if(m_model) {
        m_find_button->disconnect(this);
        m_reset_button->disconnect(this);
        delete m_model;
    }
    m_model = model;
    if(m_model) {
        connect(m_reset_button,&QPushButton::clicked,m_model,&find_model::reset);
        connect(m_find_button,&QPushButton::clicked,m_model,&find_model::find);
    }
    m_model->setParent(this);
}
