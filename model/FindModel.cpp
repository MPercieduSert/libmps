#include "FindModel.h"

using namespace modelMPS;
using namespace findNodeModel;

FindModel::FindModel(AbstractColonnesModel *model, QObject *parent)
    :   ItemNodeModel (parent), m_model(model)
    {m_data.setRoot(std::make_unique<FindNode>(this,FindNode::NoColonne,ChoiceNodeType));}

void FindModel::find(){
    if(m_model)
        m_model->find(this);
}

std::list<IterNode> FindModel::insert(const NodeIndex &parent, numt pos, numt count, int type) {
    if(checkIndex(parent)){
        if(getNode(parent).type() == OperationNodeType)
            return ItemNodeModel::insert(parent,pos,count,type);
        else if (count != 0 &&pos == 0) {
            Node node = m_data.moveNode(parent, std::make_unique<FindNode>(this,Et,OperationNodeType));
            emit dataChanged(parent,TypeRole);
            std::list<IterNode> list;
            beginInsertNodes(parent,0,1);
                list.push_back(m_data.push_front(parent,std::move(node)));
            endInsertNodes();
            list.splice(list.cend(),ItemNodeModel::insert(parent,0,count,type));
            return list;
        }
    }
    return std::list<IterNode>();
}

QMap<QString,QVariant> FindModel::nomColonnes() const {
    if(m_model) {
        QMap<QString,QVariant> map;
        for (numt i = 0; i != m_model->nbrColonne(); ++i)
            map.insert(m_model->colonne(i).header(),i);
        return map;
    }
    else
        return QMap<QString,QVariant>();
}

Node FindModel::nodeFactory(const NodeIndex &parent, numt pos, int type) {
    switch (type) {
    case ChoiceNodeType:
    case ItemNodeModel::DefaultType:
        return std::make_unique<FindNode>(this,FindNode::NoColonne,ChoiceNodeType);
    case OperationNodeType:
        return std::make_unique<FindNode>(this,Et,OperationNodeType);
    }
    return ItemNodeModel::nodeFactory(parent,pos,type);
}

Node FindModel::nodeConditionFactory(szt col){
    switch (m_model->colonne(col).type()) {
    case BoolNodeType: {
        const auto &colonne = m_model->colonne(col);
        const auto &boolColonne = static_cast<const AbstractBoolColonne &>(colonne);
        return std::make_unique<BoolNode>(this,col,boolColonne.falseLabel(),boolColonne.trueLabel());
    }
    case DateNodeType:
        return std::make_unique<DateNode>(this,col);
    case TexteNodeType:
        return std::make_unique<TexteNode>(this,col);
    default:
        return std::make_unique<FindNode>(this,FindNode::NoColonne,ChoiceNodeType);
    }
}

bool FindModel::remove(const NodeIndex &node, numt count){
    if(checkIndex(node) &&!node.isRoot()){
        if(node.parent().childCount() == count + 1) {
            if(indexToIterator(node).first())
                m_data.move(node.last(),node.parent());
            else
                m_data.move(node.first(),node.parent());
            emit dataChanged(node.parent(),TypeRole);
            return ItemNodeModel::remove(node.first(),count + 1);
        }
        else
            return ItemNodeModel::remove(node,count);
    }
    return false;
}

void FindModel::reset() {
    beginResetModel();
        m_data.setRoot(std::make_unique<FindNode>(this,FindNode::NoColonne,ChoiceNodeType));
    endResetModel();
}

bool FindModel::set(const NodeIndex &index, const QVariant &value, int role) {
    if(index.is_valid()) {
        if(index.cible() == ColonneCible &&value.toInt() >= 0 &&value.toInt() < m_model->columnCount()) {
            if(getNode(index).type() != m_model->colonne(value.toUInt()).type())
                m_data.setNode(index,nodeConditionFactory(value.toUInt()));
            else
                static_cast<FindNode &>(getNode(index)).setPos(value.toUInt());
            emit dataChanged(index.index(NodeCible),TypeRole);
            return true;
        }
        return ItemNodeModel::set(index,value,role);
    }
    return false;
}

void FindModel::setModel(AbstractColonnesModel *model) {
    beginResetModel();
        m_model = model;
    endResetModel();
}

bool FindModel::testRoot(szt id) const{
    auto &root = static_cast<const FindNode &>(m_data.getRoot());
    return root.negation() ? !root.test(id,m_model)
                           : root.test(id,m_model);
}

bool FindModel::testTree(szt id) const{
    auto iter = m_data.crbegin();
    iter.to_first_leaf();
    auto test = true;
    while (!iter.root()) {
        if(iter.leaf() &&!static_cast<const FindNode &>(**iter).empty())
            test = static_cast<const FindNode &>(**iter).test(id,m_model);
        if(static_cast<const FindNode &>(**iter).negation())
            test = !test;
        if((test &&static_cast<const FindNode &>(**iter.parent()).pos() == Ou)
                || (!test &&static_cast<const FindNode &>(**iter.parent()).pos() == Et)){
            iter.to_parent();
        }
        else
            --iter;
    }
    if(static_cast<const FindNode &>(**iter).negation())
        test = !test;
    return test;
}

///////////////////////////// FindNode//////////////////////
QVariant FindNode::data(int cible, int role, numt num) const {
    switch (cible) {
    case FindModel::ColonneCible:
        switch (role) {
        case IntRole:
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
        case IntRole:
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
            if(num == FindModel::Negationposition &&type() != FindModel::ChoiceNodeType) {
                QList<QVariant> init;
                init.append(FindModel::NegCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
            if(num == FindModel::Negationposition
                    || (num == FindModel::Colonneposition &&type() != FindModel::OperationNodeType)) {
                QList<QVariant> init;
                init.append(FindModel::ColonneCible);
                init.append(0);
                init.append(ComboBoxSubNode);
                return init;
            }
            if(num == FindModel::OperationOperationposition &&type() == FindModel::OperationNodeType) {
                QList<QVariant> init;
                init.append(FindModel::OpCible);
                init.append(0);
                init.append(ComboBoxSubNode);
                return init;
            }
        }
        break;
    }
    return ItemNode::data(cible,role,num);
}

numt FindNode::dataCount(int cible) const {
    if(cible == SubNodeCible){
        switch (type()) {
        case FindModel::ChoiceNodeType:
            return FindModel::ChoiceDataCount;
        case FindModel::OperationNodeType:
            return FindModel::OperationDatacount;
        }
    }
    return ItemNode::dataCount(cible);
}

flag FindNode::set_data(int cible, const QVariant &value, int role, numt num) {
    switch (cible) {
    case FindModel::NegCible:
        if(role == CheckStateRole){
            m_negation = value.toBool();
            return CheckStateRole;
        }
        break;
    case FindModel::ColonneCible:
    case FindModel::OpCible:
        if(role == IntRole){
            m_pos = value.toUInt();
            return IntRole;
        }
        break;
    }
    return ItemNode::set_data(cible,value,role,num);
}
///////////////////////////// ComparaisonNode ///////////////////
QVariant ComparaisonNode::data(int cible, int role, numt num) const{
    if(cible == FindModel::ComparaisonCible){
        switch (role) {
        case IntRole:
            return m_comp;
        case LabelRole:
            return "Comparaison :";
        case ListOfValues:{
            QMap<QString,QVariant> map;
            map.insert("=",FindModel::Egal);
            map.insert("\u2260",FindModel::Different);
            map.insert("<",FindModel::Inferieure);
            map.insert(">",FindModel::Superieure);
            map.insert("\u2264",FindModel::Inf_Egal);
            map.insert("\u2265",FindModel::Sup_Egal);
            return map;
        }}
    }
    else if (cible == SubNodeCible &&num == FindModel::Comparaisonposition) {
        QList<QVariant> init;
        init.append(FindModel::ComparaisonCible);
        init.append(0);
        init.append(ComboBoxSubNode);
        return init;
    }
    return FindNode::data(cible,role,num);
}

flag ComparaisonNode::set_data(int cible, const QVariant &value, int role, numt num) {
    if(cible == FindModel::ComparaisonCible &&role == NumRole) {
        m_comp = value.toUInt();
        return IntRole;
    }
    return FindNode::set_data(cible,value,role,num);
}
////////////////////////////// BoolNode /////////////////////////////
QVariant BoolNode::data(int cible, int role, numt num) const {
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
            if(num == FindModel::Trueposition) {
                QList<QVariant> init;
                init.append(FindModel::TrueCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
            if(num == FindModel::Falseposition) {
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

numt BoolNode::dataCount(int cible) const {
    if(cible == SubNodeCible)
        return FindModel::BoolDataCount;
    return  FindNode::dataCount(cible);
}

flag BoolNode::set_data(int cible, const QVariant &value, int role, numt num) {
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
    return FindNode::set_data(cible,value,role,num);
}
///////////////////////////// DateNode ///////////////////////////
QVariant DateNode::data(int cible, int role, numt num) const {
    if(cible == FindModel::DateCible) {
        if(role == LabelRole)
            return "Date :";
        if(role == DateRole)
            return m_date;
    }
    if(cible == SubNodeCible &&role == SubNodeRole &&num == FindModel::Dateposition) {
        QList<QVariant> init;
        init.append(FindModel::DateCible);
        init.append(0);
        init.append(DateSubNode);
        return init;
    }
    return ComparaisonNode::data(cible,role,num);
}

numt DateNode::dataCount(int cible) const {
    if(cible == SubNodeCible)
        return FindModel::DateDataCount;
    return  ComparaisonNode::dataCount(cible);
}

flag DateNode::set_data(int cible, const QVariant &value, int role, numt num) {
    if(cible == FindModel::DateCible &&role == DateRole) {
        m_date = value.toDate();
        return DateRole;
    }
    return ComparaisonNode::set_data(cible,value,role,num);
}

bool DateNode::testValue(const QVariant &value) const {
    switch (m_comp) {
    case FindModel::Egal:
        return m_date == value.toDate();
    case FindModel::Different:
        return m_date != value.toDate();
    case FindModel::Inferieure:
        return m_date > value.toDate();
    case FindModel::Superieure:
        return m_date < value.toDate();
    case FindModel::Inf_Egal:
        return m_date >= value.toDate();
    case FindModel::Sup_Egal:
        return m_date <= value.toDate();
    default:
        return false;
    }
}
///////////////////////////// TexteNode ///////////////////////////
QVariant TexteNode::data(int cible, int role, numt num) const {
    switch (cible) {
    case FindModel::texte_cible:
        if(role == StringRole)
            return m_texte;
        if(role == LabelRole)
            return "Texte :";
        break;
    case FindModel::CaseCible:
        if(role == CheckStateRole)
            return m_case ? Qt::Checked : Qt::Unchecked;
        if(role == LabelRole)
            return "Case";
        break;
    case FindModel::RegexCible:
        if(role == CheckStateRole)
            return m_regex ? Qt::Checked : Qt::Unchecked;
        if(role == LabelRole)
            return "Expression régulière";
        break;
    case SubNodeCible:
        if(role == SubNodeRole){
            switch (num) {
            case FindModel::Texteposition: {
                QList<QVariant> init;
                init.append(FindModel::texte_cible);
                init.append(0);
                init.append(LineEditSubNode);
                return init;
            }
            case FindModel::Caseposition: {
                QList<QVariant> init;
                init.append(FindModel::CaseCible);
                init.append(0);
                init.append(CheckSubNode);
                return init;
            }
            case FindModel::Regexposition: {
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

numt TexteNode::dataCount(int cible) const {
    if(cible == SubNodeCible)
        return FindModel::TexteDataCount;
    return  FindNode::dataCount(cible);
}

flag TexteNode::set_data(int cible, const QVariant &value, int role, numt num) {
    switch (cible) {
    case FindModel::texte_cible:
        if(role == StringRole) {
                m_texte = value.to_string();
                if(m_regex)
                    m_regular.setPattern(m_texte);
                return StringRole;
        }
        break;
    case FindModel::CaseCible:
        if(role == CheckStateRole) {
            m_regular.setPatternOptions((m_case = value.toBool())? QRegularExpression::NoPatternOption
                                                                 : QRegularExpression::CaseInsensitiveOption);
            return CheckStateRole;
        }
        break;
    case FindModel::RegexCible:
        if(role == CheckStateRole
                &&(m_regex = value.toBool())) {
            m_regular.setPattern(m_texte);
            return CheckStateRole;
        }
        break;
    }
    return FindNode::set_data(cible,value,role,num);
}

bool TexteNode::testValue(const QVariant &value) const {
    if(m_regex)
        return m_regular.match(value.to_string(),0).hasMatch();
    else
        return value.to_string().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
