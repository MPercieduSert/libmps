#include "AbstractTabModule.h"

using namespace fenMPS;

AbstractTabModule::AbstractTabModule(const std::pair<int, int> &pairIndex, QWidget *parent)
    : QWidget(parent),
    m_pairIndex(pairIndex){}

void AbstractTabModule::become_current()
    {emit actionPermise(NoActions);}
