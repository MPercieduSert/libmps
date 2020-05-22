#include "TreeColonnesModel.h"

using namespace modelMPS;

TreeColonnesModel::TreeColonnesModel(bool racine, QObject *parent) : AbstractColonnesModel (parent), m_tree(this,racine){}
