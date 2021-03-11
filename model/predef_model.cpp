#include "predef_model.h"

using namespace mps;
using namespace model_base;

////////////////////////////////////////////// permission_model ////////////////////////////////////
permission_model::permission_model(b2d::bdd_predef &bdd, enumt offset, QObject *parent)
    : item_node_bdd_model(bdd, parent), m_offset(offset) {
    m_id_nom_vec.reserve(m_bdd.nbr_entity());
    for(entidt i = 0; i < m_bdd.nbr_entity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_id_nom_vec.emplace_back(bdd.cible(i),m_bdd.managers().info(i).name());
    }
    std::sort(m_id_nom_vec.begin(),m_id_nom_vec.end(),
              [](const std::pair<int, QString> &p1, const std::pair<int, QString> &p2)->bool {return p1.second < p2.second;});
}

QVariant permission_model::data(const node_index &index, int role) const {
    if(index.cible() == Node_Cible) {
        if(index.num() == Node_Num) {
            if(role == Nombre_Role)
                return static_cast<numt>(m_cible_vec.size()) + m_offset;
        }
        else if (role == Cible_Role)
            switch (index.num()) {
            case Nc_Position:
                return Nc_Cible;
            case Nom_Position:
                return Nom_Cible;
            case Ref_Cible:
                return Ref_Cible;
            default:
                return Permission_Cible;
            }
    }
    return item_node_bdd_model::data(index,role);
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
    : permission_model(bdd,Nc_Nom_Ref_Offset,parent) {
    m_data.set_tree_ref(bdd.get_arbre<entities::mot_cle>(),[this](const entities::mot_cle &ent) {
        auto node = std::make_unique<mot_cle_permission_node>(this);
        node->set_ent(ent);
        return node;
    });
}

//////////////////////////////////////////////////////// type_permission_model ///////////////////////////////
type_permission_model::type_permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : permission_model(bdd,Nc_Nom_Ref_Offset,parent) {
    m_data.set_tree_ref(bdd.get_arbre<entities::type>(),[this](const entities::type &ent) {
        auto node = std::make_unique<type_permission_node>(this);
        node->set_ent(ent);
        return node;
    });
}
