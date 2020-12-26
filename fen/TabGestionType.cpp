#include "TabGestionType.h"

using namespace fenMPS;

TabGestionType::TabGestionType(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget * parent)
    : AbstractTabModuleWithBdd (bdd, pairIndex, parent) {
    widgetMPS::CodeWidget::Cases cases(NbrCase);
    cases.at(Attribuable) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Attribuable, bddMPS::code::Attribuable);
    cases.at(Visible) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Visible, bddMPS::code::Visible);
    // Widget
//    m_insertButton = new QPushButton("+");
//    m_saveButton = new QPushButton(tr("Sauvegarder"));
//    m_supprButton = new QPushButton("-");
    m_view = new widgetMPS::NodeView(std::make_unique<widgetMPS::RoundedArcPainter>());
    m_model = new modelMPS::TypePermissionModel(static_cast<bddMPS::BddPredef &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegateMPS::CodeStandardNodeDelegate(cases,this));
    m_cibleListWidget = new QListWidget;
    numt num = 0;
    for (auto iter = m_model->idNomVec().cbegin(); iter != m_model->idNomVec().cend(); ++iter,++num) {
        auto item = new QListWidgetItem(iter->second,m_cibleListWidget);
        item->setData(Qt::UserRole,num);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
    }
    m_cibleListWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    connect(m_cibleListWidget,&QListWidget::itemChanged,this,[this](QListWidgetItem * item)
        {m_model->setCible(item->data(Qt::UserRole).toUInt(), item->checkState() == Qt::Checked);});

    // Calque
//    m_buttonLayout = new QHBoxLayout;
//    m_buttonLayout->addWidget(m_insertButton);
//    m_buttonLayout->addWidget(m_supprButton);
//    m_buttonLayout->addWidget(m_saveButton);
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_view);
//    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addWidget(m_cibleListWidget);
}
