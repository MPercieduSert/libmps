#include "predef_model.h"

using namespace mps;
using namespace model_base;

////////////////////////////////////////////// permission_model ////////////////////////////////////
permission_model::permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : item_node_bdd_model(bdd, parent) {
    m_id_nom_vec.reserve(m_bdd.nbr_entity());
    for(entidt i = 0; i < m_bdd.nbr_entity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_id_nom_vec.emplace_back(bdd.cible(i),m_bdd.managers().info(i).name());
    }
    std::sort(m_id_nom_vec.begin(),m_id_nom_vec.end(),
              [](const std::pair<int, QString> &p1, const std::pair<int, QString> &p2)->bool {return p1.second < p2.second;});
}

void permission_model::set_cible(entidt num, bool visible){
    if(visible) {
        if(std::find(m_cible_vec.cbegin(),m_cible_vec.cend(),num) == m_cible_vec.cend()) {
            begin_reset_data();
                m_cible_vec.push_back(num);
                for (auto iter = m_data.cbegin(); iter; ++iter)
                    static_cast<abstract_permission_node &>(**iter).add_cible(m_id_nom_vec.at(num).first);
            end_reset_data();
        }
    }
    else {
        auto iter = std::find(m_cible_vec.cbegin(),m_cible_vec.cend(),num);
        if(iter != m_cible_vec.cend()) {
            model_about_to_reset_data();
                m_cible_vec.erase(iter);
            model_reset_data();
        }
    }
}

//////////////////////////////////////////////////////// mot_cle_permission_model ///////////////////////////////
mot_cle_permission_model::mot_cle_permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : permission_model(bdd,parent) {
    m_data.set_tree_ref(bdd.get_arbre<entities::mot_cle>(),[this](const entities::mot_cle &ent) {
        auto node = std::make_unique<mot_cle_permission_node>(this);
        node->set_ent(ent);
        return node;
    });
}

//////////////////////////////////////////////////////// type_permission_model ///////////////////////////////
type_permission_model::type_permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : permission_model(bdd,parent) {
    m_data.set_tree_ref(bdd.get_arbre<entities::type>(),[this](const entities::type &ent) {
        auto node = std::make_unique<type_permission_node>(this);
        node->set_ent(ent);
        return node;
    });
}

////////////////////////////////////////////////////// abstract_permission_node ////////////////////////////////
QVariant abstract_permission_node::data(int cible, int role, numt num) const {
    switch (role) {
    case Label_Role:
        switch (cible) {
        case permission_model::Nc_Cible:
            return "Nom abrégé :";
        case permission_model::Nom_Cible:
            return "Nom :";
        case permission_model::Ref_Cible:
            return "Référence :";
        case permission_model::Permission_Cible:
            return m_model->nom_cible(num);
        }
        break;
    case Type_Role:
        switch (cible) {
        case permission_model::Nc_Cible:
        case permission_model::Nom_Cible:
        case permission_model::Ref_Cible:
            return Line_Edit_Sub_Node;
        case permission_model::Permission_Cible:
            return Code_Sub_Node;
        }
        break;
    case Num_Role:
        if(cible == permission_model::Permission_Cible)
            return m_permission_map.at(m_model->cible(num)).value();
        break;
    case Cibles_Role:
        if(cible == Node_Cible && num == Node_Num){
            QList<QVariant> cibles({permission_model::Nc_Cible,
                                    permission_model::Nom_Cible,
                                    permission_model::Ref_Cible});
            for (auto i = 0; i != static_cast<int>(m_model->cible_size()); ++i)
                cibles.append(QList<QVariant>({permission_model::Permission_Cible,i}));
            return cibles;
        }
        break;
    }
    return item_bdd_node::data(cible,role,num);
}
