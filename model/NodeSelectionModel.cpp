#include "NodeSelectionModel.h"

using namespace modelMPS;

NodeSelectionModel::NodeSelectionModel(AbstractNodeModel * model, QObject * parent)
    : QObject(parent), m_model(model){

}

void NodeSelectionModel::setCurrentIndex(const NodeIndex & index, flag selectionFlag) {
    if (m_model == index.model() && index.internalPointer()) {
        if(selectionFlag.test(Select) && index.flags().test(SelectableFlagNode)) {
            auto old = m_currentIndex;
            m_currentIndex = index;
            m_hasSelection = true;
            emit currentChanged(m_currentIndex,old);
        }
        if(selectionFlag.test(Deselect) && m_currentIndex == index) {
            auto old = m_currentIndex;
            m_currentIndex = NodeIndex();
            m_hasSelection = false;
            emit currentChanged(m_currentIndex,old);
        }
        if(selectionFlag.test(Toggle)) {
            if(m_currentIndex == index)
                setCurrentIndex(index,Deselect);
            else
                setCurrentIndex(index,Select);
        }
    }
}
