#include "AbstractFindModel.h"

using namespace  delegateMPS;
using namespace modelMPS;
using namespace findModelNode;

const std::array<QString, NbrOperation> OperationNode::OperationStrings = {"Et","Ou","Ou Exclusif"};

AbstractFindModel::AbstractNode::~AbstractNode() = default;

AbstractFindModel::AbstractFindModel(QObject *parent)
    :   TreeNodeModel (true,parent) {
    // Entête
    m_header.resize(NbrColumn);
    m_header[NegColumn] = QString(tr("Négation"));
    m_header[OpColumn] = QString(tr("Opération"));
    m_header[TypeColumn] = QString(tr("Colonne"));

    // Arbre par défaut.
    auto racine = std::make_unique<ChoiceNode>();
    m_data.setTree(Tree(std::move(racine)));
}

TreeNodeModel::Node AbstractFindModel::nodeFactory(int type, int row, const QModelIndex & parent) {
    switch (type) {
    case ChoiceNodeType:
        return std::make_unique<ChoiceNode>();
    }

    return TreeNodeModel::nodeFactory(type,row,parent);
}

bool AbstractFindModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(index.isValid()) {
        if(role == Qt::EditRole && index.column() == OpColumn
                && getData(index).type() == ChoiceNodeType
                && value.toInt() >= 0 && value.toInt() < NbrOperation) {
            *m_data.getValidIter(index) = std::make_unique<OperationNode>(value.toUInt());
            dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
            insertNodes(ChoiceNodeType,0,2,index);
            return true;
        }
        return TreeNodeModel::setData(index,value,role);
    }
    return false;
}

///////////////////////////// AbstractNegationNode/////////////////////
QVariant AbstractNegationNode::data(int column, int role) const {
    if(column == NegColumn) {
        if(role == Qt::DisplayRole)
            return m_negation ? QString("Non") : QString("");
        if(role == Qt::EditRole)
            return m_negation;
    }
    return QVariant();
}
///////////////////////////// ChoiceNode //////////////////////////////
QVariant ChoiceNode::data(int column, int role) const {
    if(column == OpColumn || column == TypeColumn) {
        if(role == Qt::DisplayRole)
                return QString("?");
        if(role == Qt::EditRole)
            return -1;
    }
    return QVariant();
}
///////////////////////////// OperationNode ///////////////////////////
QVariant OperationNode::data(int column, int role) const {
    if(column == OpColumn) {
        if(role == Qt::DisplayRole)
                return  OperationStrings[m_operation];
        if(role == Qt::EditRole)
            return m_operation;
    }
    return AbstractNegationNode::data(column,role);
}
///////////////////////////// FindDelegate ////////////////////////////
QWidget * FindDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.column() == OpColumn) {
        auto * comboBox = new QComboBox(parent);
        for (szt i = 0; i != NbrOperation; ++i)
            comboBox->addItem(OperationNode::OperationStrings[i],i);
        return comboBox;
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

bool FindDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                        const QStyleOptionViewItem &option, const QModelIndex &index) {
    if(index.column() == NegColumn
            && event->type() == QEvent::MouseButtonPress
            && model->flags(index).testFlag(Qt::ItemIsEnabled)) {
        auto eventMouse = static_cast<QMouseEvent *>(event);
        if(eventMouse->button() == Qt::LeftButton)
            model->setData(index,!model->data(index,Qt::EditRole).toBool());
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void FindDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if(index.column() == OpColumn) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void FindDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if(index.column() == OpColumn) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        model->setData(index,comboBox->currentIndex());
    }
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}
