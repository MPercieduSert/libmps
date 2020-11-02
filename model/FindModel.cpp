#include "FindModel.h"
#include "ColonnesForModel.h"

using namespace modelMPS;
using namespace findNodeModel;

const std::array<QString, NbrOperation> OperationNode::Strings = {"Et","Ou","Ou Exclusif"};
const std::array<QString, NbrComparaison> AbstractComparaisonNode::Strings = {"=","\u2260","<",">","\u2264","\u2265"};

AbstractFindNode::~AbstractFindNode() = default;

FindModel::FindModel(QObject *parent)
    :   TreeNodeModel (true,parent) {
    // Arbre par défaut.
    auto racine = std::make_unique<ChoiceNode>();
    m_data.setTree(Tree(std::move(racine)));
}

void FindModel::find(){
    if(m_model)
        m_model->find(this);
}

void FindModel::insertNode(int row, const NodeIndex & parent) {
    if(getData(parent).type() == OperationNodeType)
        insertNodes(ChoiceNodeType,row,1,parent);
    else {
        auto node = std::move(m_data.getData(parent));
        m_data.getData(parent) = std::make_unique<OperationNode>();
        emit dataChanged(parent);
        beginInsertRows(parent,0,1);
            m_data.tree().push_back(m_data.getIter(parent),std::move(node));
            m_data.tree().push_back(m_data.getIter(parent),static_cast<Node &&>(std::make_unique<ChoiceNode>()));
        endInsertRows();
    }
}

TreeNodeModel::Node FindModel::nodeFactory(int type, int row, const NodeIndex & parent) {
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

void FindModel::removeNode(int row, const NodeIndex & parent){
    if(parent.isValid() && row >= 0 && row < rowCount(parent)){
        if(rowCount(parent) == 2) {
            m_data.getValidData(parent) = std::move(m_data.getValidData(index(1 - row,parent)));
            removeRows(0,2,parent);
            emit dataChanged(parent);
        }
        else
            removeRows(row,1,parent);
    }
}

bool FindModel::setData(const NodeIndex &index, int type, const QVariant &value, int role, szt num) {
    if(index.isValid()) {
        if(role == Qt::EditRole) {
            if(type == OpColumn){
                if(getData(index).type() == ChoiceNodeType
                        && value.toInt() >= 0 && value.toInt() < NbrOperation) {
                    m_data.getValidData(index) = std::make_unique<OperationNode>(value.toUInt());
                    emit dataChanged(index);
                    insertNodes(ChoiceNodeType,0,2,index);
                    return true;
                }
            }
            else if (type == ColonneColumn) {
                if(getData(index).type() != OperationNodeType
                        && value.toInt() >= 0 && value.toInt() < m_model->columnCount()) {
                    if(getData(index).type() != m_model->colonne(value.toUInt()).type())
                        m_data.getValidData(index) = nodeConditionFactory(value.toUInt());
                    else{
                        static_cast<AbstractConditionNode &>(getData(index)).setPos(value.toUInt());
                        static_cast<AbstractConditionNode &>(getData(index)).setLabel(m_model->colonne(value.toUInt()).header());
                    }
                    emit dataChanged(index);
                    return true;
                }

            }
        }
        return TreeNodeModel::setData(index,type,value,role,num);
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
QVariant AbstractNegationNode::data(int type, int role, szt /*num*/) const {
    if(type == NegType) {
        if(role == Qt::DisplayRole)
            return m_negation;
        if(role == Qt::EditRole)
            return m_negation;
    }
    return QVariant();
}
bool AbstractNegationNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    if(type == NegType && role == Qt::EditRole) {
        m_negation = value.toBool();
        return true;
    }
    return false;
}
///////////////////////////// AbstractComparaisonNode ///////////////////
QVariant AbstractComparaisonNode::data(int type, int role, szt /*num*/) const{
    if(type == ComparaisonColumn){
        if(role == Qt::DisplayRole)
            return Strings[m_comp];
        if(role == Qt::EditRole)
            return m_comp;
        if(role == Qt::UserRole)
            return ComparaisonSet;
    }
    return AbstractConditionNode::data(type,role);
}
bool AbstractComparaisonNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    if(type == ComparaisonColumn && role == Qt::EditRole) {
        m_comp = value.toUInt();
        return true;
    }
    return AbstractConditionNode::setData(type,value,role);
}
///////////////////////////// AbstractConditionNode /////////////////////
QVariant AbstractConditionNode::data(int type, int role, szt /*num*/) const{
    if(type == ColonneColumn){
        if(role == Qt::DisplayRole)
            return m_label;
        if(role == Qt::EditRole)
            return m_pos;
    }
    return AbstractNegationNode::data(type,role);
}
bool AbstractConditionNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    if(type == ColonneColumn && role == Qt::EditRole) {
        m_pos = value.toUInt();
        return true;
    }
    return AbstractNegationNode::setData(type,value,role);
}
bool AbstractConditionNode::test(szt id, AbstractColonnesModel * model) const
    {return testValue(model->colonne(m_pos).dataTest(id));}
////////////////////////////// BoolNode /////////////////////////////
QVariant BoolNode::data(int type, int role, szt /*num*/) const {
    if(type == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_trueLabel;
    }
    else if(type == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_falseLabel;
    }
    return AbstractConditionNode::data(type,role);
}
bool BoolNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    if(type == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true = value.toBool();
    }
    else if(type == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false = value.toBool();
    }
    return AbstractConditionNode::setData(type,value,role);
}
///////////////////////////// ChoiceNode //////////////////////////////
QVariant ChoiceNode::data(int type, int role, szt /*num*/) const {
    if(type == OpColumn || type == ColonneColumn) {
        if(role == Qt::DisplayRole)
            return QString("?");
        if(role == Qt::EditRole)
            return -1;
    }
    return QVariant();
}
///////////////////////////// DateNode ///////////////////////////
QVariant DateNode::data(int type, int role, szt /*num*/) const {
    if(type == DateColumn && (role == Qt::DisplayRole || role == Qt::EditRole))
        return m_date;
    return AbstractComparaisonNode::data(type,role);
}
bool DateNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    if(type == DateColumn && role == Qt::EditRole) {
        m_date = value.toDate();
        return true;
    }
    return AbstractComparaisonNode::setData(type,value,role);
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
QVariant OperationNode::data(int type, int role, szt /*num*/) const {
    if(type == OpColumn) {
        if(role == Qt::DisplayRole)
                return  Strings[m_operation];
        if(role == Qt::EditRole)
            return m_operation;
    }
    return AbstractNegationNode::data(type,role);
}
bool OperationNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    if(type == OpColumn && role == Qt::EditRole) {
        m_operation = value.toUInt();
        return true;
    }
    return AbstractNegationNode::setData(type,value,role);
}
///////////////////////////// TexteNode ///////////////////////////
QVariant TexteNode::data(int type, int role, szt /*num*/) const {
    switch (type) {
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
    return AbstractConditionNode::data(type,role);
}
bool TexteNode::setData(int type, const QVariant & value, int role, szt /*num*/) {
    switch (type) {
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
    return AbstractConditionNode::setData(type,value,role);
}
bool TexteNode::testValue(const QVariant & value) const {
    if(m_regex)
        return m_regular.match(value.toString(),0).hasMatch();
    else
        return value.toString().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
