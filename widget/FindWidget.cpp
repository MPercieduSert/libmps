#include "FindWidget.h"

using namespace widgetMPS;

FindWidget::FindWidget(FindModel *model, QWidget *parent)
 : QWidget (parent){
    // Widget
    m_view = new QTreeView;
    setFindModel(model);
    auto * delegateOld = m_view->itemDelegate();
    m_view->setItemDelegate(new Delegate(this));
    delete delegateOld;
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
    connect(m_model,&modelMPS::AbstractFindModel::rowsInserted,m_view,&QTreeView::expand);
    m_addButton = new QPushButton("+");
    m_delButton = new QPushButton("-");
    m_resetButton = new QPushButton(tr("Réinitialiser"));

    // Calque
    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_addButton);
    m_buttonsLayout->addWidget(m_delButton);
    m_buttonsLayout->addWidget(m_resetButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_buttonsLayout);
}
