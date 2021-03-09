#include "dialog_predef.h"

using namespace mps;
using namespace dialogue;

select_table_dialog::select_table_dialog(b2d::bdd::vector_id_name &&vec, QWidget *parent)
    :QDialog (parent) {
    // Widget
    m_label = new QLabel("Sélectionner la table à exporter.");
    m_list_widget = new QListWidget;
    for (auto iter = vec.cbegin(); iter != vec.cend(); ++iter) {
        auto item = new QListWidgetItem(iter->second);
        item->setData(Qt::UserRole,iter->first);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
        m_list_widget->addItem(item);
    }
    m_list_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(m_list_widget,&QListWidget::itemChanged,this,
            [this](QListWidgetItem *item) {
        if(item->checkState() == Qt::Checked)
            m_set.insert(item->data(Qt::UserRole).toUInt());
        else
            m_set.erase(item->data(Qt::UserRole).toUInt());
        m_valider_bouton->setDisabled(m_set.empty());
        });

    m_restriction_cb = new QCheckBox("Restrcition");
    m_restriction_cb->setCheckState(Qt::Checked);

    m_annuler_bouton = new QPushButton("Annuler");
    m_valider_bouton = new QPushButton("Choisir un fichier");
    m_valider_bouton->setDefault(true);
    m_valider_bouton->setDisabled(true);
    connect(m_annuler_bouton,&QPushButton::clicked,this,&select_table_dialog::reject);
    connect(m_valider_bouton,&QPushButton::clicked,this,&select_table_dialog::accept);

    // Calque
    m_bouton_layout = new QHBoxLayout;
    m_bouton_layout->addWidget(m_valider_bouton);
    m_bouton_layout->addWidget(m_annuler_bouton);
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->addWidget(m_label);
    m_main_layout->addWidget(m_list_widget);
    m_main_layout->addWidget(m_restriction_cb);
    m_main_layout->addLayout(m_bouton_layout);
}

flag select_table_dialog::option() const {
    flag op;
    if(m_restriction_cb->isChecked())
        op |= b2d::bdd::Restriction_Exportation_Xml;
    return op;
}