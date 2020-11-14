#include "NodeSelectionModel.h"

using namespace modelMPS;

NodeSelectionModel::NodeSelectionModel(AbstractNodeModel * model, QObject * parent)
    : QObject(parent), m_model(model){

}
