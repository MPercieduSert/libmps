#include "TabGestionType.h"

using namespace fenMPS;

TabGestionType::TabGestionType(b2d::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd (bdd, pairIndex, parent) {
    widgetMPS::CodeWidget::Cases cases(NbrCase);
    cases.at(Attribuable) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Attribuable, b2d::code::Attribuable);
    cases.at(Visible) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Visible, b2d::code::Visible);
    // Widget
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_view = new widgetMPS::node_ptrView(std::make_unique<widgetMPS::RoundedArcPainter>());
    m_model = new model_base::type_permission_model(static_cast<b2d::bdd_predef &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegateMPS::CodeStandardnode_ptrDelegate(cases,this));
    m_cibleListWidget = new QListWidget;
    numt num = 0;
    for (auto iter = m_model->id_nom_vec().cbegin(); iter != m_model->id_nom_vec().cend(); ++iter,++num) {
        auto item = new QListWidgetItem(iter->second,m_cibleListWidget);
        item->set_data(Qt::UserRole,num);
        item->set_flags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
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
