#include "AbstractEntitySelectWidget.h"

using namespace widgetMPS;

AbstractEntitySelectWidget::AbstractEntitySelectWidget(bddMPS::Bdd &bdd, Qt::Orientations orientation, QWidget * parent)
    : QWidget(parent), m_bdd(bdd) {
    if(orientation == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
}
