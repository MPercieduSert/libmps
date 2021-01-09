#include "FindWidget.h"

using namespace delegateMPS;
using namespace modelMPS;
using namespace findNodeModel;
using namespace widgetMPS;

/////////////////////////////////////////// FindWidget //////////////////////////////////////
FindWidget::FindWidget(FindModel *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_findButton = new QPushButton(tr("Chercher"));
    m_resetButton = new QPushButton(tr("Réinitialiser"));
    m_view = new NodeView(std::make_unique<widgetMPS::RoundedArcPainter>());
    setFindModel(model);
    m_view->setDelegate(new Delegate(this));

    // Calque
    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_findButton);
    m_buttonsLayout->addWidget(m_resetButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonsLayout);
}

void FindWidget::setFindModel(FindModel * model) {
    m_view->setModel(model);
    if(m_model) {
        m_findButton->disconnect(this);
        m_resetButton->disconnect(this);
        delete m_model;
    }
    m_model = model;
    if(m_model) {
        connect(m_resetButton,&QPushButton::clicked,m_model,&FindModel::reset);
        connect(m_findButton,&QPushButton::clicked,m_model,&FindModel::find);
    }
    m_model->setParent(this);
}
