#include "AbstractFindModel.h"
#include "AbstractColonnesModel.h"

using namespace  delegateMPS;
using namespace modelMPS;
using namespace findNodeModel;

const std::array<QString, NbrOperation> OperationNode::Strings = {"Et","Ou","Ou Exclusif"};
const std::array<QString, NbrComparaison> AbstractComparaisonNode::Strings = {"=","\u2260","<",">","\u2264","\u2265"};

AbstractFindModel::AbstractNode::~AbstractNode() = default;

AbstractFindModel::AbstractFindModel(QObject *parent)
    :   TreeNodeModel (true,parent) {
    // Entête
    m_header.resize(NbrColumn);
    m_header[NegColumn] = QString(tr("Négation"));
    m_header[OpColumn] = QString(tr("Opération"));
    m_header[ColonneColumn] = QString(tr("Colonne"));

    // Arbre par défaut.
    auto racine = std::make_unique<ChoiceNode>();
    m_data.setTree(Tree(std::move(racine)));
}

std::vector<QString> AbstractFindModel::nomColonnes() const {
    if(m_model) {
        std::vector<QString> vec(static_cast<szt>(m_model->columnCount()));
        for (szt i = 0; i != vec.size(); ++i)
            vec[i]=m_model->colonne(i).header();
        return vec;
    }
    else
        return std::vector<QString>();
}

TreeNodeModel::Node AbstractFindModel::nodeFactory(int type, int row, const QModelIndex & parent) {
    switch (type) {
    case ChoiceNodeType:
        return std::make_unique<ChoiceNode>();
    case OperationNodeType:
        return std::make_unique<OperationNode>();
    }
    return TreeNodeModel::nodeFactory(type,row,parent);
}

TreeNodeModel::Node AbstractFindModel::nodeConditionFactory(szt pos){
    const auto & colonne = m_model->colonne(pos);
    switch (m_model->colonne(pos).type()) {
    case BoolNodeType:
        {const auto & boolColonne = static_cast<const AbstractBoolColonne &>(colonne);
        return std::make_unique<BoolNode>(pos,colonne.header(),boolColonne.trueLabel(),boolColonne.falseLabel());}
    case DateNodeType:
        return std::make_unique<DateNode>(pos,colonne.header());
    case TexteNodeType:
        return std::make_unique<TexteNode>(pos,colonne.header());
    default:
        return std::make_unique<ChoiceNode>();
    }
}

bool AbstractFindModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(index.isValid()) {
        if(role == Qt::EditRole) {
            if(index.column() == OpColumn){
                if(getData(index).type() == ChoiceNodeType
                        && value.toInt() >= 0 && value.toInt() < NbrOperation) {
                    *m_data.getValidIter(index) = std::make_unique<OperationNode>(value.toUInt());
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    insertNodes(ChoiceNodeType,0,2,index);
                    return true;
                }
            }
            else if (index.column() == ColonneColumn) {
                if(getData(index).type() != OperationNodeType
                        && value.toInt() >= 0 && value.toInt() < m_model->columnCount()) {
                    if(getData(index).type() != m_model->colonne(value.toUInt()).type())
                        *m_data.getValidIter(index) = nodeConditionFactory(value.toUInt());
                    else{
                        static_cast<AbstractConditionNode &>(getData(index)).setPos(value.toUInt());
                        static_cast<AbstractConditionNode &>(getData(index)).setLabel(m_model->colonne(value.toUInt()).header());
                    }
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    return true;
                }

            }
        }
        return TreeNodeModel::setData(index,value,role);
    }
    return false;
}

void AbstractFindModel::setModel(AbstractColonnesModel * model)
    {m_model = model;}

///////////////////////////// AbstractNegationNode//////////////////////
QVariant AbstractNegationNode::data(int column, int role) const {
    if(column == NegColumn) {
        if(role == Qt::DisplayRole)
            return m_negation ? QString("Non") : QString("");
        if(role == Qt::EditRole)
            return m_negation;
    }
    return QVariant();
}
bool AbstractNegationNode::setData(int column, const QVariant & value, int role) {
    if(column == NegColumn && role == Qt::EditRole) {
        m_negation = value.toBool();
        return true;
    }
    return false;
}
///////////////////////////// AbstractComparaisonNode ///////////////////
QVariant AbstractComparaisonNode::data(int column, int role) const{
    if(column == ComparaisonColumn){
        if(role == Qt::DisplayRole)
            return Strings[m_comp];
        if(role == Qt::EditRole)
            return m_comp;
        if(role == Qt::UserRole)
            return ComparaisonSet;
    }
    return AbstractConditionNode::data(column,role);
}
bool AbstractComparaisonNode::setData(int column, const QVariant & value, int role) {
    if(column == ColonneColumn && role == Qt::EditRole) {
        m_comp = value.toUInt();
        return true;
    }
    return AbstractConditionNode::setData(column,value,role);
}
///////////////////////////// AbstractConditionNode /////////////////////
QVariant AbstractConditionNode::data(int column, int role) const{
    if(column == ColonneColumn){
        if(role == Qt::DisplayRole)
            return m_label;
        if(role == Qt::EditRole)
            return m_pos;
    }
    return AbstractNegationNode::data(column,role);
}
bool AbstractConditionNode::setData(int column, const QVariant & value, int role) {
    if(column == ColonneColumn && role == Qt::EditRole) {
        m_pos = value.toUInt();
        return true;
    }
    return AbstractNegationNode::setData(column,value,role);
}
////////////////////////////// BoolNode /////////////////////////////
QVariant BoolNode::data(int column, int role) const {
    if(column == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_trueLabel;
    }
    else if(column == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_falseLabel;
    }
    return AbstractConditionNode::data(column,role);
}
bool BoolNode::setData(int column, const QVariant & value, int role) {
    if(column == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true = value.toBool();
    }
    else if(column == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false = value.toBool();
    }
    return AbstractConditionNode::setData(column,value,role);
}
///////////////////////////// ChoiceNode //////////////////////////////
QVariant ChoiceNode::data(int column, int role) const {
    if(column == OpColumn || column == ColonneColumn) {
        if(role == Qt::DisplayRole)
            return QString("?");
        if(role == Qt::EditRole)
            return -1;
    }
    return QVariant();
}
///////////////////////////// DateNode ///////////////////////////
QVariant DateNode::data(int column, int role) const {
    if(column == DateColumn && (role == Qt::DisplayRole || role == Qt::EditRole))
        return m_date;
    return AbstractComparaisonNode::data(column,role);
}
bool DateNode::setData(int column, const QVariant & value, int role) {
    if(column == DateColumn && role == Qt::EditRole) {
        m_date = value.toDate();
        return true;
    }
    return AbstractComparaisonNode::setData(column,value,role);
}
///////////////////////////// OperationNode ///////////////////////////
QVariant OperationNode::data(int column, int role) const {
    if(column == OpColumn) {
        if(role == Qt::DisplayRole)
                return  Strings[m_operation];
        if(role == Qt::EditRole)
            return m_operation;
    }
    return AbstractNegationNode::data(column,role);
}
bool OperationNode::setData(int column, const QVariant & value, int role) {
    if(column == OpColumn && role == Qt::EditRole) {
        m_operation = value.toUInt();
        return true;
    }
    return AbstractNegationNode::setData(column,value,role);
}
///////////////////////////// TexteNode ///////////////////////////
QVariant TexteNode::data(int column, int role) const {
    switch (column) {
    case TexteColumn:
        if(role == Qt::DisplayRole || role == Qt::EditRole)
                return m_texte;
        break;
    case CaseColumn:
        if(role == Qt::CheckStateRole)
            return m_case ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return "Sensible à la case";
        break;
    case RegexColumn:
        if(role == Qt::CheckStateRole)
            return m_regex ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return "Expression Régulière";
        break;
    }
    return AbstractConditionNode::data(column,role);
}
bool TexteNode::setData(int column, const QVariant & value, int role) {
    switch (column) {
    case TexteColumn:
        if(role == Qt::EditRole)
                m_texte = value.toString();
        break;
    case CaseColumn:
        if(role == Qt::CheckStateRole)
            return m_case = value.toBool();
        break;
    case RegexColumn:
        if(role == Qt::CheckStateRole)
            return m_regex = value.toBool();
        break;
    }
    return AbstractConditionNode::setData(column,value,role);
}
///////////////////////////// FindDelegate ////////////////////////////
QWidget * FindDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.isValid()) {
        if(index.column() == OpColumn) {
            auto * comboBox = new QComboBox(parent);
            for (szt i = 0; i != NbrOperation; ++i)
                comboBox->addItem(OperationNode::Strings[i],i);
            return comboBox;
        }
        if(index.column() == ColonneColumn) {
            auto * comboBox = new QComboBox(parent);
            auto vec = static_cast<const modelMPS::AbstractFindModel *>(index.model())->nomColonnes();
            for (szt i = 0; i != vec.size(); ++i)
                comboBox->addItem(vec[i],i);
            return comboBox;
        }
        if(index.column() == ComparaisonColumn && index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet) {
            auto * comboBox = new QComboBox(parent);
            for (szt i = 0; i != NbrComparaison; ++i)
                comboBox->addItem(AbstractComparaisonNode::Strings[i],i);
            return comboBox;
        }
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

bool FindDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                        const QStyleOptionViewItem &option, const QModelIndex &index) {
    if(index.isValid() && index.column() == NegColumn
            && event->type() == QEvent::MouseButtonPress
            && model->flags(index).testFlag(Qt::ItemIsEnabled)) {
        auto eventMouse = static_cast<QMouseEvent *>(event);
        if(eventMouse->button() == Qt::LeftButton) {
            model->setData(index,!model->data(index,Qt::EditRole).toBool());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void FindDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if(index.isValid() && (index.column() == OpColumn
                           || index.column() == ColonneColumn
                           || index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet)) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(index.model()->data(index,Qt::EditRole).toInt());
    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void FindDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if(index.isValid() && (index.column() == OpColumn
                           || index.column() == ColonneColumn
                           || index.model()->data(index,Qt::UserRole).toUInt() & ComparaisonSet)) {
        auto * comboBox = static_cast<QComboBox *>(editor);
        model->setData(index,comboBox->currentIndex());
    }
    else
        QStyledItemDelegate::setModelData(editor,model,index);
}
