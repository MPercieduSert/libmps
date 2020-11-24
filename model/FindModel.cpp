#include "FindModel.h"

using namespace modelMPS;
using namespace findNodeModel;

const std::array<QString, FindModel::NbrOperation> OperationNode::Strings = {"Et","Ou","Ou Exclusif"};
const std::array<QString, FindModel::NbrComparaison> AbstractComparaisonNode::Strings = {"=","\u2260","<",">","\u2264","\u2265"};

AbstractFindNode::~AbstractFindNode() = default;

FindModel::FindModel(AbstractColonnesModel * model, QObject *parent)
    :   TreeNodeModel (true,parent), m_model(model)
    {m_data.setTree(Tree(std::make_unique<ChoiceNode>()));}

void FindModel::find(){
    if(m_model)
        m_model->find(this);
}

void FindModel::insertNode(const NodeIndex & parent, szt pos) {
    if(getData(parent).type() == OperationNodeType)
        insertNodes(parent,pos,1,ChoiceNodeType);
    else {
        auto node = std::move(m_data.getData(parent));
        m_data.getData(parent) = std::make_unique<OperationNode>();
        emit dataChanged(parent);
        beginInsertNodes(parent,0,1);
            m_data.tree().push_back(m_data.getIter(parent),std::move(node));
            m_data.tree().push_back(m_data.getIter(parent),static_cast<Node &&>(std::make_unique<ChoiceNode>()));
        endInsertNodes();
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

TreeNodeModel::Node FindModel::nodeFactory(const NodeIndex & parent, szt pos, int type) {
    switch (type) {
    case ChoiceNodeType:
        return std::make_unique<ChoiceNode>();
    case OperationNodeType:
        return std::make_unique<OperationNode>();
    }
    return TreeNodeModel::nodeFactory(parent,pos,type);
}

TreeNodeModel::Node FindModel::nodeConditionFactory(szt pos){
    const auto & colonne = m_model->colonne(pos);
    switch (m_model->colonne(pos).type()) {
    case BoolNodeType:
        {const auto & boolColonne = static_cast<const AbstractBoolColonne &>(colonne);
        return std::make_unique<BoolNode>(pos,boolColonne.falseLabel(),boolColonne.trueLabel());}
    case DateNodeType:
        return std::make_unique<DateNode>(pos);
    case TexteNodeType:
        return std::make_unique<TexteNode>(pos);
    default:
        return std::make_unique<ChoiceNode>();
    }
}

void FindModel::removeNode(const NodeIndex & node){
    if(node.isValid() && node.parent().isValid() && node.model() == this){
        if(childCount(node.parent()) == 2) {
            m_data.getValidData(node.parent()) = std::move(m_data.getValidData(index(node.parent(), 1 - node.position())));
            emit dataChanged(node.parent());
            removeNodes(node.firstBrother(),2);
        }
        else
            removeNodes(node);
    }
}

void FindModel::reset() {
    beginResetModel();
        m_data.setTree(Tree(std::make_unique<ChoiceNode>()));
    endResetModel();
}

bool FindModel::setData(const NodeIndex &index, const QVariant &value, int role) {
    if(index.isValid()) {
        if(role == DataRole) {
            if(index.cible() == OpCible){
                if(getData(index).type() == ChoiceNodeType
                        && value.toInt() >= 0 && value.toInt() < NbrOperation) {
                    m_data.getValidData(index) = std::make_unique<OperationNode>(value.toUInt());
                    emit dataChanged(index.index(NodeTypeCible));
                    insertNodes(index,0,2,ChoiceNodeType);
                    return true;
                }
            }
            else if (index.cible() == ColonneCible) {
                if(getData(index).type() != OperationNodeType
                        && value.toInt() >= 0 && value.toInt() < m_model->columnCount()) {
                    if(getData(index).type() != m_model->colonne(value.toUInt()).type())
                        m_data.getValidData(index) = nodeConditionFactory(value.toUInt());
                    else
                        static_cast<AbstractConditionNode &>(getData(index)).setPos(value.toUInt());
                    emit dataChanged(index.index(NodeTypeCible));
                    return true;
                }
            }
        }
        return TreeNodeModel::setData(index,value,role);
    }
    return false;
}

void FindModel::setModel(AbstractColonnesModel * model) {
    beginResetModel();
        m_model = model;
    endResetModel();
}

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
        if(iter.leaf() && !static_cast<const AbstractFindNode &>(**iter).empty())
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
QVariant AbstractNegationNode::data(int cible, int role, szt num) const {
    if(cible == FindModel::NegCible && role == DataRole)
            return m_negation;
    return AbstractFindNode::data(cible,role,num);
}
bool AbstractNegationNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::NegCible && role == DataRole) {
        m_negation = value.toBool();
        return true;
    }
    return AbstractFindNode::setData(cible,value,role,num);
}
///////////////////////////// AbstractComparaisonNode ///////////////////
QVariant AbstractComparaisonNode::data(int cible, int role, szt num) const{
    if(cible == FindModel::ComparaisonCible){
        if(role == DataRole)
            return m_comp;
        if(role == Qt::UserRole)
            return FindModel::ComparaisonSet;
    }
    return AbstractConditionNode::data(cible,role,num);
}
bool AbstractComparaisonNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::ComparaisonCible && role == DataRole) {
        m_comp = value.toUInt();
        return true;
    }
    return AbstractConditionNode::setData(cible,value,role,num);
}
///////////////////////////// AbstractConditionNode /////////////////////
QVariant AbstractConditionNode::data(int cible, int role, szt num) const{
    if(cible == FindModel::ColonneCible && role == DataRole)
        return m_pos;
    return AbstractNegationNode::data(cible,role,num);
}
bool AbstractConditionNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::ColonneCible && role == DataRole) {
        m_pos = value.toUInt();
        return true;
    }
    return AbstractNegationNode::setData(cible,value,role,num);
}
bool AbstractConditionNode::test(szt id, AbstractColonnesModel * model) const
    {return testValue(model->colonne(m_pos).dataTest(id));}
////////////////////////////// BoolNode /////////////////////////////
QVariant BoolNode::data(int cible, int role, szt num) const {
    if(cible == FindModel::TrueCible) {
        if(role == DataRole)
            return m_true;
        if(role == LabelRole)
            return m_trueLabel;
    }
    if(cible == FindModel::FalseCible) {
        if(role == DataRole)
            return m_false;
        if(role == LabelRole)
            return m_falseLabel;
    }
    return AbstractConditionNode::data(cible,role,num);
}
bool BoolNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::TrueCible && role == DataRole)
            return m_true = value.toBool();
    if(cible == FindModel::FalseCible && role == DataRole)
            return m_false = value.toBool();
    return AbstractConditionNode::setData(cible,value,role,num);
}
///////////////////////////// ChoiceNode //////////////////////////////
Qt::ItemFlags ChoiceNode::flags(int cible, szt /*num*/) const {
    if(cible == FindModel::OpCible || cible == FindModel::ColonneCible)
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    return Qt::NoItemFlags;
}

///////////////////////////// DateNode ///////////////////////////
QVariant DateNode::data(int cible, int role, szt num) const {
    if(cible == FindModel::DateCible && role == DataRole )
        return m_date;
    return AbstractComparaisonNode::data(cible,role,num);
}
bool DateNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::DateCible && role == DataRole) {
        m_date = value.toDate();
        return true;
    }
    return AbstractComparaisonNode::setData(cible,value,role,num);
}
bool DateNode::testValue(const QVariant & value) const {
    switch (m_comp) {
    case FindModel::Egal:
        return m_date == value.toDate();
    case FindModel::Different:
        return m_date != value.toDate();
    case FindModel::Inferieure:
        return m_date > value.toDate();
    case FindModel::Superieure:
        return m_date < value.toDate();
    case FindModel::InfEgal:
        return m_date >= value.toDate();
    case FindModel::SupEgal:
        return m_date <= value.toDate();
    default:
        return false;
    }
}
///////////////////////////// OperationNode ///////////////////////////
QVariant OperationNode::data(int cible, int role, szt num) const {
    if(cible == FindModel::OpCible && role == DataRole)
            return m_operation;
    return AbstractNegationNode::data(cible,role,num);
}
bool OperationNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::OpCible && role == DataRole) {
        m_operation = value.toUInt();
        return true;
    }
    return AbstractNegationNode::setData(cible,value,role,num);
}
///////////////////////////// TexteNode ///////////////////////////
QVariant TexteNode::data(int cible, int role, szt num) const {
    switch (cible) {
    case FindModel::TexteCible:
        if(role == DataRole)
                return m_texte;
        break;
    case FindModel::CaseCible:
        if(role == DataRole)
            return m_case;
        break;
    case FindModel::RegexCible:
        if(role == DataRole)
            return m_regex;
        break;
    }
    return AbstractConditionNode::data(cible,role,num);
}
bool TexteNode::setData(int cible, const QVariant & value, int role, szt num) {
    switch (cible) {
    case FindModel::TexteCible:
        if(role == DataRole) {
                m_texte = value.toString();
                if(m_regex)
                    m_regular.setPattern(m_texte);
        }
        break;
    case FindModel::CaseCible:
        if(role == DataRole)
            m_regular.setPatternOptions((m_case = value.toBool())? QRegularExpression::NoPatternOption
                                                                 : QRegularExpression::CaseInsensitiveOption);
        break;
    case FindModel::RegexCible:
        if(role == DataRole
                && (m_regex = value.toBool()))
            m_regular.setPattern(m_texte);

        break;
    }
    return AbstractConditionNode::setData(cible,value,role,num);
}
bool TexteNode::testValue(const QVariant & value) const {
    if(m_regex)
        return m_regular.match(value.toString(),0).hasMatch();
    else
        return value.toString().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
