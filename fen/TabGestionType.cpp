#include "TabGestionType.h"

using namespace fenMPS;

TabGestionType::TabGestionType(bmps::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd (bdd, pairIndex, parent) {
    widgetMPS::CodeWidget::Cases cases(NbrCase);
    cases.at(Attribuable) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Attribuable, bddMPS::code::Attribuable);
    cases.at(Visible) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Visible, bddMPS::code::Visible);
    // Widget
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_view = new widgetMPS::NodeView(std::make_unique<widgetMPS::RoundedArcPainter>());
    m_model = new modelMPS::type_permissionModel(static_cast<bddMPS::BddPredef &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegateMPS::CodeStandardNodeDelegate(cases,this));
    m_cibleListWidget = new QListWidget;
    numt num = 0;
    for (auto iter = m_model->idNomVec().cbegin(); iter != m_model->idNomVec().cend(); ++iter,++num) {
        auto item = new QListWidgetItem(iter->second,m_cibleListWidget);
        item->set_data(Qt::UserRole,num);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
    }
    m_cibleListWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    connect(m_cibleListWidget,&QListWidget::itemChanged,this,[this](QListWidgetItem *item)
        {m_model->set_cible(item->data(Qt::UserRole).toUInt(), item->checkState() == Qt::Checked);});
    connect(m_saveButton,&QPushButton::clicked,this,&TabGestionType::sauver);

    // Calque
    m_cibleLayout = new QVBoxLayout;
    m_cibleLayout->addWidget(m_cibleListWidget);
    m_cibleLayout->addWidget(m_saveButton);
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(m_cibleLayout);
}

void TabGestionType::become_current()
    {emit actionPermise(fenMPS::SauverAction);}
