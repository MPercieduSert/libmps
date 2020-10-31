#include "FindNoeudModel.h"
#include "ColonnesForModel.h"

using namespace modelMPS;
using namespace findNoeudModel;

const std::array<QString, NbrOperation> OperationNoeud::Strings = {"Et","Ou","Ou Exclusif"};
const std::array<QString, NbrComparaison> AbstractComparaisonNoeud::Strings = {"=","\u2260","<",">","\u2264","\u2265"};

AbstractFindNoeud::~AbstractFindNoeud() = default;

FindModel::FindModel(QObject *parent)
    :   TreeNoeudModel (true,parent) {
    // Entête
    m_header.resize(NbrColumn);
    m_header[NegColumn] = QString(tr("Négation"));
    m_header[OpColumn] = QString(tr("Opération"));
    m_header[ColonneColumn] = QString(tr("Colonne"));

    // Arbre par défaut.
    auto racine = std::make_unique<ChoiceNoeud>();
    m_data.setTree(Tree(std::move(racine)));
}

void FindModel::find(){
    if(m_model)
        m_model->find(this);
}

void FindModel::insertNoeud(int row, const QModelIndex & parent) {
    if(getData(parent).type() == OperationNoeudType)
        insertNoeuds(ChoiceNoeudType,row,1,parent);
    else {
        auto node = std::move(m_data.getData(parent));
        m_data.getData(parent) = std::make_unique<OperationNoeud>();
        dataChanged(parent,parent);
        beginInsertRows(parent,0,1);
            m_data.tree().push_back(m_data.getIter(parent),std::move(node));
            m_data.tree().push_back(m_data.getIter(parent),static_cast<Noeud &&>(std::make_unique<ChoiceNoeud>()));
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

TreeNoeudModel::Noeud FindModel::nodeFactory(int type, int row, const QModelIndex & parent) {
    switch (type) {
    case ChoiceNoeudType:
        return std::make_unique<ChoiceNoeud>();
    case OperationNoeudType:
        return std::make_unique<OperationNoeud>();
    }
    return TreeNoeudModel::nodeFactory(type,row,parent);
}

TreeNoeudModel::Noeud FindModel::nodeConditionFactory(szt pos){
    const auto & colonne = m_model->colonne(pos);
    switch (m_model->colonne(pos).type()) {
    case BoolNoeudType:
        {const auto & boolColonne = static_cast<const AbstractBoolColonne &>(colonne);
        return std::make_unique<BoolNoeud>(pos,colonne.header(),boolColonne.trueLabel(),boolColonne.falseLabel());}
    case DateNoeudType:
        return std::make_unique<DateNoeud>(pos,colonne.header());
    case TexteNoeudType:
        return std::make_unique<TexteNoeud>(pos,colonne.header());
    default:
        return std::make_unique<ChoiceNoeud>();
    }
}

void FindModel::removeNoeud(int row, const QModelIndex & parent){
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
                if(getData(index).type() == ChoiceNoeudType
                        && value.toInt() >= 0 && value.toInt() < NbrOperation) {
                    m_data.getValidData(index) = std::make_unique<OperationNoeud>(value.toUInt());
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    insertNoeuds(ChoiceNoeudType,0,2,index);
                    return true;
                }
            }
            else if (index.column() == ColonneColumn) {
                if(getData(index).type() != OperationNoeudType
                        && value.toInt() >= 0 && value.toInt() < m_model->columnCount()) {
                    if(getData(index).type() != m_model->colonne(value.toUInt()).type())
                        m_data.getValidData(index) = nodeConditionFactory(value.toUInt());
                    else{
                        static_cast<AbstractConditionNoeud &>(getData(index)).setPos(value.toUInt());
                        static_cast<AbstractConditionNoeud &>(getData(index)).setLabel(m_model->colonne(value.toUInt()).header());
                    }
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    return true;
                }

            }
        }
        return TreeNoeudModel::setData(index,value,role);
    }
    return false;
}

void FindModel::setModel(AbstractColonnesModel * model)
    {m_model = model;}

bool FindModel::testRoot(szt id) const{
    auto & root = static_cast<const AbstractConditionNoeud &>(**m_data.tree().cbegin().toFirstChild());
    return root.negation() ? !root.test(id,m_model)
                           : root.test(id,m_model);
}

bool FindModel::testTree(szt id) const{
    auto iter = m_data.tree().crbegin();
    iter.toFirstLeaf();
    auto test = true;
    while (!iter.parent().root()) {
        if(iter.leaf())
            test = static_cast<const AbstractConditionNoeud &>(**iter).test(id,m_model);
        if(static_cast<const AbstractNegationNoeud &>(**iter).negation())
            test = !test;
        if((test && static_cast<const OperationNoeud &>(**iter.parent()).operation() == Ou)
                || (!test && static_cast<const OperationNoeud &>(**iter.parent()).operation() == Et)){
            iter.toParent();
        }
        else
            --iter;
    }
    if(static_cast<const AbstractNegationNoeud &>(**iter).negation())
        test = !test;
    return test;
}

///////////////////////////// AbstractNegationNoeud//////////////////////
QVariant AbstractNegationNoeud::data(int column, int role) const {
    if(column == NegColumn) {
        if(role == Qt::DisplayRole)
            return m_negation ? QString("Non") : QString("");
        if(role == Qt::EditRole)
            return m_negation;
    }
    return QVariant();
}
bool AbstractNegationNoeud::setData(int column, const QVariant & value, int role) {
    if(column == NegColumn && role == Qt::EditRole) {
        m_negation = value.toBool();
        return true;
    }
    return false;
}
///////////////////////////// AbstractComparaisonNoeud ///////////////////
QVariant AbstractComparaisonNoeud::data(int column, int role) const{
    if(column == ComparaisonColumn){
        if(role == Qt::DisplayRole)
            return Strings[m_comp];
        if(role == Qt::EditRole)
            return m_comp;
        if(role == Qt::UserRole)
            return ComparaisonSet;
    }
    return AbstractConditionNoeud::data(column,role);
}
bool AbstractComparaisonNoeud::setData(int column, const QVariant & value, int role) {
    if(column == ComparaisonColumn && role == Qt::EditRole) {
        m_comp = value.toUInt();
        return true;
    }
    return AbstractConditionNoeud::setData(column,value,role);
}
///////////////////////////// AbstractConditionNoeud /////////////////////
QVariant AbstractConditionNoeud::data(int column, int role) const{
    if(column == ColonneColumn){
        if(role == Qt::DisplayRole)
            return m_label;
        if(role == Qt::EditRole)
            return m_pos;
    }
    return AbstractNegationNoeud::data(column,role);
}
bool AbstractConditionNoeud::setData(int column, const QVariant & value, int role) {
    if(column == ColonneColumn && role == Qt::EditRole) {
        m_pos = value.toUInt();
        return true;
    }
    return AbstractNegationNoeud::setData(column,value,role);
}
bool AbstractConditionNoeud::test(szt id, AbstractColonnesModel * model) const
    {return testValue(model->colonne(m_pos).dataTest(id));}
////////////////////////////// BoolNoeud /////////////////////////////
QVariant BoolNoeud::data(int column, int role) const {
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
    return AbstractConditionNoeud::data(column,role);
}
bool BoolNoeud::setData(int column, const QVariant & value, int role) {
    if(column == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true = value.toBool();
    }
    else if(column == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false = value.toBool();
    }
    return AbstractConditionNoeud::setData(column,value,role);
}
///////////////////////////// ChoiceNoeud //////////////////////////////
QVariant ChoiceNoeud::data(int column, int role) const {
    if(column == OpColumn || column == ColonneColumn) {
        if(role == Qt::DisplayRole)
            return QString("?");
        if(role == Qt::EditRole)
            return -1;
    }
    return QVariant();
}
///////////////////////////// DateNoeud ///////////////////////////
QVariant DateNoeud::data(int column, int role) const {
    if(column == DateColumn && (role == Qt::DisplayRole || role == Qt::EditRole))
        return m_date;
    return AbstractComparaisonNoeud::data(column,role);
}
bool DateNoeud::setData(int column, const QVariant & value, int role) {
    if(column == DateColumn && role == Qt::EditRole) {
        m_date = value.toDate();
        return true;
    }
    return AbstractComparaisonNoeud::setData(column,value,role);
}
bool DateNoeud::testValue(const QVariant & value) const {
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
///////////////////////////// OperationNoeud ///////////////////////////
QVariant OperationNoeud::data(int column, int role) const {
    if(column == OpColumn) {
        if(role == Qt::DisplayRole)
                return  Strings[m_operation];
        if(role == Qt::EditRole)
            return m_operation;
    }
    return AbstractNegationNoeud::data(column,role);
}
bool OperationNoeud::setData(int column, const QVariant & value, int role) {
    if(column == OpColumn && role == Qt::EditRole) {
        m_operation = value.toUInt();
        return true;
    }
    return AbstractNegationNoeud::setData(column,value,role);
}
///////////////////////////// TexteNoeud ///////////////////////////
QVariant TexteNoeud::data(int column, int role) const {
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
    return AbstractConditionNoeud::data(column,role);
}
bool TexteNoeud::setData(int column, const QVariant & value, int role) {
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
    return AbstractConditionNoeud::setData(column,value,role);
}
bool TexteNoeud::testValue(const QVariant & value) const {
    if(m_regex)
        return m_regular.match(value.toString(),0).hasMatch();
    else
        return value.toString().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
