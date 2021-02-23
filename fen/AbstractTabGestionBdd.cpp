#include "AbstractTabGestionBdd.h"

using namespace fenMPS;

AbstractTabGestionBdd::AbstractTabGestionBdd(QSqlDatabase &bdd, entidt nbr_entity,
                                             const std::pair<int, int> &pairIndex, QWidget *parent)
    : AbstractTabModule(pairIndex, parent),
      m_nbr_entity(nbr_entity),
      m_tables(nbr_entity) {
    m_listName = new QListWidget();
    m_stack = new QStackedWidget();
    //m_stack = new QStackedLayout();
    std::map<int,QString> nomAttributsBdd;
    QStringList nomarbre_attributs;
    nomarbre_attributs<<"Id"<<"Parent"<<"Feuille"<<"Num";
    for(auto i = m_tables.begin(); i != m_tables.end(); ++i) {
        i->model = new QSqltable_model(this,bdd);
        i->model->setTable(i->nomBdd);
        i->model->setEditStrategy(QSqltable_model::OnRowChange);
        i->model->select();
        for(int j = 0; j != i->nomAttributs.count(); ++j)
            i->model->set_headerData(j,Qt::Horizontal,i->nomAttributs.at(j));
        i->view = new QTableView();
        i->view->setModel(i->model);
        m_listName->addItem(i->nom);
        //m_stack->addWidget(i->view);
        QPushButton *insertButton = new QPushButton("Ajouter");
        QPushButton *supprButton = new QPushButton("Supprimer");
        connect(insertButton,&QPushButton::clicked,[i](){i->model->insertRow(i->model->row_count());});
        connect(supprButton,&QPushButton::clicked,[i]()
            {
                auto indexList = i->view->selectionModel()->selectedRows();
                for(auto j = indexList.cbegin(); j != indexList.cend(); ++j)
                    i->model->removeRow((*j).row());
            });
        auto *wdgt = new QWidget();
        auto *vLayout = new QVBoxLayout(wdgt);
        vLayout->addWidget(i->view);
        vLayout->addWidget(insertButton);
        vLayout->addWidget(supprButton);
        vLayout->setContentsMargins(0,0,0,0);
        m_stack->addWidget(wdgt);
    }
    connect(m_listName,SIGNAL(currentRowChanged(int)),m_stack,SLOT(setCurrentIndex(int)));
    m_splitter = new QSplitter();
    m_splitter->addWidget(m_listName);
    m_splitter->addWidget(m_stack);
    //m_splitter->addWidget(m_stack);
    /*m_secondLayout = new QHBoxLayout();
    m_secondLayout->addWidget(m_listName);
    m_secondLayout->addWidget(m_stack);*/
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_splitter);
}
