#include "manager_for_arbre.h"

using namespace mps::manager;

manager_for_arbre::manager_for_arbre(const info_bdd &info)
    : ma_sql_arbre(info, std::make_unique<arbre_unique_sql<arbre>>()) {
    // Get Parent
    m_get_parent.append("SELECT ");
    m_get_parent.append(attribut(arbre::Parent));
    m_get_parent.append(" FROM ");
    m_get_parent.append(table()).append(" WHERE ");
    m_get_parent.append(attribut(arbre::Id));
    m_get_parent.append("=?");
    m_get_parent.squeeze();
}

void manager_for_arbre::add(arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendant doit avoir un numéro nul.");
                else
                    ma_sql_arbre::add(node);
                parent.set_feuille(false);
                ma_sql_arbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max) {
                    node.set_num(max+1);
                    ma_sql_arbre::add(node);
                }
                else {
                    vector_ptr<arbre> nodes = get_list(arbre::Parent,node.parent(),
                                                 arbre::Num,node.num(),
                                                 arbre::Num,
                                                 b2d::condition::Egal,b2d::condition::Sup_Egal,
                                                 false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->set_num(i->num() + 1);
                        ma_sql_arbre::modify(*i);
                    }
                    ma_sql_arbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ma_sql_arbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void manager_for_arbre::add(const arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ma_sql_arbre::add(node);
                parent.set_feuille(false);
                ma_sql_arbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    ma_sql_arbre::add(arbre(true,max+1,node.parent()));
                else {
                    vector_ptr<arbre> nodes = get_list(arbre::Num,node.num(),arbre::Num,b2d::condition::Sup_Egal,false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->set_num(i->num() + 1);
                        ma_sql_arbre::modify(*i);
                    }
                    ma_sql_arbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ma_sql_arbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void manager_for_arbre::add_unstable(arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ma_sql_arbre::add(node);
                parent.set_feuille(false);
                ma_sql_arbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max) {
                    node.set_num(max+1);
                    ma_sql_arbre::add(node);
                }
                else {
                    deplace(node);
                    ma_sql_arbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ma_sql_arbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

void manager_for_arbre::add_unstable(const arbre &node) {
    if(node.feuille()) {
        if(node.parent()) {
            arbre parent(node.parent());
            get(parent);
            if(parent.feuille()) {
                if(node.num())
                    throw std::invalid_argument("Le premier descendnat doit avoir un numéro nul.");
                else
                    ma_sql_arbre::add(node);
                parent.set_feuille(false);
                ma_sql_arbre::modify(parent);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    ma_sql_arbre::add(arbre(true,max+1,node.parent()));
                else {
                    deplace(node);
                    ma_sql_arbre::add(node);
                }
            }
        }
        else {
            if(node.num())
                throw std::invalid_argument("Une racine doit avoir un numéro nul.");
            else
                ma_sql_arbre::add(node);
        }
    }
    else
        throw std::invalid_argument("Le nouveau noeud ajouté doit être une feuille.");
}

bool manager_for_arbre::del(idt id) {
    arbre node(id);
    if(!get(node))
        return true;
    if(!node.feuille() || !ma_sql_arbre::del(node.id()))
        return false;
    if(node.parent()) {
        if(exists(arbre::Parent,node.parent())) {
            vector_ptr<arbre> nodes = get_list(arbre::Num,node.num(),arbre::Parent,node.parent(),arbre::Num,b2d::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->set_num(i->num() - 1);
                ma_sql_arbre::modify(*i);
            }
        }
        else {
            arbre parent(node.parent());
            get(parent);
            parent.set_feuille(true);
            ma_sql_arbre::modify(parent);
        }
    }
    return true;
}

/*void manager_for_arbre::delUnstable(const arbre &node)
{
    if(!node.feuille())
    {
        list_ptr<arbre> childs(get_list(arbre::Parent,node.id()));
        for(list_ptr<arbre>::iterator i = childs.begin(); i != childs.end(); ++i)
            delUnstable(*i);
    }
    ma_sql_arbre::del(node.id());
}*/

bool manager_for_arbre::deplace(const arbre &node) {
    arbre node_bdd(node.num(),node.parent());
    if(exists_unique(node_bdd)) {
        if(node_bdd.id() != node.id()) {
            node_bdd.set_num(-node_bdd.num());
            ma_sql_arbre::modify(node_bdd);
            return true;
        }
        else
            return false;
    }
    else
        return true;
}

void manager_for_arbre::modify(const arbre &node) {
    arbre node_bdd(node.id());
    get(node_bdd);
    auto id_parent_bdd = node_bdd.parent();
    auto num_bdd = node_bdd.num();
    if(node.parent()) {
        if(node_bdd.parent() == node.parent()) {
            auto max = size(node.parent());
            auto num = node.num();
            if(num < 0 || num > max)
                num = max;
            if(num_bdd != num) {
                node_bdd.set_num(-num_bdd);
                ma_sql_arbre::modify(node_bdd);
                if(std::abs(num_bdd - num) == 1) {
                    arbre node_suivant(num,node.parent());
                    get_unique(node_suivant);
                    node_suivant.set_num(num_bdd);
                    ma_sql_arbre::modify(node_suivant);
                }
                else if(num < num_bdd) {
                    vector_ptr<arbre> nodes = get_list(arbre::Parent,node.parent(),arbre::Num,num,arbre::Num,num_bdd,
                                                arbre::Num,b2d::condition::Egal,b2d::condition::Sup_Egal,b2d::condition::Inf,false);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->set_num(i->num() + 1);
                        ma_sql_arbre::modify((*i));
                    }
                }
                else {
                    vector_ptr<arbre> nodes = get_list(arbre::Parent,node.parent(),arbre::Num,num_bdd,arbre::Num,num,
                                                arbre::Num,b2d::condition::Egal,b2d::condition::Sup,b2d::condition::Inf_Egal);
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->set_num(i->num() - 1);
                        ma_sql_arbre::modify((*i));
                    }
                }
                node_bdd.set_num(num);
            }
        }
        else {
            node_bdd.set_parent(node.parent());
            arbre parent(node_bdd.parent());
            get(parent);
            if(parent.feuille()) {
                parent.set_feuille(false);
                ma_sql_arbre::modify(parent);
                node_bdd.set_num(0);
            }
            else {
                auto max = size(node.parent());
                if(node.num() < 0 || node.num() > max)
                    node_bdd.set_num(max + 1);
                else {
                    vector_ptr<arbre> nodes(get_list(arbre::Parent,node.parent(),arbre::Num,node.num(),
                                                arbre::Num,b2d::condition::Egal,b2d::condition::Sup_Egal,false));
                    for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                        i->set_num(i->num() + 1);
                        ma_sql_arbre::modify((*i));
                    }
                    node_bdd.set_num(node.num());
                }
            }
        }
        ma_sql_arbre::modify(node_bdd);
    }
    else {
        node_bdd.set_parent(0);
        node_bdd.set_num(0);
        ma_sql_arbre::modify(node_bdd);
    }

    if(id_parent_bdd != 0 &&id_parent_bdd != node_bdd.parent()) {
        if(exists(arbre::Parent,id_parent_bdd)) {
            vector_ptr<arbre> nodes
                    = get_list(arbre::Parent,id_parent_bdd,arbre::Num,num_bdd,arbre::Num,b2d::condition::Egal,b2d::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->set_num(i->num() - 1);
                ma_sql_arbre::modify(*i);
            }
        }
        else {
            arbre parent(id_parent_bdd);
            get(parent);
            parent.set_feuille(true);
            ma_sql_arbre::modify(parent);
        }
    }
}

void manager_for_arbre::modify_unstable(const arbre &node) {
    arbre node_bdd(node.id());
    get(node_bdd);
    auto max = size(node.parent());
    if(node.num() < 0 || node.num() > max)
        ma_sql_arbre::modify(arbre(node_bdd.feuille(),max+1,node.parent(),node.id()));
    else {
        if(deplace(node))
            ma_sql_arbre::modify(node);
    }

    if(node.parent() != node_bdd.parent()) {
        if(!exists(arbre::Parent,node_bdd.parent())) {
            arbre node_bdd_parent(node_bdd.parent());
            get(node_bdd_parent);
            node_bdd_parent.set_feuille(true);
            ma_sql_arbre::modify(node_bdd_parent);
        }

        arbre node_parent(node.parent());
        get(node_parent);
        if(node_parent.feuille()) {
            node_parent.set_feuille(false);
            ma_sql_arbre::modify(node_parent);
        }
    }
}

void manager_for_arbre::modify_stable_unstable(const arbre &node) {
    arbre node_bdd(node.id());
    get(node_bdd);
    auto max = size(node.parent());
    if(node.num() < 0 || node.num() > max)
        ma_sql_arbre::modify(arbre(node_bdd.feuille(),max+1,node.parent(),node.id()));
    else {
        if(deplace(node))
            ma_sql_arbre::modify(node);
    }

    if(node_bdd.parent() != 0 &&node_bdd.parent() != node.parent()) {
        if(exists(arbre::Parent,node_bdd.parent())) {
            vector_ptr<arbre> nodes
                    = get_list(arbre::Parent,node_bdd.parent(),arbre::Num,node_bdd.num(),arbre::Num,b2d::condition::Egal,b2d::condition::Sup);
            for(auto i = nodes.begin(); i != nodes.end(); ++i) {
                i->set_num(i->num() - 1);
                ma_sql_arbre::modify((*i));
            }
        }
        else {
            arbre parent(node_bdd.parent());
            get(parent);
            parent.set_feuille(true);
            ma_sql_arbre::modify(parent);
        }
    }
}
