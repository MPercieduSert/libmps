#include "TabGestionBdd.h"

using namespace fenMPS;

TabGestionBdd::TabGestionBdd(b2d::Bdd &bdd, const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModuleWithBdd (bdd, pairIndex, parent),
      m_info(m_pairIndex.second > 0 ? m_bdd.managers().info(static_cast<entidt>(m_pairIndex.second))
                                  : m_bdd.managers().get(static_cast<entidt>(-m_pairIndex.second)).info_arbre()),
      m_Names_attributs( m_pairIndex.second > 0 ? m_bdd.managers().get(static_cast<entidt>(m_pairIndex.second)).names_attribut()
                                               : entities_base::arbre::Names_attribut())
{
    m_model = new QSqltable_model(this,m_bdd.db());
    m_model->setTable(m_info.table());
    m_model->setEditStrategy(QSqltable_model::OnManualSubmit);
    m_model->select();
    for(szt j = 0; j != m_Names_attributs.size(); ++j)
        m_model->set_headerData(static_cast<int>(j),Qt::Horizontal,m_Names_attributs[j]);
    m_view = new QTableView();
    m_view->setModel(m_model);
    m_refreshButton = new QPushButton(tr("Actualiser"));
    m_insertButton = new QPushButton(tr("Ajouter"));
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_supprButton = new QPushButton(tr("Supprimer"));
    connect(m_insertButton,&QPushButton::clicked,[this](){m_model->insertRow(m_model->row_count());});
    connect(m_saveButton,&QPushButton::clicked,m_model,&QSqltable_model::submitAll);
    connect(m_supprButton,&QPushButton::clicked,[this]()
        {
            auto indexList = m_view->selectionModel()->selectedRows();
            for(auto j = indexList.cbegin(); j != indexList.cend(); ++j)
                m_model->removeRow(j->row());
        });
    connect(m_refreshButton,&QPushButton::clicked,m_model,&QSqltable_model::submitAll);
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_refreshButton);
    buttonLayout->addWidget(m_insertButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_supprButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addLayout(buttonLayout);

    //vLayout->setContentsMargins(0,0,0,0);
}
