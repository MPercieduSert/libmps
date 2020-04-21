#include "AbstractTabModule.h"

using namespace fenMPS;

AbstractTabModule::AbstractTabModule(const std::pair<int, int> &pairIndex, QWidget *parent)
    : QWidget(parent),
    m_pairIndex(pairIndex){}

void AbstractTabModule::becomeCurrent() {
    emit effacerPermis(false);
    emit collerPermis(false);
    emit copierPermis(false);
    emit couperPermis(false);
    emit savePermis(false);
}
