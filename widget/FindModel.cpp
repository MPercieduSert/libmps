#include "FindModel.h"
#include "AbstractColonnesModel.h"

using namespace  delegateMPS;
using namespace modelMPS;
using namespace findNodeModel;

const std::array<QString, NbrOperation> OperationNode::Strings = {"Et","Ou","Ou Exclusif"};
const std::array<QString, NbrComparaison> AbstractComparaisonNode::Strings = {"=","\u2260","<",">","\u2264","\u2265"};

AbstractFindNode::~AbstractFindNode() = default;

FindModel::FindModel(QObject *parent)
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

void FindModel::find(){
    if(m_model)
        m_model->find(this);
}

void FindModel::insertNode(int row, const QModelIndex & parent) {
    if(getData(parent).type() == OperationNodeType)
        insertNodes(ChoiceNodeType,row,1,parent);
    else {
        auto node = std::move(m_data.getData(parent));
        m_data.getData(parent) = std::make_unique<OperationNode>();
        dataChanged(parent,parent);
        beginInsertRows(parent,0,1);
            m_data.tree().push_back(m_data.getIter(parent),std::move(node));
            m_data.tree().push_back(m_data.getIter(parent),std::make_unique<ChoiceNode>());
        endInsertRows();
    }
}

std::vector<QString> FindModel::nomColonnes() const {
    if(m_model) {
        std::vector<QString> vec(static_cast<szt>(m_model->columnCount()));
        for (szt i = 0; i != vec.size(); ++i)
            vec[i]=m_model->colonne(i).header();
        return vec;
    }
    else
        return std::vector<QString>();
}

TreeNodeModel::Node FindModel::nodeFactory(int type, int row, const QModelIndex & parent) {
    switch (type) {
    case ChoiceNodeType:
        return std::make_unique<ChoiceNode>();
    case OperationNodeType:
        return std::make_unique<OperationNode>();
    }
    return TreeNodeModel::nodeFactory(type,row,parent);
}

TreeNodeModel::Node FindModel::nodeConditionFactory(szt pos){
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

void FindModel::removeNode(int row, const QModelIndex & parent){
    if(parent.isValid() && row >= 0 && row < rowCount(parent)){
        if(rowCount(parent) == 2) {
            m_data.getValidData(parent) = std::move(m_data.getValidData(index(1 - row,0,parent)));
            removeRows(0,2,parent);
            dataChanged(parent,parent.siblingAtColumn(NbrColumn));
        }
        else
            removeRows(row,1,parent);
    }
}

bool FindModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(index.isValid()) {
        if(role == Qt::EditRole) {
            if(index.column() == OpColumn){
                if(getData(index).type() == ChoiceNodeType
                        && value.toInt() >= 0 && value.toInt() < NbrOperation) {
                    m_data.getValidData(index) = std::make_unique<OperationNode>(value.toUInt());
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    insertNodes(ChoiceNodeType,0,2,index);
                    return true;
                }
            }
            else if (index.column() == ColonneColumn) {
                if(getData(index).type() != OperationNodeType
                        && value.toInt() >= 0 && value.toInt() < m_model->columnCount()) {
                    if(getData(index).type() != m_model->colonne(value.toUInt()).type())
                        m_data.getValidData(index) = nodeConditionFactory(value.toUInt());
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

void FindModel::setModel(AbstractColonnesModel * model)
    {m_model = model;}

bool FindModel::testRoot(szt id) const{
    auto & root = static_cast<const AbstractConditionNode &>(**m_data.tree().cbegin().toFirstChild());
    return root.negation() ? !root.test(id,m_model)
                           : root.test(id,m_model);
}

bool FindModel::testTree(szt id) const{
    auto iter = m_data.tree().crbegin();
    iter.toFirstLeaf();
    auto test = true;
    while (!iter.parent().root()) {
        if(iter.leaf())
            test = static_cast<const AbstractConditionNode &>(**iter).test(id,m_model);
        if(static_cast<const AbstractNegationNode &>(**iter).negation())
            test = !test;
        if((test && static_cast<const OperationNode &>(**iter.parent()).operation() == Ou)
                || (!test && static_cast<const OperationNode &>(**iter.parent()).operation() == Et)){
            iter.toParent();
        }
        else
            --iter;
    }
    if(static_cast<const AbstractNegationNode &>(**iter).negation())
        test = !test;
    return test;
}

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
    if(column == ComparaisonColumn && role == Qt::EditRole) {
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
bool AbstractConditionNode::test(szt id, AbstractColonnesModel * model) const
    {return testValue(model->colonne(m_pos).dataTest(id));}
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
bool DateNode::testValue(const QVariant & value) const {
    switch (m_comp) {
    case Egal:
        return m_date == value.toDate();
    case Different:
        return m_date != value.toDate();
    case Inferieure:
        return m_date > value.toDate();
    case Superieure:
        return m_date < value.toDate();
    case InfEgal:
        return m_date >= value.toDate();
    case SupEgal:
        return m_date <= value.toDate();
    default:
        return false;
    }
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
        if(role == Qt::EditRole) {
                m_texte = value.toString();
                if(m_regex)
                    m_regular.setPattern(m_texte);
        }
        break;
    case CaseColumn:
        if(role == Qt::CheckStateRole)
            m_regular.setPatternOptions((m_case = value.toBool())? QRegularExpression::NoPatternOption
                                                                 : QRegularExpression::CaseInsensitiveOption);
        break;
    case RegexColumn:
        if(role == Qt::CheckStateRole
                && (m_regex = value.toBool()))
            m_regular.setPattern(m_texte);

        break;
    }
    return AbstractConditionNode::setData(column,value,role);
}
bool TexteNode::testValue(const QVariant & value) const {
    if(m_regex)
        return m_regular.match(value.toString(),0).hasMatch();
    else
        return value.toString().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
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
            auto vec = static_cast<const modelMPS::FindModel *>(index.model())->nomColonnes();
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
