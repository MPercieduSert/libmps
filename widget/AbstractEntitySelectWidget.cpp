#include "AbstractEntitySelectWidget.h"

using namespace widgetMPS;

AbstractEntitySelectWidget::AbstractEntitySelectWidget(bddMPS::Bdd &bdd, Qt::Orientations orientation, QWidget * parent)
    : QWidget(parent), m_bdd(bdd) {
    if(orientation == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0,0,0,0);
}

ComboBoxEntitySelectWidget::ComboBoxEntitySelectWidget(bddMPS::Bdd & bdd, const QString &label, Qt::Orientations orientation,
                                                       QWidget * parent)
    : AbstractEntitySelectWidget(bdd,orientation,parent) {
    m_label = new QLabel(label);
    m_box = new widgetMPS::IdComboBox;
    m_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_box->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(m_box,qOverload<int>(&QComboBox::currentIndexChanged),this,[this](){emit idChanged(id());});
    m_mainLayout->addWidget(m_label);
    m_mainLayout->addWidget(m_box);
}
