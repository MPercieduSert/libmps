#include "SpinBoxNumExclu.h"

using namespace widgetMPS;

SpinBoxNumExclu::SpinBoxNumExclu(const QList<int> & liste, QWidget *parent)
    : QAbstractSpinBox(parent), m_value(-1), m_liste(liste)
{
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    stepBy(1);
    printValue();
}

void SpinBoxNumExclu::printValue()
{
    if(m_value == 0)
        lineEdit()->setText(QString());
    else
        lineEdit()->setText(QString::number(m_value));
}

void SpinBoxNumExclu::setListe(const QList<int> &liste)
{
    m_liste = liste;
    m_value = -1;
    stepBy(1);
    printValue();
}

void SpinBoxNumExclu::stepBy(int steps)
{
    if(steps > 0)
    {
        m_value += steps;
        while(m_liste.contains(m_value))
            ++m_value;
        printValue();
        emit valueChanged(m_value);
    }
    else if(steps < 0)
    {
        int value = m_value;
        value += steps;
        while(value >=0 && m_liste.contains(value))
            --value;
        if(value >= 0)
        {
            m_value = value;
            printValue();
            emit valueChanged(m_value);
        }
    }
}

QAbstractSpinBox::StepEnabled SpinBoxNumExclu::stepEnabled() const
    {return (StepUpEnabled| StepDownEnabled);}
