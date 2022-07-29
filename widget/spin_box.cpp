#include "spin_box.h"

using namespace mps;
using namespace widget;

//////////////////////////////////////////// spin_box_annee //////////////////////////////////////////
spin_box_annee::spin_box_annee(int value, QWidget *parent): QAbstractSpinBox(parent), m_value(value) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    print_value();
}

void spin_box_annee::print_value()
    {lineEdit()->setText(QString::number(m_value)+"-"+QString::number(m_value+1));}

void spin_box_annee::stepBy(int steps) {
    if(m_value + steps > 0) {
        m_value += steps;
        print_value();
    }
}

////////////////////////////////////////////// spin_box_num_exclu /////////////////////////////////////////
spin_box_num_exclu::spin_box_num_exclu(const std::list<int> &liste, QWidget *parent)
    : QAbstractSpinBox(parent), m_list(liste) {
    setReadOnly(true);
    setAlignment(Qt::AlignHCenter);
    stepBy(1);
    print_value();
}

void spin_box_num_exclu::print_value() {
    if(!m_min_visible &&m_value == m_minimum)
        lineEdit()->setText(QString());
    else
        lineEdit()->setText(QString::number(m_value + m_offset));
}

void spin_box_num_exclu::set_list(const std::list<int> &list) {
    m_list = list;
    set_value(m_value);
}

void spin_box_num_exclu::set_value(int val) {
    if(val >= m_minimum) {
        m_value = val;
        if(std::find(m_list.cbegin(),m_list.cend(),val) == m_list.cend()) {
            print_value();
            emit value_changed(m_value);
        }
        else
            stepBy(1);
    }
}

void spin_box_num_exclu::stepBy(int steps) {
    if(steps > 0) {
        m_value += steps;
        while(std::find(m_list.cbegin(),m_list.cend(),m_value) != m_list.cend())
            ++m_value;
        print_value();
        emit value_changed(m_value);
    }
    else if(steps < 0) {
        int value = m_value;
        value += steps;
        while(std::find(m_list.cbegin(),m_list.cend(),value) != m_list.cend())
            --value;
        if(value >= m_minimum) {
            m_value = value;
            print_value();
            emit value_changed(m_value);
        }
    }
}
