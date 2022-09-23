#include "predef_model.h"

using namespace mps;
using namespace model_base;

////////////////////////////////////////////// permission_model ////////////////////////////////////
permission_model::permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : item_node_bdd_model(bdd, parent) {
    for(entidt i = 0; i < m_bdd.nbr_entity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_id_nom[bdd.cible(i)] = m_bdd.managers().info(i).name();
    }
}

//void permission_model::set_cible(entidt num, bool visible){
//    if(visible) {
//        if(std::find(m_cible_vec.cbegin(),m_cible_vec.cend(),num) == m_cible_vec.cend()) {
//            begin_reset_data();
//                m_cible_vec.push_back(num);
//                for (auto iter = m_data.cbegin(); iter; ++iter)
//                    static_cast<abstract_permission_node &>(**iter).add_cible(m_id_nom.at(num).first);
//            end_reset_data();
//        }
//    }
//    else {
//        auto iter = std::find(m_cible_vec.cbegin(),m_cible_vec.cend(),num);
//        if(iter != m_cible_vec.cend()) {
//            model_about_to_reset_data();
//                m_cible_vec.erase(iter);
//            model_reset_data();
//        }
//    }
//}

/////////////////////////////////////////////// cible_permission_interface_model ////////////////////////////////
cible_permission_interface_model::cible_permission_interface_model(permission_model *model, QObject *parent)
    : interface_node_model(model, parent) {
    for(auto iter = model->id_nom().cbegin(); iter != model->id_nom().cend(); ++iter) {
        auto node = std::make_unique<cible_permission_interface_node>(this);
        node->set_cible(iter->first, iter->second);
        m_data.push_back(m_data.cbegin(),std::move(node));
    }
}

node_ptr cible_permission_interface_model::node_factory(const node_index &/*parent*/, numt /*pos*/, int /*type*/)
    {return std::make_unique<cible_permission_interface_node>(this);}
////////////////////////////////////////////// cible_permission_interface_node ///////////////////////////////////
QVariant cible_permission_interface_node::data(int cible, int role, numt num) const {
    switch (role) {
    case Label_Role:
        if(cible == cible_permission_interface_model::Nom_Cible)
            return m_nom;
        break;
    case Type_Role:
        switch (cible) {
        case cible_permission_interface_model::Nom_Cible:
            return Line_Edit_Sub_Node;
        case cible_permission_interface_model::Permission_Cible:
            return Code_Sub_Node;
        }
        break;
//    case Num_Role:
//        if(cible == permission_model::Permission_Cible)
//            return m_permission_map.at(m_model->cible(num)).value();
//        break;
    case Cibles_Role:
        if(cible == Node_Cible && num == Node_Num){
            QList<QVariant> cibles({permission_model::Nom_Cible});
//            for (auto i = 0; i != static_cast<int>(m_model->cible_size()); ++i)
//                cibles.append(QList<QVariant>({permission_model::Permission_Cible,i}));
            return cibles;
        }
        break;
    }
    return item_node::data(cible,role,num);
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
            return m_permission_map.at(num).value();
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
