#include "MotCleWidget.h"

using namespace widgetMPS;

MotCleWidget::MotCleWidget(bddMPS::BddPredef * bd, int cible, QWidget *parent) : QWidget(parent)
{
    m_afficheListMotCle = new QTextEdit();
    m_chercheMotCle = new QLineEdit();
    m_treeView = new QTreeView();
    m_model = new modelMPS::MotCleModel(bd,cible,this);
    m_treeView->setModel(m_model);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_afficheListMotCle);
    m_mainLayout->addWidget(m_chercheMotCle);
    m_mainLayout->addWidget(m_treeView);
}

void MotCleWidget::setIdSet(std::set<idt> idSet)
{
    m_model->setIdSet(idSet);
    //emit idChanged(id);
}
