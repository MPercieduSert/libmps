#include "AbstractEntitySelectWidget.h"

using namespace widgetMPS;

AbstractEntitySelectWidget::AbstractEntitySelectWidget(Qt::Orientations orientation, QWidget * parent)
    : QWidget(parent) {
    if(orientation == Qt::Horizontal)
        m_mainLayout = new QHBoxLayout(this);
    else
        m_mainLayout = new QVBoxLayout(this);
}
