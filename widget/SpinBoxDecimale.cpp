#include "SpinBoxDecimale.h"

SpinBoxDecimale::SpinBoxDecimale(int value, QWidget *parent): QAbstractSpinBox(parent)
{
    setReadOnly(true);
    setAlignment(Qt::AlignRight);
    setValue(value);
    printValue();
}

void SpinBoxDecimale::printValue()
    {lineEdit()->setText(QString::number(AttributDecimale::atome(m_indice)));}

void SpinBoxDecimale::setValue(int value)
{
    int i = AttributDecimale::indice(value);
    if(i != AttributDecimale::NotFind)
        m_indice = i;
    else
        throw std::invalid_argument(QString("La valeur donner en argument ne fait pas partie des valeurs autorisées").toStdString());
    printValue();
}

void SpinBoxDecimale::stepBy(int steps)
{
    if(0 <= m_indice + steps && m_indice + steps < AttributDecimale::NbrValues)
    {
        m_indice += steps;
        printValue();
    }
}

QAbstractSpinBox::StepEnabled SpinBoxDecimale::stepEnabled() const
    {return (StepUpEnabled| StepDownEnabled);}
