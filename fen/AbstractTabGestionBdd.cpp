#include "AbstractTabGestionBdd.h"

using namespace fenMPS;

AbstractTabGestionBdd::AbstractTabGestionBdd(QSqlDatabase & bdd, szt nbrEntity, const QString &title, QWidget *parent)
    : AbstractTabModule(parent), m_nbrEntity(nbrEntity), m_tables(nbrEntity) {
    m_listName = new QListWidget();
    m_stack = new QStackedWidget();
    //m_stack = new QStackedLayout();
    std::map<int,QString> nomAttributsBdd;
    QStringList nomArbreAttributs;
    nomArbreAttributs<<"Id"<<"Parent"<<"Feuille"<<"Num";
    for(auto i = m_tables.begin(); i != m_tables.end(); ++i) {
        i->model = new QSqlTableModel(this,bdd);
        i->model->setTable(i->nomBdd);
        i->model->setEditStrategy(QSqlTableModel::OnRowChange);
        i->model->select();
        for(int j = 0; j != i->nomAttributs.count(); ++j)
            i->model->setHeaderData(j,Qt::Horizontal,i->nomAttributs.at(j));
        i->view = new QTableView();
        i->view->setModel(i->model);
        m_listName->addItem(i->nom);
        //m_stack->addWidget(i->view);
        QPushButton * insertButton = new QPushButton("Ajouter");
        QPushButton * supprButton = new QPushButton("Supprimer");
        connect(insertButton,&QPushButton::clicked,[i](){i->model->insertRow(i->model->rowCount());});
        connect(supprButton,&QPushButton::clicked,[i]()
            {
                auto indexList = i->view->selectionModel()->selectedRows();
                for(auto j = indexList.cbegin(); j != indexList.cend(); ++j)
                    i->model->removeRow((*j).row());
            });
        auto * wdgt = new QWidget();
        auto * vLayout = new QVBoxLayout(wdgt);
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
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_splitter);
}
