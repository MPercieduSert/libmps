#include "TabGestionType.h"

using namespace fenMPS;

TabGestionType::TabGestionType(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget * parent)
    : AbstractTabModuleWithBdd (bdd, pairIndex, parent), m_cases(2) {
    m_cases.at(Attribuable) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Attribuable, bddMPS::code::Attribuable);
    m_cases.at(Visible) = widgetMPS::CodeWidget::caseStyle(widgetMPS::CodeWidget::Visible, bddMPS::code::Visible);
    // Button
    m_insertButton = new QPushButton("+");
    m_saveButton = new QPushButton(tr("Sauvegarder"));
    m_supprButton = new QPushButton("-");

    // Calque
    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_insertButton);
    m_buttonLayout->addWidget(m_supprButton);
    m_buttonLayout->addWidget(m_saveButton);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addLayout(m_buttonLayout);
}
