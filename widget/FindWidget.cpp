#include "FindWidget.h"

using namespace widgetMPS;

FindWidget::FindWidget(FindModel *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_addButton = new QPushButton("+");
    m_delButton = new QPushButton("-");
    m_findButton = new QPushButton(tr("Chercher"));
    m_resetButton = new QPushButton(tr("Réinitialiser"));
    m_view = new QTreeView;
    setFindModel(model);
    auto * delegateOld = m_view->itemDelegate();
    m_view->setItemDelegate(new Delegate(this));
    delete delegateOld;
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
    connect(m_model,&modelMPS::FindModel::rowsInserted,m_view,&QTreeView::expand);

    // Calque
    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_findButton);
    m_buttonsLayout->addWidget(m_addButton);
    m_buttonsLayout->addWidget(m_delButton);
    m_buttonsLayout->addWidget(m_resetButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonsLayout);
}

void FindWidget::setFindModel(FindModel * model) {
    m_view->setModel(model);
    if(m_model) {
        m_addButton->disconnect(this);
        m_delButton->disconnect(this);
        m_findButton->disconnect(this);
        m_resetButton->disconnect(this);
        delete m_model;
    }
    m_model = model;
    if(m_model) {
        connect(m_addButton,&QPushButton::clicked,this,
                [this](){if(m_view->selectionModel()->hasSelection())
                            m_model->insertNode(0,m_view->selectionModel()->currentIndex().siblingAtColumn(0));});
        connect(m_delButton,&QPushButton::clicked,this,
                [this](){if(m_view->selectionModel()->hasSelection())
                            m_model->removeNode(m_view->selectionModel()->currentIndex().row(),
                                                m_view->selectionModel()->currentIndex().parent().siblingAtColumn(0));});
        connect(m_findButton,&QPushButton::clicked,m_model,&FindModel::find);
    }
    m_model->setParent(this);
}
