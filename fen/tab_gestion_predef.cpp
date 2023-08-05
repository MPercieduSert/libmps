#include "tab_gestion_predef.h"

using namespace mps;
using namespace fenetre;

/////////////////////////////////////////////////// tab_gestion_bdd ////////////////////////////////////////////////////
tab_gestion_bdd::tab_gestion_bdd(b2d::bdd &bdd, const tab_index &index, QWidget *parent)
    : abstract_tab_module_with_bdd (bdd, index, parent),
      m_info(m_index.second > 0 ? m_bdd.managers().info(static_cast<entidt>(m_index.second))
                                  : m_bdd.managers().get(static_cast<entidt>(-m_index.second)).info_arbre()),
      m_attribut_names( m_index.second > 0 ? m_bdd.managers().get(static_cast<entidt>(m_index.second)).attribut_names()
                                               : entities_base::arbre::Names())
{
    m_model = new QSqlTableModel(this,m_bdd.db());
    m_model->setTable(m_info.table());
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();
    for(szt j = 0; j != m_attribut_names.size(); ++j)
        m_model->setHeaderData(static_cast<int>(j),Qt::Horizontal,m_attribut_names[j]);
    m_view = new QTableView();
    m_view->setModel(m_model);
    m_refresh_bouton = new QPushButton(tr("Actualiser"));
    m_insert_bouton = new QPushButton(tr("Ajouter"));
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    m_suppr_bouton = new QPushButton(tr("Supprimer"));
    connect(m_insert_bouton,&QPushButton::clicked,this,[this](){m_model->insertRow(m_model->rowCount());});
    connect(m_save_bouton,&QPushButton::clicked,m_model,&QSqlTableModel::submitAll);
    connect(m_suppr_bouton,&QPushButton::clicked,this,[this]()
        {
            auto indexList = m_view->selectionModel()->selectedRows();
            for(auto j = indexList.cbegin(); j != indexList.cend(); ++j)
                m_model->removeRow(j->row());
        });
    connect(m_refresh_bouton,&QPushButton::clicked,m_model,&QSqlTableModel::submitAll);
    auto *button_layout = new QHBoxLayout;
    button_layout->addWidget(m_refresh_bouton);
    button_layout->addWidget(m_insert_bouton);
    button_layout->addWidget(m_save_bouton);
    button_layout->addWidget(m_suppr_bouton);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(button_layout);

    //vLayout->setContentsMargins(0,0,0,0);
}

/////////////////////////////////////////////////////// tab_gestion_type //////////////////////////////////////////////
tab_gestion_type::tab_gestion_type(b2d::bdd &bdd, const tab_index &index, QWidget *parent)
    : abstract_tab_module_with_bdd (bdd, index, parent) {
    widget::code_widget::option_code option;
    option.cases.resize(NbrCase);
    option.cases.at(Attribuable) = widget::code_widget::case_style(widget::code_widget::Attribuable, b2d::code::Attribuable);
    option.cases.at(Visible) = widget::code_widget::case_style(widget::code_widget::Visible, b2d::code::Visible);
    option.transition = [](flag code_value, flag code_case)-> flag {
        if(code_case == b2d::code::Visible)
            return code_value ? b2d::code::Interdit : b2d::code::Visible;
        return code_value == (b2d::code::Visible | b2d::code::Attribuable) ? b2d::code::Visible : b2d::code::Visible | b2d::code::Attribuable;
    };
    // Widget
    m_save_bouton = new QPushButton(tr("Sauvegarder"));
    m_view = new widget::node_view(std::make_unique<widget::rounded_arc_painter>());
    m_model = new model_base::type_permission_model(static_cast<b2d::bdd_predef &>(bdd),this);
    m_view->set_model(m_model);
    m_view->set_delegate(new delegate::standard_node_delegate(this));
    m_cible_label = new QLabel(tr("Cible associées au type sélectionné :"));
    m_cible_view = new widget::node_view;
    m_interface = new model_base::cible_permission_interface_model(m_model,this);
    m_cible_view->set_model(m_interface);
    m_cible_view->set_delegate(new delegate::code_standard_node_delegate(option,this));

    connect(m_view->selection_model(),&model_base::node_selection_model::current_changed,
            m_interface,&model_base::cible_permission_interface_model::set_current);
    connect(m_save_bouton,&QPushButton::clicked,this,&tab_gestion_type::sauver);

    // Calque
    m_cible_layout = new QVBoxLayout;
    m_cible_layout->addWidget(m_cible_label);
    m_cible_layout->addWidget(m_cible_view);
    m_cible_layout->addWidget(m_save_bouton);
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_view);
    m_main_layout->addLayout(m_cible_layout);
}

void tab_gestion_type::become_current()
    {emit action_permise(fenetre::Sauver_Action);}

void tab_gestion_type::set_current_permission(const mps::model_base::node_index &/*index*/) {

//    for (auto it = m_model->id_nom_vec().cbegin(); it != m_model->id_nom_vec().cend(); ++it) {
//        auto item = new QListWidgetItem(it->second,m_cible_list_widget);
//        item->setData(Qt::UserRole,it->first);
//        item->setFlags(Qt::ItemIsUserCheckable);
//        item->setCheckState(Qt::Unchecked);
//    }
}
