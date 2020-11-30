#include "FindModel.h"

using namespace modelMPS;
using namespace findNodeModel;

FindNode::~FindNode() = default;

FindModel::FindModel(AbstractColonnesModel * model, QObject *parent)
    :   TreeNodeModel (true,parent), m_model(model)
    {m_data.setTree(Tree(std::make_unique<FindNode>(this,Et,ChoiceNodeType)));}

void FindModel::find(){
    if(m_model)
        m_model->find(this);
}

void FindModel::insertNode(const NodeIndex & parent, szt pos) {
    if(getData(parent).type() == OperationNodeType)
        insertNodes(parent,pos,1,ChoiceNodeType);
    else {
        auto node = std::move(m_data.getData(parent));
        m_data.getData(parent) = std::make_unique<FindNode>(this,Et,OperationNodeType);
        emit dataChanged(parent,TypeRole);
        beginInsertNodes(parent,0,1);
            m_data.tree().push_back(m_data.getIter(parent),std::move(node));
            m_data.tree().push_back(m_data.getIter(parent),static_cast<Node &&>(std::make_unique<FindNode>(this,Et,ChoiceNodeType)));
        endInsertNodes();
    }
}

QMap<QString,QVariant> FindModel::nomColonnes() const {
    if(m_model) {
        QMap<QString,QVariant> map;
        for (szt i = 0; i != m_model->nbrColonne(); ++i)
            map.insert(m_model->colonne(i).header(),i);
        return map;
    }
    else
        return QMap<QString,QVariant>();
}

TreeNodeModel::Node FindModel::nodeFactory(const NodeIndex & parent, szt pos, int type) {
    switch (type) {
    case ChoiceNodeType:
        return std::make_unique<FindNode>(this,0,ChoiceNodeType);
    case OperationNodeType:
        return std::make_unique<FindNode>(this,Et,OperationNodeType);
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
        return std::make_unique<FindNode>(this,Et,ChoiceNodeType);
    }
}

void FindModel::removeNode(const NodeIndex & node){
    if(node.isValid() && node.parent().isValid() && node.model() == this){
        if(childCount(node.parent()) == 2) {
            m_data.getValidData(node.parent()) = std::move(m_data.getValidData(index(node.parent(), 1 - node.position())));
            emit dataChanged(node.parent(),TypeRole);
            removeNodes(node.firstBrother(),2);
        }
        else
            removeNodes(node);
    }
}

void FindModel::reset() {
    beginResetModel();
        m_data.setTree(Tree(std::make_unique<FindNode>(this,Et,ChoiceNodeType)));
    endResetModel();
}

bool FindModel::setData(const NodeIndex &index, const QVariant &value, int role) {
    if(index.isValid()) {
        if(role == DataRole) {
            if(index.cible() == OpCible){
                if(getData(index).type() == ChoiceNodeType
                        && value.toInt() >= 0 && value.toInt() < NbrOperation) {
                    m_data.getValidData(index) = std::make_unique<FindNode>(this,value.toUInt(),OperationNodeType);
                    emit dataChanged(index.index(NodeCible),TypeRole);
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
                        static_cast<FindNode &>(getData(index)).setPos(value.toUInt());
                    emit dataChanged(index.index(NodeCible),TypeRole);
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
    auto & root = static_cast<const FindNode &>(**m_data.tree().cbegin().toFirstChild());
    return root.negation() ? !root.test(id,m_model)
                           : root.test(id,m_model);
}

bool FindModel::testTree(szt id) const{
    auto iter = m_data.tree().crbegin();
    iter.toFirstLeaf();
    auto test = true;
    while (!iter.parent().root()) {
        if(iter.leaf() && !static_cast<const FindNode &>(**iter).empty())
            test = static_cast<const FindNode &>(**iter).test(id,m_model);
        if(static_cast<const FindNode &>(**iter).negation())
            test = !test;
        if((test && static_cast<const FindNode &>(**iter.parent()).pos() == Ou)
                || (!test && static_cast<const FindNode &>(**iter.parent()).pos() == Et)){
            iter.toParent();
        }
        else
            --iter;
    }
    if(static_cast<const FindNode &>(**iter).negation())
        test = !test;
    return test;
}

///////////////////////////// FindNode//////////////////////
QVariant FindNode::data(int cible, int role, szt num) const {
    switch (cible) {
    case FindModel::ColonneCible:
        switch (role) {
        case DataRole:
            return type() == FindModel::ChoiceNodeType ? QVariant(Vide) : m_pos;
        case LabelRole:
            return "Colonne :";
        case ListOfValues:
            auto map = m_model->nomColonnes();
            if(type() == FindModel::ChoiceNodeType)
                map.insert("",Vide);
            return map;
        }
        break;
    case FindModel::NegCible:
        switch (role) {
        case CheckStateRole:
            return m_negation ? Qt::Checked : Qt::Unchecked;
        case LabelRole:
            return "Négation";
        }
        break;
    case FindModel::OpCible:
        switch (role) {
        case DataRole:
            return type() == FindModel::ChoiceNodeType ? QVariant(Vide) : m_pos;
        case LabelRole:
            return "Opération :";
        case ListOfValues:
            QMap<QString,QVariant> map;
            map.insert("Et",FindModel::Et);
            map.insert("Ou",FindModel::Ou);
            map.insert("Ou exclusif",FindModel::OuExclusif);
            if(type() == FindModel::ChoiceNodeType)
                map.insert("",Vide);
            return map;
        }
        break;
    case SubNodeCible:
        if(role == SubNodeRole) {
            if(num == FindModel::NegationPosition && type() != FindModel::ChoiceNodeType) {
                QList<QVariant> init;
                init.append(FindModel::NegCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
            if(num == FindModel::ColonnePosition && type() != FindModel::OperationNodeType) {
                QList<QVariant> init;
                init.append(FindModel::ColonneCible);
                init.append(0);
                init.append(ComboBoxSubNode);
                return init;
            }
            if((num == FindModel::OperationChoicePosition && type() == FindModel::ChoiceNodeType)
                    || (num == FindModel::OperationOperationPosition && type() == FindModel::OperationNodeType)) {
                QList<QVariant> init;
                init.append(FindModel::OpCible);
                init.append(0);
                init.append(ComboBoxSubNode);
                return init;
            }
        }
        break;
    }
    return AbstractNode::data(cible,role,num);
}

szt FindNode::dataCount(int cible) const {
    if(cible == SubNodeCible){
        switch (type()) {
        case FindModel::ChoiceNodeType:
            return FindModel::ChoiceDataCount;
        case FindModel::OperationNodeType:
            return FindModel::OperationDatacount;
        }
    }
    return AbstractNode::dataCount(cible);
}

flag FindNode::setData(int cible, const QVariant & value, int role, szt num) {
    switch (cible) {
    case FindModel::NegCible:
        if(role == CheckStateRole){
            m_negation = value.toBool();
            return CheckStateRole;
        }
        break;
    case FindModel::ColonneCible:
    case FindModel::OpCible:
        if(role == DataRole){
            m_pos = value.toUInt();
            return DataRole;
        }
        break;
    }
    return AbstractNode::setData(cible,value,role,num);
}
///////////////////////////// ComparaisonNode ///////////////////
QVariant ComparaisonNode::data(int cible, int role, szt num) const{
    if(cible == FindModel::ComparaisonCible){
        switch (role) {
        case DataRole:
            return m_comp;
        case LabelRole:
            return "Comparaison :";
        case ListOfValues:{
            QMap<QString,QVariant> map;
            map.insert("=",FindModel::Egal);
            map.insert("\u2260",FindModel::Different);
            map.insert("<",FindModel::Inferieure);
            map.insert(">",FindModel::Superieure);
            map.insert("\u2264",FindModel::InfEgal);
            map.insert("\u2265",FindModel::SupEgal);
            return map;
        }}
    }
    else if (cible == SubNodeCible && num == FindModel::ComparaisonPosition) {
        QList<QVariant> init;
        init.append(FindModel::ComparaisonCible);
        init.append(0);
        init.append(ComboBoxSubNode);
        return init;
    }
    return FindNode::data(cible,role,num);
}

flag ComparaisonNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::ComparaisonCible && role == DataRole) {
        m_comp = value.toUInt();
        return DataRole;
    }
    return FindNode::setData(cible,value,role,num);
}
////////////////////////////// BoolNode /////////////////////////////
QVariant BoolNode::data(int cible, int role, szt num) const {
    switch (cible) {
    case FindModel::TrueCible:
        if(role == CheckStateRole)
            return m_true ? Qt::Checked : Qt::Unchecked;
        if(role == LabelRole)
            return m_trueLabel;
        break;
    case FindModel::FalseCible:
        if(role == CheckStateRole)
            return m_false ? Qt::Checked : Qt::Unchecked;
        if(role == LabelRole)
            return m_falseLabel;
        break;
    case SubNodeCible:
        if(role == SubNodeRole){
            if(num == FindModel::TruePosition) {
                QList<QVariant> init;
                init.append(FindModel::TrueCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
            if(num == FindModel::FalsePosition) {
                QList<QVariant> init;
                init.append(FindModel::FalseCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
        }
        break;
    }
    return FindNode::data(cible,role,num);
}

szt BoolNode::dataCount(int cible) const {
    if(cible == SubNodeCible)
        return FindModel::BoolDataCount;
    return  FindNode::dataCount(cible);
}

flag BoolNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(role == CheckStateRole) {
        if(cible == FindModel::TrueCible){
            m_true = value.toBool();
            return CheckStateRole;
        }
        if(cible == FindModel::FalseCible){
            m_false = value.toBool();
            return CheckStateRole;
        }
    }
    return FindNode::setData(cible,value,role,num);
}
///////////////////////////// DateNode ///////////////////////////
QVariant DateNode::data(int cible, int role, szt num) const {
    if(cible == FindModel::DateCible) {
        if(role == LabelRole)
            return "Date :";
        if(role == DataRole)
            return m_date;
    }
    if(cible == SubNodeCible && role == SubNodeRole && num == FindModel::DatePosition) if(num == FindModel::TruePosition) {
        QList<QVariant> init;
        init.append(FindModel::DateCible);
        init.append(0);
        init.append(DateSubNode);
        return init;
    }
    return ComparaisonNode::data(cible,role,num);
}

szt DateNode::dataCount(int cible) const {
    if(cible == SubNodeCible)
        return FindModel::DateDataCount;
    return  ComparaisonNode::dataCount(cible);
}

flag DateNode::setData(int cible, const QVariant & value, int role, szt num) {
    if(cible == FindModel::DateCible && role == DataRole) {
        m_date = value.toDate();
        return DataRole;
    }
    return ComparaisonNode::setData(cible,value,role,num);
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
///////////////////////////// TexteNode ///////////////////////////
QVariant TexteNode::data(int cible, int role, szt num) const {
    switch (cible) {
    case FindModel::TexteCible:
        if(role == DisplayRole)
                return m_texte;
        break;
    case FindModel::CaseCible:
        if(role == CheckStateRole)
            return m_case ? Qt::Checked : Qt::Unchecked;
        break;
    case FindModel::RegexCible:
        if(role == CheckStateRole)
            return m_regex ? Qt::Checked : Qt::Unchecked;
        break;
    case SubNodeCible:
        if(role == SubNodeRole){
            switch (num) {
            case FindModel::TextePosition: {
                QList<QVariant> init;
                init.append(FindModel::TexteCible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
            case FindModel::CasePosition: {
                QList<QVariant> init;
                init.append(FindModel::CaseCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
            case FindModel::RegexPosition: {
                QList<QVariant> init;
                init.append(FindModel::RegexCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }}
        }
        break;
    }
    return FindNode::data(cible,role,num);
}

szt TexteNode::dataCount(int cible) const {
    if(cible == SubNodeCible)
        return FindModel::TexteDataCount;
    return  FindNode::dataCount(cible);
}

flag TexteNode::setData(int cible, const QVariant & value, int role, szt num) {
    switch (cible) {
    case FindModel::TexteCible:
        if(role == DisplayRole) {
                m_texte = value.toString();
                if(m_regex)
                    m_regular.setPattern(m_texte);
        }
        break;
    case FindModel::CaseCible:
        if(role == CheckStateRole)
            m_regular.setPatternOptions((m_case = value.toBool())? QRegularExpression::NoPatternOption
                                                                 : QRegularExpression::CaseInsensitiveOption);
        break;
    case FindModel::RegexCible:
        if(role == CheckStateRole
                && (m_regex = value.toBool()))
            m_regular.setPattern(m_texte);

        break;
    }
    return FindNode::setData(cible,value,role,num);
}

bool TexteNode::testValue(const QVariant & value) const {
    if(m_regex)
        return m_regular.match(value.toString(),0).hasMatch();
    else
        return value.toString().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
