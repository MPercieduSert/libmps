#include "SpinBoxAnnee.h"

using namespace widgetMPS;

SpinBoxAnnee::SpinBoxAnnee(int value, QWidget *parent): QAbstractSpinBox(parent), m_value(value) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    printValue();
}

void SpinBoxAnnee::printValue()
    {lineEdit()->setText(QString::number(m_value)+"-"+QString::number(m_value+1));}

void SpinBoxAnnee::stepBy(int steps) {
    if(m_value + steps > 0) {
        m_value += steps;
        printValue();
    }
}

QAbstractSpinBox::StepEnabled SpinBoxAnnee::stepEnabled() const
    {return (StepUpEnabled| StepDownEnabled);}
