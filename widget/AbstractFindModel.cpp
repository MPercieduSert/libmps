#include "AbstractFindModel.h"

using namespace  delegateMPS;
using namespace modelMPS;

const std::array<QString,AbstractFindModel::NbrOperation> AbstractFindModel::OperationStrings = {"Et","Ou","Ou Exclusif"};

AbstractFindModel::AbstractNode::~AbstractNode() = default;

AbstractFindModel::AbstractFindModel(QObject *parent)
    :   AbstractTreeModel (parent),
      m_tree(this,true),
      m_header(NbrColumn) {
//    // Nombre de colonne.
//    m_nbrColumn[Operation] = OperationNbrCol;
//    m_nbrColumn[Booleen] = BooleenNbrCol;
//    m_nbrColumn[Constante] = ConstanteNbrCol;
//    m_nbrColumn[Condition] = ConditionNbrCol;
//    m_nbrColumn[Date] = DateNbrCol;
//    m_nbrColumn[DateTime] = DateTimeNbrCol;
//    m_nbrColumn[Double] = DoubleNbrCol;
//    m_nbrColumn[Entier] = EntierNbrCol;
//    m_nbrColumn[Texte] = TexteNbrCol;

    // Label des opérations.
//    m_operationStrings[Et] = QString(tr("Et"));
//    m_operationStrings[Ou] = QString(tr("Ou"));
//    m_operationStrings[OuExclusif] = QString(tr("Ou Exclusif"));

    // Entête
    m_header[NegColumn] = QString(tr("Négation"));
    m_header[OpColumn] = QString(tr("Opération"));
    m_header[TypeColumn] = QString(tr("Colonne"));

    // Arbre par défaut.
    auto racine = std::make_unique<ChoiceNode>();
    m_tree.setTree(Tree(std::move(racine)));
}

QVariant AbstractFindModel::data(const QModelIndex &index, int role) const {
    if(index.isValid())
        return m_tree.getValidData(index)->data(index.column(),role);
    return QVariant();
}

Qt::ItemFlags AbstractFindModel::flags(const QModelIndex & index) const {
    if(index.isValid())
        return m_tree.getValidData(index)->flags(index.column());
    return Qt::NoItemFlags;
}

bool AbstractFindModel::setData(const QModelIndex &ind, const QVariant &value, int role) {
    if(ind.isValid()) {
        if(role == Qt::EditRole && ind.column() == OpColumn
                && getData(ind).type() == Choice
                && value.toInt() >= 0 && value.toInt() < NbrOperation) {
            *m_tree.getValidIter(ind) = std::make_unique<OperationNode>(value.toUInt());
            dataChanged(index(ind.row(),0,ind.parent()),index(ind.row(),NbrColumn,ind.parent()));
            beginInsertRows(ind,0,1);
                m_tree.insertRows([](int,const QModelIndex &){return std::make_unique<ChoiceNode>();}
                                    ,0,2,ind);
            endInsertRows();
            return true;
        }
        if(m_tree.getValidData(ind)->setData(ind.column(),value,role)) {
            dataChanged(ind,ind);
            return true;
        }
    }
    return false;
}

///////////////////////////// AbstractNegationNode/////////////////////
QVariant AbstractNegationNode::data(int column, int role) const {
    if(column == Afm::NegColumn) {
        if(role == Qt::DisplayRole)
            return m_negation ? QString("Non") : QString("");
        if(role == Qt::EditRole)
            return m_negation;
    }
    return QVariant();
}
///////////////////////////// ChoiceNode //////////////////////////////
QVariant ChoiceNode::data(int column, int role) const {
    if(column == Afm::OpColumn || column == Afm::TypeColumn) {
        if(role == Qt::DisplayRole)
                return QString("?");
        if(role == Qt::EditRole)
            return -1;
    }
    return QVariant();
}
///////////////////////////// OperationNode ///////////////////////////
QVariant OperationNode::data(int column, int role) const {
    if(column == Afm::OpColumn) {
        if(role == Qt::DisplayRole)
                return  Afm::OperationStrings[m_operation];
        if(role == Qt::EditRole)
            return m_operation;
    }
    return AbstractNegationNode::data(column,role);
}
///////////////////////////// FindDelegate ////////////////////////////
QWidget * FindDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.column() == Afm::OpColumn) {
        auto * comboBox = new QComboBox(parent);
        for (szt i = 0; i != Afm::NbrOperation; ++i)
            comboBox->addItem(Afm::OperationStrings[i],i);
        return comboBox;
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

bool FindDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                        const QStyleOptionViewItem &option, const QModelIndex &index) {
    if(index.column() == Afm::NegColumn
            && event->type() == QEvent::MouseButtonPress
            && model->flags(index).testFlag(Qt::ItemIsEnabled)) {
        auto eventMouse = static_cast<QMouseEvent *>(event);
        if(eventMouse->button() == Qt::LeftButton)
            model->setData(index,!model->data(index,Qt::EditRole).toBool());
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void FindDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if(index.column() == Afm::OpColumn) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void FindDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if(index.column() == AbstractFindModel::OpColumn) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        model->setData(index,comboBox->currentIndex());
    }
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}
