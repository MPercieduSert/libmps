#include "abstract_tab_gestion_bdd.h"

using namespace mps;
using namespace fenetre;

abstract_tab_gestion_bdd::abstract_tab_gestion_bdd(QSqlDatabase &bd, entidt nbr_entity,
                                             const tab_index &index, QWidget *parent)
        : abstract_tab_module(index, parent),
        m_nbr_entity(nbr_entity),
        m_tables(nbr_entity) {
    m_name_list = new QListWidget();
    m_stack = new QStackedWidget();
    std::map<int,QString> attribut_bdd_noms;
    QStringList nomarbre_attributs;
    nomarbre_attributs<<"Id"<<"Parent"<<"Feuille"<<"Num";
    for(auto it = m_tables.begin(); it != m_tables.end(); ++it) {
        it->model = new QSqlTableModel(this,bd);
        it->model->setTable(it->bdd_nom);
        it->model->setEditStrategy(QSqlTableModel::OnRowChange);
        it->model->select();
        for(int j = 0; j != it->attribut_noms.count(); ++j)
            it->model->setHeaderData(j,Qt::Horizontal,it->attribut_noms.at(j));
        it->view = new QTableView();
        it->view->setModel(it->model);
        m_name_list->addItem(it->nom);
        QPushButton *inset_bouton = new QPushButton("Ajouter");
        QPushButton *suppr_bouton = new QPushButton("Supprimer");
        connect(inset_bouton,&QPushButton::clicked,[it](){it->model->insertRow(it->model->rowCount());});
        connect(suppr_bouton,&QPushButton::clicked,[it]() {
                auto index_list = it->view->selectionModel()->selectedRows();
                for(auto list_it = index_list.cbegin(); list_it != index_list.cend(); ++list_it)
                    it->model->removeRow((*list_it).row());
            });
        auto *wdgt = new QWidget();
        auto *v_layout = new QVBoxLayout(wdgt);
        v_layout->addWidget(it->view);
        v_layout->addWidget(inset_bouton);
        v_layout->addWidget(suppr_bouton);
        v_layout->setContentsMargins(0,0,0,0);
        m_stack->addWidget(wdgt);
    }
    connect(m_name_list,SIGNAL(currentRowChanged(int)),m_stack,SLOT(setCurrentIndex(int)));
    m_splitter = new QSplitter();
    m_splitter->addWidget(m_name_list);
    m_splitter->addWidget(m_stack);
    m_main_layout = new QHBoxLayout(this);
    m_main_layout->addWidget(m_splitter);
}
