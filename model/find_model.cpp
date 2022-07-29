#include "find_model.h"

using namespace mps;
using namespace model_base;
using namespace find_node_model;

find_model::find_model(abstract_colonnes_model *model, QObject *parent)
    : item_node_model(parent), m_model(model)
    {m_data.set_root(std::make_unique<find_node>(this,find_node::No_Colonne,Choice_Node_Type));}

void find_model::find(){
    if(m_model)
        m_model->find(this);
}

std::list<node_iter> find_model::insert(const node_index &parent, numt pos, numt count, int type) {
    if(check_index(parent)){
        if(get_node(parent).type() == Operation_Node_Type)
            return item_node_model::insert(parent,pos,count,type);
        else if (count != 0 &&pos == 0) {
            node_ptr node = m_data.move_node(parent, std::make_unique<find_node>(this,Et,Operation_Node_Type));
            emit data_changed(parent,Type_Change_Flag);
            std::list<node_iter> list;
            begin_insert_nodes(parent,0,1);
                list.push_back(m_data.push_front(parent,std::move(node)));
            end_insert_nodes();
            list.splice(list.cend(),item_node_model::insert(parent,0,count,type));
            return list;
        }
    }
    return std::list<node_iter>();
}

QMap<QString,QVariant> find_model::nom_colonnes() const {
    if(m_model) {
        QMap<QString,QVariant> map;
        for (numt i = 0; i != m_model->nbr_colonne(); ++i)
            map.insert(m_model->colonne(i).header(),i);
        return map;
    }
    else
        return QMap<QString,QVariant>();
}

node_ptr find_model::node_factory(const node_index &parent, numt pos, int type) {
    switch (type) {
    case Choice_Node_Type:
    case item_node_model::Default_Type:
        return std::make_unique<find_node>(this,find_node::No_Colonne,Choice_Node_Type);
    case Operation_Node_Type:
        return std::make_unique<find_node>(this,Et,Operation_Node_Type);
    }
    return item_node_model::node_factory(parent,pos,type);
}

node_ptr find_model::node_condition_factory(szt col){
    switch (m_model->colonne(col).type()) {
    case Bool_Node_Type: {
        const auto &colonne = m_model->colonne(col);
        const auto &boolColonne = static_cast<const abstract_bool_colonne &>(colonne);
        return std::make_unique<bool_node>(this,col,boolColonne.false_label(),boolColonne.true_label());
    }
    case Date_Node_Type:
        return std::make_unique<date_node>(this,col);
    case Texte_Node_Type:
        return std::make_unique<texte_node>(this,col);
    default:
        return std::make_unique<find_node>(this,find_node::No_Colonne,Choice_Node_Type);
    }
}

bool find_model::remove(const node_index &node, numt count){
    if(check_index(node) &&!node.is_root()){
        if(node.parent().child_count() == count + 1) {
            if(index_to_iterator(node).first())
                m_data.move(node.last_brother(),node.parent());
            else
                m_data.move(node.first_brother(),node.parent());
            emit data_changed(node.parent(),Type_Change_Flag);
            return item_node_model::remove(node.first_brother(),count + 1);
        }
        else
            return item_node_model::remove(node,count);
    }
    return false;
}

void find_model::reset() {
    begin_reset_model();
        m_data.set_root(std::make_unique<find_node>(this,find_node::No_Colonne,Choice_Node_Type));
    end_reset_model();
}

bool find_model::set(const node_index &index, const QVariant &value, int role) {
    if(index.is_valid()) {
        if(index.cible() == Colonne_Cible &&value.toInt() >= 0 &&value.toInt() < m_model->columnCount()) {
            if(get_node(index).type() != m_model->colonne(value.toUInt()).type())
                m_data.set_node(index,node_condition_factory(value.toUInt()));
            else
                static_cast<find_node &>(get_node(index)).set_pos(value.toUInt());
            emit data_changed(index.index(Node_Cible),Type_Change_Flag);
            return true;
        }
        return item_node_model::set(index,value,role);
    }
    return false;
}

void find_model::set_model(abstract_colonnes_model *model) {
    begin_reset_model();
        m_model = model;
    end_reset_model();
}

bool find_model::test_root(szt id) const{
    auto &root = static_cast<const find_node &>(m_data.get_root());
    return root.negation() ? !root.test(id,m_model)
                           : root.test(id,m_model);
}

bool find_model::test_tree(szt id) const{
    auto iter = m_data.crbegin();
    iter.to_first_leaf();
    auto test = true;
    while (!iter.root()) {
        if(iter.leaf() &&!static_cast<const find_node &>(**iter).empty())
            test = static_cast<const find_node &>(**iter).test(id,m_model);
        if(static_cast<const find_node &>(**iter).negation())
            test = !test;
        if((test &&static_cast<const find_node &>(**iter.parent()).pos() == Ou)
                || (!test &&static_cast<const find_node &>(**iter.parent()).pos() == Et)){
            iter.to_parent();
        }
        else
            --iter;
    }
    if(static_cast<const find_node &>(**iter).negation())
        test = !test;
    return test;
}

///////////////////////////// find_node//////////////////////
QVariant find_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case find_model::Colonne_Cible:
        switch (role) {
        case Variant_Role:
            return type() == find_model::Choice_Node_Type ? QVariant(Vide) : m_pos;
        case Label_Role:
            return "Colonne :";
        case Type_Role:
            return Combo_Box_Sub_Node;
        case Map_Role:
            auto map = m_model->nom_colonnes();
            if(type() == find_model::Choice_Node_Type)
                map.insert("",Vide);
            return map;
        }
        break;
    case find_model::Neg_Cible:
        switch (role) {
        case Check_State_Role:
            return m_negation ? Qt::Checked : Qt::Unchecked;
        case Label_Role:
            return "Négation";
        case Type_Role:
            return Check_Sub_Node;
        }
        break;
    case find_model::Op_Cible:
        switch (role) {
        case Variant_Role:
            return type() == find_model::Choice_Node_Type ? QVariant(Vide) : m_pos;
        case Label_Role:
            return "Opération :";
        case Type_Role:
            return Combo_Box_Sub_Node;
        case Map_Role:
            QMap<QString,QVariant> map;
            map.insert("Et",find_model::Et);
            map.insert("Ou",find_model::Ou);
            map.insert("Ou exclusif",find_model::Ou_Exclusif);
            if(type() == find_model::Choice_Node_Type)
                map.insert("",Vide);
            return map;
        }
        break;
    case Node_Cible:
        if(num == Node_Num && role == Cibles_Role){
            QList<QVariant> cibles;
            if(type() != find_model::Choice_Node_Type)
                cibles.append(find_model::Neg_Cible);
            if(type() == find_model::Operation_Node_Type)
                cibles.append(find_model::Op_Cible);
            else
                cibles.append(find_model::Colonne_Cible);
            return cibles;
        }
    }
    return item_node::data(cible,role,num);
}

flag find_node::set_data(int cible, const QVariant &value, int role, numt num) {
    switch (cible) {
    case find_model::Neg_Cible:
        if(role == Check_State_Role){
            m_negation = value.toBool();
            return Main_Same_Change_Flag;
        }
        break;
    case find_model::Colonne_Cible:
    case find_model::Op_Cible:
        if(role == Variant_Role){
            m_pos = value.toUInt();
            return Main_Same_Change_Flag;
        }
        break;
    }
    return item_node::set_data(cible,value,role,num);
}
///////////////////////////// comparaison_node ///////////////////
QVariant comparaison_node::data(int cible, int role, numt num) const{
    if(cible == find_model::Comparaison_Cible){
        switch (role) {
        case Variant_Role:
            return m_comp;
        case Label_Role:
            return "Comparaison :";
        case Type_Role:
            return Combo_Box_Sub_Node;
        case Map_Role:{
            QMap<QString,QVariant> map;
            map.insert("=",find_model::Egal);
            map.insert("\u2260",find_model::Different);
            map.insert("<",find_model::Inferieure);
            map.insert(">",find_model::Superieure);
            map.insert("\u2264",find_model::Inf_Egal);
            map.insert("\u2265",find_model::Sup_Egal);
            return map;
        }}
    }
    if(cible == Node_Cible && num == Node_Num && role == Cibles_Role)
        return find_node::data(cible,role,num).toList() << find_model::Comparaison_Cible;
    return find_node::data(cible,role,num);
}

flag comparaison_node::set_data(int cible, const QVariant &value, int role, numt num) {
    if(cible == find_model::Comparaison_Cible && role == Variant_Role) {
        m_comp = value.toUInt();
        return Main_Same_Change_Flag;
    }
    return find_node::set_data(cible,value,role,num);
}
////////////////////////////// bool_node /////////////////////////////
QVariant bool_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case find_model::True_Cible:
        switch (role) {
        case Check_State_Role:
            return m_true ? Qt::Checked : Qt::Unchecked;
        case Label_Role:
            return m_true_label;
        case Type_Role:
            return Check_Sub_Node;
        }
        break;
    case find_model::False_Cible:
        switch (role) {
        case Check_State_Role:
            return m_false ? Qt::Checked : Qt::Unchecked;
        case Label_Role:
            return m_false_label;
        case Type_Role:
            return Check_Sub_Node;
        }
        break;
    case Node_Cible:
        if(num == Node_Num && role == Cibles_Role)
            return find_node::data(cible,role,num).toList() << find_model::True_Cible
                                                            << find_model::False_Cible;
    }
    return find_node::data(cible,role,num);
}

flag bool_node::set_data(int cible, const QVariant &value, int role, numt num) {
    if(role == Check_State_Role) {
        if(cible == find_model::True_Cible){
            m_true = value.toBool();
            return Main_Same_Change_Flag;
        }
        if(cible == find_model::False_Cible){
            m_false = value.toBool();
            return Main_Same_Change_Flag;
        }
    }
    return find_node::set_data(cible,value,role,num);
}
///////////////////////////// date_node ///////////////////////////
QVariant date_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case find_model::Date_Cible:
        switch (role) {
        case Label_Role:
            return "Date :";
        case Date_Role:
            return m_date;
        case Type_Role:
            return Date_Sub_Node;
        }
        break;
    case Node_Cible:
        if(num == Node_Num && role == Cibles_Role)
            return comparaison_node::data(cible,role,num).toList() << find_model::Date_Cible;
    }
    return comparaison_node::data(cible,role,num);
}


flag date_node::set_data(int cible, const QVariant &value, int role, numt num) {
    if(cible == find_model::Date_Cible &&role == Date_Role) {
        m_date = value.toDate();
        return Main_Same_Change_Flag;
    }
    return comparaison_node::set_data(cible,value,role,num);
}

bool date_node::test_value(const QVariant &value) const {
    switch (m_comp) {
    case find_model::Egal:
        return m_date == value.toDate();
    case find_model::Different:
        return m_date != value.toDate();
    case find_model::Inferieure:
        return m_date > value.toDate();
    case find_model::Superieure:
        return m_date < value.toDate();
    case find_model::Inf_Egal:
        return m_date >= value.toDate();
    case find_model::Sup_Egal:
        return m_date <= value.toDate();
    default:
        return false;
    }
}
///////////////////////////// texte_node ///////////////////////////
texte_node::texte_node(find_model *model, numt pos,const QString &texte, bool c,bool regex)
    : find_node(model,pos,find_model::Texte_Node_Type), m_texte(texte), m_case(c), m_regex(regex) {
    if(m_regex){
        m_regular.setPattern(m_texte);
        if(!m_case)
            m_regular.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    }
}

QVariant texte_node::data(int cible, int role, numt num) const {
    switch (cible) {
    case find_model::Texte_Cible:
        switch (role ) {
        case String_Role:
            return m_texte;
        case Label_Role:
            return "Texte :";
        case Type_Role:
            return Line_Edit_Sub_Node;
        }
        break;
    case find_model::Case_Cible:
        switch (role) {
        case Check_State_Role:
            return m_case ? Qt::Checked : Qt::Unchecked;
        case Label_Role:
            return "Case";
        case Type_Role:
            return Check_Sub_Node;
        }
        break;
    case find_model::Regex_Cible:
        switch (role) {
        case Check_State_Role:
            return m_regex ? Qt::Checked : Qt::Unchecked;
        case Label_Role:
            return "Expression régulière";
        case Type_Role:
            return Check_Sub_Node;
        }
        break;
    case Node_Cible:
        if(num == Node_Num && role == Cibles_Role)
            return find_node::data(cible,role,num).toList() << find_model::Texte_Cible
                                                            << find_model::Case_Cible
                                                            << find_model::Regex_Cible;
    }
    return find_node::data(cible,role,num);
}

flag texte_node::set_data(int cible, const QVariant &value, int role, numt num) {
    switch (cible) {
    case find_model::Texte_Cible:
        if(role == String_Role) {
                m_texte = value.toString();
                if(m_regex)
                    m_regular.setPattern(m_texte);
                return Main_Same_Change_Flag;
        }
        break;
    case find_model::Case_Cible:
        if(role == Check_State_Role) {
            m_regular.setPatternOptions((m_case = value.toBool())? QRegularExpression::NoPatternOption
                                                                 : QRegularExpression::CaseInsensitiveOption);
            return Main_Same_Change_Flag;
        }
        break;
    case find_model::Regex_Cible:
        if(role == Check_State_Role
                && (m_regex = value.toBool())) {
            m_regular.setPattern(m_texte);
            return Main_Same_Change_Flag;
        }
        break;
    }
    return find_node::set_data(cible,value,role,num);
}

bool texte_node::test_value(const QVariant &value) const {
    if(m_regex)
        return m_regular.match(value.toString(),0).hasMatch();
    else
        return value.toString().contains(m_texte, m_case ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
