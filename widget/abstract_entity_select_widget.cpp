#include "abstract_entity_select_widget.h"

using namespace mps;
using namespace widget;

abstract_entity_select_widget::abstract_entity_select_widget(b2d::bdd &bdd, Qt::Orientations orientation, QWidget *parent)
    : QWidget(parent), m_bdd(bdd) {
    if(orientation == Qt::Horizontal)
        m_main_layout = new QHBoxLayout(this);
    else
        m_main_layout = new QVBoxLayout(this);
    m_main_layout->setContentsMargins(0,0,0,0);
}

combo_box_entity_select_widget::combo_box_entity_select_widget(b2d::bdd &bdd, const QString &label, Qt::Orientations orientation,
                                                       QWidget *parent)
    : abstract_entity_select_widget(bdd,orientation,parent) {
    m_label = new QLabel(label);
    m_box = new widget::id_combo_box;
    m_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_box->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_box,qOverload<int>(&QComboBox::currentIndexChanged),this,[this](){emit id_changed(id());});
    m_main_layout->addWidget(m_label);
    m_main_layout->addWidget(m_box);
}
