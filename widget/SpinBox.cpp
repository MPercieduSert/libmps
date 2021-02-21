#include "SpinBox.h"

using namespace widgetMPS;

//////////////////////////////////////////// SpinBoxAnnee //////////////////////////////////////////
SpinBoxAnnee::SpinBoxAnnee(int value, QWidget *parent): QAbstractSpinBox(parent), m_value(value) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    printValue();
}

void SpinBoxAnnee::printValue()
    {lineEdit()->set_text(QString::number(m_value)+"-"+QString::number(m_value+1));}

void SpinBoxAnnee::stepBy(int steps) {
    if(m_value + steps > 0) {
        m_value += steps;
        printValue();
    }
}

QAbstractSpinBox::StepEnabled SpinBoxAnnee::stepEnabled() const noexcept
    {return StepUpEnabled|StepDownEnabled;}

////////////////////////////////////////////// SpinBoxNumExclu /////////////////////////////////////////
SpinBoxNumExclu::SpinBoxNumExclu(const std::list<int> &liste, QWidget *parent)
    : QAbstractSpinBox(parent), m_list(liste) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    stepBy(1);
    printValue();
}

void SpinBoxNumExclu::printValue() {
    if(!m_minVisible &&m_value == m_minimum)
        lineEdit()->set_text(QString());
    else
        lineEdit()->set_text(QString::number(m_value + m_offset));
}

void SpinBoxNumExclu::setList(const std::list<int> &list) {
    m_list = list;
    set_value(m_value);
}

void SpinBoxNumExclu::set_value(int val) {
    if(val >= m_minimum) {
        m_value = val;
        if(std::find(m_list.cbegin(),m_list.cend(),val) == m_list.cend()) {
            printValue();
            emit valueChanged(m_value);
        }
        else
            stepBy(1);
    }
}

void SpinBoxNumExclu::stepBy(int steps) {
    if(steps > 0) {
        m_value += steps;
        while(std::find(m_list.cbegin(),m_list.cend(),m_value) != m_list.cend())
            ++m_value;
        printValue();
        emit valueChanged(m_value);
    }
    else if(steps < 0) {
        int value = m_value;
        value += steps;
        while(std::find(m_list.cbegin(),m_list.cend(),value) != m_list.cend())
            --value;
        if(value >= m_minimum) {
            m_value = value;
            printValue();
            emit valueChanged(m_value);
        }
    }
}

QAbstractSpinBox::StepEnabled SpinBoxNumExclu::stepEnabled() const
    {return (StepUpEnabled| StepDownEnabled);}
