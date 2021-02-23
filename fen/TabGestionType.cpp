#include "TabGestionType.h"

using namespace fenMPS;

TabGestionType::TabGestionType(b2d::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd (bdd, pairIndex, parent) {
    widget::code_widget::vec_option_case cases(NbrCase);
    cases.at(Attribuable) = widget::code_widget::case_style(widget::code_widget::Attribuable, b2d::code::Attribuable);
    cases.at(Visible) = widget::code_widget::case_style(widget::code_widget::Visible, b2d::code::Visible);
    // Widget
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_view = new widget::node_view(std::make_unique<widget::rounded_arc_painter>());
    m_model = new model_base::type_permission_model(static_cast<b2d::bdd_predef &>(bdd),this);
    m_view->setModel(m_model);
    m_view->setDelegate(new delegate::code_standard_node_delegate(cases,this));
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
    m_cible_layout = new QVBoxLayout;
    m_cible_layout->addWidget(m_cibleListWidget);
    m_cible_layout->addWidget(m_saveButton);
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_cible_layout);
}

void TabGestionType::become_current()
    {emit actionPermise(fenMPS::SauverAction);}
