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

numt permission_model::data_count(const node_index &index) const {
    switch (index.cible()) {
    case permission_model::Nc_Cible:
    case permission_model::Nom_Cible:
    case permission_model::Ref_Cible:
        return 1;
    case permission_model::Permission_Cible:
        return static_cast<numt>(m_cible_vec.size());
    case Sub_Node_Cible:
        return static_cast<numt>(m_cible_vec.size()) + m_offset;
    }
    return item_node_model::data_count(index);
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
    m_data.set_tree(bdd.get_arbre<entities::mot_cle>(),[this](const entities::mot_cle &ent) {
        auto node = std::make_unique<mot_cle_permission_node>(this);
        node->set_ent(ent);
        return node;
    });
}

//////////////////////////////////////////////////////// type_permission_model ///////////////////////////////
type_permission_model::type_permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : permission_model(bdd,Nc_Nom_Ref_Offset,parent) {
    m_data.set_tree(bdd.get_arbre<entities::type>(),[this](const entities::type &ent) {
        auto node = std::make_unique<type_permission_node>(this);
        node->set_ent(ent);
        return node;
    });
}
