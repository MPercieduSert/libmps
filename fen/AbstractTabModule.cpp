#include "AbstractTabModule.h"

using namespace fenMPS;

AbstractTabModule::AbstractTabModule(QWidget *parent)
    : QWidget(parent) {}

void AbstractTabModule::becomeCurrent() {
    emit collerPermis(false);
    emit copierPermis(false);
    emit couperPermis(false);
    emit savePermis(false);
}
