#include "FindWidget.h"

using namespace delegateMPS;
using namespace model_base;
using namespace find_node_model;
using namespace widgetMPS;

/////////////////////////////////////////// FindWidget //////////////////////////////////////
FindWidget::FindWidget(find_model *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_findButton = new QPushButton(tr("Chercher"));
    m_resetButton = new QPushButton(tr("Réinitialiser"));
    m_view = new node_ptrView(std::make_unique<widgetMPS::RoundedArcPainter>());
    set_find_model(model);
    m_view->setDelegate(new Delegate(this));

    // Calque
    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_findButton);
    m_buttonsLayout->addWidget(m_resetButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonsLayout);
}

void FindWidget::set_find_model(find_model *model) {
    m_view->setModel(model);
    if(m_model) {
        m_findButton->disconnect(this);
        m_resetButton->disconnect(this);
        delete m_model;
    }
    m_model = model;
    if(m_model) {
        connect(m_resetButton,&QPushButton::clicked,m_model,&find_model::reset);
        connect(m_findButton,&QPushButton::clicked,m_model,&find_model::find);
    }
    m_model->set_parent(this);
}
