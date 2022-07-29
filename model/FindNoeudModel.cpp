#include "FindNoeudModel.h"

using namespace model_base;
using namespace findNoeudModel;

const std::array<QString, Nbr_Operation> OperationNoeud::Strings = {"Et","Ou","Ou Exclusif"};
const std::array<QString, Nbr_Comparaison> AbstractComparaisonNoeud::Strings = {"=","\u2260","<",">","\u2264","\u2265"};

AbstractFindNoeud::~AbstractFindNoeud() = default;

FindNoeudModel::FindNoeudModel(QObject *parent)
    :   TreeNoeudModel (true,parent) {
    // Entête
    m_header.resize(NbrColumn);
    m_header[NegColumn] = QString(tr("Négation"));
    m_header[OpColumn] = QString(tr("Opération"));
    m_header[ColonneColumn] = QString(tr("Colonne"));

    // arbre par défaut.
    auto racine = std::make_unique<ChoiceNoeud>();
    m_data.set_tree(Tree(std::move(racine)));
}

void FindNoeudModel::find(){
    if(m_model)
        m_model->find(this);
}

void FindNoeudModel::insertNoeud(int row, const QModelIndex &parent) {
    if(get_data(parent).type() == OperationNoeudType)
        insertNoeuds(ChoiceNoeudType,row,1,parent);
    else {
        auto node = std::move(m_data.get_data(parent));
        m_data.get_data(parent) = std::make_unique<OperationNoeud>();
        emit dataChanged(parent,parent);
        beginInsertRows(parent,0,1);
            m_data.tree().push_back(m_data.get_iter(parent),std::move(node));
            m_data.tree().push_back(m_data.get_iter(parent),static_cast<Noeud &&>(std::make_unique<ChoiceNoeud>()));
        endInsertRows();
    }
}

std::vector<QString> FindNoeudModel::nom_colonnes() const {
    if(m_model) {
        std::vector<QString> vec(static_cast<szt>(m_model->columnCount()));
        for (szt i = 0; i != vec.size(); ++i)
            vec[i]=m_model->colonne(i).header();
        return vec;
    }
    else
        return std::vector<QString>();
}

TreeNoeudModel::Noeud FindNoeudModel::node_factory(int type, int row, const QModelIndex &parent) {
    switch (type) {
    case ChoiceNoeudType:
        return std::make_unique<ChoiceNoeud>();
    case OperationNoeudType:
        return std::make_unique<OperationNoeud>();
    }
    return TreeNoeudModel::node_factory(type,row,parent);
}

TreeNoeudModel::Noeud FindNoeudModel::node_condition_factory(szt pos){
    const auto &colonne = m_model->colonne(pos);
    switch (m_model->colonne(pos).type()) {
    case BoolNoeudType:
        {const auto &boolColonne = static_cast<const abstract_bool_colonne &>(colonne);
        return std::make_unique<BoolNoeud>(pos,colonne.header(),boolColonne.true_label(),boolColonne.false_label());}
    case DateNoeudType:
        return std::make_unique<DateNoeud>(pos,colonne.header());
    case TexteNoeudType:
        return std::make_unique<TexteNoeud>(pos,colonne.header());
    default:
        return std::make_unique<ChoiceNoeud>();
    }
}

void FindNoeudModel::removeNoeud(int row, const QModelIndex &parent){
    if(parent.is_valid() &&row >= 0 &&row < row_count(parent)){
        if(row_count(parent) == 2) {
            m_data.get_valid_data(parent) = std::move(m_data.get_valid_data(index(1 - row,0,parent)));
            remove_rows(0,2,parent);
            dataChanged(parent,parent.siblingAtColumn(NbrColumn));
        }
        else
            remove_rows(row,1,parent);
    }
}

bool FindNoeudModel::set_data(const QModelIndex &index, const QVariant &value, int role) {
    if(index.is_valid()) {
        if(role == Qt::EditRole) {
            if(index.column() == OpColumn){
                if(get_data(index).type() == ChoiceNoeudType
                        &&value.toInt() >= 0 &&value.toInt() < Nbr_Operation) {
                    m_data.get_valid_data(index) = std::make_unique<OperationNoeud>(value.toUInt());
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    insertNoeuds(ChoiceNoeudType,0,2,index);
                    return true;
                }
            }
            else if (index.column() == ColonneColumn) {
                if(get_data(index).type() != OperationNoeudType
                        &&value.toInt() >= 0 &&value.toInt() < m_model->columnCount()) {
                    if(get_data(index).type() != m_model->colonne(value.toUInt()).type())
                        m_data.get_valid_data(index) = node_condition_factory(value.toUInt());
                    else{
                        static_cast<AbstractConditionNoeud &>(get_data(index)).set_pos(value.toUInt());
                        static_cast<AbstractConditionNoeud &>(get_data(index)).setLabel(m_model->colonne(value.toUInt()).header());
                    }
                    dataChanged(index.siblingAtColumn(0),index.siblingAtColumn(NbrColumn));
                    return true;
                }

            }
        }
        return TreeNoeudModel::set_data(index,value,role);
    }
    return false;
}

void FindNoeudModel::setModel(abstract_colonnes_model *model)
    {m_model = model;}

bool FindNoeudModel::test_root(szt id) const{
    auto &root = static_cast<const AbstractConditionNoeud &>(**m_data.tree().cbegin().to_first());
    return root.negation() ? !root.test(id,m_model)
                           : root.test(id,m_model);
}

bool FindNoeudModel::test_tree(szt id) const{
    auto iter = m_data.tree().crbegin();
    iter.to_first_leaf();
    auto test = true;
    while (!iter.parent().root()) {
        if(iter.leaf())
            test = static_cast<const AbstractConditionNoeud &>(**iter).test(id,m_model);
        if(static_cast<const AbstractNegationNoeud &>(**iter).negation())
            test = !test;
        if((test &&static_cast<const OperationNoeud &>(**iter.parent()).operation() == Ou)
                || (!test &&static_cast<const OperationNoeud &>(**iter.parent()).operation() == Et)){
            iter.to_parent();
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
bool AbstractNegationNoeud::set_data(int column, const QVariant &value, int role) {
    if(column == NegColumn &&role == Qt::EditRole) {
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
bool AbstractComparaisonNoeud::set_data(int column, const QVariant &value, int role) {
    if(column == ComparaisonColumn &&role == Qt::EditRole) {
        m_comp = value.toUInt();
        return true;
    }
    return AbstractConditionNoeud::set_data(column,value,role);
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
bool AbstractConditionNoeud::set_data(int column, const QVariant &value, int role) {
    if(column == ColonneColumn &&role == Qt::EditRole) {
        m_pos = value.toUInt();
        return true;
    }
    return AbstractNegationNoeud::set_data(column,value,role);
}
bool AbstractConditionNoeud::test(szt id, abstract_colonnes_model *model) const
    {return test_value(model->colonne(m_pos).data_test(id));}
////////////////////////////// BoolNoeud /////////////////////////////
QVariant BoolNoeud::data(int column, int role) const {
    if(column == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_true_label;
    }
    else if(column == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return m_false_label;
    }
    return AbstractConditionNoeud::data(column,role);
}
bool BoolNoeud::set_data(int column, const QVariant &value, int role) {
    if(column == TrueColumn){
        if(role == Qt::CheckStateRole)
            return m_true = value.toBool();
    }
    else if(column == FalseColumn){
        if(role == Qt::CheckStateRole)
            return m_false = value.toBool();
    }
    return AbstractConditionNoeud::set_data(column,value,role);
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
    if(column == DateColumn &&(role == Qt::DisplayRole || role == Qt::EditRole))
        return m_date;
    return AbstractComparaisonNoeud::data(column,role);
}
bool DateNoeud::set_data(int column, const QVariant &value, int role) {
    if(column == DateColumn &&role == Qt::EditRole) {
        m_date = value.toDate();
        return true;
    }
    return AbstractComparaisonNoeud::set_data(column,value,role);
}
bool DateNoeud::test_value(const QVariant &value) const {
    switch (m_comp) {
    case Egal:
        return m_date == value.toDate();
    case Different:
        return m_date != value.toDate();
    case Inferieure:
        return m_date > value.toDate();
    case Superieure:
        return m_date < value.toDate();
    case Inf_Egal:
        return m_date >= value.toDate();
    case Sup_Egal:
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
bool OperationNoeud::set_data(int column, const QVariant &value, int role) {
    if(column == OpColumn &&role == Qt::EditRole) {
        m_operation = value.toUInt();
        return true;
    }
    return AbstractNegationNoeud::set_data(column,value,role);
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
bool TexteNoeud::set_data(int column, const QVariant &value, int role) {
    switch (column) {
    case TexteColumn:
        if(role == Qt::EditRole) {
                m_texte = value.to_string();
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
                &&(m_regex = value.toBool()))
            m_regular.setPattern(m_texte);

        break;
    }
    return AbstractConditionNoeud::set_data(column,value,role);
}
bool TexteNoeud::test_value(const QVariant &value) const {
    if(m_regex)
        return m_regular.match(value.to_string(),0).hasMatch();
    else
        return value.to_string().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
