#include "predef_model.h"

using namespace mps;
using namespace model_base;

////////////////////////////////////////////// permission_model ////////////////////////////////////
permission_model::permission_model(b2d::bdd_predef &bdd, QObject *parent)
    : item_node_bdd_model(bdd, parent) {
    for(entidt i = 0; i < m_bdd.nbr_entity(); ++i) {
        if(m_bdd.managers().valide(i))
            m_id_nom[static_cast<numt>(bdd.cible(i))] = m_bdd.managers().info(i).name();
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
    m_data.set_root(std::make_unique<cible_permission_interface_node>(this));
    for(auto iter = model->id_nom().cbegin(); iter != model->id_nom().cend(); ++iter) {
        auto node = std::make_unique<cible_permission_interface_node>(this);
        node->set_cible(iter->first);
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
            return m_model->model()->id_nom().at(m_cible);
        break;
    case Type_Role:
        switch (cible) {
        case cible_permission_interface_model::Nom_Cible:
            return Label_Sub_Node;
        case cible_permission_interface_model::Permission_Cible:
            return Code_Sub_Node;
        }
        break;
//    case Num_Role:
//        if(cible == permission_model::Permission_Cible)
//            return ;
//        break;
    case Cibles_Role:
        if(cible == Node_Cible && num == Node_Num){
            QList<QVariant> cibles({cible_permission_interface_model::Nom_Cible,
                                   cible_permission_interface_model::Permission_Cible});//,permission_model::Permission_Cible});
//            for (auto i = 0; i != static_cast<int>(m_model->cible_size()); ++i)
//                cibles.append(QList<QVariant>({permission_model::Permission_Cible,i}));
            return cibles;
        }
        break;
    }
    return item_node::data(cible,role,num);
}

flag cible_permission_interface_node::flags(int cible, numt num) const{
    if(m_iter.root())
        return No_Flag_Node;
    return item_node::flags(cible,num);
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
            return cibles;
        }
        break;
    }
    return item_bdd_node::data(cible,role,num);
}
